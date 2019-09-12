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


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_HPP_INCLUDED_
#	error "Do not include directly; please include <cppdevtk/config/platform_compiler.hpp> instead!!!"
#endif

#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif

#if (!CPPDEVTK_COMPILER_GCC)
#	error "This file is GCC specific!!!"
#endif


#ifndef CPPDEVTK_CONFIG_PLATFORM_COMPILER_UNIX_GCC_HPP_INCLUDED_
#define CPPDEVTK_CONFIG_PLATFORM_COMPILER_UNIX_GCC_HPP_INCLUDED_


//#include <unistd.h>


/// \cond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared library export/import.
#define CPPDEVTK_EXPORT_API __attribute__((visibility("default")))
#define CPPDEVTK_IMPORT_API __attribute__((visibility("default")))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform Compiler Features.
#define CPPDEVTK_HAVE_STRERROR_R 1


// Evaluate expr, and repeat as long as it returns -1 with errno set to EINTR.
// Please see The GNU C Library: Interrupted Primitives, TEMP_FAILURE_RETRY(expression)
// http://www.gnu.org/software/libc/manual/html_node/Interrupted-Primitives.html
#ifndef TEMP_FAILURE_RETRY
#	define TEMP_FAILURE_RETRY(expr)	\
		(	\
			__extension__	\
			({	\
				long int retCode = -1L;	\
				do {	\
					retCode = (long int)(expr);	\
				}	\
				while ((retCode == -1L) && (errno == EINTR));	\
				retCode;	\
			})	\
		)
#endif


/// \endcond


#endif	// CPPDEVTK_CONFIG_PLATFORM_COMPILER_UNIX_GCC_HPP_INCLUDED_
