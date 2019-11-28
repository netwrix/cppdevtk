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


#include "throw_lock_exception.hpp"
#include <cppdevtk/base/lock_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/cassert.hpp>


namespace cppdevtk {
namespace base {
namespace detail {


#if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)

void ThrowLockException(const ::std::system_error& exc) {
	const ::std::error_code& kErrorCode = exc.code();
	
	if (kErrorCode == ::std::errc::resource_deadlock_would_occur) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA(exc.what());
	}
	
	if (kErrorCode.category() == ::std::system_category()) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(ErrorCode(kErrorCode.value(), SystemCategoryRef()), exc.what());
	}
	
	CPPDEVTK_ASSERT(kErrorCode.category() == ::std::generic_category());
	throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(ErrorCode(kErrorCode.value(), GenericCategoryRef()), exc.what());
}

#endif


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
