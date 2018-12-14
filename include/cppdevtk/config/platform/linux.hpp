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


#ifndef CPPDEVTK_CONFIG_PLATFORM_HPP_INCLUDED_
#	error "Do not include directly; please include <cppdevtk/config/platform.hpp> instead!!!"
#endif

#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_LINUX_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_LINUX_HPP_INCLUDED_


#include "unix.hpp"
#include <linux/version.h>
#if (CPPDEVTK_PLATFORM_ANDROID)
#	include <android/api-level.h>
#endif


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement.
#	define CPPDEVTK_DETAIL_MIN_KERNEL_VERSION KERNEL_VERSION(2, 6, 32)
#	if (CPPDEVTK_KERNEL_VERSION < CPPDEVTK_DETAIL_MIN_KERNEL_VERSION)
#		error "CPPDEVTK_KERNEL_VERSION < CPPDEVTK_DETAIL_MIN_KERNEL_VERSION!!!"
#	endif

#if (!CPPDEVTK_PLATFORM_ANDROID)
#	if (LINUX_VERSION_CODE < CPPDEVTK_KERNEL_VERSION)
#		error "LINUX_VERSION_CODE < CPPDEVTK_KERNEL_VERSION!!!"
#	endif
#else
#	define CPPDEVTK_DETAIL_MIN_ANDROID_API 9
#	if (CPPDEVTK_ANDROID_API < CPPDEVTK_DETAIL_MIN_ANDROID_API)
#		error "CPPDEVTK_ANDROID_API < CPPDEVTK_DETAIL_MIN_ANDROID_API!!!"
#	endif

#	if (__ANDROID_API__ < CPPDEVTK_ANDROID_API)
#		error "__ANDROID_API__ < CPPDEVTK_ANDROID_API!!!"
#	endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Features.
#define CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE 1
#define CPPDEVTK_HAVE_SIGACTION 1

#define CPPDEVTK_CHECK_INTERRUPT_REL_TIME 25


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_LINUX_HPP_INCLUDED_
