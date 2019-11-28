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


#ifndef CPPDEVTK_BASE_TCHAR_H_INCLUDED_
#define CPPDEVTK_BASE_TCHAR_H_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <tchar.h>
#endif
#include <ctype.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>
#include <stdio.h>


#define CPPDEVTK_T(str) CPPDEVTK_DETAIL_T_HELPER(str)

#if (CPPDEVTK_DISABLE_UNICODE || CPPDEVTK_DISABLE_WIDE)
#	define CPPDEVTK_DETAIL_T_HELPER(str) str
#else
#	define CPPDEVTK_DETAIL_T_HELPER(str) L ## str
#endif


// TODO: add more T and U functions for (wide) char/string manipulation


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T

#if (CPPDEVTK_DISABLE_UNICODE || CPPDEVTK_DISABLE_WIDE)
#	define CPPDEVTK_TPRINTF printf
#	define CPPDEVTK_TCSCPY(dest, src) strcpy(dest, src)
#	define CPPDEVTK_TCSCAT(dest, src) strcat(dest, src)
#	define CPPDEVTK_TCSLEN(str) strlen(str)
#	define CPPDEVTK_TCSDUP(str) strdup(str)
#else	// T == wide (UTF-32 on Unix and UTF-16 on Windows)
#	define CPPDEVTK_TPRINTF wprintf
#	define CPPDEVTK_TCSCPY(dest, src) wcscpy(dest, src)
#	define CPPDEVTK_TCSCAT(dest, src) wcscat(dest, src)
#	define CPPDEVTK_TCSLEN(str) wcslen(str)
#	if (CPPDEVTK_PLATFORM_UNIX)
#		define CPPDEVTK_TCSDUP(str) wcsdup(str)
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
#		define CPPDEVTK_TCSDUP(str) _wcsdup(str)
#	else
#		error "Unsupported platform!!!"
#	endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// U

#if (CPPDEVTK_DISABLE_UNICODE || CPPDEVTK_PLATFORM_UNIX)
#	define CPPDEVTK_UPRINTF printf
#	define CPPDEVTK_UCSCPY(dest, src) strcpy(dest, src)
#	define CPPDEVTK_UCSCAT(dest, src) strcat(dest, src)
#	define CPPDEVTK_UCSLEN(str) strlen(str)
#	define CPPDEVTK_UCSDUP(str) strdup(str)
#elif (CPPDEVTK_PLATFORM_WINDOWS)	// U == wide (UTF-16)
#	define CPPDEVTK_UPRINTF wprintf
#	define CPPDEVTK_UCSCPY(dest, src) wcscpy(dest, src)
#	define CPPDEVTK_UCSCAT(dest, src) wcscat(dest, src)
#	define CPPDEVTK_UCSLEN(str) wcslen(str)
#	define CPPDEVTK_UCSDUP(str) _wcsdup(str)
#else
#	error "Unsupported platform!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tchar

// sizeof(wchar_t):
// - Linux, Mac OS X: 4 (UTF-32)
// - Windows: 2	(UCS-2, UTF-16)

#if (CPPDEVTK_DISABLE_UNICODE || CPPDEVTK_DISABLE_WIDE)
typedef char tchar;
#else
typedef wchar_t tchar;
#endif


#endif	// CPPDEVTK_BASE_TCHAR_H_INCLUDED_
