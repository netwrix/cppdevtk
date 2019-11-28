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


#include <cppdevtk/jni/config.hpp>
#if (CPPDEVTK_FORCE_DBC_IN_JNI_API)
#undef CPPDEVTK_ENABLE_DBC
#define CPPDEVTK_ENABLE_DBC 1
#undef CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION
#define CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION (!CPPDEVTK_ENABLE_DBC || 0)
#endif

#include <cppdevtk/jni/jni_loader.hpp>

#include <cppdevtk/base/unused.h>
#include <cppdevtk/base/dbc.hpp>

#include <cstddef>


namespace cppdevtk {
namespace jni_base {


CPPDEVTK_JNI_API JavaVM* gpJniJvm;
CPPDEVTK_JNI_API ::cppdevtk::base::DefaultMutex gpJniJvmMtx;


}
}


CPPDEVTK_JNI_API jint JNICALL JNI_OnLoad(JavaVM* pJavaVm, void* reserved) {
	CPPDEVTK_UNUSED(reserved);
	
	::cppdevtk::jni_base::gpJniJvm = pJavaVm;
	
	return CPPDEVTK_JNI_VERSION;
}

CPPDEVTK_JNI_API void JNICALL JNI_OnUnload(JavaVM* pJavaVm, void* reserved) {
	CPPDEVTK_UNUSED(pJavaVm);
	CPPDEVTK_UNUSED(reserved);
	
	::cppdevtk::jni_base::gpJniJvm = NULL;
}
