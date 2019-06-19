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


#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QCoreApplication>
#include <QtCore/QMessageLogContext>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#endif

#include <cstddef>
#include <cstdlib>


namespace cppdevtk {
namespace base {
namespace detail {


static void LogFileMsgHandler(QtMsgType msgType,
#		if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
		const char* msg
#		else
		const QMessageLogContext& msgLogCtx, const QString& msg
#		endif
);


Q_GLOBAL_STATIC(QFile, gLogFile);
Q_GLOBAL_STATIC(QTextStream, gLogTextStream);


}	// namespace detail


CPPDEVTK_BASE_API QString GetLogFileName() {
	const QStringList kTempPaths = QStringList() << QDir::tempPath()
			<< QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
#			if (CPPDEVTK_PLATFORM_ANDROID)
			<< "/sdcard" << "/mnt/sdcard" << "/storage/sdcard0" << "/storage/emulated/0" << "/storage/emulated/legacy"
#			endif
	;
	
	QString logFileName;
	
	for (QStringList::ConstIterator kIter = kTempPaths.constBegin(); kIter != kTempPaths.constEnd(); ++kIter) {
		QFileInfo tempFileInfo(*kIter);
		if (!tempFileInfo.exists()) {
			continue;
		}
		if (!tempFileInfo.isDir()) {
			continue;
		}
		if (!tempFileInfo.isWritable()) {
			continue;
		}
		
		//qcout << "found writeable temp path: '" << *kIter << "'" << endl;
		logFileName = *kIter;
		break;
	}
	
	if (!logFileName.isEmpty() && !logFileName.endsWith('/')) {
		logFileName += '/';
	}
	logFileName += "cppdevtk_" + QString::number(QCoreApplication::applicationPid());
	
	return logFileName;
}

CPPDEVTK_BASE_API bool InstallLogFileMsgHandler(const QString& logFileName) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(logFileName.isEmpty(), "logFileName");
	
	QFile* pLogFile = detail::gLogFile;
	if (pLogFile == NULL) {
		return false;
	}
	
	QTextStream* pLogTextStream = detail::gLogTextStream;
	if (pLogTextStream == NULL) {
		return false;
	}
	
	
	if (pLogTextStream->device() != NULL) {
		pLogTextStream->setDevice(NULL);
	}
	
	if (pLogFile->isOpen()) {
		pLogFile->close();
	}
	
	
	pLogFile->setFileName(logFileName);
	if (!pLogFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		qcerr << "Failed to open log file: " << pLogFile->fileName()
				<< "; errorString: " << pLogFile->errorString() << endl;
		
		return false;
	}
	
	pLogTextStream->setDevice(pLogFile);
	pLogTextStream->setCodec("UTF-8");
	if (pLogTextStream->status() != QTextStream::Ok) {
		qcerr << "Failed to setup logging to file: " << pLogFile->fileName()
				<< "; status: " << pLogTextStream->status() << endl;
		
		pLogTextStream->setDevice(NULL);
		pLogFile->close();
		
		return false;
	}
	
#	if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
	qInstallMsgHandler(detail::LogFileMsgHandler);
#	else
	qInstallMessageHandler(detail::LogFileMsgHandler);
#	endif
	
	qcout << "Logs will be placed in file: " << pLogFile->fileName() << endl;
	
	return true;
}


namespace detail {


static void LogFileMsgHandler(QtMsgType msgType,
#		if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
		const char* msg
#		else
		const QMessageLogContext& msgLogCtx, const QString& msg
#		endif
		) {
#	if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(msg != NULL);
#	else
	SuppressUnusedWarning(msgLogCtx);
#	endif
	
	// qDebug() family is thread safe in Qt 5 but not in Qt 4
#	if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
	static QMutex mutex;
	QMutexLocker mutexLocker(&mutex);
#	endif
	
	QTextStream* pLogTextStream = detail::gLogTextStream;
	if (pLogTextStream == NULL) {
		return;
	}
	if (pLogTextStream->status() != QTextStream::Ok) {
		return;
	}
	
	QString logMsg;
	switch (msgType) {
		case QtDebugMsg:
			logMsg = "[qDebug] ";
			break;
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
		case QtInfoMsg:
			logMsg = "[qInfo] ";
			break;
#		endif
		case QtWarningMsg:
			logMsg = "[qWarning] ";
			break;
		case QtCriticalMsg:
			logMsg = "[qCritical] ";
			break;
		case QtFatalMsg:
			logMsg = "[qFatal] ";
			break;
		default:
			CPPDEVTK_ASSERT(0 && "Invalid message type");
			logMsg = "[qInvalid] ";
			break;
	}
	
	logMsg += msg;
	
	*pLogTextStream << logMsg << endl;
	
	if (msgType == QtFatalMsg) {
#		if (CPPDEVTK_PLATFORM_WINDOWS)
		::std::exit(EXIT_FAILURE);
#		elif (CPPDEVTK_PLATFORM_UNIX)
		::std::abort();	// util dump
#		else
#		error "Only Unix and Windows are supported!!!"
#		endif
	}
}


ScopeLogger::ScopeLogger(const QString& scopeName, const SourceCodeInfo& sourceCodeInfo): NonCopyable(),
		kScopeName_(scopeName), kSourceCodeInfo_(sourceCodeInfo) {
#if (!CPPDEVTK_ENABLE_LOG)
#undef CPPDEVTK_ENABLE_LOG
#define CPPDEVTK_ENABLE_LOG 1
#define CPPDEVTK_DISABLE_ENABLE_LOG 1
#endif
	CPPDEVTK_LOG("ENTERING_SCOPE: " << kScopeName_ << " [" << kSourceCodeInfo_.ToString() << "]", llTrace);
#if (CPPDEVTK_DISABLE_ENABLE_LOG)
#undef CPPDEVTK_ENABLE_LOG
#define CPPDEVTK_ENABLE_LOG 0
#endif
#undef CPPDEVTK_DISABLE_ENABLE_LOG
}

ScopeLogger::~ScopeLogger() {
#if (!CPPDEVTK_ENABLE_LOG)
#undef CPPDEVTK_ENABLE_LOG
#define CPPDEVTK_ENABLE_LOG 1
#define CPPDEVTK_DISABLE_ENABLE_LOG 1
#endif
	CPPDEVTK_LOG("LEAVING_SCOPE: " << kScopeName_ << " [" << kSourceCodeInfo_.ToString() << "]", llTrace);
#if (CPPDEVTK_DISABLE_ENABLE_LOG)
#undef CPPDEVTK_ENABLE_LOG
#define CPPDEVTK_ENABLE_LOG 0
#endif
#undef CPPDEVTK_DISABLE_ENABLE_LOG
}


CPPDEVTK_BASE_API QString LogLevelToString(LogLevel logLevel) {
	CPPDEVTK_ASSERT(((llFirst <= logLevel) && (logLevel <= llLast)) && "logLevel has invalid enum value (UB)");
	
	QString logLevelName;
	
	switch (logLevel) {
		case llTrace:
			logLevelName = "TRACE";
			break;
		case llDebug:
			logLevelName = "DEBUG";
			break;
		case llInfo:
			logLevelName = "INFO";
			break;
		case llWarn:
			logLevelName = "WARN";
			break;
		case llError:
			logLevelName = "ERROR";
			break;
		case llFatal:
			logLevelName = "FATAL";
			break;
		default:
			logLevelName = "INVALID_LOG_LEVEL";
			break;
	}
	
	CPPDEVTK_ASSERT(!logLevelName.isEmpty());
	return logLevelName;
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
