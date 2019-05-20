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


#include <cppdevtk/jni/config.hpp>
#if (CPPDEVTK_FORCE_DBC_IN_JNI_API)
#undef CPPDEVTK_ENABLE_DBC
#define CPPDEVTK_ENABLE_DBC 1
#undef CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION
#define CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION (!CPPDEVTK_ENABLE_DBC || 0)
#endif

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include <cppdevtk/jni/cpp_this_field.hpp>
#include <cppdevtk/jni/exceptions_support.hpp>
#include <cppdevtk/jni/exceptions.hpp>

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <cstddef>


namespace cppdevtk {
namespace jni {
namespace detail {


CPPDEVTK_JNI_API jfieldID GetCppThisFieldId(JNIEnv* pJniEnv, jobject jObject) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pJniEnv);
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(jObject);
	CPPDEVTK_DBC_CHECK_PRECONDITION(pJniEnv->ExceptionCheck() == JNI_FALSE);
	
	jclass jClass = pJniEnv->GetObjectClass(jObject);
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pJniEnv)(&jClass)) {
		pJniEnv->DeleteLocalRef(jClass);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	CPPDEVTK_THROW_CPP_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jClass, "failed to get object class", false);
	
	jfieldID jFieldId = pJniEnv->GetFieldID(jClass, "cppThis_", "J");	// J is the type signature for long
	CPPDEVTK_THROW_CPP_EXC_IF_JAVA_PENDING_EXC_OR_NULL_PTR(pJniEnv, jFieldId, "failed to get cppThis_ field id", false);
	
	return jFieldId;
}


}	// namespace detail
}	// namespace jni
}	// namespace cppdevtk
