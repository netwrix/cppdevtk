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


#include <cppdevtk/util/sleep.hpp>
#include <cppdevtk/base/cerrno.hpp>
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API void Sleep(sec_t sec) {
	::sleep(sec);
}

CPPDEVTK_UTIL_API void MSleep(msec_t msec) {
	USleep(msec * 1000);
}

CPPDEVTK_UTIL_API void USleep(usec_t usec) {
	if (::usleep(usec) == EINVAL) {
		::sleep(usec / 1000000);
		::usleep(usec % 1000000);
	}
}


}	// namespace util
}	// namespace cppdevtk
