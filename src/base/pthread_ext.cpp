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


#include "time_utils.hpp"
#include "pthread_ext.h"


#if (CPPDEVTK_HAVE_PTHREADS)


#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/system_exception.hpp>

#include <cstddef>
#include <cstdlib>


#if (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)


/* CPPDEVTK_BASE_API */ int pthread_mutex_timedlock(pthread_mutex_t* mutex, const struct timespec* absTime) {
	CPPDEVTK_ASSERT(mutex != NULL);
	
	if (absTime == NULL) {
		return pthread_mutex_lock(mutex);
	}
	
	int retCode = ESUCCESS;
	
	while ((retCode = pthread_mutex_trylock(mutex)) == EBUSY) {
		// EINVAL The  process  or  thread would have blocked, and the abstime parameter specified a nanoseconds
		// field value less than zero or greater than or equal to 1000 million.
		if (/* (absTime->tv_sec < 0) || */ (absTime->tv_nsec < 0) || (absTime->tv_nsec >= 1000000000)) {
			retCode = EINVAL;
			break;
		}
		
		timespec timeSpec = {0, 0};
		retCode = clock_gettime(CLOCK_REALTIME, &timeSpec);
		if (retCode != ESUCCESS) {
			retCode = errno;
			CPPDEVTK_LOG_ERROR("clock_gettime() failed; error code: "
					<< ::cppdevtk::base::MakeSystemErrorCode(retCode).ToString());
			CPPDEVTK_ASSERT(errno != EINTR);
			CPPDEVTK_ASSERT(errno != EINVAL);
			break;
		}
		CPPDEVTK_ASSERT((timeSpec.tv_sec >= 0) && ((timeSpec.tv_nsec >= 0) && (timeSpec.tv_nsec < 1000000000)));
		
		const long kNanoSecToNanoSleep = CPPDEVTK_CHECK_INTERRUPT_REL_TIME * 1000000;
		if ((absTime->tv_sec < timeSpec.tv_sec)
				|| ((absTime->tv_sec == timeSpec.tv_sec) && (absTime->tv_nsec <= (timeSpec.tv_nsec + kNanoSecToNanoSleep)))) {
			retCode = ETIMEDOUT;
			break;
		}
		
		timeSpec.tv_sec = 0;
		timeSpec.tv_nsec = kNanoSecToNanoSleep;
		retCode = nanosleep(&timeSpec, NULL);
		CPPDEVTK_ASSERT((retCode == ESUCCESS) || ((retCode == -1) && (errno != EINVAL)));
	}
	
	CPPDEVTK_ASSERT(retCode != EINTR);
	return retCode;
}

#endif	// (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)


#endif	// (CPPDEVTK_HAVE_PTHREADS)
