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


#ifndef CPPDEVTK_CONFIG_PLATFORM_HPP_INCLUDED_
#	error "Do not include directly; please include <cppdevtk/config/platform.hpp> instead!!!"
#endif

#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_MACOSX_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_MACOSX_HPP_INCLUDED_


#include "unix.hpp"


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Target OS.
#include <TargetConditionals.h>
#if (!TARGET_OS_MAC)
#	error "Target OS is not Mac OS X!!!"
#endif
#if (TARGET_OS_EMBEDDED)
//#	error "TARGET_OS_EMBEDDED is not supported!!!"
#endif

#if (TARGET_OS_IPHONE || TARGET_OS_SIMULATOR)
#	define CPPDEVTK_PLATFORM_IOS 1
#else
#	define CPPDEVTK_PLATFORM_IOS 0
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement.

#if (CPPDEVTK_COMPILER_GCC)
#	define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED 1040	// MAC_OS_X_VERSION_10_4
#elif (CPPDEVTK_COMPILER_CLANG)
#	define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED 1070	// MAC_OS_X_VERSION_10_7
#else
#	error "Unsupported compiler!!!"
#endif
#if (CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED)
#	error "CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#endif

#define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED
#if (CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED > CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED)
#	error "CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED > CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED"
#endif
#if (CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED)
#	error "CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED!!!"
#endif


#define CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED 80000	// __IPHONE_8_0
#if (CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED)
#	error "CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED!!!"
#endif
#define CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED
#if (CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED)
#	error "CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED"
#endif
#if (CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED)
#	error "CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED!!!"
#endif

#if (CPPDEVTK_PLATFORM_IOS)
#	ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
#		if (__IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED)
#			error "__IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED!!!"
#		endif
#	else
#		if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#			define __IPHONE_OS_VERSION_MIN_REQUIRED CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED
#		endif
#	endif

#	ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#		if (__IPHONE_OS_VERSION_MAX_ALLOWED > CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED)
#			ifdef CPPDEVTK_DETAIL_BUILD
#				error "__IPHONE_OS_VERSION_MAX_ALLOWED > CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED!!!"
#			endif
#		endif
#	else
#		if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#			define __IPHONE_OS_VERSION_MAX_ALLOWED CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED
#		endif
#	endif
#else
#	ifdef MAC_OS_X_VERSION_MIN_REQUIRED
#		if (MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED)
#			error "MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#		endif
#	else
#		if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#			define MAC_OS_X_VERSION_MIN_REQUIRED CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED
#		endif
#	endif

#	ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
#		if (__MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED)
#			error "__MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#		endif
#	else
#		if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#			define __MAC_OS_X_VERSION_MIN_REQUIRED CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED
#		endif
#	endif


#	ifdef MAC_OS_X_VERSION_MAX_ALLOWED
#		if (MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED)
#			ifdef CPPDEVTK_DETAIL_BUILD
#				error "MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED!!!"
#			endif
#		endif
#	else
#		if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#			define MAC_OS_X_VERSION_MAX_ALLOWED CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED
#		endif
#	endif

#	ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#		if (__MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED)
#			ifdef CPPDEVTK_DETAIL_BUILD
#				error "__MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED!!!"
#			endif
#		endif
#	else
#		if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#			define __MAC_OS_X_VERSION_MAX_ALLOWED CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED
#		endif
#	endif
#endif


// NOTE: The Availability.h header is for targeting iOS and for targeting OS X v10.6 and later.
// The older AvailabilityMacros.h header was introduced in OS X v10.2.
#if ((__MAC_OS_X_VERSION_MIN_REQUIRED >= 1060) || (CPPDEVTK_PLATFORM_IOS))
#	include <Availability.h>
#endif
#include <AvailabilityMacros.h>


#ifndef MAC_OS_X_VERSION_10_4
#	error "Mac OS X < 10.4 is unsupported!!!"
#endif
#ifndef MAC_OS_X_VERSION_10_5
#	define MAC_OS_X_VERSION_10_5 1050
#endif
#ifndef MAC_OS_X_VERSION_10_6
#	define MAC_OS_X_VERSION_10_6 1060
#endif
#ifndef MAC_OS_X_VERSION_10_7
#	define MAC_OS_X_VERSION_10_7 1070
#endif
#ifndef MAC_OS_X_VERSION_10_8
#	define MAC_OS_X_VERSION_10_8 1080
#endif
#ifndef MAC_OS_X_VERSION_10_9
#	define MAC_OS_X_VERSION_10_9 1090
#endif
#ifndef MAC_OS_X_VERSION_10_10
#	define MAC_OS_X_VERSION_10_10 101000
#endif
#ifndef MAC_OS_X_VERSION_10_10_2
#	define MAC_OS_X_VERSION_10_10_2 101002
#endif
#ifndef MAC_OS_X_VERSION_10_10_3
#	define MAC_OS_X_VERSION_10_10_3 101003
#endif
#ifndef MAC_OS_X_VERSION_10_11
#	define MAC_OS_X_VERSION_10_11 101100
#endif
#ifndef MAC_OS_X_VERSION_10_11_2
#   define MAC_OS_X_VERSION_10_11_2 101102
#endif
#ifndef MAC_OS_X_VERSION_10_11_3
#   define MAC_OS_X_VERSION_10_11_3 101103
#endif
#ifndef MAC_OS_X_VERSION_10_11_4
#   define MAC_OS_X_VERSION_10_11_4 101104
#endif
#ifndef MAC_OS_X_VERSION_10_12
#	define MAC_OS_X_VERSION_10_12 101200
#endif
#ifndef MAC_OS_X_VERSION_10_12_1
#	define MAC_OS_X_VERSION_10_12_1 101201
#endif
#ifndef MAC_OS_X_VERSION_10_12_2
#	define MAC_OS_X_VERSION_10_12_2 101202
#endif
#ifndef MAC_OS_X_VERSION_10_12_4
#	define MAC_OS_X_VERSION_10_12_4 101204
#endif
#ifndef MAC_OS_X_VERSION_10_13
#	define MAC_OS_X_VERSION_10_13 101300
#endif
// TODO: keep updated


#if (CPPDEVTK_PLATFORM_IOS)
#	ifndef __IPHONE_8_0
#		error "iOS < 8.0 is unsupported!!!"
#	endif
#	ifndef __IPHONE_6_1
#		define __IPHONE_6_1 60100
#	endif
#	ifndef __IPHONE_7_0
#		define __IPHONE_7_0 70000
#	endif
#	ifndef __IPHONE_7_1
#		define __IPHONE_7_1 70100
#	endif
#	ifndef __IPHONE_8_0
#		define __IPHONE_8_0 80000
#	endif
#	ifndef __IPHONE_8_1
#		define __IPHONE_8_1 80100
#	endif
#	ifndef __IPHONE_8_2
#		define __IPHONE_8_2 80200
#	endif
#	ifndef __IPHONE_8_3
#		define __IPHONE_8_3 80300
#	endif
#	ifndef __IPHONE_8_4
#		define __IPHONE_8_4 80400
#	endif
#	ifndef __IPHONE_9_0
#		define __IPHONE_9_0 90000
#	endif
#	ifndef __IPHONE_9_1
#		define __IPHONE_9_1 90100
#	endif
#	ifndef __IPHONE_9_2
#		define __IPHONE_9_2 90200
#	endif
#	ifndef __IPHONE_9_3
#		define __IPHONE_9_3 90300
#	endif
#	ifndef __IPHONE_10_0
#		define __IPHONE_10_0 100000
#	endif
#	ifndef __IPHONE_10_1
#		define __IPHONE_10_1 100100
#	endif
#	ifndef __IPHONE_10_2
#		define __IPHONE_10_2 100200
#	endif
#	ifndef __IPHONE_10_3
#		define __IPHONE_10_3 100300
#	endif
#	ifndef __IPHONE_11_0
#		define __IPHONE_11_0 110000
#	endif
	// TODO: keep updated
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Features.
#define CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE 0
#define CPPDEVTK_HAVE_SIGACTION 1

#define CPPDEVTK_CHECK_INTERRUPT_REL_TIME 50


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_MACOSX_HPP_INCLUDED_
