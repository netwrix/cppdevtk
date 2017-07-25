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


#ifndef CPPDEVTK_BASE_STATIC_ASSERT_HPP_INCLUDED_
#define CPPDEVTK_BASE_STATIC_ASSERT_HPP_INCLUDED_


#include "config.hpp"
#include "join.h"
#include <cppdevtk/config/compiler.hpp>


/// C++11 static_assert(bool_constexpr, message)
#define CPPDEVTK_STATIC_ASSERT_W_MSG(expr, msg) ((void)0)
#undef CPPDEVTK_STATIC_ASSERT_W_MSG

/// C++17 static_assert(bool_constexpr)
#define CPPDEVTK_STATIC_ASSERT(expr) ((void)0)
#undef CPPDEVTK_STATIC_ASSERT


/// \cond

#if (CPPDEVTK_HAVE_CPP11_STATIC_ASSERT)
#	define CPPDEVTK_STATIC_ASSERT_W_MSG(expr, msg)	\
		static_assert(expr, msg)
	
#	define CPPDEVTK_STATIC_ASSERT(expr)	\
		static_assert(expr, #expr)
#else	// (CPPDEVTK_HAVE_CPP11_STATIC_ASSERT)
#	define CPPDEVTK_STATIC_ASSERT_W_MSG(expr, msg)	\
		CPPDEVTK_STATIC_ASSERT(expr)
	
#	ifdef __COUNTER__
#		define CPPDEVTK_STATIC_ASSERT(expr)	\
			typedef ::cppdevtk::base::detail::StaticAssert<	\
					sizeof(::cppdevtk::base::detail::StaticAssertFailed<(bool)(expr)>)>	\
					CPPDEVTK_JOIN(CppDevTkStaticAssert_, __COUNTER__)
#	else
#		define CPPDEVTK_STATIC_ASSERT(expr)	\
			typedef ::cppdevtk::base::detail::StaticAssert<	\
					sizeof(::cppdevtk::base::detail::StaticAssertFailed<(bool)(expr)>)>	\
					CPPDEVTK_JOIN(CppDevTkStaticAssert_, __LINE__)
#	endif
#endif	// (CPPDEVTK_HAVE_CPP11_STATIC_ASSERT)

/// \endcond


#if (!CPPDEVTK_HAVE_CPP11_STATIC_ASSERT)


namespace cppdevtk {
namespace base {
namespace detail {


template <int>
struct StaticAssert {};


template <bool>
struct StaticAssertFailed;

template <>
struct StaticAssertFailed<true> {};


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// (!CPPDEVTK_HAVE_CPP11_STATIC_ASSERT)


#endif	// CPPDEVTK_BASE_STATIC_ASSERT_HPP_INCLUDED_
