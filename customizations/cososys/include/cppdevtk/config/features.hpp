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


#ifdef APSTUDIO_INVOKED
#	error "This file is not editable by Microsoft Visual C++!!!"
#endif


#ifndef CPPDEVTK_CONFIG_FEATURES_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_FEATURES_HPP_INCLUDED_


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup config_features Features
/// \ingroup config
/// @{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup config_features_run_time Run Time Features
/// Features that can be modified by library users
/// @{

/// \brief Platform (target OS) version.
#ifndef CPPDEVTK_ENABLE_PLATFORM_VERSION
#	ifdef CPPDEVTK_DETAIL_BUILD
#		define CPPDEVTK_ENABLE_PLATFORM_VERSION 1
#	else
#		define CPPDEVTK_ENABLE_PLATFORM_VERSION 0
#	endif
#endif

/// \brief Disable compiler warnings caused by third party libraries (mainly <a href="http://www.qt.io">Qt</a>).
#ifndef CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS
#	ifdef CPPDEVTK_DETAIL_BUILD
#		define CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS 1
#	else
#		define CPPDEVTK_DISABLE_THIRD_PARTY_WARNINGS 0
#	endif
#endif

/// \brief Disable compiler warnings caused by cppdevtk.
#ifndef CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS
#	ifdef CPPDEVTK_DETAIL_BUILD
#		define CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS 1
#	else
#		define CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS 0
#	endif
#endif

/// \brief Disable Qt output.
/// \sa \c QT_NO_DEBUG_OUTPUT and \c QT_NO_WARNING_OUTPUT
#ifndef CPPDEVTK_DISABLE_QT_OUTPUT
#	ifdef NDEBUG
#		define CPPDEVTK_DISABLE_QT_OUTPUT 1
#	else
#		define CPPDEVTK_DISABLE_QT_OUTPUT 0
#	endif
#endif

#if (defined(__APPLE__) && defined(__MACH__))
	// FIXME: 1 is conflicting with Mac's qDebug; investigate!
#	undef CPPDEVTK_DISABLE_QT_OUTPUT
#	define CPPDEVTK_DISABLE_QT_OUTPUT 0
#endif


/// \defgroup config_features_logging Logging
/// @{

/// \brief Enable logging.
#ifndef CPPDEVTK_ENABLE_LOG
#	ifdef NDEBUG
#		define CPPDEVTK_ENABLE_LOG 0
#	else
#		define CPPDEVTK_ENABLE_LOG 1
#	endif
#endif

/// \brief Log level.
/// \sa ::cppdevtk::base::LogLevel
#ifndef CPPDEVTK_LOG_LEVEL
#	ifdef NDEBUG
#		define CPPDEVTK_LOG_LEVEL ::cppdevtk::base::llError
#	else
#		define CPPDEVTK_LOG_LEVEL ::cppdevtk::base::llDebug
#	endif
#endif

/// \brief Date format in log record.
/// \sa <a href="http://doc.qt.io/qt-4.8/qdatetime.html#toString">QDateTime::toString()</a>
#ifndef CPPDEVTK_LOGGER_DATE_FORMAT
#	define CPPDEVTK_LOGGER_DATE_FORMAT "yyyy-MMM-dd hh:mm:ss:zzz"
#endif

/// @}	// config_features_logging

/// \defgroup config_features_dbc DBC
/// @{

/// \brief Enable Design By Contract (rudimentary...) support.
#ifndef CPPDEVTK_ENABLE_DBC
#	ifdef NDEBUG
#		define CPPDEVTK_ENABLE_DBC 0
#	else
#		define CPPDEVTK_ENABLE_DBC 1
#	endif
#endif

/// \brief If enabled assert on failure, throw exception otherwise.
#ifndef CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE
#	define CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE (!CPPDEVTK_ENABLE_DBC || 0)
#endif

#ifndef CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION
#	define CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION (!CPPDEVTK_ENABLE_DBC || 0)
#endif

#ifndef CPPDEVTK_DBC_DISABLE_CHECK_POSTCONDITION
#	define CPPDEVTK_DBC_DISABLE_CHECK_POSTCONDITION (!CPPDEVTK_ENABLE_DBC || 0)
#endif

#ifndef CPPDEVTK_DBC_DISABLE_CHECK_INVARIANT
#	define CPPDEVTK_DBC_DISABLE_CHECK_INVARIANT (!CPPDEVTK_ENABLE_DBC || 0)
#endif


/// @}	// config_features_dbc


/// @}	// config_features_run_time


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup config_features_compile_time Compile Time Features
/// Features fixed at compile time (can not be modified at runtime by users)
/// @{

/// \brief Minimum OS requirement.
// TODO: keep in sync with common_customization.pri

#if (defined(__APPLE__) && defined(__MACH__))
	// Mac OS X is Unix
#	ifndef __unix__
#		define __unix__
#	endif
#	ifndef __unix
#		define __unix
#	endif
#endif

#if ((defined(__unix__) || defined(__unix)))
#	if (defined(__linux__) || defined(linux) || defined(__linux))
#		define CPPDEVTK_KERNEL_VERSION KERNEL_VERSION(2, 6, 32)	// NOTE: can not be less than 2.6.32
#		if (defined(ANDROID) || defined(__ANDROID__))
#			define CPPDEVTK_ANDROID_API 19
#		endif
#	elif (defined(__APPLE__) && defined(__MACH__))
#		if (defined(__GNUC__))
#			if (defined(__clang__))
				// NOTE: can not be less than 10.7
#				define CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED 1070	// MAC_OS_X_VERSION_10_7
#			else
				// NOTE: can not be less than 10.4
#				define CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED 1040	// MAC_OS_X_VERSION_10_4
#			endif
#		else
#			error "Unsupported compiler for Mac OS X platform!!!"
#		endif
#		define CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED
#		if (CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED > CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED)
#			error "CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED > CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED"
#		endif

#		define CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED 100000	// __IPHONE_10_0
#		define CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED
#		if (CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED > CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED)
#			error "CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED > CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED"
#		endif
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (defined(_WIN32))
#	if ((_MSC_VER >= 1700) && !defined(_USING_V110_SDK71_))
		// Win Vista SP2
#		define CPPDEVTK_WIN32_WINNT 0x0600	// _WIN32_WINNT_VISTA
#		define CPPDEVTK_NTDDI_VERSION 0x06000200	// NTDDI_VISTASP2
#		define CPPDEVTK_WINVER CPPDEVTK_WIN32_WINNT
#		define CPPDEVTK_WIN32_IE 0x0700	// _WIN32_IE_WIN6, _WIN32_IE_IE70
#	else
		// NOTE: can not be less than XP SP3
		// Win XP SP3
#		define CPPDEVTK_WIN32_WINNT 0x0501	// _WIN32_WINNT_WINXP
#		define CPPDEVTK_NTDDI_VERSION 0x05010300	// NTDDI_WINXPSP3
#		define CPPDEVTK_WINVER CPPDEVTK_WIN32_WINNT
#		define CPPDEVTK_WIN32_IE 0x0603	// _WIN32_IE_XPSP2, _WIN32_IE_IE60SP2
#	endif
#else
#	error "Unsupported platform!!!"
#endif

#define CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE 0
#define CPPDEVTK_EXCEPTION_ENABLE_STACK_TRACE_IN_RELEASE 0

/// Terminate or throw exception if cppdevtk::base::Throwable::DoThrow() is not or incorrectly overriden.
#define CPPDEVTK_THROWABLE_DISABLE_TERMINATE 0

/// Terminate or throw exception if cppdevtk::base::Cloneable::DoClone() is not or incorrectly overriden.
#define CPPDEVTK_CLONEABLE_DISABLE_TERMINATE 0

/// C++ 11 std, 30.4.1.2.1 Class mutex: The behavior of a program is undefined if it destroys a mutex object
/// owned by any thread or a thread terminates while owning a mutex object.
/// The same applies for recursive_mutex and timed mutex types, please see std.
/// This is why we can use pthread non-robust mutex (PThread is used in mutex implementation on Unix).
/// But undefined behavior may very well include return error so we can also use robust mutex.
/// Robust mutexes may be slower than non-robust mutexes.
/// Mac OS X does not have robust/non-robust concept (Linux does, but Android doesn't...)
#if ((defined(__unix__) || defined(__unix)))
#	if (defined(__linux__) || defined(linux) || defined(__linux))
#		if (!(defined(ANDROID) || defined(__ANDROID__)))
#			ifdef NDEBUG
#				define CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX 1	// 1 non-robust, 0 or not defined robust
#			else
#				define CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX 0	// 1 non-robust, 0 or not defined robust
#			endif
#		else
#			define CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX 1
#		endif
#	elif (defined(__APPLE__) && defined(__MACH__))
#		define CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX 1
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#endif


#if (defined(__unix__) || defined(__unix))
#	define CPPDEVTK_DISABLE_UNICODE 0
#	define CPPDEVTK_DISABLE_WIDE 1
#endif

#ifdef NDEBUG
#	define CPPDEVTK_STACK_TRACE_ENABLE_MS_SYM_SRV 0
#else
#	define CPPDEVTK_STACK_TRACE_ENABLE_MS_SYM_SRV 0	// 1
#endif

#define CPPDEVTK_FORCE_DBC_IN_JNI_API 1


/// If not needed disable for performance
#define CPPDEVTK_ENABLE_THREAD_INTERRUPTION (CPPDEVTK_HAVE_THREAD_STORAGE && 1)

// TODO: keep in sync with cppdevtk_with_zlib in common_customization.pri
/// In case zlib is missing (on Windows); affects QZipReader and QZipWriter
#define CPPDEVTK_WITH_ZLIB (CPPDEVTK_PLATFORM_UNIX || 1)

// TODO: keep in sync with CPPDEVTK_ENABLE_QTSOLUTIONS in common_customization.pri
#define CPPDEVTK_ENABLE_QT_SOLUTIONS 1

// TODO: modify as desired; be warned: it may be expl inst by other libs (common type)
#define CPPDEVTK_ENABLE_BYTE_VECTOR_EXPL_INST 0

/// @}	// config_features_compile_time


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup config_features_auto_detect Auto-Detected Features
/// @{

#ifdef _WIN32
#	if (defined(UNICODE) || defined(_UNICODE))
#		define CPPDEVTK_DISABLE_UNICODE 0
#		define CPPDEVTK_DISABLE_WIDE 0
#	else
#		define CPPDEVTK_DISABLE_UNICODE 1
#		define CPPDEVTK_DISABLE_WIDE 1
#	endif
#endif
// Safety checks.
#if (!CPPDEVTK_DISABLE_WIDE && CPPDEVTK_DISABLE_UNICODE)
#	error "wide requires unicode!!!"
#endif
#ifdef _WIN32
#	if (!CPPDEVTK_DISABLE_UNICODE && CPPDEVTK_DISABLE_WIDE)
#		error "unicode requires wide on Windows!!!"
#	endif
#endif


#define CPPDEVTK_HAVE_POWER_NOTIFIER CPPDEVTK_PLATFORM_DESKTOP
#define CPPDEVTK_HAVE_SCREENSAVER CPPDEVTK_PLATFORM_DESKTOP
#define CPPDEVTK_HAVE_SESSION CPPDEVTK_PLATFORM_DESKTOP
#define CPPDEVTK_HAVE_SESSION_MANAGER CPPDEVTK_PLATFORM_DESKTOP
#define CPPDEVTK_HAVE_STORAGE_DEVICE_NOTIFIER CPPDEVTK_PLATFORM_DESKTOP


/// @}	// config_features_auto_detect

/// @}	// config_features


#endif	// CPPDEVTK_CONFIG_FEATURES_HPP_INCLUDED_
