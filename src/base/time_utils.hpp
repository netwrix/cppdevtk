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


#ifndef CPPDEVTK_BASE_TIME_UTILS_HPP_INCLUDED_
#define CPPDEVTK_BASE_TIME_UTILS_HPP_INCLUDED_


#if (defined(__APPLE__) && defined(__MACH__))
#	undef _DARWIN_FEATURE_CLOCK_GETTIME
#	define _DARWIN_FEATURE_CLOCK_GETTIME 0
#endif
#include <cppdevtk/base/config.hpp>


#if (CPPDEVTK_PLATFORM_MACOSX)
#include <time.h>
#endif

#if (CPPDEVTK_HAVE_PTHREADS)
#include <ctime>
#endif


#if (CPPDEVTK_PLATFORM_MACOSX)


typedef enum cppdevtk_clockid {
	cid_realtime = 0
} cppdevtk_clockid_t;

#ifdef CLOCK_REALTIME
#	error "CLOCK_REALTIME defined; please make sure this header is included before ctime/time.h"
#endif
#define CLOCK_REALTIME cid_realtime


#ifdef __cplusplus
extern "C" {
#endif


int cppdevtk_clock_gettime(cppdevtk_clockid_t clk_id, struct timespec* tp);

#define clock_gettime(clk_id, tp) cppdevtk_clock_gettime((clk_id), (tp))


#ifdef __cplusplus
}
#endif


#endif	// CPPDEVTK_PLATFORM_MACOSX


namespace cppdevtk {
namespace base {
namespace detail {


#if (CPPDEVTK_HAVE_PTHREADS)


timespec RelTimeToAbsTime(int relTime);


#endif


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_TIME_UTILS_HPP_INCLUDED_
