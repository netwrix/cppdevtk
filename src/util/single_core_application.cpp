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


#include <cppdevtk/util/single_core_application.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>


namespace cppdevtk {
namespace util {


SingleCoreApplication::SingleCoreApplication(int& argc, char** argv):
		::cppdevtk::qtsol::QtSingleCoreApplication(GetUniqueId(), argc, argv), CoreApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	SetCurrentLanguageInfo(GetDefaultLanguageInfo());
}

SingleCoreApplication::SingleCoreApplication(int& argc, char** argv, const QString& id):
		::cppdevtk::qtsol::QtSingleCoreApplication(id, argc, argv), CoreApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(id.isEmpty(), "id");
	
	SetCurrentLanguageInfo(GetDefaultLanguageInfo());
}

SingleCoreApplication::~SingleCoreApplication() {
	 CPPDEVTK_LOG_TRACE_FUNCTION();
}

void SingleCoreApplication::SetNotifyThrowAction(int notifyThrowAction) {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((ntaFirst <= notifyThrowAction) && (notifyThrowAction <= ntaLast)),
			"notifyThrowAction is not valid NotifyThrowAction enum value");
	
	CoreApplicationBase::SetNotifyThrowAction(static_cast<NotifyThrowAction>(notifyThrowAction));
}


}	// namespace util
}	// namespace cppdevtk
