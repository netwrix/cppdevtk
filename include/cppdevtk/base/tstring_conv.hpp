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


#ifndef CPPDEVTK_BASE_TSTRING_CONV_HPP_INCLUDED_
#define CPPDEVTK_BASE_TSTRING_CONV_HPP_INCLUDED_


#include "config.hpp"
#include "string_conv.hpp"


/*

Summary:

T: CppDevTk Latin1/Unicode (UTF-8/16/32) (controlled by CPPDEVTK_DISABLE_UNICODE and CPPDEVTK_DISABLE_WIDE)
U: platform Latin1/native Unicode (UTF-8 on Unix and UTF-16 on Windows) (controlled by CPPDEVTK_DISABLE_UNICODE)


////////////////////
// T

CPPDEVTK_A2T(str)
CPPDEVTK_W2T(str)
CPPDEVTK_W2T_STRICT(str)
CPPDEVTK_Q2T(str)
CPPDEVTK_Q2T_STRICT(str)
CPPDEVTK_UTF82T(str)
CPPDEVTK_UTF82T_STRICT(str)

CPPDEVTK_T2A(str)
CPPDEVTK_T2A_STRICT(str)
CPPDEVTK_T2W(str)
CPPDEVTK_T2Q(str)
CPPDEVTK_T2UTF8(str)


////////////////////
// U

CPPDEVTK_A2U(str)
CPPDEVTK_W2U(str)
CPPDEVTK_W2U_STRICT(str)
CPPDEVTK_Q2U(str)
CPPDEVTK_Q2U_STRICT(str)
CPPDEVTK_UTF82U(str)
CPPDEVTK_UTF82U_STRICT(str)

CPPDEVTK_U2A(str)
CPPDEVTK_U2A_STRICT(str)
CPPDEVTK_U2W(str)
CPPDEVTK_U2Q(str)
CPPDEVTK_U2UTF8(str)


////////////////////
// T, U

CPPDEVTK_T2U(str)
CPPDEVTK_U2T(str)

*/


// doxygen workarounds

#define CPPDEVTK_A2T(str) ((void)str)
#define CPPDEVTK_W2T(str) ((void)str)
#define CPPDEVTK_W2T_STRICT(str) ((void)str)
#define CPPDEVTK_Q2T(str) ((void)str)
#define CPPDEVTK_Q2T_STRICT(str) ((void)str)
#define CPPDEVTK_UTF82T(str) ((void)str)
#define CPPDEVTK_UTF82T_STRICT(str) ((void)str)
#define CPPDEVTK_T2A(str) ((void)str)
#define CPPDEVTK_T2A_STRICT(str) ((void)str)
#define CPPDEVTK_T2W(str) ((void)str)
#define CPPDEVTK_T2Q(str) ((void)str)
#define CPPDEVTK_T2UTF8(str) ((void)str)
#define CPPDEVTK_A2U(str) ((void)str)
#define CPPDEVTK_W2U(str) ((void)str)
#define CPPDEVTK_W2U_STRICT(str) ((void)str)
#define CPPDEVTK_Q2U(str) ((void)str)
#define CPPDEVTK_Q2U_STRICT(str) ((void)str)
#define CPPDEVTK_UTF82U(str) ((void)str)
#define CPPDEVTK_UTF82U_STRICT(str) ((void)str)
#define CPPDEVTK_U2A(str) ((void)str)
#define CPPDEVTK_U2A_STRICT(str) ((void)str)
#define CPPDEVTK_U2W(str) ((void)str)
#define CPPDEVTK_U2Q(str) ((void)str)
#define CPPDEVTK_U2UTF8(str) ((void)str)
#define CPPDEVTK_T2U(str) ((void)str)
#define CPPDEVTK_U2T(str) ((void)str)


/// \cond


#undef CPPDEVTK_A2T
#undef CPPDEVTK_W2T
#undef CPPDEVTK_W2T_STRICT
#undef CPPDEVTK_Q2T
#undef CPPDEVTK_Q2T_STRICT
#undef CPPDEVTK_UTF82T
#undef CPPDEVTK_UTF82T_STRICT
#undef CPPDEVTK_T2A
#undef CPPDEVTK_T2A_STRICT
#undef CPPDEVTK_T2W
#undef CPPDEVTK_T2Q
#undef CPPDEVTK_T2UTF8
#undef CPPDEVTK_A2U
#undef CPPDEVTK_W2U
#undef CPPDEVTK_W2U_STRICT
#undef CPPDEVTK_Q2U
#undef CPPDEVTK_Q2U_STRICT
#undef CPPDEVTK_UTF82U
#undef CPPDEVTK_UTF82U_STRICT
#undef CPPDEVTK_U2A
#undef CPPDEVTK_U2A_STRICT
#undef CPPDEVTK_U2W
#undef CPPDEVTK_U2Q
#undef CPPDEVTK_U2UTF8
#undef CPPDEVTK_T2U
#undef CPPDEVTK_U2T


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T

#if (CPPDEVTK_DISABLE_UNICODE)	// T == Latin1
#	define CPPDEVTK_A2T(str) ::std::string(str)
#	define CPPDEVTK_W2T(str) ::cppdevtk::base::W2A(str)
#	define CPPDEVTK_W2T_STRICT(str) ::cppdevtk::base::W2A(str, true)
#	define CPPDEVTK_Q2T(str) ::cppdevtk::base::Q2A(str)
#	define CPPDEVTK_Q2T_STRICT(str) ::cppdevtk::base::Q2A(str, true)
#	define CPPDEVTK_UTF82T(str) ::cppdevtk::base::Utf82A(str)
#	define CPPDEVTK_UTF82T_STRICT(str) ::cppdevtk::base::Utf82A(str, true)
#	define CPPDEVTK_T2A(str) ::std::string(str)
#	define CPPDEVTK_T2A_STRICT(str) ::std::string(str)
#	define CPPDEVTK_T2W(str) ::cppdevtk::base::A2W(str)
#	define CPPDEVTK_T2Q(str) ::cppdevtk::base::A2Q(str)
#	define CPPDEVTK_T2UTF8(str) ::cppdevtk::base::A2Utf8(str)
#else
#	if (CPPDEVTK_DISABLE_WIDE)	// T == UTF-8
#		define CPPDEVTK_A2T(str) ::cppdevtk::base::A2Utf8(str)
#		define CPPDEVTK_W2T(str) ::cppdevtk::base::W2Utf8(str)
#		define CPPDEVTK_W2T_STRICT(str) ::cppdevtk::base::W2Utf8(str)
#		define CPPDEVTK_Q2T(str) ::cppdevtk::base::Q2Utf8(str)
#		define CPPDEVTK_Q2T_STRICT(str) ::cppdevtk::base::Q2Utf8(str)
#		define CPPDEVTK_UTF82T(str) ::std::string(str)
#		define CPPDEVTK_UTF82T_STRICT(str) ::std::string(str)
#		define CPPDEVTK_T2A(str) ::cppdevtk::base::Utf82A(str)
#		define CPPDEVTK_T2A_STRICT(str) ::cppdevtk::base::Utf82A(str, true)
#		define CPPDEVTK_T2W(str) ::cppdevtk::base::Utf82W(str)
#		define CPPDEVTK_T2Q(str) ::cppdevtk::base::Utf82Q(str)
#		define CPPDEVTK_T2UTF8(str) ::std::string(str)
#	else	// T == wide (UTF-32 on Unix and UTF-16 on Windows)
#		define CPPDEVTK_A2T(str) ::cppdevtk::base::A2W(str)
#		define CPPDEVTK_W2T(str) ::std::wstring(str)
#		define CPPDEVTK_W2T_STRICT(str) ::std::wstring(str)
#		define CPPDEVTK_Q2T(str) ::cppdevtk::base::Q2W(str)
#		define CPPDEVTK_Q2T_STRICT(str) ::cppdevtk::base::Q2W(str)
#		define CPPDEVTK_UTF82T(str) ::cppdevtk::base::Utf82W(str)
#		define CPPDEVTK_UTF82T_STRICT(str) ::cppdevtk::base::Utf82W(str)
#		define CPPDEVTK_T2A(str) ::cppdevtk::base::W2A(str)
#		define CPPDEVTK_T2A_STRICT(str) ::cppdevtk::base::W2A(str, true)
#		define CPPDEVTK_T2W(str) ::std::wstring(str)
#		define CPPDEVTK_T2Q(str) ::cppdevtk::base::W2Q(str)
#		define CPPDEVTK_T2UTF8(str) ::cppdevtk::base::W2Utf8(str)
#	endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// U

#if (CPPDEVTK_DISABLE_UNICODE)	// U == Latin1
#	define CPPDEVTK_A2U(str) ::std::string(str)
#	define CPPDEVTK_W2U(str) ::cppdevtk::base::W2A(str)
#	define CPPDEVTK_W2U_STRICT(str) ::cppdevtk::base::W2A(str, true)
#	define CPPDEVTK_Q2U(str) ::cppdevtk::base::Q2A(str)
#	define CPPDEVTK_Q2U_STRICT(str) ::cppdevtk::base::Q2A(str, true)
#	define CPPDEVTK_UTF82U(str) ::cppdevtk::base::Utf82A(str)
#	define CPPDEVTK_UTF82U_STRICT(str) ::cppdevtk::base::Utf82A(str, true)
#	define CPPDEVTK_U2A(str) ::std::string(str)
#	define CPPDEVTK_U2A_STRICT(str) ::std::string(str)
#	define CPPDEVTK_U2W(str) ::cppdevtk::base::A2W(str)
#	define CPPDEVTK_U2Q(str) ::cppdevtk::base::A2Q(str)
#	define CPPDEVTK_U2UTF8(str) ::cppdevtk::base::A2Utf8(str)
#else
#	if (CPPDEVTK_PLATFORM_UNIX)	// U == UTF-8
#		define CPPDEVTK_A2U(str) ::cppdevtk::base::A2Utf8(str)
#		define CPPDEVTK_W2U(str) ::cppdevtk::base::W2Utf8(str)
#		define CPPDEVTK_W2U_STRICT(str) ::cppdevtk::base::W2Utf8(str)
#		define CPPDEVTK_Q2U(str) ::cppdevtk::base::Q2Utf8(str)
#		define CPPDEVTK_Q2U_STRICT(str) ::cppdevtk::base::Q2Utf8(str)
#		define CPPDEVTK_UTF82U(str) ::std::string(str)
#		define CPPDEVTK_UTF82U_STRICT(str) ::std::string(str)
#		define CPPDEVTK_U2A(str) ::cppdevtk::base::Utf82A(str)
#		define CPPDEVTK_U2A_STRICT(str) ::cppdevtk::base::Utf82A(str, true)
#		define CPPDEVTK_U2W(str) ::cppdevtk::base::Utf82W(str)
#		define CPPDEVTK_U2Q(str) ::cppdevtk::base::Utf82Q(str)
#		define CPPDEVTK_U2UTF8(str) ::std::string(str)
#elif (CPPDEVTK_PLATFORM_WINDOWS)	// U == wide (UTF-16)
#		define CPPDEVTK_A2U(str) ::cppdevtk::base::A2W(str)
#		define CPPDEVTK_W2U(str) ::std::wstring(str)
#		define CPPDEVTK_W2U_STRICT(str) ::std::wstring(str)
#		define CPPDEVTK_Q2U(str) ::cppdevtk::base::Q2W(str)
#		define CPPDEVTK_Q2U_STRICT(str) ::cppdevtk::base::Q2W(str)
#		define CPPDEVTK_UTF82U(str) ::cppdevtk::base::Utf82W(str)
#		define CPPDEVTK_UTF82U_STRICT(str) ::cppdevtk::base::Utf82W(str)
#		define CPPDEVTK_U2A(str) ::cppdevtk::base::W2A(str)
#		define CPPDEVTK_U2A_STRICT(str) ::cppdevtk::base::W2A(str, true)
#		define CPPDEVTK_U2W(str) ::std::wstring(str)
#		define CPPDEVTK_U2Q(str) ::cppdevtk::base::W2Q(str)
#		define CPPDEVTK_U2UTF8(str) ::cppdevtk::base::W2Utf8(str)
#	else
#		error "Unsupported platform!!!"
#	endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T, U

#if (CPPDEVTK_DISABLE_UNICODE)	// T == Latin1, U == Latin1
#	define CPPDEVTK_T2U(str) ::std::string(str)
#	define CPPDEVTK_U2T(str) ::std::string(str)
#else
#	if (CPPDEVTK_DISABLE_WIDE)	// T == UTF-8
#		if (CPPDEVTK_PLATFORM_UNIX)	// U == UTF-8
#			define CPPDEVTK_T2U(str) ::std::string(str)
#			define CPPDEVTK_U2T(str) ::std::string(str)
#		elif (CPPDEVTK_PLATFORM_WINDOWS)	// U == wide (UTF-16)
#			define CPPDEVTK_T2U(str) ::cppdevtk::base::Utf82W(str)
#			define CPPDEVTK_U2T(str) ::cppdevtk::base::W2Utf8(str)
#		else
#			error "Unsupported platform!!!"
#		endif
#	else
#		if (CPPDEVTK_PLATFORM_UNIX)	// T == wide (UTF-32), U == UTF-8
#			define CPPDEVTK_T2U(str) ::cppdevtk::base::W2Utf8(str)
#			define CPPDEVTK_U2T(str) ::cppdevtk::base::Utf82W(str)
#		elif (CPPDEVTK_PLATFORM_WINDOWS)	// T == wide (UTF-16), U == wide (UTF-16)
#			define CPPDEVTK_T2U(str) ::std::wstring(str)
#			define CPPDEVTK_U2T(str) ::std::wstring(str)
#		else
#			error "Unsupported platform!!!"
#		endif
#	endif
#endif


/// \endcond


#endif	// CPPDEVTK_BASE_TSTRING_CONV_HPP_INCLUDED_
