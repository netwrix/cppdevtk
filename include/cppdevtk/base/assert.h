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


// NOTE: No include guard.
//#ifndef CPPDEVTK_BASE_ASSERT_H_INCLUDED_
//#define CPPDEVTK_BASE_ASSERT_H_INCLUDED_


#include "config.hpp"
#include <cppdevtk/config/compiler.hpp>

#include <assert.h>	// make assert() accessible


#undef CPPDEVTK_ASSERT


/// \brief Assert macro.
/// \sa <a href="http://www.cplusplus.com/reference/cassert/assert">assert()</a>, CPPDEVTK_VERIFY
#ifndef NDEBUG
#	define CPPDEVTK_ASSERT(expr)	\
		((!(expr)) ? CppDevTkAssert(#expr, __FILE__, __LINE__, CPPDEVTK_FUNCTION_LONG_NAME) : ((void)0))
#else
#	define CPPDEVTK_ASSERT(expr)	\
		((void)0)
#endif


#ifdef __cplusplus
extern "C" {
#endif

CPPDEVTK_BASE_API void CppDevTkAssert(const char* assertion, const char* file, int line, const char* function);

#ifdef __cplusplus
}
#endif


//#endif	// CPPDEVTK_BASE_ASSERT_H_INCLUDED_
