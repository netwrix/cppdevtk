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


#ifdef APSTUDIO_INVOKED
#	error "This file is not editable by Microsoft Visual C++!!!"
#endif


#ifndef CPPDEVTK_BASE_INFO_TR_HPP_INCLUDED_
#define CPPDEVTK_BASE_INFO_TR_HPP_INCLUDED_


#include "info.hpp"

#include <QtCore/QtGlobal>


#define CPPDEVTK_BASE_DESCRIPTION CPPDEVTK_LONG_NAME " base library."


// config subproject is headers only and it does not support translations; this is why we put these here.

#define CPPDEVTK_INFO_TR_CTX "cppdevtk_info"

#define CPPDEVTK_COMPANY_SHORT_NAME_TR QT_TRANSLATE_NOOP("cppdevtk_info", "CoSoSys")
#define CPPDEVTK_COMPANY_LONG_NAME_TR QT_TRANSLATE_NOOP("cppdevtk_info", "CoSoSys Ltd.")
#define CPPDEVTK_COMPANY_COPYRIGHT_TR QT_TRANSLATE_NOOP("cppdevtk_info", "Copyright (C) 2015 - 2018 CoSoSys Ltd. All rights reserved. info@cososys.com.")
#define CPPDEVTK_COMPANY_TRADEMARKS_TR QT_TRANSLATE_NOOP("cppdevtk_info", "TM CoSoSys Ltd.")

#define CPPDEVTK_SHORT_NAME_TR QT_TRANSLATE_NOOP("cppdevtk_info", "CppDevTk")
#define CPPDEVTK_LONG_NAME_TR QT_TRANSLATE_NOOP("cppdevtk_info", "C++ Development Toolkit")
#define CPPDEVTK_DESCRIPTION_TR QT_TRANSLATE_NOOP("cppdevtk_info", "CppDevTk is a collection of libraries for C++ development.")
#define CPPDEVTK_COPYRIGHT_TR CPPDEVTK_COMPANY_COPYRIGHT_TR
#define CPPDEVTK_TRADEMARKS_TR CPPDEVTK_COMPANY_TRADEMARKS_TR


#endif	// CPPDEVTK_BASE_INFO_TR_HPP_INCLUDED_
