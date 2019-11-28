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


#ifndef CPPDEVTK_BASE_PTHREAD_EXT_H_INCLUDED_
#define CPPDEVTK_BASE_PTHREAD_EXT_H_INCLUDED_


#include <cppdevtk/base/config.hpp>


#if (CPPDEVTK_HAVE_PTHREADS)


#include <pthread.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


#if (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)

/* CPPDEVTK_BASE_API */ int pthread_mutex_timedlock(pthread_mutex_t* mutex, const struct timespec* absTime);

#endif


#ifdef __cplusplus
}
#endif


#endif	// (CPPDEVTK_HAVE_PTHREADS)
#endif	// CPPDEVTK_BASE_PTHREAD_EXT_H_INCLUDED_
