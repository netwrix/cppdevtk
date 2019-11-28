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

#undef CPPDEVTK_PLATFORM_IOS
#if (TARGET_OS_IPHONE || TARGET_OS_SIMULATOR)
#	define CPPDEVTK_PLATFORM_IOS 1
#else
#	define CPPDEVTK_PLATFORM_IOS 0
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement.

#if (CPPDEVTK_COMPILER_GCC)
#	if (CPPDEVTK_DISABLE_OLD_OS)
#		define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED 1050	// MAC_OS_X_VERSION_10_5
#	else
#		define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED 1040	// MAC_OS_X_VERSION_10_4
#	endif
#elif (CPPDEVTK_COMPILER_CLANG)
#	if (CPPDEVTK_DISABLE_OLD_OS)
#		define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED 101000	// MAC_OS_X_VERSION_10_10
#	else
#		define CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MIN_REQUIRED 1070	// MAC_OS_X_VERSION_10_7
#	endif
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
//#if (CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED)
//#	error "CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_MAC_OS_X_VERSION_MAX_ALLOWED!!!"
//#endif


#if (CPPDEVTK_DISABLE_OLD_OS)
#	define CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED 100000	// __IPHONE_10_0
#else
#	define CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED 90000	// __IPHONE_9_0
#endif
#if (CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED)
#	error "CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED!!!"
#endif

#define CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED
#if (CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED)
#	error "CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MIN_REQUIRED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED"
#endif
//#if (CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED)
//#	error "CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED > CPPDEVTK_DETAIL_IPHONE_OS_VERSION_MAX_ALLOWED!!!"
//#endif


#if (CPPDEVTK_PLATFORM_IOS)
#	if (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
#			error "__IPHONE_OS_VERSION_MIN_REQUIRED is defined"
#		endif
#		define __IPHONE_OS_VERSION_MIN_REQUIRED CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED
#	else
#		if (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && (__IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED))
#			error "__IPHONE_OS_VERSION_MIN_REQUIRED < CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED!!!"
#		endif
#	endif

#	if (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#			error "__IPHONE_OS_VERSION_MAX_ALLOWED is defined"
#		endif
#		define __IPHONE_OS_VERSION_MAX_ALLOWED CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED
#	else
#		if (defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && (__IPHONE_OS_VERSION_MAX_ALLOWED < CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED))
#			error "__IPHONE_OS_VERSION_MAX_ALLOWED < CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED!!!"
#		endif
#	endif
#else
#	if (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		ifdef MAC_OS_X_VERSION_MIN_REQUIRED
#			error "MAC_OS_X_VERSION_MIN_REQUIRED is defined"
#		endif
#		define MAC_OS_X_VERSION_MIN_REQUIRED CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED
#	else
#		if (defined(MAC_OS_X_VERSION_MIN_REQUIRED) && (MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED))
#			error "MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#		endif
#	endif

#	if (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		ifdef MAC_OS_X_VERSION_MAX_ALLOWED
#			error "MAC_OS_X_VERSION_MAX_ALLOWED is defined"
#		endif
#		define MAC_OS_X_VERSION_MAX_ALLOWED CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED
#	else
#		if (defined(MAC_OS_X_VERSION_MAX_ALLOWED) && (MAC_OS_X_VERSION_MAX_ALLOWED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIREDD))
#			error "MAC_OS_X_VERSION_MAX_ALLOWED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#		endif
#	endif


#	if (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
#			error "__MAC_OS_X_VERSION_MIN_REQUIRED is defined"
#		endif
#		define __MAC_OS_X_VERSION_MIN_REQUIRED CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED
#	else
#		if (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && (__MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED))
#			error "__MAC_OS_X_VERSION_MIN_REQUIRED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#		endif
#	endif

#	if (defined(CPPDEVTK_DETAIL_BUILD) || CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#			error "__MAC_OS_X_VERSION_MAX_ALLOWED is defined"
#		endif
#		define __MAC_OS_X_VERSION_MAX_ALLOWED CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED
#	else
#		if (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && (__MAC_OS_X_VERSION_MAX_ALLOWED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED))
#			error "__MAC_OS_X_VERSION_MAX_ALLOWED < CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED!!!"
#		endif
#	endif
#endif


// NOTE: The Availability.h header is for targeting iOS and for targeting OS X v10.6 and later.
// The older AvailabilityMacros.h header was introduced in OS X v10.2.
#include <AvailabilityMacros.h>
#if (defined(MAC_OS_X_VERSION_10_6) || CPPDEVTK_PLATFORM_IOS)
#	include <Availability.h>
#endif


#if (CPPDEVTK_COMPILER_GCC)
#	if (CPPDEVTK_DISABLE_OLD_OS)
#		ifndef MAC_OS_X_VERSION_10_5
#			error "Mac OS X < 10.5 is unsupported!!!"
#		endif
#	else
#		ifndef MAC_OS_X_VERSION_10_4
#			error "Mac OS X < 10.4 is unsupported!!!"
#		endif
#		ifndef MAC_OS_X_VERSION_10_5
#			define MAC_OS_X_VERSION_10_5 1050
#		endif
#	endif
#endif
#ifndef MAC_OS_X_VERSION_10_6
#	define MAC_OS_X_VERSION_10_6 1060
#endif
#if (CPPDEVTK_COMPILER_CLANG)
#	if (CPPDEVTK_DISABLE_OLD_OS)
#		ifndef MAC_OS_X_VERSION_10_10
#			error "Mac OS X < 10.10 is unsupported!!!"
#		endif
#	else
#		ifndef MAC_OS_X_VERSION_10_7
#			error "Mac OS X < 10.7 is unsupported!!!"
#		endif
#		ifndef MAC_OS_X_VERSION_10_8
#			define MAC_OS_X_VERSION_10_8 1080
#		endif
#		ifndef MAC_OS_X_VERSION_10_9
#			define MAC_OS_X_VERSION_10_9 1090
#		endif
#		ifndef MAC_OS_X_VERSION_10_10
#			define MAC_OS_X_VERSION_10_10 101000
#		endif
#	endif
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
#ifndef MAC_OS_X_VERSION_10_13_1
#	define MAC_OS_X_VERSION_10_13_1 101301
#endif
#ifndef MAC_OS_X_VERSION_10_13_2
#	define MAC_OS_X_VERSION_10_13_2 101302
#endif
#ifndef MAC_OS_X_VERSION_10_13_4
#	define MAC_OS_X_VERSION_10_13_4 101304
#endif
#ifndef MAC_OS_X_VERSION_10_14
#	define MAC_OS_X_VERSION_10_14 101400
#endif
#ifndef MAC_OS_X_VERSION_10_14_1
#	define MAC_OS_X_VERSION_10_14_1 101401
#endif
#ifndef MAC_OS_X_VERSION_10_14_4
#	define MAC_OS_X_VERSION_10_14_4 101404
#endif
#ifndef MAC_OS_X_VERSION_10_14_6
#	define MAC_OS_X_VERSION_10_14_6 101406
#endif
// TODO: keep updated


#if (CPPDEVTK_PLATFORM_IOS)
#	if (CPPDEVTK_DISABLE_OLD_OS)
#		ifndef __IPHONE_10_0
#			error "iOS < 10.0 is unsupported!!!"
#		endif
#	else
#		ifndef __IPHONE_9_0
#			error "iOS < 9.0 is unsupported!!!"
#		endif
#		ifndef __IPHONE_9_1
#			define __IPHONE_9_1 90100
#		endif
#		ifndef __IPHONE_9_2
#			define __IPHONE_9_2 90200
#		endif
#		ifndef __IPHONE_9_3
#			define __IPHONE_9_3 90300
#		endif
#		ifndef __IPHONE_10_0
#			define __IPHONE_10_0 100000
#		endif
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
#	ifndef __IPHONE_11_1
#		define __IPHONE_11_1 110100
#	endif
#	ifndef __IPHONE_11_2
#		define __IPHONE_11_2 110200
#	endif
#	ifndef __IPHONE_11_3
#		define __IPHONE_11_3 110300
#	endif
#	ifndef __IPHONE_11_4
#		define __IPHONE_11_4 110400
#	endif
#	ifndef __IPHONE_12_0
#		define __IPHONE_12_0 120000
#	endif
#	ifndef __IPHONE_12_1
#		define __IPHONE_12_1 120100
#	endif
#	ifndef __IPHONE_12_2
#		define __IPHONE_12_2 120200
#	endif
#	ifndef __IPHONE_12_3
#		define __IPHONE_12_3 120300
#	endif
#	ifndef __IPHONE_12_4
#		define __IPHONE_12_4 120400
#	endif
	// TODO: keep updated
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Features.
#define CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE 0
#define CPPDEVTK_HAVE_SIGACTION 1

#define CPPDEVTK_CHECK_INTERRUPT_REL_TIME 50


#include <unistd.h>


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_MACOSX_HPP_INCLUDED_
