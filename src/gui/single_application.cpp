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


#include <cppdevtk/gui/single_application.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>


namespace cppdevtk {
namespace gui {


SingleApplication::SingleApplication(int& argc, char** argv):
		::cppdevtk::qtsol::QtSingleApplication(GetUniqueId(), argc, argv), ApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "single_application");
	
	SetCurrentLanguageInfo(GetDefaultLanguageInfo());
}

SingleApplication::SingleApplication(int& argc, char** argv, const QString& id):
		::cppdevtk::qtsol::QtSingleApplication(id, argc, argv), ApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(id.isEmpty(), "id");
	
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "single_application");
	
	SetCurrentLanguageInfo(GetDefaultLanguageInfo());
}

SingleApplication::~SingleApplication() {
	 CPPDEVTK_LOG_TRACE_FUNCTION();
}

void SingleApplication::SetNotifyThrowAction(int notifyThrowAction) {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((ntaFirst <= notifyThrowAction) && (notifyThrowAction <= ntaLast)),
			"notifyThrowAction is not valid NotifyThrowAction enum value");
	
	ApplicationBase::SetNotifyThrowAction(static_cast<NotifyThrowAction>(notifyThrowAction));
}


}	// namespace gui
}	// namespace cppdevtk
