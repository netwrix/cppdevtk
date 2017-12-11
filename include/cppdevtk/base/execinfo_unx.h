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


#ifndef CPPDEVTK_BASE_EXECINFO_UNX_H_INCLUDED_
#define CPPDEVTK_BASE_EXECINFO_UNX_H_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif

#if (CPPDEVTK_HAVE_EXECINFO_H)
#include <execinfo.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


#if (!CPPDEVTK_HAVE_BACKTRACE)

CPPDEVTK_BASE_API int backtrace(void** buffer, int size);

#endif	// (!CPPDEVTK_HAVE_BACKTRACE)


#if (!CPPDEVTK_HAVE_BACKTRACE_SYMBOLS)

CPPDEVTK_BASE_API char** backtrace_symbols(void* const* buffer, int size);
CPPDEVTK_BASE_API void backtrace_symbols_fd(void* const* buffer, int size, int fd);

#endif	// !CPPDEVTK_HAVE_BACKTRACE_SYMBOLS)


#ifdef __cplusplus
}
#endif


#endif	// CPPDEVTK_BASE_EXECINFO_UNX_H_INCLUDED_
