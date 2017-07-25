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


#ifndef CPPDEVTK_BASE_CSTDINT_HPP_INCLUDED_
#define CPPDEVTK_BASE_CSTDINT_HPP_INCLUDED_


#include "config.hpp"
#include "stdint.h"

#if (CPPDEVTK_HAVE_STDINT_H)
//#	include <cstdint>	// cstdint is C++11
#endif


namespace cppdevtk {
namespace base {


// 7.18.1 Integer types

// 7.18.1.1 Exact-width integer types
using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::int64_t;
using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::uint64_t;

// 7.18.1.2 Minimum-width integer types
using ::int_least8_t;
using ::int_least16_t;
using ::int_least32_t;
using ::int_least64_t;
using ::uint_least8_t;
using ::uint_least16_t;
using ::uint_least32_t;
using ::uint_least64_t;

// 7.18.1.3 Fastest minimum-width integer types
using ::int_fast8_t;
using ::int_fast16_t;
using ::int_fast32_t;
using ::int_fast64_t;
using ::uint_fast8_t;
using ::uint_fast16_t;
using ::uint_fast32_t;
using ::uint_fast64_t;

// 7.18.1.4 Integer types capable of holding object pointers
using ::intptr_t;
using ::uintptr_t;

// 7.18.1.5 Greatest-width integer types
using ::intmax_t;
using ::uintmax_t;


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CSTDINT_HPP_INCLUDED_
