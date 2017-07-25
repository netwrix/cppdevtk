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


#ifndef CPPDEVTK_BASE_WSTRINGIZE_H_INCLUDED_
#define CPPDEVTK_BASE_WSTRINGIZE_H_INCLUDED_


/// Stringize wchar.
#define CPPDEVTK_STRINGIZE_W(text) CPPDEVTK_DETAIL_STRINGIZE_W_HELPER_1(text)


#define CPPDEVTK_DETAIL_STRINGIZE_W_HELPER_1(text) CPPDEVTK_DETAIL_STRINGIZE_W_HELPER_2(#text)
#define CPPDEVTK_DETAIL_STRINGIZE_W_HELPER_2(str) L ## str


#endif	// CPPDEVTK_BASE_WSTRINGIZE_H_INCLUDED_
