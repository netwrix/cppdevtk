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


#ifndef CPPDEVTK_JNI_EXCEPTIONS_SUPPORT_HPP_INCLUDED_
#define CPPDEVTK_JNI_EXCEPTIONS_SUPPORT_HPP_INCLUDED_


#include "config.hpp"
#include "exceptions.hpp"

#include <jni.h>

#include <cstddef>


#if (1)
	// buggy JNI: ExceptionDescribe() will clear the pending exception (at least on win and lnx)
	// please see: https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html#ExceptionDescribe
#	define CPPDEVTK_JNI_EXCEPTION_DESCRIBE(pJinEnv)	\
		do {} while (0)
#else
#ifdef NDEBUG
#	define CPPDEVTK_JNI_EXCEPTION_DESCRIBE(pJinEnv)	\
		do {} while (0)
#else
#	define CPPDEVTK_JNI_EXCEPTION_DESCRIBE(pJinEnv)	\
			(pJniEnv)->ExceptionDescribe()
#endif
#endif

#define CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC(pJniEnv, msg, clearPendingException)	\
	do {	\
		if ((pJniEnv)->ExceptionCheck() == JNI_TRUE) {	\
			CPPDEVTK_JNI_EXCEPTION_DESCRIBE((pJinEnv));	\
			if ((clearPendingException)) {	\
				(pJniEnv)->ExceptionClear();	\
			}	\
			throw CPPDEVTK_JNI_EXCEPTION((msg));	\
		}	\
	}	\
	while (0)

#define CPPDEVTK_THROW_JNI_EXC_IF_NULL_PTR(ptr, msg)	\
	do {	\
		if ((ptr) == NULL) {	\
			throw CPPDEVTK_JNI_EXCEPTION((msg));	\
		}	\
	}	\
	while (0)

#define CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, ptr, msg, clearPendingException)	\
		CPPDEVTK_THROW_JNI_EXC_IF_JAVA_PENDING_EXC((pJniEnv), (msg), (clearPendingException));	\
		CPPDEVTK_THROW_JNI_EXC_IF_NULL_PTR((ptr), (msg));

#define CPPDEVTK_THROW_CPP_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, ptr, msg, clearPendingException)	\
		::cppdevtk::jni::ThrowCppExceptionFromJavaPendingException((pJniEnv), (clearPendingException));	\
		CPPDEVTK_THROW_JNI_EXC_IF_NULL_PTR((ptr), (msg));


namespace cppdevtk {
namespace jni {


/// Throws a Java exception from catched C++ exception if a Java exception is not pending.
/// \attention
/// - Use only in catch() block.
/// - calls FatalError() if fails and there is no pending exception!
/// \remark Except bad_alloc that maps to OutOfMemoryError (so Error) all other exceptions (including exception and unknown)
/// maps to something that inherits from Exception, not Error.
/// \pre \a pJniEnv != NULL
///
/// Example:
/// \code
/// jstring Java_com_Foo_Bar(JNIEnv* pJniEnv, jclass jFoo) try {
/// 	// ...
/// }
/// catch (...) {
/// 	ThrowJavaExceptionFromCatchedCppException(pJniEnv);
///		return NULL;	// silence compiler warning: not all control paths return a value
/// }
/// \endcode
CPPDEVTK_JNI_API void ThrowJavaExceptionFromCatchedCppException(JNIEnv* pJniEnv);

/// If there is a Java pending exception throws corresponding C++ exception.
/// \pre \a pJniEnv != NULL
/// \attention Calls FatalError() if \a clearPendingException is false and fails with no pending exception!
CPPDEVTK_JNI_API void ThrowCppExceptionFromJavaPendingException(JNIEnv* pJniEnv, bool clearPendingException = false);


namespace detail {


/// \note arguments may not be NULL and no Java exception pending
/// \attention Does not clear Java pending exception if creates one
CPPDEVTK_JNI_API void ThrowJavaException(JNIEnv* pJniEnv, const char* excClassName, const char* excMsg);

/// \note arguments may not be NULL/empty and no Java exception pending
/// \attention Does not clear Java pending exception if creates one
CPPDEVTK_JNI_API bool IsBaseOf(JNIEnv* pJniEnv, const QString& baseClassName, jclass jDerivedClass);


}	// namespace detail
}	// namespace jni
}	// namespace cppdevtk


#endif	// CPPDEVTK_JNI_EXCEPTIONS_SUPPORT_HPP_INCLUDED_
