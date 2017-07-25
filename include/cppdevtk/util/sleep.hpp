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


#ifndef CPPDEVTK_UTIL_SLEEP_HPP_INCLUDED_
#define CPPDEVTK_UTIL_SLEEP_HPP_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_UNIX)
#	include <unistd.h>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include <windows.h>
#else
#error "Unsupported platform!!!"
#endif


namespace cppdevtk {
namespace util {


#if (CPPDEVTK_PLATFORM_UNIX)
typedef unsigned int sec_t;
typedef useconds_t msec_t;
typedef useconds_t usec_t;
#elif (CPPDEVTK_PLATFORM_WINDOWS)
typedef DWORD sec_t;
typedef DWORD msec_t;
typedef DWORD usec_t;
#else
#error "Unsupported platform!!!"
#endif


CPPDEVTK_UTIL_API void Sleep(sec_t sec);

CPPDEVTK_UTIL_API void MSleep(msec_t msec);

/// \arg usec_t may be greater than 1000000
CPPDEVTK_UTIL_API void USleep(usec_t usec);


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_SLEEP_HPP_INCLUDED_
