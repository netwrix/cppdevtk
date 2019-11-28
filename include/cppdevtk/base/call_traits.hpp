/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_BASE_CALL_TRAITS_HPP_INCLUDED_
#define CPPDEVTK_BASE_CALL_TRAITS_HPP_INCLUDED_


#include "config.hpp"
#include "type_traits.hpp"

#include <cstddef>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa
/// - <a href="http://www.boost.org/doc/libs/1_66_0/libs/utility/call_traits.htm">boost call_traits</a>
/// - <a href="https://sourceforge.net/p/loki-lib/code/HEAD/tree/trunk/include/loki/TypeTraits.h#l2083">Loki TypeTraits::ParameterType</a>
template <typename T>
struct CallTraits {
	typedef T ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef typename Conditional<CPPDEVTK_TR1_NS::is_scalar<T>::value, T, const T&>::Type ParameterType;
};


template <typename T>
struct CallTraits<T&> {
	typedef T& ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T& ParameterType;
};


template <typename T, ::std::size_t N>
struct CallTraits<T[N]> {
private:
	typedef T ArrayType[N];
public:
	typedef const T* ValueType;
	typedef ArrayType& Reference;
	typedef const ArrayType& ConstReference;
	typedef const T* ParameterType;
};


template <typename T, ::std::size_t N>
struct CallTraits<const T[N]> {
private:
	typedef const T ArrayType[N];
public:
	typedef const T* ValueType;
	typedef ArrayType& Reference;
	typedef const ArrayType& ConstReference;
	typedef const T* ParameterType;
};


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CALL_TRAITS_HPP_INCLUDED_
