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


#ifndef CPPDEVTK_BASE_STRING_CONV_HPP_INCLUDED_
#define CPPDEVTK_BASE_STRING_CONV_HPP_INCLUDED_


#include "config.hpp"
#include "tstring.hpp"
#include "invalid_string_conversion_exception.hpp"

#include <QtCore/QString>


// NOTE: In Qt spirit A is Latin1 (not ASCII)


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API ::std::wstring A2W(const char* str);
CPPDEVTK_BASE_API ::std::wstring A2W(const ::std::string& str);
CPPDEVTK_BASE_API QString A2Q(const char* str);
CPPDEVTK_BASE_API QString A2Q(const ::std::string& str);
CPPDEVTK_BASE_API ::std::string A2Utf8(const char* str);
CPPDEVTK_BASE_API ::std::string A2Utf8(const ::std::string& str);

CPPDEVTK_BASE_API ::std::string W2A(const wchar_t* str, bool strict = false);
CPPDEVTK_BASE_API ::std::string W2A(const ::std::wstring& str, bool strict = false);
CPPDEVTK_BASE_API QString W2Q(const wchar_t* str);
CPPDEVTK_BASE_API QString W2Q(const ::std::wstring& str);
CPPDEVTK_BASE_API ::std::string W2Utf8(const wchar_t* str);
CPPDEVTK_BASE_API ::std::string W2Utf8(const ::std::wstring& str);

CPPDEVTK_BASE_API ::std::string Q2A(const QString& str, bool strict = false);
CPPDEVTK_BASE_API ::std::wstring Q2W(const QString& str);
CPPDEVTK_BASE_API ::std::string Q2Utf8(const QString& str);

CPPDEVTK_BASE_API ::std::string Utf82A(const char* str, bool strict = false);
CPPDEVTK_BASE_API ::std::string Utf82A(const ::std::string& str, bool strict = false);
CPPDEVTK_BASE_API ::std::wstring Utf82W(const char* str);
CPPDEVTK_BASE_API ::std::wstring Utf82W(const ::std::string& str);
CPPDEVTK_BASE_API QString Utf82Q(const char* str);
CPPDEVTK_BASE_API QString Utf82Q(const ::std::string& str);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline CPPDEVTK_BASE_API ::std::wstring A2W(const ::std::string& str) {
	return A2Q(str).toStdWString();
}

inline CPPDEVTK_BASE_API ::std::string A2Utf8(const ::std::string& str) {
	return str;
}

inline CPPDEVTK_BASE_API QString W2Q(const ::std::wstring& str) {
	return QString::fromStdWString(str);
}

inline CPPDEVTK_BASE_API ::std::wstring Q2W(const QString& str) {
	return str.toStdWString();
}

inline CPPDEVTK_BASE_API ::std::wstring Utf82W(const ::std::string& str) {
	return Utf82Q(str).toStdWString();
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_STRING_CONV_HPP_INCLUDED_
