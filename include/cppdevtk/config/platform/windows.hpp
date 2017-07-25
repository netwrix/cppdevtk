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

#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_WINDOWS_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_WINDOWS_HPP_INCLUDED_


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement.

#define CPPDEVTK_DETAIL_WIN32_WINNT_WINXP 0x0501
#define CPPDEVTK_DETAIL_WIN32_WINNT_VISTA 0x0600
#define CPPDEVTK_DETAIL_WIN32_WINNT_WIN7 0x0601

#define CPPDEVTK_DETAIL_WIN32_WINNT CPPDEVTK_DETAIL_WIN32_WINNT_WINXP
#if (CPPDEVTK_WIN32_WINNT < CPPDEVTK_DETAIL_WIN32_WINNT_WINXP)
#	error "CPPDEVTK_WIN32_WINNT < CPPDEVTK_DETAIL_WIN32_WINNT_WINXP!!!"
#endif

#include <winsdkver.h>

// Safety checks.
#if (_WIN32_WINNT_MAXVER < CPPDEVTK_DETAIL_WIN32_WINNT_WINXP)
#	error "Old SDK: Windows < XP is not supported!!!"
#endif
#if (CPPDEVTK_WIN32_WINNT < CPPDEVTK_DETAIL_WIN32_WINNT_VISTA)
#	if (_WIN32_WINNT_MAXVER > CPPDEVTK_DETAIL_WIN32_WINNT_WIN7)	// SDK > 7.1 A
#		ifndef _USING_V110_SDK71_
#			error "Please select *_xp platform toolset (Win < Vista and SDK > 7.1 A)!!!"
#		endif
#	endif
#endif


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
#define CPPDEVTK_HAVE_UNISTD_H 0
#define CPPDEVTK_HAVE_SIGACTION 0


// Safety check
#ifndef UNICODE
#	error "UNICODE must be defined (for Qt)!!!"
#endif


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_WINDOWS_HPP_INCLUDED_
