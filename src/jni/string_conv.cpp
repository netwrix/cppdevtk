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


#include <cppdevtk/jni/string_conv.hpp>
#if (CPPDEVTK_FORCE_DBC_IN_JNI_API)
#undef CPPDEVTK_ENABLE_DBC
#define CPPDEVTK_ENABLE_DBC 1
#undef CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE
#define CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE 0
#undef CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION
#define CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION 0
#endif

#include <cppdevtk/jni/exceptions.hpp>
#include <cppdevtk/jni/exceptions_support.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/on_block_exit.hpp>

#include <cstddef>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif


namespace cppdevtk {
namespace jni {


using namespace ::cppdevtk::base;


CPPDEVTK_JNI_API QString J2Q(JNIEnv* pJniEnv, jstring jString) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pJniEnv);
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(jString);
	CPPDEVTK_DBC_CHECK_PRECONDITION(pJniEnv->ExceptionCheck() == JNI_FALSE);
	
	const char* utf8String = pJniEnv->GetStringUTFChars(jString, NULL);
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pJniEnv)(&jString)(&utf8String)) {
		// ReleaseStringUTFChars() needs to be called even if GetStringUTFChars() returns NULL
		pJniEnv->ReleaseStringUTFChars(jString, utf8String);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_JNI_EXC_IF_NULL_PTR(utf8String, "GetStringUTFChars() failed");
	CPPDEVTK_ASSERT(pJniEnv->ExceptionCheck() == JNI_FALSE);
	
	// GetStringUTFChars() does not guarantee to return NULL terminated string...
	const jsize kUtf8StringLength = pJniEnv->GetStringUTFLength(jString);
	CPPDEVTK_ASSERT(pJniEnv->ExceptionCheck() == JNI_FALSE);
	QString qString;
	if (kUtf8StringLength > 0) {
		qString = QString::fromUtf8(utf8String, kUtf8StringLength);
	}
	
	return qString;
}

CPPDEVTK_JNI_API jstring Q2J(JNIEnv* pJniEnv, const QString& str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pJniEnv);
	CPPDEVTK_DBC_CHECK_PRECONDITION(pJniEnv->ExceptionCheck() == JNI_FALSE);
	
	jstring jString = pJniEnv->NewStringUTF(str.toUtf8().constData());
	bool commit = false;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&pJniEnv)(&jString)) {
		if (!commit) {
			pJniEnv->DeleteLocalRef(jString);
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_CPP_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jString, "NewStringUTF() failed", false);
	
	commit = true;
	return jString;
}


}	// namespace jni
}	// namespace cppdevtk
