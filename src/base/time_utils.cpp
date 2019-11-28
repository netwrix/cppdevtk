/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#include "time_utils.h"	// must be included first!
#include <cppdevtk/base/time_utils.hpp>

#if (CPPDEVTK_PLATFORM_MACOSX)
#include <cppdevtk/base/errno.h>
#include <cppdevtk/base/assert.h>

#include <sys/time.h>
#include <stddef.h>
#endif

#if (CPPDEVTK_HAVE_PTHREADS)
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#endif

#include <cppdevtk/base/system_exception.hpp>

#include <cstddef>


#if (CPPDEVTK_PLATFORM_MACOSX)

int cppdevtk_clock_gettime(cppdevtk_clockid_t clk_id, struct timespec* tp) {
	int retCode = -1;
	
	CPPDEVTK_ASSERT(tp != NULL);
	
	if (clk_id == CLOCK_REALTIME) {
		timeval tv = {0, 0};
		retCode = gettimeofday(&tv, NULL);
		CPPDEVTK_ASSERT(retCode == ESUCCESS);
		
		tp->tv_sec = tv.tv_sec;
		tp->tv_nsec = tv.tv_usec * 1000;
	}
	else {
		errno = EINVAL;
	}
	
	return retCode;
}

#endif	// CPPDEVTK_PLATFORM_MACOSX


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API ::std::time_t GetCurrentTime() {
	using ::std::time_t;
	
	const time_t kCurrTime = ::std::time(NULL);
	if (kCurrTime == (time_t)-1) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EV_WA(errno, GenericCategoryRef(), "time() failed");
	}
	return kCurrTime;
}


#if (CPPDEVTK_HAVE_PTHREADS)

CPPDEVTK_BASE_API timespec RelTimeToAbsTime(int relTime) {
	timespec absTime;
	
	const int kRetCode = clock_gettime(CLOCK_REALTIME, &absTime);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "clock_gettime() failed");
	}
	
	absTime.tv_sec += relTime / 1000;
	absTime.tv_nsec += (relTime % 1000) * 1000000;
	if (absTime.tv_nsec >= 1000000000) {
		absTime.tv_sec += absTime.tv_nsec / 1000000000;
		absTime.tv_nsec = absTime.tv_nsec % 1000000000;
	}
	
	return absTime;
}

#endif	// (CPPDEVTK_HAVE_PTHREADS)


}	// namespace base
}	// namespace cppdevtk
