/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
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
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>

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


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API bool RemoveDirectoryRecursively(const QString& path, bool failIfNotExists,
		::cppdevtk::base::ErrorCode& errorCode) {
	typedef ::std::list<QString> Paths;
	
	
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(path), "path must be valid");
	
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
			if (!DeleteFile(kFileName, false, errorCode)) {
				CPPDEVTK_LOG_ERROR("failed to delete file '" << kFileName << "'\nerror code: " << errorCode.ToString());
				retCode = false;
			}
		}
		
		if (!RemoveDirectory(kPath, (paths.empty() ? failIfNotExists : false), errorCode)) {
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
	
	if (!MakePath(dstPath, failIfDstPathExists, errorCode)) {
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
		if (!MakeDirectory(kDstPath, false, errorCode)) {
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
			if (!CopyFile(kSrcFileName, kDstFileName, failIfFileExists, errorCode)) {
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
	
	localizedFileName = fileNamePrefix + (locale.name().split("_", QString::SkipEmptyParts))[0] + '.' + fileExt;
	if (QFile::exists(localizedFileName)) {
		return localizedFileName;
	}
	
	if (locale != QLocale(QLocale::English, QLocale::UnitedStates)) {
		localizedFileName = fileNamePrefix + "en_US" + '.' + fileExt;
		if (QFile::exists(localizedFileName)) {
			return localizedFileName;
		}
		
		localizedFileName = fileNamePrefix + "en" + '.' + fileExt;
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


}	// namespace util
}	// namespace cppdevtk
