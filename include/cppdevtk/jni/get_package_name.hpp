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


#ifndef CPPDEVTK_JNI_GET_PACKAGE_NAME_HPP_INCLUDED_
#define CPPDEVTK_JNI_GET_PACKAGE_NAME_HPP_INCLUDED_


#include "config.hpp"

#include <QtCore/QString>


#if (!CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is Android specific!!!"
#endif


namespace cppdevtk {
namespace jni {


/// \attention If there is a Java pending exception, does not clear it.
CPPDEVTK_JNI_API QString GetPackageName();


}	// namespace jni
}	// namespace cppdevtk


#endif	// CPPDEVTK_JNI_GET_PACKAGE_NAME_HPP_INCLUDED_
