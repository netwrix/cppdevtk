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


#ifndef CPPDEVTK_BASE_LOGGER_HPP_INCLUDED_
#define CPPDEVTK_BASE_LOGGER_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "source_code_info.hpp"
#include "cerrno.hpp"
#include "system_exception.hpp"
#include "cassert.hpp"

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>


#define CPPDEVTK_LOG_TRACE(msg)	\
	CPPDEVTK_LOG(msg, ::cppdevtk::base::llTrace)
#define CPPDEVTK_LOG_DEBUG(msg)	\
	CPPDEVTK_LOG(msg, ::cppdevtk::base::llDebug)
#define CPPDEVTK_LOG_INFO(msg)	\
	CPPDEVTK_LOG(msg, ::cppdevtk::base::llInfo)
#define CPPDEVTK_LOG_WARN(msg)	\
	CPPDEVTK_LOG(msg, ::cppdevtk::base::llWarn)
#define CPPDEVTK_LOG_ERROR(msg)	\
	CPPDEVTK_LOG(msg, ::cppdevtk::base::llError)
#define CPPDEVTK_LOG_FATAL(msg)	\
	CPPDEVTK_LOG(msg, ::cppdevtk::base::llFatal)

#define CPPDEVTK_LOG_TRACE_FUNCTION()	\
	CPPDEVTK_LOG_TRACE_SCOPE(CPPDEVTK_FUNCTION_LONG_NAME)


#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#	define CPPDEVTK_Q_DEBUG() qDebug().nospace().noquote()
#	define CPPDEVTK_Q_WARN() qWarning().nospace().noquote()
#	define CPPDEVTK_Q_ERROR() qCritical().nospace().noquote()
#else
#	define CPPDEVTK_Q_DEBUG() qDebug().nospace()
#	define CPPDEVTK_Q_WARN() qWarning().nospace()
#	define CPPDEVTK_Q_ERROR() qCritical().nospace()
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
#	define CPPDEVTK_Q_INFO() qInfo().nospace().noquote()
#else
#	define CPPDEVTK_Q_INFO() CPPDEVTK_Q_DEBUG()
#endif


#if (CPPDEVTK_ENABLE_LOG)
#	define CPPDEVTK_LOG(logMsg, logLevel)	\
		do {	\
			if (logLevel >= CPPDEVTK_LOG_LEVEL) {	\
				const int kErrNo = errno;	\
				const ::cppdevtk::base::ErrorCode kLastSystemErrorCode = ::cppdevtk::base::GetLastSystemErrorCode();	\
				try {	\
					QString logEntry;	\
					{	\
						QTextStream logTextStream(&logEntry, QIODevice::WriteOnly);	\
						logTextStream << "[" << ::cppdevtk::base::detail::LogLevelToString(logLevel) << "]: " << logMsg	\
								<< " [" << __FILE__ << " : " << __LINE__ << " : " << CPPDEVTK_FUNCTION_LONG_NAME << "] "	\
								<< QDateTime::currentDateTime().toString(CPPDEVTK_LOGGER_DATE_FORMAT);	\
					}	\
					switch (logLevel) {	\
						case ::cppdevtk::base::llTrace:	\
						case ::cppdevtk::base::llDebug:	\
							CPPDEVTK_Q_DEBUG() << logEntry;	\
							break;	\
						case ::cppdevtk::base::llInfo:	\
							CPPDEVTK_Q_INFO() << logEntry;	\
							break;	\
						case ::cppdevtk::base::llWarn:	\
							CPPDEVTK_Q_WARN() << logEntry;	\
							break;	\
						case ::cppdevtk::base::llError:	\
							CPPDEVTK_Q_ERROR() << logEntry;	\
							break;	\
						case ::cppdevtk::base::llFatal:	\
							/* qFatal(::cppdevtk::base::Q2A(logEntry).c_str()); */	\
							CPPDEVTK_Q_ERROR() << logEntry;	\
							break;	\
						default:	\
							/* qFatal("UB: invalid log level enum value!!!"); */	\
							CPPDEVTK_ASSERT(0 && "UB: invalid log level enum value!!!");	\
							break;	\
					}	\
				}	\
				catch (...) {}	\
				::cppdevtk::base::SetLastSystemErrorCode(kLastSystemErrorCode);	\
				errno = kErrNo;	\
			}	\
		}	\
		while (0)
	
#	define CPPDEVTK_LOG_TRACE_SCOPE(scopeName)	\
		::cppdevtk::base::detail::ScopeLogger scopeLogger(scopeName,	\
		CPPDEVTK_SOURCE_CODE_INFO())
#else	// (CPPDEVTK_ENABLE_LOG)
#	define CPPDEVTK_LOG(logMsg, logLevel)	\
		((void)0)
	
#	define CPPDEVTK_LOG_TRACE_SCOPE(scopeName)	\
		((void)0)
#endif	// (CPPDEVTK_ENABLE_LOG)


namespace cppdevtk {
namespace base {


enum LogLevel {
	llFirst,
	llTrace = llFirst,
	llDebug,
	llInfo,
	llWarn,
	llError,
	llFatal,
	llLast = llFatal
};


namespace detail {


class CPPDEVTK_BASE_API ScopeLogger: private NonCopyable {
public:
	ScopeLogger(const QString& scopeName, const SourceCodeInfo& sourceCodeInfo);
	~ScopeLogger();
private:
	const QString kScopeName_;
	const SourceCodeInfo kSourceCodeInfo_;
};


CPPDEVTK_BASE_API QString LogLevelToString(LogLevel logLevel);


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_LOGGER_HPP_INCLUDED_
