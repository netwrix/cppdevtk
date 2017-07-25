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


#ifndef CPPDEVTK_BASE_IF_THEN_ELSE_HPP_INCLUDED_
#define CPPDEVTK_BASE_IF_THEN_ELSE_HPP_INCLUDED_


#include "config.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Compile time if/then/else. If \a cond is \c true the member typedef \c Type shall equal \a TThen; otherwise \a TElse.
/// Primary template.
template <bool cond, typename TThen, typename TElse>
struct IfThenElse {
	typedef TThen Type;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Partial class template specialization for \a cond \c false (else branch).
template <typename TThen, typename TElse>
struct IfThenElse<false, TThen, TElse> {
	typedef TElse Type;
};


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_IF_THEN_ELSE_HPP_INCLUDED_
