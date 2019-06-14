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
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/zeroize.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#if (CPPDEVTK_PLATFORM_MACOSX)
#include <uuid/uuid.h>
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#include <android/api-level.h>
#endif

#include <vector>
#include <cstddef>


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API QString GetUserName() {
#	if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 21))
	
	// there is no getlogin_r() on Mac OS X, only getlogin()
	// so we use getuid() + getpwuid_r() that works on both Linux and Mac OS X
	
	QString userName;
	
	const uid_t kUid = getuid();
	
	struct passwd pwd;
	//base::Zeroize(&pwd, sizeof(pwd));
	
	long bufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufSize == -1) {
		CPPDEVTK_LOG_WARN("failed to autodetect bufSize");
		bufSize = 1024;
	}
	::std::vector<char> buf(bufSize);
	
	struct passwd* pResult = NULL;
	
	int retCode = ESUCCESS;
	do {
		CPPDEVTK_ASSERT((::std::size_t)bufSize == buf.size());
		
		retCode = getpwuid_r(kUid, &pwd, &buf[0], bufSize, &pResult);
		if (retCode == ERANGE) {
			bufSize *= 2;
			CPPDEVTK_LOG_DEBUG("getpwuid_r() failed; buffer too small; increasing from " << buf.size() << " to " << bufSize);
			buf.resize(bufSize);
		}
	}
	while ((retCode == EINTR) || (retCode == ERANGE));
	
	if (retCode != ESUCCESS) {
		CPPDEVTK_LOG_ERROR("getpwuid_r() failed; error code: "
				<< ::cppdevtk::base::ErrorCode(retCode, ::cppdevtk::base::SystemCategoryRef()).ToString());
		return userName;
	}
	if (pResult == NULL) {
		CPPDEVTK_LOG_ERROR("getpwuid_r() failed; no matching password record was found");
		return userName;
	}
	
	CPPDEVTK_ASSERT(pwd.pw_name != NULL);
	userName = CPPDEVTK_U2Q(pwd.pw_name);
	
	return userName;
	
#	else
	// TODO: Android port for API level < 21
	
	CPPDEVTK_COMPILER_WARNING("GetUserName(): Not working on Android < 5.0.1 (API level < 21)!");
	return QString("");
#	endif
}


}	// namespace util
}	// namespace cppdevtk
