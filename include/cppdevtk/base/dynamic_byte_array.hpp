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


#ifndef CPPDEVTK_BASE_DYNAMIC_BYTE_ARRAY_HPP_INCLUDED_
#define CPPDEVTK_BASE_DYNAMIC_BYTE_ARRAY_HPP_INCLUDED_


#include "config.hpp"
#include "architecture_types.hpp"

#include <vector>

#include <QtCore/QString>


#if (CPPDEVTK_ENABLE_TMPL_EXPL_INST)
#define CPPDEVTK_ENABLE_DYNAMIC_BYTE_ARRAY_EXPL_INST 0	// TODO: modify as desired; be warned: it may be expl inst by other libs (common type)
#endif

#if (CPPDEVTK_ENABLE_DYNAMIC_BYTE_ARRAY_EXPL_INST)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API ::std::allocator< ::cppdevtk::base::byte>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API ::std::vector< ::cppdevtk::base::byte,
		::std::allocator< ::cppdevtk::base::byte> >;
#endif


namespace cppdevtk {
namespace base {


typedef ::std::vector< ::cppdevtk::base::byte> DynamicByteArray;


/// \pre \a len (-1 <= len) && (len <= dynamicByteArray.size())
/// \pre \a base Valid values: 0, 2, 8, 10, 16
CPPDEVTK_BASE_API QString Stringize(const DynamicByteArray& dynamicByteArray, int len = -1, int base = 16);


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_DYNAMIC_BYTE_ARRAY_HPP_INCLUDED_
