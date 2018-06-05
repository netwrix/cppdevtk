/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
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

#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_WINDOWS_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_WINDOWS_HPP_INCLUDED_


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement.

#ifndef RC_INVOKED	// for RC4011: identifier truncated to 'identifier'...

#define CPPDEVTK_DETAIL_WIN32_WINNT_WINXP 0x0501
#define CPPDEVTK_DETAIL_WIN32_WINNT_VISTA 0x0600
#define CPPDEVTK_DETAIL_WIN32_WINNT_WIN7 0x0601

#define CPPDEVTK_DETAIL_WIN32_WINNT CPPDEVTK_DETAIL_WIN32_WINNT_WINXP
#if (CPPDEVTK_WIN32_WINNT < CPPDEVTK_DETAIL_WIN32_WINNT)
#	error "CPPDEVTK_WIN32_WINNT < CPPDEVTK_DETAIL_WIN32_WINNT!!!"
#endif

#endif	// RC_INVOKED


#include <winsdkver.h>


#ifndef RC_INVOKED	// for RC4011: identifier truncated to 'identifier'...

// Safety checks.
#if (_WIN32_WINNT_MAXVER < CPPDEVTK_DETAIL_WIN32_WINNT_WINXP)
#	error "Old SDK: Windows < XP is not supported!!!"
#endif
#if (CPPDEVTK_WIN32_WINNT == CPPDEVTK_DETAIL_WIN32_WINNT_WINXP)
#	if (_WIN32_WINNT_MAXVER > CPPDEVTK_DETAIL_WIN32_WINNT_WIN7)	// SDK > 7.1 A
#		ifndef _USING_V110_SDK71_
#			error "Please select *_xp platform toolset (Win XP and SDK > 7.1 A)!!!"
#		endif
#	endif
#endif

#endif	// RC_INVOKED


#ifdef _WIN32_WINNT
#	if (_WIN32_WINNT < CPPDEVTK_WIN32_WINNT)
#		error "_WIN32_WINNT < CPPDEVTK_WIN32_WINNT!!!"
#	endif
#else
#	if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		define _WIN32_WINNT CPPDEVTK_WIN32_WINNT
#	endif
#endif	// _WIN32_WINNT

#ifdef NTDDI_VERSION
#	if (NTDDI_VERSION < CPPDEVTK_NTDDI_VERSION)
#		error "NTDDI_VERSION < CPPDEVTK_NTDDI_VERSION!!!"
#	endif
#else
#	if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		if (_WIN32_WINNT == CPPDEVTK_WIN32_WINNT)
#			define NTDDI_VERSION CPPDEVTK_NTDDI_VERSION
#		endif
#	endif
#endif	// NTDDI_VERSION

#ifdef WINVER
#	if (WINVER < CPPDEVTK_WINVER)
#		error "WINVER < CPPDEVTK_WINVER!!!"
#	endif
#else
#	if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		if (_WIN32_WINNT == CPPDEVTK_WIN32_WINNT)
#			define WINVER CPPDEVTK_WINVER
#		endif
#	endif
#endif	// WINVER

#ifdef _WIN32_IE
#	if (_WIN32_IE < CPPDEVTK_WIN32_IE)
#		error "_WIN32_IE < CPPDEVTK_WIN32_IE!!!"
#	endif
#else
#	if (CPPDEVTK_ENABLE_PLATFORM_VERSION)
#		if (_WIN32_WINNT == CPPDEVTK_WIN32_WINNT)
#			define _WIN32_IE CPPDEVTK_WIN32_IE
#		endif
#	endif
#endif	// _WIN32_IE

#include <sdkddkver.h>


#ifndef _WIN32_WINNT_WINXP
//#define _WIN32_WINNT_WINXP                0x0501
#error "_WIN32_WINNT_WINXP must be defined"
#endif
#ifndef _WIN32_WINNT_WS03
#define _WIN32_WINNT_WS03                   0x0502
#endif
#ifndef _WIN32_WINNT_WIN6
#define _WIN32_WINNT_WIN6                   0x0600
#endif
#ifndef _WIN32_WINNT_VISTA
#define _WIN32_WINNT_VISTA                  0x0600
#endif
#ifndef _WIN32_WINNT_WS08
#define _WIN32_WINNT_WS08                   0x0600
#endif
#ifndef _WIN32_WINNT_LONGHORN
#define _WIN32_WINNT_LONGHORN               0x0600
#endif
#ifndef _WIN32_WINNT_WIN7
#define _WIN32_WINNT_WIN7                   0x0601
#endif
#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8                   0x0602
#endif
#ifndef _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT_WINBLUE                0x0603
#endif
#ifndef _WIN32_WINNT_WINTHRESHOLD
#define _WIN32_WINNT_WINTHRESHOLD           0x0A00
#endif
#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10                  0x0A00
#endif

#ifndef NTDDI_WINXP
//#define NTDDI_WINXP                       0x05010000
#error "NTDDI_WINXP not defined"
#endif
#ifndef NTDDI_WINXPSP1
#define NTDDI_WINXPSP1                      0x05010100
#endif
#ifndef NTDDI_WINXPSP2
#define NTDDI_WINXPSP2                      0x05010200
#endif
#ifndef NTDDI_WINXPSP3
#define NTDDI_WINXPSP3                      0x05010300
#endif
#ifndef NTDDI_WINXPSP4
#define NTDDI_WINXPSP4                      0x05010400
#endif
#ifndef NTDDI_WS03
#define NTDDI_WS03                          0x05020000
#endif
#ifndef NTDDI_WS03SP1
#define NTDDI_WS03SP1                       0x05020100
#endif
#ifndef NTDDI_WS03SP2
#define NTDDI_WS03SP2                       0x05020200
#endif
#ifndef NTDDI_WS03SP3
#define NTDDI_WS03SP3                       0x05020300
#endif
#ifndef NTDDI_WS03SP4
#define NTDDI_WS03SP4                       0x05020400
#endif
#ifndef NTDDI_WIN6
#define NTDDI_WIN6                          0x06000000
#endif
#ifndef NTDDI_WIN6SP1
#define NTDDI_WIN6SP1                       0x06000100
#endif
#ifndef NTDDI_WIN6SP2
#define NTDDI_WIN6SP2                       0x06000200
#endif
#ifndef NTDDI_WIN6SP3
#define NTDDI_WIN6SP3                       0x06000300
#endif
#ifndef NTDDI_WIN6SP4
#define NTDDI_WIN6SP4                       0x06000400
#endif
#ifndef NTDDI_VISTA
#define NTDDI_VISTA                         NTDDI_WIN6
#endif
#ifndef NTDDI_VISTASP1
#define NTDDI_VISTASP1                      NTDDI_WIN6SP1
#endif
#ifndef NTDDI_VISTASP2
#define NTDDI_VISTASP2                      NTDDI_WIN6SP2
#endif
#ifndef NTDDI_VISTASP3
#define NTDDI_VISTASP3                      NTDDI_WIN6SP3
#endif
#ifndef NTDDI_VISTASP4
#define NTDDI_VISTASP4                      NTDDI_WIN6SP4
#endif
#ifndef NTDDI_LONGHORN
#define NTDDI_LONGHORN						NTDDI_VISTA
#endif
#ifndef NTDDI_WS08
#define NTDDI_WS08                          NTDDI_WIN6SP1
#endif
#ifndef NTDDI_WS08SP2
#define NTDDI_WS08SP2                       NTDDI_WIN6SP2
#endif
#ifndef NTDDI_WS08SP3
#define NTDDI_WS08SP3                       NTDDI_WIN6SP3
#endif
#ifndef NTDDI_WS08SP4
#define NTDDI_WS08SP4                       NTDDI_WIN6SP4
#endif
#ifndef NTDDI_WIN7
#define NTDDI_WIN7                          0x06010000
#endif
#ifndef NTDDI_WIN8
#define NTDDI_WIN8                          0x06020000
#endif
#ifndef NTDDI_WINBLUE
#define NTDDI_WINBLUE                       0x06030000
#endif
#ifndef NTDDI_WINTHRESHOLD
#define NTDDI_WINTHRESHOLD                  0x0A000000
#endif
#ifndef NTDDI_WIN10
#define NTDDI_WIN10                         0x0A000000
#endif
#ifndef NTDDI_WIN10_TH2
#define NTDDI_WIN10_TH2                     0x0A000001
#endif
#ifndef NTDDI_WIN10_RS1
#define NTDDI_WIN10_RS1                     0x0A000002
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other Windows specific defines

#ifdef CPPDEVTK_DETAIL_BUILD
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#endif

#ifndef NOMINMAX
#	define NOMINMAX
#endif

#ifndef STRICT
#	define STRICT
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Features.
#define CPPDEVTK_HAVE_PTHREADS 0
#define CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE 0
#define CPPDEVTK_HAVE_UNISTD_H 0
#define CPPDEVTK_HAVE_SIGACTION 0

#define CPPDEVTK_CHECK_INTERRUPT_REL_TIME 100


// Safety check
#ifndef UNICODE
#	error "UNICODE must be defined (for Qt)!!!"
#endif


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_WINDOWS_HPP_INCLUDED_
