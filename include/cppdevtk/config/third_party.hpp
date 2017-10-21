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
/// - The oldest Qt 5 supported is 5.6.1 (CentOS 7.3, SLED12SP2); openSUSE 13.2 has 5.4.2 in updates and 5.7.1 in KDE Qt5
/// @{

/// \cond

#if (CPPDEVTK_DISABLE_QT_OUTPUT)
#	ifndef QT_NO_DEBUG_OUTPUT
#		define QT_NO_DEBUG_OUTPUT
#	endif
#	ifndef QT_NO_WARNING_OUTPUT
#		define QT_NO_WARNING_OUTPUT
#	endif
#else
#	undef QT_NO_DEBUG_OUTPUT
#	undef QT_NO_WARNING_OUTPUT
#endif

/// \endcond

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
#include <QtCore/QException>
#include <QtCore/QUnhandledException>
#define CPPDEVTK_QT_EXCEPTION QException
#define CPPDEVTK_QT_UNHANDLED_EXCEPTION QUnhandledException
#else
#include <QtCore/QtCore>
#define CPPDEVTK_QT_EXCEPTION QtConcurrent::Exception
#define CPPDEVTK_QT_UNHANDLED_EXCEPTION QtConcurrent::UnhandledException
#endif

#endif	// __cplusplus

/// @}	// config_third_party_supported_qt


/// \defgroup config_third_party_supported_boost Supported Boost
/// Boost >= 1.38.0 is required.
/// @{

#ifdef __cplusplus

#if (BOOST_VERSION < 103800)
#	error "Boost >= 1.38.0 is required!!!"
#endif

#endif	// __cplusplus

/// @}	// config_third_party_supported_boost


#if (CPPDEVTK_PLATFORM_WINDOWS)
#	ifdef CPPDEVTK_SHARED
#		ifndef ZLIB_WINAPI
#			define ZLIB_WINAPI
#		endif

#		ifndef ZLIB_DLL
#			define ZLIB_DLL
#		endif
#	endif
#endif


/// @}	// config_third_party


#endif	// CPPDEVTK_CONFIG_THIRD_PARTY_HPP_INCLUDED_
