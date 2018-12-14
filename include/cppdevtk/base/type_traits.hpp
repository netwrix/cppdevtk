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


#ifndef CPPDEVTK_BASE_TYPE_TRAITS_HPP_INCLUDED_
#define CPPDEVTK_BASE_TYPE_TRAITS_HPP_INCLUDED_


#include "config.hpp"
#include CPPDEVTK_TR1_HEADER(type_traits)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \c enable_if is missing from TR1; it was added in C++11.
/// \sa
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2240.html">Two missing traits: enable_if and conditional</a>
/// - C++11, 20.9.7.6 Other transformations, \c enable_if
template <bool condition, typename T = void>
struct EnableIf {};

template <typename T>
struct EnableIf<true, T> {
	typedef T Type;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// If \a condition is false, the member typedef \c Type shall equal \a T ; otherwise, there shall be no member typedef \c Type .
template <bool condition, typename T = void>
struct DisableIf {
	typedef T Type;
};

template <typename T>
struct DisableIf<true, T> {};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \c conditional is missing from TR1; it was added in C++11.
/// \sa
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2240.html">Two missing traits: enable_if and conditional</a>
/// - C++11, 20.9.7.6 Other transformations, \c conditional
template <bool condition, typename TIfTrue, typename TIfFalse>
struct Conditional {
	typedef TIfTrue Type;
};

template <typename TIfTrue, typename TIfFalse>
struct Conditional<false, TIfTrue, TIfFalse> {
	typedef TIfFalse Type;
};


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_TYPE_TRAITS_HPP_INCLUDED_
