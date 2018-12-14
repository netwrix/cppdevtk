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


#ifndef CPPDEVTK_BASE_TSSTREAM_HPP_INCLUDED_
#define CPPDEVTK_BASE_TSSTREAM_HPP_INCLUDED_


#include "config.hpp"
#include "tchar.hpp"
#include "tstring.hpp"
#include "tios.hpp"
#include "tstreambuf.hpp"

#include "stl_explicit_instantiation.hpp"


namespace cppdevtk {
namespace base {


typedef ::std::basic_stringbuf< ::cppdevtk::base::tchar> tstringbuf;
typedef ::std::basic_istringstream< ::cppdevtk::base::tchar> tistringstream;
typedef ::std::basic_ostringstream< ::cppdevtk::base::tchar> tostringstream;
typedef ::std::basic_stringstream< ::cppdevtk::base::tchar> tstringstream;


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_TSSTREAM_HPP_INCLUDED_
