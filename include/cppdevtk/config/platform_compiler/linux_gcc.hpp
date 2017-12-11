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


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_HPP_INCLUDED_
#	error "Do not include directly; please include <cppdevtk/config/platform_compiler.hpp> instead!!!"
#endif

#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#if (!CPPDEVTK_COMPILER_GCC)
#	error "This file is GCC specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_LINUX_GCC_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_COMPILER_LINUX_GCC_HPP_INCLUDED_


#include "unix_gcc.hpp"

#include <unistd.h>


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Compiler Features.

#define CPPDEVTK_HAVE_FEATURES_H 1
#if (!CPPDEVTK_PLATFORM_ANDROID)
#	if (((_POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE >= 600)) && !defined(_GNU_SOURCE))
#		define CPPDEVTK_STRERROR_R_CHAR_P 0
#	else
#		define CPPDEVTK_STRERROR_R_CHAR_P 1
#	endif
#else
#	define CPPDEVTK_STRERROR_R_CHAR_P 0
#endif
#if (!CPPDEVTK_PLATFORM_ANDROID)
#	define CPPDEVTK_HAVE_EXECINFO_H 1
#	define CPPDEVTK_HAVE_BACKTRACE 1
#	define CPPDEVTK_HAVE_BACKTRACE_SYMBOLS 1
#else
#	define CPPDEVTK_HAVE_EXECINFO_H 0
#	define CPPDEVTK_HAVE_BACKTRACE 0
#	define CPPDEVTK_HAVE_BACKTRACE_SYMBOLS 0
#endif

#define CPPDEVTK_HAVE_THREAD_STORAGE 1

#define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0
#define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0
#define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 2, 0))
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
#	if (CPPDEVTK_ENABLE_TMPL_EXPL_INST || CPPDEVTK_ENABLE_TSTRING_EXPL_INST || CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST	\
			|| CPPDEVTK_ENABLE_TIOS_EXPL_INST || CPPDEVTK_ENABLE_TISTREAM_EXPL_INST || CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST	\
			|| CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST || CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST)
#		pragma GCC diagnostic ignored "-Wattributes"	// type attributes ignored after type is already defined
#	endif
#endif
#endif

#ifdef __cplusplus
#	ifdef __EXCEPTIONS
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 1
#	else
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 0
#	endif
#else
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 0
#endif


/// \endcond


#if CPPDEVTK_HAVE_FEATURES_H
#	include <features.h>
#else
#	include <limits.h>
#endif
#if (!CPPDEVTK_PLATFORM_ANDROID)
#ifndef __GLIBC__
#	error "This file is glibc specific!!!"
#endif
#endif

#ifdef __cplusplus

// Bug 65473 - Including <ciso646> does not define __GLIBCXX__
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65473
#if 0
#include <ciso646>
#else
#include <bits/c++config.h>
#endif
#ifndef __GLIBCXX__
#	error "This file is libstdc++ specific!!!"
#endif

#endif	// __cplusplus


#endif	// CPPDEVTK_CONFIG_PLATFORM_COMPILER_LINUX_GCC_HPP_INCLUDED_
