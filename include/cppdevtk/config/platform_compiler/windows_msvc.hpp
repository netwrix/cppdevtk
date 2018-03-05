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

#ifdef RC_INVOKED
#	error "Internal error: This file should not be included when RC_INVOKED!!!"
#endif

#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#if (!CPPDEVTK_COMPILER_MSVC)
#	error "This file is MSVC specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_WINDOWS_MSVC_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_COMPILER_WINDOWS_MSVC_HPP_INCLUDED_


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared library export/import.
#define CPPDEVTK_EXPORT_API __declspec(dllexport)
#define CPPDEVTK_IMPORT_API __declspec(dllimport)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Compiler Features.

#define CPPDEVTK_HAVE_FEATURES_H 0
#define CPPDEVTK_HAVE_STRERROR_R 0
#define CPPDEVTK_STRERROR_R_CHAR_P 0
#define CPPDEVTK_HAVE_EXECINFO_H 0
#define CPPDEVTK_HAVE_BACKTRACE 0
#define CPPDEVTK_HAVE_BACKTRACE_SYMBOLS 0

#define CPPDEVTK_HAVE_THREAD_STORAGE 1

#if (CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL)
#	ifdef CPPDEVTK_SHARED
#		if (_MSC_VER == 1500)	// MSVC 2008 (9)
#			define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#			define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0
#		elif (_MSC_VER == 1600)	// MSVC 2010 (10) TODO: test
#			define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#			define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0	// 1
#		elif (_MSC_VER == 1700)	// MSVC 2012 (11) TODO: test
#			define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#			define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0	// 1
#		elif (_MSC_VER == 1800)	// MSVC 2013 (12) TODO: test
#			define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#			define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0	// 1
#		elif (_MSC_VER >= 1900)	// MSVC >= 2015 (14)
#			define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#			define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#			define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0	// 1
#			define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0	// 1
#		endif
#	else	// CPPDEVTK_SHARED
#		define CPPDEVTK_ENABLE_TMPL_EXPL_INST 1
#		define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0
#		define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#		define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#		define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#		define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#		define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0
#		define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0
#	endif
#else	// CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL
#	define CPPDEVTK_ENABLE_TMPL_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TSTRING_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TIOS_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST 0
#	define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST 0
#endif

#ifdef __cplusplus
#	ifdef _CPPUNWIND
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 1
#	else
#		define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 0
#	endif
#else
#	define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS 0
#endif


/// \endcond


// Safety checks.

#if ((defined(UNICODE) && !defined(_UNICODE)) || (!defined(UNICODE) && defined(_UNICODE)))
#	error "Both UNICODE and _UNICODE must be (un)defined!!!"
#endif
#if (!(defined(UNICODE) && defined(_UNICODE)))
#	error "Both UNICODE and _UNICODE must be defined (for Qt)!!!"
#endif

#ifdef __cplusplus
#	include <ciso646>
#	ifndef _CPPLIB_VER
#		error "This file is Dinkumware (MSVC) specific!!!"
#	endif
#endif


#endif	// CPPDEVTK_CONFIG_PLATFORM_COMPILER_WINDOWS_MSVC_HPP_INCLUDED_
