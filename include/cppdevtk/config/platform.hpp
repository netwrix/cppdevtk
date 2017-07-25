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


#ifdef APSTUDIO_INVOKED
#	error "This file is not editable by Microsoft Visual C++!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_HPP_INCLUDED_


#include "features.hpp"
#include "compiler.hpp"


/// \defgroup config_platform Platform
/// \ingroup config
/// @{

/// \defgroup config_platform_supported_platforms Supported Platforms
/// \sa https://sourceforge.net/p/predef/wiki/OperatingSystems
/// @{

/// \defgroup config_platform_supported_platforms_unix Unix
/// @{

/// \brief Unix platform (including Mac OS X).
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_UNIX)
/// // Unix specific code
/// #endif
/// \endcode
#if (defined(__unix__) || defined(__unix))
#	define CPPDEVTK_PLATFORM_UNIX 1
#else
#	define CPPDEVTK_PLATFORM_UNIX 0
#endif

/// @}	// config_platform_supported_platforms_unix


/// \defgroup config_platform_supported_platforms_linux Linux
/// Minimum requirement: kernel >= 2.6.32\n
/// @{

/// \brief Linux platform (is also Unix).
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_LINUX)
/// // Linux specific code
/// #endif
/// \endcode
#if	(defined(__linux__) || defined(linux) || defined(__linux))
#	define CPPDEVTK_PLATFORM_LINUX	1
#else
#	define CPPDEVTK_PLATFORM_LINUX	0
#endif

/// @}	// config_platform_supported_platforms_linux


/// \defgroup config_platform_supported_platforms_android Android
/// Minimum requirement: Android >= 4.0.3 (API level >= 15)\n
/// @{

/// \brief Android platform (is also Linux and Unix).
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_ANDROID)
/// // Android specific code
/// #endif
/// \endcode
#if (defined(ANDROID) || defined(__ANDROID__))
#	define CPPDEVTK_PLATFORM_ANDROID 1
#else
#	define CPPDEVTK_PLATFORM_ANDROID 0
#endif

/// @}	// config_platform_supported_platforms_android

/// \defgroup config_platform_supported_platforms_macosx Mac OS X
/// Minimum requirement: Mac OS X 10.4\n
/// @{

/// \brief Mac OS X platform (is also Unix).
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_MACOSX)
/// // Mac OS X specific code
/// #endif
/// \endcode
#if ((defined(__APPLE__) && defined(__MACH__)) /* && TARGET_OS_MAC */)
#	define CPPDEVTK_PLATFORM_MACOSX 1

	// Mac OS X is Unix
#	ifndef __unix__
#		define __unix__
#	endif
#	ifndef __unix
#		define __unix
#	endif
#else
#	define CPPDEVTK_PLATFORM_MACOSX 0
#endif

/// @}	// config_platform_supported_platforms_macosx


/// \defgroup config_platform_supported_platforms_ios iOS
/// Minimum requirement: iOS >= 6.0\n
/// @{

/// \brief iOS platform (is also Mac OS X and Unix).
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_IOS)
/// // iOS specific code
/// #endif
/// \endcode
#define CPPDEVTK_PLATFORM_IOS 1
#undef CPPDEVTK_PLATFORM_IOS

/// @}	// config_platform_supported_platforms_android


/// \defgroup config_platform_supported_platforms_windows Windows
/// Minimum requirement: Windows XP SP3\n
/// @{

/// \brief Windows platform.
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_WINDOWS)
/// // Windows specific code
/// #endif
/// \endcode
#ifdef _WIN32
#	define CPPDEVTK_PLATFORM_WINDOWS 1
#else
#	define CPPDEVTK_PLATFORM_WINDOWS 0
#endif

/// @}	// config_platform_supported_platforms_windows

/// \brief Current platform.
///
/// Example:
/// \code
/// #if (CPPDEVTK_PLATFORM_UNIX)
/// 	// Unix common code
/// #	if (CPPDEVTK_PLATFORM_LINUX)
/// 		// Linux + Android common code
/// #		if (CPPDEVTK_PLATFORM_ANDROID)
///				// Android specific code
/// #		else
///				// Linux specific code
/// #		endif
/// #	elif (CPPDEVTK_PLATFORM_MACOSX)
/// 		// Mac OS X + iOS common code
/// #		if (CPPDEVTK_PLATFORM_IOS)
///				// iOS specific code
/// #		else
///				// Mac OS X specific code
/// #		endif
/// #	else
/// #		error "Unsupported Unix platform!!!"
/// #	endif
/// #elif (CPPDEVTK_PLATFORM_WINDOWS)
/// 	// Windows specific code
/// #else
/// #	error "Unsupported platform!!!"
/// #endif
/// \endcode

/// @}	// config_platform_supported_platforms


/// \defgroup config_platform_features Platform Features
/// Define to 1 if feature is available, 0 otherwise.
/// - CPPDEVTK_HAVE_PTHREADS
/// - CPPDEVTK_HAVE_UNISTD_H
/// - CPPDEVTK_HAVE_SIGACTION
/// \attention Must be implemented by each \ref config_platform_supported_platforms.


/// @}	// config_platform


/// \cond


#ifndef RC_INVOKED	// for RC4011: identifier truncated to 'identifier'...

#if (CPPDEVTK_PLATFORM_UNIX)
#	include "platform/unix.hpp"
#	if (CPPDEVTK_PLATFORM_LINUX)
#		include "platform/linux.hpp"
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#		include "platform/macosx.hpp"
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include "platform/windows.hpp"
#else
#	error "Unsupported platform!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Safety checks.

#if (!(CPPDEVTK_PLATFORM_LINUX || CPPDEVTK_PLATFORM_ANDROID || CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_IOS	\
		|| CPPDEVTK_PLATFORM_WINDOWS))
#	error "Unsupported platform!!!"
#endif

#if (CPPDEVTK_PLATFORM_LINUX)
#	if (!CPPDEVTK_PLATFORM_UNIX)
#		error "Linux is Unix"
#	endif
#endif

#if (CPPDEVTK_PLATFORM_MACOSX)
#	if (!CPPDEVTK_PLATFORM_UNIX)
#		error "Mac OS X is Unix"
#	endif
#endif

#if (CPPDEVTK_PLATFORM_IOS)
#	if (!CPPDEVTK_PLATFORM_UNIX)
#		error "iOS is Unix"
#	endif
#endif

#if (CPPDEVTK_PLATFORM_ANDROID)
#	if (!CPPDEVTK_PLATFORM_LINUX)
#		error "Android is Linux"
#	endif
#endif

#if (CPPDEVTK_PLATFORM_IOS)
#	if (!CPPDEVTK_PLATFORM_MACOSX)
#		error "iOS is Mac OS X"
#	endif
#endif


#ifndef CPPDEVTK_HAVE_UNISTD_H
#	error "Please define CPPDEVTK_HAVE_UNISTD_H properly for current platform!!!"
#endif
#ifndef CPPDEVTK_HAVE_SIGACTION
#	error "Please define CPPDEVTK_HAVE_SIGACTION properly for current platform!!!"
#endif


#endif	// RC_INVOKED


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_HPP_INCLUDED_
