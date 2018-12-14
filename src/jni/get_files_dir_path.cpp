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


#include <cppdevtk/jni/get_files_dir_path.hpp>
#if (CPPDEVTK_FORCE_DBC_IN_JNI_API)
#undef CPPDEVTK_ENABLE_DBC
#define CPPDEVTK_ENABLE_DBC 1
#undef CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE
#define CPPDEVTK_DBC_DISABLE_THROW_ON_FAILURE 0
#undef CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION
#define CPPDEVTK_DBC_DISABLE_CHECK_PRECONDITION 0
#endif

#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/jni/exceptions_support.hpp>
#include <cppdevtk/jni/exceptions.hpp>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
#include <QtAndroidExtras/QtAndroid>
#endif
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>

#include <jni.h>

#include <cstddef>


namespace cppdevtk {
namespace jni {


CPPDEVTK_JNI_API QString GetFilesDirPath() {
	QAndroidJniEnvironment androidJniEnvironment;
	JNIEnv* pJniEnv = (JNIEnv*)androidJniEnvironment;
	if (pJniEnv == NULL) {
		throw CPPDEVTK_JNI_EXCEPTION("NULL JNIEnv");
	}
	ThrowCppExceptionFromJavaPendingException(pJniEnv);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
	QAndroidJniObject activity = ::QtAndroid::androidActivity();
#	else
	QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity",
			"()Landroid/app/Activity;");
#	endif
	ThrowCppExceptionFromJavaPendingException(pJniEnv);
	if (!activity.isValid()) {
		throw CPPDEVTK_JNI_EXCEPTION("qt jni activity is not valid");
	}
	
	QAndroidJniObject applicationContext = activity.callObjectMethod("getApplicationContext",
			"()Landroid/content/Context;");
	ThrowCppExceptionFromJavaPendingException(pJniEnv);
	if (!applicationContext.isValid()) {
		throw CPPDEVTK_JNI_EXCEPTION("jni applicationContext is not valid");
	}
	
	QAndroidJniObject filesDir = applicationContext.callObjectMethod("getFilesDir", "()Ljava/io/File;");
	ThrowCppExceptionFromJavaPendingException(pJniEnv);
	if (!filesDir.isValid()) {
		throw CPPDEVTK_JNI_EXCEPTION("failed to get jni filesDir");
	}
	
	QAndroidJniObject filesDirPath = filesDir.callObjectMethod("getPath", "()Ljava/lang/String;");
	ThrowCppExceptionFromJavaPendingException(pJniEnv);
	if (!filesDirPath.isValid() || filesDirPath.toString().isEmpty()) {
		throw CPPDEVTK_JNI_EXCEPTION("failed to get jni filesDirPath");
	}
	//CPPDEVTK_LOG_DEBUG("filesDirPath: " << filesDirPath.toString());
	
	return filesDirPath.toString();
}


}	// namespace jni
}	// namespace cppdevtk
