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


#include <cppdevtk/base/zeroize.hpp>
#include <cppdevtk/base/zeroize.h>
#include <cppdevtk/base/assert.h>

#include <string.h>
#include <wchar.h>


CPPDEVTK_BASE_API void ZeroizeMemory(void* ptr, size_t num) {
	CPPDEVTK_ASSERT((num == 0) || (ptr != NULL));
	
	::memset(ptr, 0, num);
}

CPPDEVTK_BASE_API void ZeroizeString(char* str) {
	CPPDEVTK_ASSERT(str != NULL);
	
	::std::fill(str, (str + ::strlen(str)), '0');
}

CPPDEVTK_BASE_API void ZeroizeWString(wchar_t* str) {
	CPPDEVTK_ASSERT(str != NULL);
	
	::std::fill(str, (str + ::wcslen(str)), L'0');
}
