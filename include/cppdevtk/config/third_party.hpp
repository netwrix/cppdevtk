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


#ifndef CPPDEVTK_CONFIG_THIRD_PARTY_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_THIRD_PARTY_HPP_INCLUDED_


#include "features.hpp"
#include "platform_compiler.hpp"

#ifdef __cplusplus
#	include <boost/version.hpp>
#endif


/// \defgroup config_third_party Third Party
/// \ingroup config
/// @{


/// \defgroup config_third_party_supported_qt Supported Qt
/// - Both Qt 4 and Qt 5 are supported.
/// - The oldest Qt 4 supported is 4.6.2 (CentOS 6.0).
/// - The oldest Qt 5 supported is 5.6.1 (Android)
/// @{

#ifdef __cplusplus

#include <QtCore/QtGlobal>

#if ((QT_VERSION < QT_VERSION_CHECK(4, 0, 0)) || (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)))
#	error "Qt 4 and Qt 5 are supported!!!"
#endif
#if (QT_VERSION < QT_VERSION_CHECK(4, 6, 2))
#	error "The oldest Qt 4 supported is 4.6.2!!!"
#endif
#if ((QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)) && (QT_VERSION < QT_VERSION_CHECK(5, 6, 1)))
#	error "The oldest Qt 5 supported is 5.6.1!!!"
#endif

#if (CPPDEVTK_PLATFORM_ANDROID)
	// keep 5.6.1 until Qt bug #63407 is fixed
	// Please see Qt bug #63407: segfault on Android when exception thrown in slot even if caught in reimplemented Application::notify()
	// https://bugreports.qt.io/browse/QTBUG-63407
#	if (QT_VERSION != QT_VERSION_CHECK(5, 6, 1))
#		error "Qt 5.6.1 required!!!"
#	endif
#endif
#if (CPPDEVTK_PLATFORM_IOS)
	// Qt static lib bug: iphonesimulator builds as iphone in Qt 5.6 >= 5.6.2; fixed in 5.9.1 (did not tested 5.7.x, 5.8.x, 5.9.0)
#	if ((QT_VERSION != QT_VERSION_CHECK(5, 6, 1)) && (QT_VERSION < QT_VERSION_CHECK(5, 9, 1)))
#		error "Qt 5.6.1 or >= 5.9.1 required!!!"
#	endif
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#if (!defined(QT_SHARED) && !defined(QT_STATIC))
#	error "Both QT_SHARED and QT_STATIC are not defined. Please check your Qt configuration!!!"
#endif
#if (defined(QT_SHARED) && defined(QT_STATIC))
#	error "Both QT_SHARED and QT_STATIC are defined. Please check your Qt configuration!!!"
#endif

#if (CPPDEVTK_PLATFORM_MACOSX)

#ifndef QT_MAC_USE_COCOA
#	define QT_MAC_USE_COCOA 1
#else
#	if (QT_MAC_USE_COCOA != 1)
#		error "QT_MAC_USE_COCOA is not 1"
#	endif
#endif

#if (!CPPDEVTK_COMPILER_CLANG)
//#	error "Qt5 Mac require clang!!!"
#endif

#endif	// (CPPDEVTK_PLATFORM_MACOSX)

#else	// (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#include <QtCore/QtCore>

#if (CPPDEVTK_PLATFORM_MACOSX)
#	if (!CPPDEVTK_COMPILER_GCC)
//#		error "Qt4 Mac require gcc!!!"
#	endif
#endif

#endif	// (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#if (CPPDEVTK_PLATFORM_WINDOWS)
#	if (defined(CPPDEVTK_SHARED) && (defined(QT_STATIC) || defined(QT_NODLL)))
#		error "On Windows CppDevTk shared with static Qt is a dangerous combination that is not supported!!!"
#	endif
#endif

#if (CPPDEVTK_PLATFORM_DESKTOP)
#	ifdef QT_NO_DBUS
#		error "QtDBus is required on desktop platforms!!!"
#	endif
#endif

#endif	// __cplusplus

/// @}	// config_third_party_supported_qt


/// \defgroup config_third_party_supported_boost Supported Boost
/// Boost >= 1.38.0 is required.
/// @{

#ifdef __cplusplus

// Boost.ScopeExit was introduced in 1.38.0
// Support for void was added in 1.50.0
#if (BOOST_VERSION < 103800)
#	error "Boost >= 1.38.0 is required!!!"
#endif

#endif	// __cplusplus

/// @}	// config_third_party_supported_boost


#if (CPPDEVTK_WITH_ZLIB)
#	if (CPPDEVTK_PLATFORM_WINDOWS)
#		ifndef ZLIB_WINAPI
#			define ZLIB_WINAPI
#		endif
#		ifdef CPPDEVTK_SHARED
#			ifndef ZLIB_DLL
#				define ZLIB_DLL
#			endif
#		endif
#	endif
#endif


/// @}	// config_third_party


#endif	// CPPDEVTK_CONFIG_THIRD_PARTY_HPP_INCLUDED_
