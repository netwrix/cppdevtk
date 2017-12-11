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


#ifndef CPPDEVTK_BASE_INIT_RESOURCES_HPP_INCLUDED_
#define CPPDEVTK_BASE_INIT_RESOURCES_HPP_INCLUDED_


#include "config.hpp"

#ifdef __cplusplus
#	include <QtCore/QDir>
#endif


#ifdef __cplusplus


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize resources for cppdevtk/base subproject.
/// \remark Not in namespace.
CPPDEVTK_BASE_API void CppDevTkBaseInitResources();




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline CPPDEVTK_BASE_API void CppDevTkBaseInitResources() {
#	ifndef CPPDEVTK_SHARED
	Q_INIT_RESOURCE(cppdevtk_base);
	Q_INIT_RESOURCE(cppdevtk_base_customization);
#	endif
}


#endif	// __cplusplus


#ifdef __cplusplus
extern "C" {
#endif


CPPDEVTK_BASE_API int cppdevtk_base_init_resources(void);


#ifdef __cplusplus
}
#endif


#endif	// CPPDEVTK_BASE_INIT_RESOURCES_HPP_INCLUDED_
