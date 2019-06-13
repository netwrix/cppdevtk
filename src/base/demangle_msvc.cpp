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


#include <cppdevtk/base/demangle.hpp>
#if (!CPPDEVTK_COMPILER_MSVC)
#	error "This file is msvc specific!!!"
#endif

#include <cppdevtk/base/string_conv.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QString>

#include CPPDEVTK_TR1_HEADER(memory)
#include <cstddef>
#include <cstdlib>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTE:
// __unDName() is not documented but documented UnDecorateSymbolName() is not good for us...
// TODO: UnDecorateSymbolName() is not thread safe (as all DbgHelp functions), but __unDName() being from CRT is? Test!
// __unDName() taken from Wine
// Please see:
// https://source.winehq.org/git/wine.git/blob/HEAD:/dlls/msvcrt/msvcrt.h
// https://source.winehq.org/git/wine.git/blob/HEAD:/dlls/msvcrt/undname.c


/*
 *  Demangle VC++ symbols into C function prototypes
 *
 *  Copyright 2000 Jon Griffiths
 *            2004 Eric Pouech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */


/* __unDName/__unDNameEx flags */
#define UNDNAME_COMPLETE                 (0x0000)
#define UNDNAME_NO_LEADING_UNDERSCORES   (0x0001) /* Don't show __ in calling convention */
#define UNDNAME_NO_MS_KEYWORDS           (0x0002) /* Don't show calling convention at all */
#define UNDNAME_NO_FUNCTION_RETURNS      (0x0004) /* Don't show function/method return value */
#define UNDNAME_NO_ALLOCATION_MODEL      (0x0008)
#define UNDNAME_NO_ALLOCATION_LANGUAGE   (0x0010)
#define UNDNAME_NO_MS_THISTYPE           (0x0020)
#define UNDNAME_NO_CV_THISTYPE           (0x0040)
#define UNDNAME_NO_THISTYPE              (0x0060)
#define UNDNAME_NO_ACCESS_SPECIFIERS     (0x0080) /* Don't show access specifier (public/protected/private) */
#define UNDNAME_NO_THROW_SIGNATURES      (0x0100)
#define UNDNAME_NO_MEMBER_TYPE           (0x0200) /* Don't show static/virtual specifier */
#define UNDNAME_NO_RETURN_UDT_MODEL      (0x0400)
#define UNDNAME_32_BIT_DECODE            (0x0800)
#define UNDNAME_NAME_ONLY                (0x1000) /* Only report the variable/method name */
#define UNDNAME_NO_ARGUMENTS             (0x2000) /* Don't show method arguments */
#define UNDNAME_NO_SPECIAL_SYMS          (0x4000)
#define UNDNAME_NO_COMPLEX_TYPE          (0x8000)


#ifdef __cplusplus
extern "C" {
#endif


// C4229: anachronism used : modifiers on data are ignored
typedef void* (* /* __cdecl */ malloc_func_t)(size_t);
typedef void (* /* __cdecl */ free_func_t)(void*);


/*********************************************************************
 *		__unDName (MSVCRT.@)
 */
char* __cdecl __unDName(char* buffer, const char* mangled, int buflen,
                      malloc_func_t memget, free_func_t memfree,
                      unsigned short int flags);


#ifdef __cplusplus
}
#endif


// End of code from Wine
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API QString Demangle(const QString& mangledName) {
	if (mangledName.isEmpty() || !(mangledName.startsWith('?') || mangledName.startsWith('_') || mangledName.startsWith('@'))) {
		return "";
	}
	
	QString demangledName;
	
	CPPDEVTK_TR1_NS::shared_ptr<char> pDemangledName(
			__unDName(NULL, Q2A(mangledName).c_str(), 0, &::std::malloc, &::std::free, UNDNAME_COMPLETE),
			&::std::free);
	if (pDemangledName.get() != NULL) {
		demangledName = A2Q(pDemangledName.get());
	}
	
	return demangledName;
}


}	// namespace base
}	// namespace cppdevtk
