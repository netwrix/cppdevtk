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


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_COMPILER_HPP_INCLUDED_


#include "features.hpp"
#include "compiler.hpp"
#include "platform.hpp"


/// \defgroup config_platform_compiler Platform Compiler
/// \ingroup config
/// @{

/// \defgroup config_platform_compiler_shared_library_support Shared Library Support
/// Define CPPDEVTK_EXPORT_API and CPPDEVTK_IMPORT_API properly

/// \defgroup config_platform_compiler_features Platform Compiler Features
/// Define to 1 if feature is available, 0 otherwise.
/// - CPPDEVTK_HAVE_FEATURES_H
/// - CPPDEVTK_HAVE_STRERROR_R
/// - CPPDEVTK_STRERROR_R_CHAR_P
/// - CPPDEVTK_HAVE_EXECINFO_H
/// - CPPDEVTK_HAVE_BACKTRACE
/// - CPPDEVTK_HAVE_BACKTRACE_SYMBOLS
/// - CPPDEVTK_HAVE_THREAD_STORAGE
/// - CPPDEVTK_ENABLE_TSTRING_EXPL_INST
/// - CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST
/// - CPPDEVTK_ENABLE_TIOS_EXPL_INST
/// - CPPDEVTK_ENABLE_TISTREAM_EXPL_INST
/// - CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST
/// - CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST
/// - CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST

// - CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS

/// @}	// config_platform_compiler


/// \cond


#ifndef RC_INVOKED	// for RC4011: identifier truncated to 'identifier'...


#if (CPPDEVTK_PLATFORM_UNIX)
#	if (CPPDEVTK_COMPILER_GCC)
#		include "platform_compiler/unix_gcc.hpp"
#	elif (CPPDEVTK_COMPILER_CLANG)
#		include "platform_compiler/unix_clang.hpp"
#	else
#		error "Unsupported compiler for Unix platform!!!"
#	endif

#	if (CPPDEVTK_PLATFORM_LINUX)
#		if (CPPDEVTK_COMPILER_GCC)
#			include "platform_compiler/linux_gcc.hpp"
#		else
#			error "Unsupported compiler for Linux platform!!!"
#		endif
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#		if (CPPDEVTK_COMPILER_GCC)
#			include "platform_compiler/macosx_gcc.hpp"
#		elif (CPPDEVTK_COMPILER_CLANG)
#			include "platform_compiler/macosx_clang.hpp"
#		else
#			error "Unsupported compiler for Mac OS X platform!!!"
#		endif
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	if (CPPDEVTK_COMPILER_MSVC)
#		include "platform_compiler/windows_msvc.hpp"
#	else
#		error "Unsupported compiler for Windows platform!!!"
#	endif
#else
#	error "Unsupported platform!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety checks.

#ifndef CPPDEVTK_EXPORT_API
#	error "Please define CPPDEVTK_EXPORT_API properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_IMPORT_API
#	error "Please define CPPDEVTK_IMPORT_API properly for current platform/compiler!!!"
#endif

#ifndef CPPDEVTK_HAVE_FEATURES_H
#	error "Please define CPPDEVTK_HAVE_FEATURES_H properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_STRERROR_R
#	error "Please define CPPDEVTK_HAVE_STRERROR_R properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_STRERROR_R_CHAR_P
#	error "Please define CPPDEVTK_STRERROR_R_CHAR_P properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_EXECINFO_H
#	error "Please define CPPDEVTK_HAVE_EXECINFO_H properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_BACKTRACE
#	error "Please define CPPDEVTK_HAVE_BACKTRACE properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_BACKTRACE_SYMBOLS
#	error "Please define CPPDEVTK_HAVE_BACKTRACE_SYMBOLS properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_HAVE_THREAD_STORAGE
#	error "Please define CPPDEVTK_HAVE_THREAD_STORAGE properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TSTRING_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TSTRING_EXPL_INST properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TSTREAMBUF_EXPL_INST properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TIOS_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TIOS_EXPL_INST properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TISTREAM_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TISTREAM_EXPL_INST properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TOSTREAM_EXPL_INST properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TSSTREAM_EXPL_INST properly for current platform/compiler!!!"
#endif
#ifndef CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST
#	error "Please define CPPDEVTK_ENABLE_TFSTREAM_EXPL_INST properly for current platform/compiler!!!"
#endif

#ifndef CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS
#	error "Please define CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS properly for current platform/compiler!!!"
#endif
#ifdef __cplusplus
#	if (!CPPDEVTK_DETAIL_COMPILER_ENABLE_EXCEPTIONS)
#		error "Please enable exceptions for current platform/compiler!!!"
#	endif
#endif


#endif	// RC_INVOKED


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_COMPILER_HPP_INCLUDED_
