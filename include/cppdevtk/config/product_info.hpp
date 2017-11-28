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


#ifdef APSTUDIO_INVOKED
#	error "This file is not editable by Microsoft Visual C++!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PRODUCT_INFO_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PRODUCT_INFO_HPP_INCLUDED_


#include "features.hpp"
#include "company_info.hpp"
#include "compiler.hpp"


/// \defgroup config_info_product_info Product Info
/// \ingroup config_info
/// @{

/// \defgroup config_info_product_info_version Version
/// Version format: MM.mm.pp
/// \sa <a href="http://semver.org">Semantic Versioning</a>
/// @{

// TODO: keep updated
#define CPPDEVTK_VERSION_MAJOR 1	///< \attention May have maximum 2 digits.
#define CPPDEVTK_VERSION_MINOR 0	///< \attention May have maximum 2 digits.
#define CPPDEVTK_VERSION_PATCH 5	///< \attention May have maximum 2 digits.
#ifdef _WIN32
#define CPPDEVTK_VERSION_BUILD 1
#endif

/// Example:
/// \code
/// #if (CPPDEVTK_VERSION_NUMBER < CPPDEVTK_VERSION_NUMBER_FROM_COMPONENTS(1, 2, 3))
/// #	error "MyLib requires CppDevTk >= 1.2.3!!!"
/// #endif
/// \endcode
#define CPPDEVTK_VERSION_NUMBER	\
		CPPDEVTK_VERSION_NUMBER_FROM_COMPONENTS(CPPDEVTK_VERSION_MAJOR, CPPDEVTK_VERSION_MINOR, CPPDEVTK_VERSION_PATCH)

/// Example:
/// \code
/// cout << "CppDevTk version: " << CPPDEVTK_VERSION_STRING << endl;
/// \endcode
#define CPPDEVTK_VERSION_STRING	\
		CPPDEVTK_VERSION_STRING_FROM_COMPONENTS(CPPDEVTK_VERSION_MAJOR, CPPDEVTK_VERSION_MINOR, CPPDEVTK_VERSION_PATCH)

/// Example:
/// \code
/// #if (CPPDEVTK_VERSION_NUMBER < CPPDEVTK_VERSION_NUMBER_FROM_COMPONENTS(1, 2, 3))
/// #	error "MyLib requires CppDevTk >= 1.2.3!!!"
/// #endif
/// \endcode
#define CPPDEVTK_VERSION_NUMBER_FROM_COMPONENTS(verMajor, verMinor, verPatch)	\
	((verMajor * 10000) + (verMinor * 100) + (verPatch * 1))

#define CPPDEVTK_VERSION_STRING_FROM_COMPONENTS(verMajor, verMinor, verPatch)	\
	CPPDEVTK_STRINGIZE(verMajor) "." CPPDEVTK_STRINGIZE(verMinor) "." CPPDEVTK_STRINGIZE(verPatch)

/// @}	// config_info_product_info_version


#define CPPDEVTK_SHORT_NAME "CppDevTk"
#define CPPDEVTK_SHORT_NAME_SANITIZED QString(CPPDEVTK_SHORT_NAME).toLower().replace(' ', '_')
#define CPPDEVTK_LONG_NAME "C++ Development Toolkit"
#define CPPDEVTK_LONG_NAME_SANITIZED QString(CPPDEVTK_LONG_NAME).toLower().replace(' ', '_')
#define CPPDEVTK_DESCRIPTION "CppDevTk is a collection of libraries for C++ development."
#define CPPDEVTK_COPYRIGHT CPPDEVTK_COMPANY_COPYRIGHT
#define CPPDEVTK_TRADEMARKS CPPDEVTK_COMPANY_TRADEMARKS
#define CPPDEVTK_HOMEPAGE "https://github.com/cososys/cppdevtk"
#define CPPDEVTK_ONLINE_SUPPORT ""
#define CPPDEVTK_ONLINE_REGISTRATION ""
#define CPPDEVTK_ESTORE ""


/// @}	// config_info_product_info


#endif	// CPPDEVTK_CONFIG_PRODUCT_INFO_HPP_INCLUDED_
