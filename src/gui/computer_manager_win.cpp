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


#include <cppdevtk/gui/computer_manager.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include <cppdevtk/base/zeroize.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/system_exception.hpp>

#include <QtCore/QLocale>

#include <windows.h>

#include <cstring>


using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;


namespace cppdevtk {
namespace gui {
namespace detail {

static bool EnableShutdownPrivilege();

}	// namespace detail


bool ComputerManager::ShutdownComputer() {
	static bool isShutdownPrivilegeEnabled = false;
	
	
	if (!isShutdownPrivilegeEnabled) {
		isShutdownPrivilegeEnabled = detail::EnableShutdownPrivilege();
		if (!isShutdownPrivilegeEnabled) {
			CPPDEVTK_LOG_ERROR("failed to enable shutdown privilege");
			return false;
		}
	}
	
	if (!ExitWindowsEx(EWX_POWEROFF, (SHTDN_REASON_MAJOR_SYSTEM | SHTDN_REASON_MINOR_SECURITY))) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("failed to ShutdownComputer; error code: " << kErrorCode.ToString());
		return false;
	}
	
	return true;
}

bool ComputerManager::LockComputer() {
	if (!LockWorkStation()) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("failed to LockComputer; error code: " << kErrorCode.ToString());
		return false;
	}
	
	return true;
}

bool ComputerManager::LogoutUser() {
	if (!ExitWindows(0, 0)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("failed to LogoutUser; error code: " << kErrorCode.ToString());
		return false;
	}
	
	return true;
}


namespace detail {


bool EnableShutdownPrivilege() {
	HANDLE hToken = INVALID_HANDLE_VALUE;
	if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES /* | TOKEN_QUERY */), &hToken)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("OpenProcessToken() failed for current process with TOKEN_ADJUST_PRIVILEGES access"
				"; error code: " <<  kErrorCode.ToString());
		return false;
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hToken)) {
		if (!CloseHandle(hToken)) {
			const ErrorCode kErrorCode = GetLastSystemErrorCode();
			CPPDEVTK_LOG_WARN("CloseHandle() failed for current process access token"
					"; error code: " << kErrorCode.ToString());
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	TOKEN_PRIVILEGES tokenPrivileges;
	base::Zeroize(&tokenPrivileges, sizeof(tokenPrivileges));
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tokenPrivileges.Privileges[0].Luid)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("LookupPrivilegeValue() failed for SE_SHUTDOWN_NAME"
				"; error code: " << kErrorCode.ToString());
		return false;
	}
	
	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL)) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("failed to get SE_SHUTDOWN_NAME privilege; AdjustTokenPrivileges() failed"
				"; error code: " << kErrorCode.ToString());
		return false;
	}
	
	const ErrorCode kErrorCode = GetLastSystemErrorCode();
	if (kErrorCode.GetValue() == ERROR_NOT_ALL_ASSIGNED) {
		CPPDEVTK_LOG_ERROR("failed to get SE_SHUTDOWN_NAME privilege; AdjustTokenPrivileges() succeeded"
				" but not all specified privileges adjusted; error code: " << kErrorCode.ToString());
		return false;
	}
	
	CPPDEVTK_LOG_DEBUG("enabled SE_SHUTDOWN_NAME privilege");
	
	return true;
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
