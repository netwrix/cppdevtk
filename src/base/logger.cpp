/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
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
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>


namespace cppdevtk {
namespace base {
namespace detail {


ScopeLogger::ScopeLogger(const QString& scopeName, const SourceCodeInfo& sourceCodeInfo): NonCopyable(),
		kScopeName_(scopeName), kSourceCodeInfo_(sourceCodeInfo) {
#if (!CPPDEVTK_ENABLE_LOG)
#undef CPPDEVTK_ENABLE_LOG
#define CPPDEVTK_ENABLE_LOG 1
#define CPPDEVTK_DISABLE_ENABLE_LOG 1
#endif
	CPPDEVTK_LOG("ENTERING SCOPE: " << kScopeName_ << " [" << kSourceCodeInfo_.ToString() << "]", llTrace);
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
	CPPDEVTK_LOG("LEAVING SCOPE: " << kScopeName_ << " [" << kSourceCodeInfo_.ToString() << "]", llTrace);
#if (CPPDEVTK_DISABLE_ENABLE_LOG)
#undef CPPDEVTK_ENABLE_LOG
#define CPPDEVTK_ENABLE_LOG 0
#endif
#undef CPPDEVTK_DISABLE_ENABLE_LOG
}


CPPDEVTK_BASE_API QString LogLevelToString(LogLevel logLevel) {
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
			CPPDEVTK_DBC_CHECK_IN_RANGE(((llFirst <= logLevel) && (logLevel <= llLast)), "logLevel");
			logLevelName = "INVALID";
			break;
	}
	
	CPPDEVTK_ASSERT(!logLevelName.isEmpty());
	return logLevelName;
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
