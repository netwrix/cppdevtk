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


#include <cppdevtk/util/get_user_name.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/tstring_conv.hpp>

#include <windows.h>
#include <lmcons.h>


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API QString GetUserName() {
	QString userName;
	
	TCHAR buf[UNLEN + 1];
	DWORD bufSize = sizeof(buf) / sizeof(buf[0]);
	if (!::GetUserName(buf, &bufSize)) {
		const base::ErrorCode kErrorCode = base::GetLastSystemErrorCode();
		CPPDEVTK_ASSERT(kErrorCode.GetValue() != ERROR_INSUFFICIENT_BUFFER);
		CPPDEVTK_LOG_ERROR("GetUserName() failed; error code: " << kErrorCode.ToString());
		
		return userName;
	}
	
	userName = CPPDEVTK_T2Q(buf);
	
	return userName;
}


}	// namespace util
}	// namespace cppdevtk
