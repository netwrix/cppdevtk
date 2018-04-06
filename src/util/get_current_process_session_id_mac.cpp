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
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_IOS)
#	error "This file is not for iOS!!!"
#endif
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/stdexcept.hpp>

#include <Security/AuthSession.h>


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API QString GetCurrentProcessSessionId() {
	SecuritySessionId securitySessionId;
	SessionAttributeBits sessionAttributeBits;
	const OSStatus kSessionsApiRetCode = SessionGetInfo(callerSecuritySession, &securitySessionId, &sessionAttributeBits);
	if (kSessionsApiRetCode != errSessionSuccess) {
		if (kSessionsApiRetCode == errSessionAuthorizationDenied) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("SessionGetInfo() failed: errSessionAuthorizationDenied");
		}
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("SessionGetInfo() failed; kSessionsApiRetCode:: %1").arg(kSessionsApiRetCode));
	}
	return QString::number(securitySessionId);
}


}	// namespace util
}	// namespace cppdevtk
