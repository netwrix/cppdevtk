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


#ifndef CPPDEVTK_JNI_STRING_CONV_HPP_INCLUDED_
#define CPPDEVTK_JNI_STRING_CONV_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/base/tstring_conv.hpp>

#include <QtCore/QString>

#include <string>

#include <jni.h>


namespace cppdevtk {
namespace jni {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup jni_string_conv JNI string conversion.
/// Support for jstring <-> C++ string conversion.
/// \pre pJniEnv->ExceptionCheck() == JNI_FALSE
/// \remarks
/// - Pointer arguments must not be NULL
/// - Functions returning pointer (jstring) may not return NULL
/// - *2J functions do not clear Java pending exception if creates one
/// @{


CPPDEVTK_JNI_API ::std::string J2A(JNIEnv* pJniEnv, jstring jString);
CPPDEVTK_JNI_API ::std::wstring J2W(JNIEnv* pJniEnv, jstring jString);
CPPDEVTK_JNI_API ::std::string J2Utf8(JNIEnv* pJniEnv, jstring jString);
CPPDEVTK_JNI_API QString J2Q(JNIEnv* pJniEnv, jstring jString);

CPPDEVTK_JNI_API jstring A2J(JNIEnv* pJniEnv, const char* str);
CPPDEVTK_JNI_API jstring A2J(JNIEnv* pJniEnv, const ::std::string& str);
CPPDEVTK_JNI_API jstring W2J(JNIEnv* pJniEnv, const wchar_t* str);
CPPDEVTK_JNI_API jstring W2J(JNIEnv* pJniEnv, const ::std::wstring& str);
CPPDEVTK_JNI_API jstring Utf82J(JNIEnv* pJniEnv, const char* str);
CPPDEVTK_JNI_API jstring Utf82J(JNIEnv* pJniEnv, const ::std::string& str);
CPPDEVTK_JNI_API jstring Q2J(JNIEnv* pJniEnv, const QString& str);


/// @}	// jni_string_conv
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline CPPDEVTK_JNI_API ::std::string J2A(JNIEnv* pJniEnv, jstring jString) {
	return ::cppdevtk::base::Q2A(J2Q(pJniEnv, jString));
}

inline CPPDEVTK_JNI_API ::std::wstring J2W(JNIEnv* pJniEnv, jstring jString) {
	return ::cppdevtk::base::Q2W(J2Q(pJniEnv, jString));
}

inline CPPDEVTK_JNI_API ::std::string J2Utf8(JNIEnv* pJniEnv, jstring jString) {
	return ::cppdevtk::base::Q2Utf8(J2Q(pJniEnv, jString));
}

inline CPPDEVTK_JNI_API jstring A2J(JNIEnv* pJniEnv, const char* str) {
	return Q2J(pJniEnv, ::cppdevtk::base::A2Q(str));
}

inline CPPDEVTK_JNI_API jstring A2J(JNIEnv* pJniEnv, const ::std::string& str) {
	return Q2J(pJniEnv, ::cppdevtk::base::A2Q(str));
}

inline CPPDEVTK_JNI_API jstring W2J(JNIEnv* pJniEnv, const wchar_t* str) {
	return Q2J(pJniEnv, ::cppdevtk::base::W2Q(str));
}

inline CPPDEVTK_JNI_API jstring W2J(JNIEnv* pJniEnv, const ::std::wstring& str) {
	return Q2J(pJniEnv, ::cppdevtk::base::W2Q(str));
}

inline CPPDEVTK_JNI_API jstring Utf82J(JNIEnv* pJniEnv, const char* str) {
	return Q2J(pJniEnv, ::cppdevtk::base::Utf82Q(str));
}

inline CPPDEVTK_JNI_API jstring Utf82J(JNIEnv* pJniEnv, const ::std::string& str) {
	return Q2J(pJniEnv, ::cppdevtk::base::Utf82Q(str));
}


}	// namespace jni
}	// namespace cppdevtk


#endif	// CPPDEVTK_JNI_STRING_CONV_HPP_INCLUDED_
