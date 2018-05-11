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


#ifndef CPPDEVTK_JNI_CONFIG_HPP_INCLUDED_
#define CPPDEVTK_JNI_CONFIG_HPP_INCLUDED_


#include <cppdevtk/util/config.hpp>
#include "info.hpp"

#include <jni.h>


#if (CPPDEVTK_PLATFORM_MACOSX && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7))
#	ifndef JNI_VERSION_1_8
#		error "JNI >= 1.8 required"
#	endif
#	define CPPDEVTK_JNI_VERSION JNI_VERSION_1_8
#else
#	ifndef JNI_VERSION_1_6
#		error "JNI >= 1.6 required"
#	endif
#	define CPPDEVTK_JNI_VERSION JNI_VERSION_1_6
#endif


#ifdef CPPDEVTK_SHARED
#	ifdef CPPDEVTK_DETAIL_JNI_BUILD
#		define CPPDEVTK_JNI_API CPPDEVTK_EXPORT_API
#	else
#		define CPPDEVTK_JNI_API CPPDEVTK_IMPORT_API
#	endif
#else
#	define CPPDEVTK_JNI_API
#endif


#if (CPPDEVTK_COMPILER_HAVE_TMPL_EXPL_INST_DECL)
#	if (CPPDEVTK_PLATFORM_UNIX)
#		define CPPDEVTK_JNI_TMPL_EXPL_INST extern
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
#		ifdef CPPDEVTK_DETAIL_JNI_BUILD
#			define CPPDEVTK_JNI_TMPL_EXPL_INST
#		else
#			define CPPDEVTK_JNI_TMPL_EXPL_INST extern
#		endif
#	else
#		error "Unsupported platform!!!"
#	endif
#endif


#endif	// CPPDEVTK_JNI_CONFIG_HPP_INCLUDED_
