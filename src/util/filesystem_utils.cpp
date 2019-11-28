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
#include <cppdevtk/util/get_user_name.hpp>
#include <cppdevtk/util/language_info.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/static_assert.hpp>
#include <cppdevtk/base/ios.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QTextStream>

#include <utility>
#include <queue>
#include <list>
#include <new>
#include <sys/types.h>


// check large file support; known issuse:
//https://android.googlesource.com/platform/bionic/+/master/docs/32-bit-abi.md
//https://developercommunity.visualstudio.com/content/problem/308714/in-c-header-systypesh-off-t-is-defined-as-32-bit-s.html

#if (CPPDEVTK_COMPILER_GCC || CPPDEVTK_COMPILER_CLANG)
#	if (CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_ARM)
#		ifndef _LARGEFILE_SOURCE
#			error "_LARGEFILE_SOURCE is not defined"
#		endif
#		ifndef _FILE_OFFSET_BITS
#			error "_FILE_OFFSET_BITS is not defined"
#		else
#			if (_FILE_OFFSET_BITS != 64)
#				error "_FILE_OFFSET_BITS != 64"
#			endif
#		endif
#	endif
#endif

#if (CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_ARM)

#if (!((CPPDEVTK_PLATFORM_LINUX && !CPPDEVTK_PLATFORM_ANDROID) || CPPDEVTK_PLATFORM_ANDROID	\
		|| (CPPDEVTK_PLATFORM_MACOSX && !CPPDEVTK_PLATFORM_IOS) || CPPDEVTK_PLATFORM_IOS))
CPPDEVTK_STATIC_ASSERT(sizeof(qint64) == sizeof(::std::streamsize));
#endif
CPPDEVTK_STATIC_ASSERT(sizeof(qint64) == sizeof(::std::streamoff));
#if (!(CPPDEVTK_COMPILER_MSVC || CPPDEVTK_PLATFORM_ANDROID))
CPPDEVTK_STATIC_ASSERT(sizeof(qint64) == sizeof(off_t));
#endif

#else	// (CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_ARM)

CPPDEVTK_STATIC_ASSERT(sizeof(qint64) == sizeof(::std::streamsize));
CPPDEVTK_STATIC_ASSERT(sizeof(qint64) == sizeof(::std::streamoff));
#if (!CPPDEVTK_COMPILER_MSVC)
CPPDEVTK_STATIC_ASSERT(sizeof(qint64) == sizeof(off_t));
#endif

#endif	// (CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_ARM)


namespace cppdevtk {
namespace util {


using ::cppdevtk::base::SystemException;
using ::cppdevtk::base::ErrorCode;
using ::std::bad_alloc;


CPPDEVTK_UTIL_API bool RemoveDirectoryRecursively(const QString& path, bool failIfNotExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	typedef ::std::list<QString> Paths;
	
	
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(path), "path must be valid");
	
	errorCode.Clear();
	
	Paths paths;
	paths.push_back(path);
	for (Paths::iterator iter = paths.begin(); iter != paths.end(); ++iter) {
		const QStringList kPaths = QDir(*iter).entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
		for (QStringList::ConstIterator kIter = kPaths.constBegin(); kIter != kPaths.constEnd(); ++kIter) {
			const QString kPath = *iter + '/' + *kIter;
			paths.push_back(kPath);
		}
	}
	CPPDEVTK_LOG_TRACE("there are " << paths.size() << " directories to remove in (including): " << path);
	
	bool retCode = true;
	
	do {
		const QString kPath = paths.back();
		paths.pop_back();
		
		const QStringList kFiles = QDir(kPath).entryList(QDir::Files | QDir::Hidden | QDir::System);
		CPPDEVTK_LOG_TRACE("there are " << kFiles.size() << " files to delete in directory " << kPath);
		for (QStringList::ConstIterator kIter = kFiles.constBegin(); kIter != kFiles.constEnd(); ++kIter) {
			const QString kFileName = kPath + '/' + *kIter;
			ErrorCode tmpErrorCode;
			if (!DeleteFile(kFileName, false, tmpErrorCode)) {
				errorCode = tmpErrorCode;
				CPPDEVTK_LOG_ERROR("failed to delete file '" << kFileName << "'\nerror code: " << errorCode.ToString());
				retCode = false;
			}
		}
		
		ErrorCode tmpErrorCode;
		if (!RemoveDirectory(kPath, (paths.empty() ? failIfNotExists : false), tmpErrorCode)) {
			errorCode = tmpErrorCode;
			CPPDEVTK_LOG_ERROR("failed to remove directory '" << kPath << "'\nerror code: " << errorCode.ToString());
			retCode = false;
		}
	}
	while (!paths.empty());
	
	return retCode;
}

CPPDEVTK_UTIL_API bool CopyDirectoryRecursively(const QString& srcPath, const QString& dstPath, bool failIfDstPathExists,
		bool failIfFileExists, ::cppdevtk::base::ErrorCode& errorCode) {
	typedef ::std::pair<QString, QString> SrcDstPathPair;
	
	
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(srcPath), "srcPath must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(dstPath), "dstPath must be valid");
	
	errorCode.Clear();
	
	ErrorCode tmpErrorCode;
	if (!MakePath(dstPath, failIfDstPathExists, tmpErrorCode)) {
		errorCode = tmpErrorCode;
		CPPDEVTK_LOG_ERROR("failed to make dstPath: '" << dstPath << "'\nerror code: " << errorCode.ToString());
		return false;
	}
	
	bool retValue = true;
	
	::std::queue<SrcDstPathPair> srcDstPathPairQueue;
	srcDstPathPairQueue.push(::std::make_pair(srcPath, dstPath));
	do {
		const SrcDstPathPair kSrcDstPathPair = srcDstPathPairQueue.front();
		srcDstPathPairQueue.pop();
		
		const QString kSrcPath = kSrcDstPathPair.first;
		QDir srcDir(kSrcPath);
		if (!srcDir.exists()) {
			CPPDEVTK_LOG_ERROR("source directory '" << kSrcPath << "' does not exists");
			errorCode = MakeErrorCode(base::errc::no_such_file_or_directory);
			retValue = false;
			continue;
		}
		
		const QString kDstPath = kSrcDstPathPair.second;
		if (!MakeDirectory(kDstPath, false, tmpErrorCode)) {
			errorCode = tmpErrorCode;
			CPPDEVTK_LOG_ERROR("failed to make destination directory: '" << kDstPath
					<< "'\nerror code: " << errorCode.ToString());
			retValue = false;
			//continue;
		}
		
		const QStringList kSrcFiles = srcDir.entryList(QDir::Files | QDir::Hidden | QDir::System);
		CPPDEVTK_LOG_TRACE("source directory '" << kSrcPath << "' has " << kSrcFiles.size() << " files");
		for (QStringList::ConstIterator kIter = kSrcFiles.constBegin(); kIter != kSrcFiles.constEnd(); ++kIter) {
			const QString kSrcFileName = kSrcPath + '/' + *kIter;
			const QString kDstFileName = kDstPath + '/' + *kIter;
			if (!CopyFile(kSrcFileName, kDstFileName, failIfFileExists, tmpErrorCode)) {
				errorCode = tmpErrorCode;
				CPPDEVTK_LOG_ERROR("failed to copy file: '" << kSrcFileName <<  "' to '" << kDstFileName
						<< "'\nerror code: " << errorCode.ToString());
				retValue = false;
				//continue;
			}
		}
		
		const QStringList kSrcPaths = srcDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
		CPPDEVTK_LOG_TRACE("source directory '" << kSrcPath << "' has " << kSrcPaths.size() << " subdirectories");
		for (QStringList::ConstIterator kIter = kSrcPaths.constBegin(); kIter != kSrcPaths.constEnd(); ++kIter) {
			srcDstPathPairQueue.push(::std::make_pair((kSrcPath + '/' + *kIter), (kDstPath + '/' + *kIter)));
		}
	}
	while (!srcDstPathPairQueue.empty());
	
	return retValue;
}

CPPDEVTK_UTIL_API bool ReadTextFile(const QString& textFilePath, QString& textFileContent) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(textFilePath), "textFilePath must be valid");
	
	QFile textFile(textFilePath);
	if (!textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		CPPDEVTK_LOG_ERROR("Failed to open text file: " << textFile.fileName()
				<< "; error: " << textFile.error() << " (" << textFile.errorString() << ')');
		return false;
	}
	
	QTextStream textStream(&textFile);
	textFileContent = textStream.readAll();
	if (textStream.status() != QTextStream::Ok) {
		CPPDEVTK_LOG_ERROR("Failed to read text file: " << textFile.fileName()
				<< "; textStream.status(): " << textStream.status());
		return false;
	}
	
	return true;
}

CPPDEVTK_UTIL_API QString GetLocalizedFileName(const QString& fileNamePrefix, const QString& fileExt,
		const QLocale& locale) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(fileNamePrefix), "fileNamePrefix must be valid");
	//CPPDEVTK_DBC_CHECK_ARGUMENT(fileNamePrefix.startsWith(":/"), "fileNamePrefix must be a Qt resource");
	
	QString localizedFileName = fileNamePrefix + locale.name() + '.' + fileExt;
	if (QFile::exists(localizedFileName)) {
		return localizedFileName;
	}
	
	localizedFileName = fileNamePrefix + locale.name().split("_", QString::SkipEmptyParts)[0] + '.' + fileExt;
	if (QFile::exists(localizedFileName)) {
		return localizedFileName;
	}
	
	if (locale.name() != LanguageInfo::GetCodeName()) {
		localizedFileName = fileNamePrefix + LanguageInfo::GetCodeName() + '.' + fileExt;
		if (QFile::exists(localizedFileName)) {
			return localizedFileName;
		}
		
		localizedFileName = fileNamePrefix + LanguageInfo::GetCodeName().split("_", QString::SkipEmptyParts)[0] + '.' + fileExt;
		if (QFile::exists(localizedFileName)) {
			return localizedFileName;
		}
	}
	
	localizedFileName = fileNamePrefix + '.' + fileExt;
	if (QFile::exists(localizedFileName)) {
		return localizedFileName;
	}
	
	localizedFileName.clear();
	return localizedFileName;
}




CPPDEVTK_UTIL_API bool DeleteFile(const QString& fileName, bool failIfNotExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	try {
		DeleteFile(fileName, failIfNotExists);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return false;
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
		return false;
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
		return false;
	}
	
	return true;
}

CPPDEVTK_UTIL_API bool CopyFile(const QString& srcFileName, const QString& dstFileName, bool failIfExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	try {
		CopyFile(srcFileName, dstFileName, failIfExists);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return false;
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
		return false;
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
		return false;
	}
	
	return true;
}

CPPDEVTK_UTIL_API bool MakeDirectory(const QString& dirName, bool failIfExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	try {
		MakeDirectory(dirName, failIfExists);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return false;
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
		return false;
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
		return false;
	}
	
	return true;
}

CPPDEVTK_UTIL_API bool MakePath(const QString& dirPath, bool failIfExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	try {
		MakePath(dirPath, failIfExists);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return false;
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
		return false;
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
		return false;
	}
	
	return true;
}

CPPDEVTK_UTIL_API bool RemoveDirectory(const QString& path, bool failIfNotExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	try {
		RemoveDirectory(path, failIfNotExists);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return false;
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
		return false;
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
		return false;
	}
	
	return true;
}

CPPDEVTK_UTIL_API QStringList GetMountPoints(bool ignoreSpecialFileSystems, ::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	QStringList mountPoints;
	try {
		mountPoints = GetMountPoints(ignoreSpecialFileSystems);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
	}
	
	return mountPoints;
}

CPPDEVTK_UTIL_API QStringList GetMountPointsFromPath(const QString& path, ::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	QStringList mountPoints;
	try {
		mountPoints = GetMountPointsFromPath(path);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
	}
	
	return mountPoints;
}

CPPDEVTK_UTIL_API bool GetFileSystemSpaceInfo(const QString& path, FileSystemSpaceInfo& fileSystemSpaceInfo,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode.Clear();
	
	try {
		GetFileSystemSpaceInfo(path, fileSystemSpaceInfo);
	}
	catch (const SystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return false;
	}
	catch (const bad_alloc&) {
		errorCode = MakeErrorCode(base::errc::not_enough_memory);
		return false;
	}
	catch (...) {
		errorCode = MakeErrorCode(base::errc::no_message_available);
		return false;
	}
	
	return true;
}


}	// namespace util
}	// namespace cppdevtk
