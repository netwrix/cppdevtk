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


#ifndef CPPDEVTK_BASE_NON_COPYABLE_HPP_INCLUDED_
#define CPPDEVTK_BASE_NON_COPYABLE_HPP_INCLUDED_


#include "config.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for non-copyable classes.
/// \note Inherit \c private.
/// \attention In C++11 class is also non-moveable!
class CPPDEVTK_BASE_API NonCopyable {
protected:
#	if (CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS)
	CPPDEVTK_CONSTEXPR NonCopyable() = default;
	/* virtual */ ~NonCopyable() = default;
	
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
#	else
	NonCopyable();
	/* virtual */ ~NonCopyable();
private:
	NonCopyable(const NonCopyable&);	// not implemented
	NonCopyable& operator=(const NonCopyable&);	// not implemented
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

#if (!CPPDEVTK_HAVE_CPP11_DEFAULTED_AND_DELETED_FUNCTIONS)

inline NonCopyable::NonCopyable() {}

inline NonCopyable::~NonCopyable() {}

#endif


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_NON_COPYABLE_HPP_INCLUDED_
