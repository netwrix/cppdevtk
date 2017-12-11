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


#include <cppdevtk/base/version.h>
#include <cppdevtk/config/product_info.hpp>


CPPDEVTK_BASE_API int GetCppDevTkVersionMajor(void) {
	return CPPDEVTK_VERSION_MAJOR;
}

CPPDEVTK_BASE_API int GetCppDevTkVersionMinor(void) {
	return CPPDEVTK_VERSION_MINOR;
}

CPPDEVTK_BASE_API int GetCppDevTkVersionPatch(void) {
	return CPPDEVTK_VERSION_PATCH;
}

CPPDEVTK_BASE_API int GetCppDevTkVersionNumber(void) {
	return CPPDEVTK_VERSION_NUMBER;
}

CPPDEVTK_BASE_API const char* GetCppDevTkVersionString(void) {
	return CPPDEVTK_VERSION_STRING;
}
