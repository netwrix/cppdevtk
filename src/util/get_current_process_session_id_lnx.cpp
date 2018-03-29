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


#include <cppdevtk/util/get_current_process_session_id.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#if (CPPDEVTK_HAVE_LOGIND)
#include <cppdevtk/util/logind_manager_lnx.hpp>
#include <cppdevtk/util/logind_session_lnx.hpp>
#else
#include <cppdevtk/util/console_kit_manager_lnx.hpp>
#include <cppdevtk/util/console_kit_session_lnx.hpp>
#endif


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API QString GetCurrentProcessSessionId() {
#	if (CPPDEVTK_HAVE_LOGIND)
	return LogindManager::GetInstance().GetCurrentSession()->GetId();
#	else
	return ConsoleKitManager::GetInstance().GetCurrentSession()->GetId().path();
#	endif
}


}	// namespace util
}	// namespace cppdevtk
