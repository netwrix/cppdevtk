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


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include <cppdevtk/gui/session_manager.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/zeroize.hpp>
#include <cppdevtk/base/on_block_exit.hpp>

#include <new>

#include <Reason.h>


namespace cppdevtk {
namespace gui {


using base::ErrorCode;
using base::GetLastSystemErrorCode;


bool SessionManager::Logout() {
	if (!ExitWindows(0, 0)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("ExitWindows() failed; errorCode: " << kErrorCode.ToString());
		return false;
	}
	
	return true;
}

bool SessionManager::Shutdown() {
	if (!ExitWindowsEx(EWX_POWEROFF, (SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("failed to Shutdown; errorCode: " << kErrorCode.ToString());
		return false;
	}
	
	return true;
}

::std::auto_ptr< ::cppdevtk::gui::Session> SessionManager::GetCurrentProcessSession() const {
	return ::std::auto_ptr<Session>(new Session());
}

SessionManager::SessionManager(): QObject(), ::cppdevtk::base::MeyersSingleton<SessionManager>() {
	EnableShutdownPrivilege();
}

SessionManager::~SessionManager() {}

void SessionManager::EnableShutdownPrivilege() {
	HANDLE hToken = INVALID_HANDLE_VALUE;
	if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES /* | TOKEN_QUERY */), &hToken)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
				"OpenProcessToken() failed for current process with TOKEN_ADJUST_PRIVILEGES access");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hToken)) {
		if (!CloseHandle(hToken)) {
			const ErrorCode kErrorCode = GetLastSystemErrorCode();
			CPPDEVTK_LOG_WARN("CloseHandle() failed for current process access token"
					"; errorCode: " << kErrorCode.ToString());
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	TOKEN_PRIVILEGES tokenPrivileges;
	base::Zeroize(&tokenPrivileges, sizeof(tokenPrivileges));
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tokenPrivileges.Privileges[0].Luid)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
				"LookupPrivilegeValue() failed for SE_SHUTDOWN_NAME");
	}
	
	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
				"failed to get SE_SHUTDOWN_NAME privilege; AdjustTokenPrivileges() failed");
	}
	
	const ErrorCode kErrorCode = GetLastSystemErrorCode();
	if (kErrorCode.GetValue() == ERROR_NOT_ALL_ASSIGNED) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(kErrorCode,
				"failed to get SE_SHUTDOWN_NAME privilege; AdjustTokenPrivileges() succeeded"
				" but not all specified privileges adjusted");
	}
}


}	// namespace gui
}	// namespace cppdevtk
