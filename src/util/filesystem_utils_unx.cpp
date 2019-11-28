/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppdevtk/util/filesystem_utils.hpp>
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>
#include <cppdevtk/base/zeroize.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/tstring.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/string_utils.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/architecture_types.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/tstring.hpp>
#include <cppdevtk/base/dynamic_byte_array.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#if (CPPDEVTK_PLATFORM_ANDROID)
#include <android/api-level.h>
#endif
#if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 20))
#include <sys/statvfs.h>
#endif
#if (CPPDEVTK_PLATFORM_MACOSX)
#include <sys/uio.h>
#endif

#include <cstdio>


// NOTE: On Linux if close() fail and errno is EINTR we should not retry; please see https://lkml.org/lkml/2005/9/10/129
// The same (probably) for Mac: https://code.google.com/p/chromium/issues/detail?id=269623
// Between a failing close() and a EBADF or worst closing other fd, we choose to ignore EINTR for close()


using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API void DeleteFile(const QString& fileName, bool failIfNotExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(fileName), "fileName must be valid");
	
	const QString kNativeFileName = QDir::toNativeSeparators(fileName);
	
	if (unlink(CPPDEVTK_Q2U(kNativeFileName).c_str()) != ESUCCESS) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		
		CPPDEVTK_ASSERT(kErrorCode.GetValue() != EINTR);
		
		if (!failIfNotExists) {
			if (kErrorCode.GetValue() == ENOENT) {
				return;
			}
		}
		
		if (kErrorCode.GetValue() == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(fileName);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "failed to delete file", fileName);
		}
	}
}

CPPDEVTK_UTIL_API void CopyFile(const QString& srcFileName, const QString& dstFileName, bool failIfExists) {
	using ::cppdevtk::base::Zeroize;
	
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(srcFileName), "srcFileName must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dstFileName), "dstFileName must be valid");
	
	const QString kNativeSrcFileName = QDir::toNativeSeparators(srcFileName);
	const QString kNativeDstFileName = QDir::toNativeSeparators(dstFileName);
	
	const int kSrcFd = TEMP_FAILURE_RETRY(open(CPPDEVTK_Q2U(kNativeSrcFileName).c_str(), O_RDONLY));
	if (kSrcFd == -1) {
		if (errno == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(srcFileName);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "failed to open file for reading", srcFileName);
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((kSrcFd)(&kNativeSrcFileName)) {
		if (close(kSrcFd) == -1) {
			CPPDEVTK_LOG_WARN("failed to close file: " << kNativeSrcFileName << "\nerror code: " << GetLastSystemErrorCode().ToString());
			base::SuppressUnusedWarning(kNativeSrcFileName);
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	struct stat srcStat;
	//Zeroize(&srcStat, sizeof(srcStat));
	if (fstat(kSrcFd, &srcStat) != ESUCCESS) {
		CPPDEVTK_ASSERT(errno != EINTR);
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "fstat() failed", srcFileName);
	}
	
	const int kDstFlags = O_WRONLY | O_APPEND | O_CREAT | (failIfExists ? O_EXCL : O_TRUNC);
	const int kDstFd = TEMP_FAILURE_RETRY(open(CPPDEVTK_Q2U(kNativeDstFileName).c_str(), kDstFlags, srcStat.st_mode));
	if (kDstFd == -1) {
		if (errno == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(dstFileName);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "failed to open file for writing", dstFileName);
		}
	}
	bool closeDstFd = true;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&closeDstFd)(kDstFd)(&kNativeDstFileName)) {
		if (closeDstFd) {
			if (close(kDstFd) == -1) {
				CPPDEVTK_LOG_WARN("failed to close file: " << kNativeDstFileName
						<< "\nerror code: " << GetLastSystemErrorCode().ToString());
				base::SuppressUnusedWarning(kNativeDstFileName);
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	//::cppdevtk::base::byte buf[BUFSIZ];
	::cppdevtk::base::DynamicByteArray buf(srcStat.st_blksize);
	for (;;) {
		const ssize_t kReaded = TEMP_FAILURE_RETRY(read(kSrcFd, &buf[0], buf.size()));
		if (kReaded == -1) {
			CPPDEVTK_ASSERT(errno != EINTR);
			CPPDEVTK_ASSERT((errno != EAGAIN) && (errno != EWOULDBLOCK));
			CPPDEVTK_ASSERT(errno != EBADF);
			CPPDEVTK_ASSERT(errno != EFAULT);
			CPPDEVTK_ASSERT(errno != EINVAL);
			
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "failed to read from file", srcFileName);
		}
		
		if (kReaded == 0) {	// EOF
			break;
		}
		
		CPPDEVTK_ASSERT(kReaded > 0);
		
		ssize_t totalWritten = 0;
		do {
			const ssize_t kCurrentWritten = TEMP_FAILURE_RETRY(write(kDstFd, &buf[totalWritten], (kReaded - totalWritten)));
			if (kCurrentWritten == -1) {
				CPPDEVTK_ASSERT(errno != EINTR);
				CPPDEVTK_ASSERT((errno != EAGAIN) && (errno != EWOULDBLOCK));
				CPPDEVTK_ASSERT(errno != EBADF);
				
				throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "failed to write to file", dstFileName);
			}
			
			if (kCurrentWritten == 0) {	// this should never happen...
				throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(),
						"failed to write to file (write() returned 0 for count > 0 and blocking)", dstFileName);
			}
			
			CPPDEVTK_ASSERT(kCurrentWritten > 0);
			
			totalWritten += kCurrentWritten;
			CPPDEVTK_ASSERT(totalWritten <= kReaded);
		}
		while (totalWritten != kReaded);
	}
	
	const int kRetCode = TEMP_FAILURE_RETRY(fsync(kDstFd));
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("fsync() failed for file: " << kNativeDstFileName << "\nerror code: " << GetLastSystemErrorCode().ToString());
	}
	
	closeDstFd = false;
	if (close(kDstFd) == -1) {
		if (errno != EINTR) {
			CPPDEVTK_ASSERT(errno != EBADF);
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "failed to close file", dstFileName);
		}
		
		CPPDEVTK_LOG_WARN("failed to close file: " << kNativeDstFileName << "\nerror code: " << GetLastSystemErrorCode().ToString());
	}
}

CPPDEVTK_UTIL_API void MakeDirectory(const QString& dirName, bool failIfExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dirName), "dirName must be valid");
	
	const QString kNativeDirName = QDir::toNativeSeparators(dirName);
	
	if (mkdir(CPPDEVTK_Q2U(kNativeDirName).c_str(), (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) != ESUCCESS) {
		CPPDEVTK_ASSERT(errno != EINTR);
		
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		
		if (!failIfExists) {
			if (kErrorCode.GetValue() == EEXIST) {
				return;
			}
		}
		
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "failed to make directory", dirName);
	}
}

CPPDEVTK_UTIL_API void MakePath(const QString& dirPath, bool failIfExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dirPath), "dirPath must be valid");
	
	const QString kCleanDirPath = QDir::cleanPath(dirPath);
	CPPDEVTK_ASSERT(!kCleanDirPath.isEmpty());
	const QStringList kDirPaths = kCleanDirPath.split('/', QString::SkipEmptyParts);
	CPPDEVTK_ASSERT(!kDirPaths.isEmpty());
	const int kNumDirPathsToMake = kDirPaths.size() - 1;
	QString dirPathToMake;
	if (QDir::isAbsolutePath(dirPath)) {
		dirPathToMake += '/';
	}
	for (int cnt = 0; cnt < kNumDirPathsToMake; ++cnt) {
		dirPathToMake += kDirPaths[cnt];
		MakeDirectory(dirPathToMake, false);
		dirPathToMake += '/';
	}
	dirPathToMake += kDirPaths[kNumDirPathsToMake];
	CPPDEVTK_ASSERT(dirPathToMake == kCleanDirPath);
	MakeDirectory(dirPathToMake, failIfExists);
}

CPPDEVTK_UTIL_API void RemoveDirectory(const QString& path, bool failIfNotExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(path), "path must be valid");
	
	const QString kNativePath = QDir::toNativeSeparators(path);
	
	if (rmdir(CPPDEVTK_Q2U(kNativePath).c_str()) != ESUCCESS) {
		CPPDEVTK_ASSERT(errno != EINTR);
		
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		
		if (!failIfNotExists) {
			if (kErrorCode.GetValue() == ENOENT) {
				return;
			}
		}
		
		if (kErrorCode.GetValue() == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(path);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "failed to remove directory", path);
		}
	}
}

CPPDEVTK_UTIL_API void GetFileSystemSpaceInfo(const QString& path, FileSystemSpaceInfo& fileSystemSpaceInfo) {
#	if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 20))
	
	typedef FileSystemSpaceInfo::size_type fssi_size_type;
	
	
	CPPDEVTK_DBC_CHECK_ARGUMENT((IsValidPath(path)), "path must be valid");
	
	const QString kNativeAbsPath = QDir::toNativeSeparators(QFileInfo(path).absolutePath());
	struct statvfs statVfs;
	//Zeroize(&statVfs, sizeof(statVfs));
	const int kRetCode = TEMP_FAILURE_RETRY(statvfs(CPPDEVTK_Q2U(kNativeAbsPath).c_str(), &statVfs));
	if (kRetCode != ESUCCESS) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		if (kErrorCode.GetValue() == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(path);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "statvfs() failed", path);
		}
	}
	
	fileSystemSpaceInfo.size_ = ((fssi_size_type)statVfs.f_blocks) * ((fssi_size_type)statVfs.f_frsize);
	fileSystemSpaceInfo.totalFree_ = ((fssi_size_type)statVfs.f_bfree) * ((fssi_size_type)statVfs.f_frsize);
	fileSystemSpaceInfo.userFree_ = ((fssi_size_type)statVfs.f_bavail) * ((fssi_size_type)statVfs.f_frsize);
	
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.size_ > 0);
#	if 0	// FileSystemSpaceInfo::size_type is unsigned
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.totalFree_ >= 0);
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.userFree_ >= 0);
#	endif
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.size_ >= fileSystemSpaceInfo.totalFree_);
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.totalFree_ >= fileSystemSpaceInfo.userFree_);
	
#	else
	// TODO: Android port for API level < 20
	
	::cppdevtk::base::SuppressUnusedWarning(path);
	::cppdevtk::base::SuppressUnusedWarning(fileSystemSpaceInfo);
	
	CPPDEVTK_COMPILER_WARNING("GetFileSystemSpaceInfo(): Not working on Android < 4.4W2 (API level < 20)!");
	
	fileSystemSpaceInfo.size_ = 0;
	fileSystemSpaceInfo.totalFree_ = 0;
	fileSystemSpaceInfo.userFree_ = 0;
#	endif
}

CPPDEVTK_UTIL_API QStringList GetMountPoints(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(serialNumber.isEmpty(), "serialNumber");
	
	CPPDEVTK_LOG_DEBUG("deviceType: " << deviceType << "; serialNumber: " << serialNumber
			<< "; vendorId: " << base::NumToHexStr(vendorId) << "; productId: " << base::NumToHexStr(productId));
	
	const QString kDeviceName = GetDeviceName(deviceType, serialNumber, vendorId, productId);
	if (kDeviceName.isEmpty()) {
		CPPDEVTK_LOG_ERROR("failed to get device name");
		return QStringList();
	}
	
	CPPDEVTK_LOG_DEBUG("kDeviceName: " << kDeviceName);
	
	return GetMountPointsFromDeviceName(kDeviceName);
}




CPPDEVTK_UTIL_API QString GetDeviceNameFromMountPoint(const QString& mountPoint,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	QString deviceName;
	try {
		deviceName = GetDeviceNameFromMountPoint(mountPoint);
	}
	catch (const ::cppdevtk::base::SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
	}
	catch (const ::std::bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
	}
	
	return deviceName;
}


}	// namespace util
}	// namespace cppdevtk
