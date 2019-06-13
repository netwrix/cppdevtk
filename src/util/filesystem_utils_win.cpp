/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/tstring.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <windows.h>

#include <cstddef>


using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API bool IsValidPath(const QString& path, bool ignorePathSeparator) {
	if (path.isEmpty()) {
		CPPDEVTK_LOG_ERROR("path is empty");
		return false;
	}
	if (path.contains('\\')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' is not in Qt format");
		return false;
	}
	
	// please see: https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247%28v=vs.85%29.aspx#naming_conventions
	
	if (path.contains('<')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character <");
		return false;
	}
	if (path.contains('>')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character >");
		return false;
	}
	if (path.contains('\"')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character \"");
		return false;
	}
	if (path.contains('|')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character |");
		return false;
	}
	if (path.contains('\?')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character ?");
		return false;
	}
	if (path.contains('*')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character *");
		return false;
	}
	
	/*
	if (path.endsWith(' ')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' ends with space");
		return false;
	}
	if (path.endsWith('.')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' ends with period");
		return false;
	}
	*/
	
	int cnt = path.count(':');
	if (cnt > 1) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character :");
		return false;
	}
	if (cnt == 1) {
		if (!QDir::isAbsolutePath(path)) {
			CPPDEVTK_LOG_ERROR("path '" << path << "' contains invalid character : and is not absolute");
			return false;
		}
		if (!((path[0].isLetter() && (path[1] == ':')) || (path.startsWith(":/")))) {
			CPPDEVTK_LOG_ERROR("path '" << path
					<< "' contains invalid character : and is not a drive letter or a Qt resource path");
			return false;
		}
	}
	
	return ignorePathSeparator ? true : !path.contains('/');
}

CPPDEVTK_UTIL_API void DeleteFile(const QString& fileName, bool failIfNotExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(fileName), "fileName must be valid");
	
	QString nativeFileName = QDir::toNativeSeparators(fileName);
#	if (!CPPDEVTK_DISABLE_UNICODE)
	if (QDir::isAbsolutePath(fileName)) {
		nativeFileName.prepend("\\\\?\\");
	}
#	endif
	
	const DWORD kOldFileAttributes = ::GetFileAttributes(CPPDEVTK_Q2T(nativeFileName).c_str());
	if (kOldFileAttributes != INVALID_FILE_ATTRIBUTES) {
		DWORD newFileAttributes = kOldFileAttributes;
		if (newFileAttributes & FILE_ATTRIBUTE_READONLY) {
			newFileAttributes ^= FILE_ATTRIBUTE_READONLY;
		}
		if (newFileAttributes != kOldFileAttributes) {
			if (!::SetFileAttributes(CPPDEVTK_Q2T(nativeFileName).c_str(), newFileAttributes)) {
				const ErrorCode kErrorCode = GetLastSystemErrorCode();
				if ((kErrorCode.GetValue() != ERROR_FILE_NOT_FOUND) && (kErrorCode.GetValue() != ERROR_PATH_NOT_FOUND)) {
					CPPDEVTK_LOG_WARN("SetFileAttributes() failed for " << nativeFileName
							<< "\nerror code: " << kErrorCode.ToString());
				}
			}
		}
	}
	else {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		if ((kErrorCode.GetValue() != ERROR_FILE_NOT_FOUND) && (kErrorCode.GetValue() != ERROR_PATH_NOT_FOUND)) {
			CPPDEVTK_LOG_WARN("GetFileAttributes() failed for " << nativeFileName
					<< "\nerror code: " << kErrorCode.ToString());
		}
	}
	
	if (!
#		if (CPPDEVTK_DISABLE_UNICODE)
		::DeleteFileA
#		else
		::DeleteFileW
#		endif
			(CPPDEVTK_Q2T(nativeFileName).c_str())) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		
		if (!failIfNotExists) {
			if ((kErrorCode.GetValue() == ERROR_FILE_NOT_FOUND) || (kErrorCode.GetValue() == ERROR_PATH_NOT_FOUND)) {
				return;
			}
		}
		
		if ((kErrorCode.GetValue() == ERROR_FILE_NOT_FOUND) || (kErrorCode.GetValue() == ERROR_PATH_NOT_FOUND)) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(fileName);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "failed to delete file", fileName);
		}
	}
}

CPPDEVTK_UTIL_API void CopyFile(const QString& srcFileName, const QString& dstFileName, bool failIfExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(srcFileName), "srcFileName must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dstFileName), "dstFileName must be valid");
	
	QString nativeSrcFileName = QDir::toNativeSeparators(srcFileName);
#	if (!CPPDEVTK_DISABLE_UNICODE)
	if (QDir::isAbsolutePath(srcFileName)) {
		nativeSrcFileName.prepend("\\\\?\\");
	}
#	endif
	
	QString nativeDstFileName = QDir::toNativeSeparators(dstFileName);
#	if (!CPPDEVTK_DISABLE_UNICODE)
	if (QDir::isAbsolutePath(dstFileName)) {
		nativeDstFileName.prepend("\\\\?\\");
	}
#	endif
	
	if (!failIfExists) {
		const DWORD kOldFileAttributes = ::GetFileAttributes(CPPDEVTK_Q2T(nativeDstFileName).c_str());
		if (kOldFileAttributes != INVALID_FILE_ATTRIBUTES) {
			DWORD newFileAttributes = kOldFileAttributes;
			if (newFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
				newFileAttributes ^= FILE_ATTRIBUTE_HIDDEN;
			}
			if (newFileAttributes & FILE_ATTRIBUTE_READONLY) {
				newFileAttributes ^= FILE_ATTRIBUTE_READONLY;
			}
			if (newFileAttributes != kOldFileAttributes) {
				if (!::SetFileAttributes(CPPDEVTK_Q2T(nativeDstFileName).c_str(), newFileAttributes)) {
					const ErrorCode kErrorCode = GetLastSystemErrorCode();
					if (kErrorCode.GetValue() != ERROR_FILE_NOT_FOUND) {
						CPPDEVTK_LOG_WARN("SetFileAttributes() failed for " << nativeDstFileName
								<< "\nerror code: " << kErrorCode.ToString());
					}
				}
			}
		}
		else {
			const ErrorCode kErrorCode = GetLastSystemErrorCode();
			if (kErrorCode.GetValue() != ERROR_FILE_NOT_FOUND) {
				CPPDEVTK_LOG_WARN("GetFileAttributes() failed for " << nativeDstFileName
						<< "\nerror code: " << kErrorCode.ToString());
			}
		}
	}
	
	if (!
#		if (CPPDEVTK_DISABLE_UNICODE)
		::CopyFileA
#		else
		::CopyFileW
#		endif
			(CPPDEVTK_Q2T(nativeSrcFileName).c_str(), CPPDEVTK_Q2T(nativeDstFileName).c_str(), failIfExists)) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC_DST(GetLastSystemErrorCode(), "failed to copy file",
				srcFileName, dstFileName);
	}
}

CPPDEVTK_UTIL_API void MakeDirectory(const QString& dirName, bool failIfExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dirName), "dirName must be valid");
	
	QString nativeDirName = QDir::toNativeSeparators(dirName);
#	if (!CPPDEVTK_DISABLE_UNICODE)
	if (QDir::isAbsolutePath(dirName)) {
		nativeDirName.prepend("\\\\?\\");
	}
#	endif
	
	if (!
#		if (CPPDEVTK_DISABLE_UNICODE)
		::CreateDirectoryA
#		else
		::CreateDirectoryW
#		endif
			(CPPDEVTK_Q2T(nativeDirName).c_str(), NULL)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		
		if (!failIfExists) {
			if (kErrorCode.GetValue() == ERROR_ALREADY_EXISTS) {
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
	for (int cnt = 0; cnt < kNumDirPathsToMake; ++cnt) {
		dirPathToMake += kDirPaths[cnt];
		if (cnt == 0) {	// skip drive
			if (!QDir::isAbsolutePath(dirPathToMake)) {
				MakeDirectory(dirPathToMake, false);
			}
		}
		else {
			MakeDirectory(dirPathToMake, false);
		}
		dirPathToMake += '/';
	}
	dirPathToMake += kDirPaths[kNumDirPathsToMake];
	CPPDEVTK_ASSERT(dirPathToMake == kCleanDirPath);
	MakeDirectory(dirPathToMake, failIfExists);
}

CPPDEVTK_UTIL_API void RemoveDirectory(const QString& path, bool failIfNotExists) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(path), "path must be valid");
	
	QString nativePath = QDir::toNativeSeparators(path);
#	if (!CPPDEVTK_DISABLE_UNICODE)
	if (QDir::isAbsolutePath(path)) {
		nativePath.prepend("\\\\?\\");
	}
#	endif
	
	if (!
#		if (CPPDEVTK_DISABLE_UNICODE)
		::RemoveDirectoryA
#		else
		::RemoveDirectoryW
#		endif
			(CPPDEVTK_Q2T(nativePath).c_str())) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		
		if (!failIfNotExists) {
			if ((kErrorCode.GetValue() == ERROR_FILE_NOT_FOUND) /*|| (kErrorCode.GetValue() == ERROR_PATH_NOT_FOUND)*/) {
				return;
			}
		}
		
		if ((kErrorCode.GetValue() == ERROR_FILE_NOT_FOUND) /*|| (kErrorCode.GetValue() == ERROR_PATH_NOT_FOUND)*/) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(path);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "failed to remove directory", path);
		}
	}
}

CPPDEVTK_UTIL_API QStringList GetMountPointsFromPath(const QString& path) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((IsValidPath(path)), "path must be valid");
	
	QStringList mountPoints;
	
	const QString kAbsolutePath = QFileInfo(path).absolutePath();

	const QFileInfoList kFileInfoList = QDir::drives();
	for (QFileInfoList::ConstIterator kIter = kFileInfoList.constBegin(); kIter != kFileInfoList.constEnd(); ++kIter) {
		const QString kDrive = kIter->path().toUpper();
		CPPDEVTK_ASSERT((kDrive.length() == 3) && kDrive[0].isLetter() && (kDrive[1] == ':') && (kDrive[2] == '/'));
		if (kAbsolutePath.startsWith(kDrive, Qt::CaseInsensitive)) {
			mountPoints.append(kDrive);
			break;
		}
	}
	
	CPPDEVTK_ASSERT(mountPoints.isEmpty() || (mountPoints.size() == 1));
	
	return mountPoints;
}

CPPDEVTK_UTIL_API void GetFileSystemSpaceInfo(const QString& path, FileSystemSpaceInfo& fileSystemSpaceInfo) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((IsValidPath(path)), "path must be valid");
	
	ULARGE_INTEGER freeBytesAvailable;
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;
	
	QString nativeDirAbsPath = QDir::toNativeSeparators(QFileInfo(path).absolutePath());
#	if (!CPPDEVTK_DISABLE_UNICODE)
	nativeDirAbsPath.prepend("\\\\?\\");
#	endif
	
	if (!GetDiskFreeSpaceEx(CPPDEVTK_Q2T(nativeDirAbsPath).c_str(), &freeBytesAvailable, &totalNumberOfBytes,
			&totalNumberOfFreeBytes)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		if ((kErrorCode.GetValue() == ERROR_FILE_NOT_FOUND) || (kErrorCode.GetValue() == ERROR_PATH_NOT_FOUND)) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(path);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(kErrorCode, "GetDiskFreeSpaceEx() failed", path);
		}
	}
	
	fileSystemSpaceInfo.size_ = totalNumberOfBytes.QuadPart;
	fileSystemSpaceInfo.totalFree_ = totalNumberOfFreeBytes.QuadPart;
	fileSystemSpaceInfo.userFree_ = freeBytesAvailable.QuadPart;
	
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.size_ > 0);
#	if 0	// FileSystemSpaceInfo::size_type is unsigned
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.totalFree_ >= 0);
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.userFree_ >= 0);
#	endif
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.size_ >= fileSystemSpaceInfo.totalFree_);
	CPPDEVTK_ASSERT(fileSystemSpaceInfo.totalFree_ >= fileSystemSpaceInfo.userFree_);
}




CPPDEVTK_UTIL_API QStringList GetMountPoints(bool ignoreSpecialFileSystems) {
	base::SuppressUnusedWarning(ignoreSpecialFileSystems);
	
	QStringList mountPoints;

	const QFileInfoList kFileInfoList = QDir::drives();
	for (QFileInfoList::ConstIterator kIter = kFileInfoList.constBegin(); kIter != kFileInfoList.constEnd(); ++kIter) {
		const QString kDrive = kIter->path().toUpper();
		CPPDEVTK_ASSERT((kDrive.length() == 3) && kDrive[0].isLetter() && (kDrive[1] == ':') && (kDrive[2] == '/'));
		mountPoints.append(kDrive);
	}
	
	return mountPoints;
}


}	// namespace util
}	// namespace cppdevtk
