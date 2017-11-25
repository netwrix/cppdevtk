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


#ifndef CPPDEVTK_BASE_THREAD_DATA_TYPES_HPP_INCLUDED_
#define CPPDEVTK_BASE_THREAD_DATA_TYPES_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_PLATFORM_UNIX)
#	include <pthread.h>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include "cstdint.hpp"
#else
#	error "Unsupported platform!!!"
#endif

#include CPPDEVTK_TR1_HEADER(functional)


namespace cppdevtk {
namespace base {
namespace detail {


#if (CPPDEVTK_PLATFORM_UNIX)
typedef pthread_t ThreadNativeHandleType;
/// \note POSIX: \c pthread_t is not required to be arithmetic types, thus allowing pthread_t to be defined as a structure.
/// On all POSIX platforms we support \c pthread_t is arithmetic so use it to simplify things.
/// \sa <a http://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html">POSIX Issue 6 pthread_t</a>
typedef pthread_t ThreadNativeIdType;
#elif (CPPDEVTK_PLATFORM_WINDOWS)
typedef uintptr_t ThreadNativeHandleType;
typedef unsigned ThreadNativeIdType;
#endif

/// \note
/// - Return type is \c int same as \c main()
/// - A generic ReturnType will transform \c Thread class in template because it is needed by constructor and join
/// and this is not acceptable.
typedef ::std::function<int ()> ThreadMainFunctionType;


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_THREAD_DATA_TYPES_HPP_INCLUDED_
