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
//#ifndef CPPDEVTK_BASE_VERIFY_H_INCLUDED_
//#define CPPDEVTK_BASE_VERIFY_H_INCLUDED_


#include "config.hpp"
#include "assert.h"


#undef CPPDEVTK_VERIFY

/// \brief Verify macro.
/// \details Same as CPPDEVTK_ASSERT but if \c NDEBUG is defined it evaluates the expression
/// but does not print a message or terminate the program.
/// \sa <a href="https://msdn.microsoft.com/en-us/library/fcatwy09%28v=vs.90%29.aspx">VERIFY()</a>,
/// CPPDEVTK_ASSERT
#ifndef NDEBUG
#	define CPPDEVTK_VERIFY(expr)	\
		CPPDEVTK_ASSERT(expr)
#else
#	define CPPDEVTK_VERIFY(expr)	\
		((bool)(expr))
#endif


//#endif	// CPPDEVTK_BASE_VERIFY_H_INCLUDED_
