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


#ifndef CPPDEVTK_BASE_ZEROIZE_HPP_INCLUDED_
#define CPPDEVTK_BASE_ZEROIZE_HPP_INCLUDED_


#include "config.hpp"
#include "zeroize.h"
#include "dynamic_byte_array.hpp"

#include <QtCore/QString>
#include <QtCore/QByteArray>

#include <string>
#include <algorithm>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API void Zeroize(void* ptr, ::std::size_t num);
CPPDEVTK_BASE_API void Zeroize(char* str);
CPPDEVTK_BASE_API void Zeroize(wchar_t* str);


CPPDEVTK_BASE_API void Zeroize(::cppdevtk::base::DynamicByteArray& dynamicByteArray);
CPPDEVTK_BASE_API void Zeroize(::std::string& str);
CPPDEVTK_BASE_API void Zeroize(::std::wstring& str);
CPPDEVTK_BASE_API void Zeroize(QString& str);
CPPDEVTK_BASE_API void Zeroize(QByteArray& byteArray);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline CPPDEVTK_BASE_API void Zeroize(void* ptr, ::std::size_t num) {
	return ZeroizeMemory(ptr, num);
}

inline CPPDEVTK_BASE_API void Zeroize(char* str) {
	return ZeroizeString(str);
}

inline CPPDEVTK_BASE_API void Zeroize(wchar_t* str) {
	return ZeroizeWString(str);
}

inline CPPDEVTK_BASE_API void Zeroize(::cppdevtk::base::DynamicByteArray& dynamicByteArray) {
	::std::fill(dynamicByteArray.begin(), dynamicByteArray.end(), (::cppdevtk::base::DynamicByteArray::value_type)0);
}

inline CPPDEVTK_BASE_API void Zeroize(::std::string& str) {
	::std::fill(str.begin(), str.end(), '0');
}

inline CPPDEVTK_BASE_API void Zeroize(::std::wstring& str) {
	::std::fill(str.begin(), str.end(), L'0');
}

inline CPPDEVTK_BASE_API void Zeroize(QString& str) {
	str.fill('0');
}

inline CPPDEVTK_BASE_API void Zeroize(QByteArray& byteArray) {
	byteArray.fill('0');
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_ZEROIZE_HPP_INCLUDED_
