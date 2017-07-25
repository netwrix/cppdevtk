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


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_HPP_INCLUDED_
#	error "Do not include directly; please include <cppdevtk/config/platform_compiler.hpp> instead!!!"
#endif

#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif

#if (!CPPDEVTK_COMPILER_CLANG)
#	error "This file is Clang specific!!!"
#endif

#ifndef __apple_build_version__
#	error "This file is Apple Clang specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_MACOSX_CLANG_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_COMPILER_MACOSX_CLANG_HPP_INCLUDED_


#include "unix_clang.hpp"


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Minimum requirement: highest Xcode on Mac OS X 10.7 is 4.6.3:
// clang: Apple LLVM version 4.2 (clang-425.0.28) (based on LLVM 3.2svn)
#if (CPPDEVTK_CLANG_VERSION_NUM < CPPDEVTK_CLANG_VERSION_NUM_FROM_COMPONENTS(4, 2, 0))
#	error "Apple Clang >= 4.2 required!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Compiler Features.

#define CPPDEVTK_HAVE_FEATURES_H 0
#define CPPDEVTK_STRERROR_R_CHAR_P 0

#if (!(defined(MAC_OS_X_VERSION_MIN_REQUIRED) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)))
#	error "Internal error!!!"	// AvailabilityMacros.h/Availability.h should have been included by platform/macosx.hpp
#endif
#ifndef MAC_OS_X_VERSION_10_5
#	error "Internal error!!!"	// if not defined in AvailabilityMacros.h then defined in platform/macosx.hpp
#endif
#if ((MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
#	define CPPDEVTK_HAVE_EXECINFO_H 1
#	define CPPDEVTK_HAVE_BACKTRACE 1
#	define CPPDEVTK_HAVE_BACKTRACE_SYMBOLS 1
#else
#	define CPPDEVTK_HAVE_EXECINFO_H 0
#	define CPPDEVTK_HAVE_BACKTRACE 0
#	define CPPDEVTK_HAVE_BACKTRACE_SYMBOLS 0
#endif

#define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0
#define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0
#define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0

// Please see Clang Language Extensions, C++ exceptions: http://clang.llvm.org/docs/LanguageExtensions.html
#ifdef __cplusplus
#	if (__has_feature(cxx_exceptions))
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 1
#	else
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 0
#	endif
#else
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 0
#endif


/// \endcond


#ifdef __cplusplus
#	include <ciso646>
#	ifndef _LIBCPP_VERSION
#		error "This file is libc++ specific!!!"
#	endif
#endif


#endif	// CPPDEVTK_CONFIG_PLATFORM_COMPILER_MACOSX_CLANG_HPP_INCLUDED_
