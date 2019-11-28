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


#ifndef CPPDEVTK_GUI_CONFIG_HPP_INCLUDED_
#define CPPDEVTK_GUI_CONFIG_HPP_INCLUDED_


#include <cppdevtk/util/config.hpp>
#include "info.hpp"


/// \sa <a href="https://msdn.microsoft.com/en-us/library/3y1sfaz2%28v=vs.90%29.aspx">dllexport, dllimport</a>
#ifdef CPPDEVTK_SHARED
#	ifdef CPPDEVTK_DETAIL_GUI_BUILD
#		define CPPDEVTK_GUI_API CPPDEVTK_EXPORT_API
#	else
#		define CPPDEVTK_GUI_API CPPDEVTK_IMPORT_API
#	endif
#else
#	define CPPDEVTK_GUI_API
#endif


/// \sa <a href="https://support.microsoft.com/en-us/kb/168958">How to export STL from DLL</a>
#if (CPPDEVTK_PLATFORM_UNIX)
#	define CPPDEVTK_GUI_TMPL_EXPL_INST extern
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	ifdef CPPDEVTK_DETAIL_GUI_BUILD
#		define CPPDEVTK_GUI_TMPL_EXPL_INST
#	else
#		define CPPDEVTK_GUI_TMPL_EXPL_INST extern
#	endif
#else
#	error "Unsupported platform!!!"
#endif


#endif	// CPPDEVTK_GUI_CONFIG_HPP_INCLUDED_
