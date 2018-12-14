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


#ifndef CPPDEVTK_QTSOLUTIONS_QTSINGLEAPPLICATION_CONFIG_HPP_INCLUDED_
#define CPPDEVTK_QTSOLUTIONS_QTSINGLEAPPLICATION_CONFIG_HPP_INCLUDED_


#include <cppdevtk/util/config.hpp>
#if (!CPPDEVTK_ENABLE_QT_SOLUTIONS)
#error "This file require QtSolutions enabled!!!"
#endif


#ifdef CPPDEVTK_SHARED
#	ifdef CPPDEVTK_DETAIL_QTSINGLEAPP_BUILD
#		define CPPDEVTK_QTSINGLEAPP_EXPORT CPPDEVTK_EXPORT_API
#	else
#		define CPPDEVTK_QTSINGLEAPP_EXPORT CPPDEVTK_IMPORT_API
#	endif
#else
#	define CPPDEVTK_QTSINGLEAPP_EXPORT
#endif


#endif	// CPPDEVTK_QTSOLUTIONS_QTSINGLEAPPLICATION_CONFIG_HPP_INCLUDED_
