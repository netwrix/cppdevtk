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


#ifndef CPPDEVTK_JNI_CPP_THIS_FIELD_HPP_INCLUDED_
#define CPPDEVTK_JNI_CPP_THIS_FIELD_HPP_INCLUDED_


#include "config.hpp"

#include <jni.h>


namespace cppdevtk {
namespace jni {
namespace detail {


/// \pre \a pJniEnv != NULL
/// \pre \a jObject != NULL
/// \pre pJniEnv->ExceptionCheck() == JNI_FALSE
/// \attention
/// - Java classes must have "long cppThis_" field
/// - Does not clear Java pending exception if creates one
CPPDEVTK_JNI_API jfieldID GetCppThisFieldId(JNIEnv* pJniEnv, jobject jObject);


}	// namespace detail


/// \pre \a pJniEnv != NULL
/// \pre \a jObject != NULL
/// \pre \a pJniEnv->ExceptionCheck() == JNI_FALSE
/// \attention
/// - Java classes must have "long cppThis_" field
/// - Does not clear Java pending exception if creates one
template <typename T>
T* GetCppThisField(JNIEnv* pJniEnv, jobject jObject);

/// \pre \a pJniEnv != NULL
/// \pre \a jObject != NULL
/// \pre \a pJniEnv->ExceptionCheck() == JNI_FALSE
/// \attention
/// - Java classes must have "long cppThis_" field
/// - Does not clear Java pending exception if creates one
template <typename T>
void SetCppThisField(JNIEnv* pJniEnv, jobject jObject, T* cppThis);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

template <typename T>
inline T* GetCppThisField(JNIEnv* pJniEnv, jobject jObject) {
	jfieldID jFieldId = detail::GetCppThisFieldId(pJniEnv, jObject);
	return reinterpret_cast<T*>(pJniEnv->GetLongField(jObject, jFieldId));
}

template <typename T>
inline void SetCppThisField(JNIEnv* pJniEnv, jobject jObject, T* cppThis) {
	jfieldID jFieldId = detail::GetCppThisFieldId(pJniEnv, jObject);
	pJniEnv->SetLongField(jObject, jFieldId, reinterpret_cast<jlong>(cppThis));
}


}	// namespace jni
}	// namespace cppdevtk


#endif	// CPPDEVTK_JNI_CPP_THIS_FIELD_HPP_INCLUDED_
