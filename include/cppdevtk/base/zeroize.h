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


#ifndef CPPDEVTK_BASE_ZEROIZE_H_INCLUDED_
#define CPPDEVTK_BASE_ZEROIZE_H_INCLUDED_


#include "config.hpp"

#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


CPPDEVTK_BASE_API void ZeroizeMemory(void* ptr, size_t num);	///< fill with 0
CPPDEVTK_BASE_API void ZeroizeString(char* str);	///< fill with '0'
CPPDEVTK_BASE_API void ZeroizeWString(wchar_t* str);	///< fill with L'0'


#ifdef __cplusplus
}
#endif


#endif	// CPPDEVTK_BASE_ZEROIZE_H_INCLUDED_
