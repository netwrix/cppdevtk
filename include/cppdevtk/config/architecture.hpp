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


#ifndef CPPDEVTK_CONFIG_ARCHITECTURE_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_ARCHITECTURE_HPP_INCLUDED_


#include "features.hpp"


/// \defgroup config_architecture Architecture
/// \ingroup config
/// @{

/// \defgroup config_architecture_supported_architectures Supported Architectures
/// \sa https://sourceforge.net/p/predef/wiki/Architectures
/// @{

/// \brief X86 architecture.
///
/// Example:
/// \code
/// #if (CPPDEVTK_ARCHITECTURE_X86)
/// // X86 specific code
/// #endif
/// \endcode
#if (defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86))
#	define CPPDEVTK_ARCHITECTURE_X86 1
#else
#	define CPPDEVTK_ARCHITECTURE_X86 0
#endif

/// \brief X86_64 architecture.
///
/// Example:
/// \code
/// if (CPPDEVTK_ARCHITECTURE_X86_64)
/// // X86_64 specific code
/// #endif
/// \endcode
#if (defined(__amd64__) || defined(__amd64) || defined(__x86_64__)	\
		|| defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64))
#	define CPPDEVTK_ARCHITECTURE_X86_64 1
#else
#	define CPPDEVTK_ARCHITECTURE_X86_64 0
#endif

/// \brief ARM architecture.
///
/// Example:
/// \code
/// if (CPPDEVTK_ARCHITECTURE_ARM)
/// // ARM specific code
/// #endif
/// \endcode
#if (defined(__arm__) || defined(_M_ARM))
#	define CPPDEVTK_ARCHITECTURE_ARM 1
#else
#	define CPPDEVTK_ARCHITECTURE_ARM 0
#endif

/// \brief ARM64 architecture.
///
/// Example:
/// \code
/// if (CPPDEVTK_ARCHITECTURE_ARM64)
/// // ARM64 specific code
/// #endif
/// \endcode
#ifdef __aarch64__
#	define CPPDEVTK_ARCHITECTURE_ARM64 1
#else
#	define CPPDEVTK_ARCHITECTURE_ARM64 0
#endif

/// \brief Current architecture.
///
/// Example:
/// \code
/// #if (CPPDEVTK_ARCHITECTURE_X86)
/// // X86 specific code
/// #elif (CPPDEVTK_ARCHITECTURE_X86_64)
/// // X86_64 specific code
/// #elif (CPPDEVTK_ARCHITECTURE_ARM)
/// // ARM specific code
/// #elif (CPPDEVTK_ARCHITECTURE_ARM64)
/// // ARM64 specific code
/// #else
/// #	error "Unsupported architecture!!!"
/// #endif
/// \endcode

// Safety check.
#if (!(CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_X86_64	\
		|| CPPDEVTK_ARCHITECTURE_ARM || CPPDEVTK_ARCHITECTURE_ARM64))
#	error "Unsupported architecture!!!"
#endif

/// @}	// config_architecture_supported_architectures

/// @}	// config_architecture


#endif	// CPPDEVTK_CONFIG_ARCHITECTURE_HPP_INCLUDED_
