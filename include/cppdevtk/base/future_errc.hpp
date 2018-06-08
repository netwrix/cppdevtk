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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_FUTURE_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/future.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_FUTURE_ERRC_HPP_INCLUDED_
#define CPPDEVTK_BASE_FUTURE_ERRC_HPP_INCLUDED_


#include "config.hpp"
#include "system_exception.hpp"


namespace cppdevtk {
namespace base {


namespace future_errc {


enum future_errc_t {
	broken_promise,
	promise_already_satisfied,
	no_state
};


CPPDEVTK_BASE_API ErrorCode MakeErrorCode(future_errc_t futureErrC) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API ErrorCondition MakeErrorCondition(future_errc_t futureErrC) CPPDEVTK_NOEXCEPT;


}	// namespace future_errc


template <>
struct CPPDEVTK_BASE_API IsErrorCodeEnum<future_errc::future_errc_t>: public CPPDEVTK_TR1_NS::true_type {};

CPPDEVTK_BASE_API const ErrorCategory& GetFutureCategory() CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

namespace future_errc {


inline CPPDEVTK_BASE_API ErrorCode MakeErrorCode(future_errc_t futureErrC) CPPDEVTK_NOEXCEPT {
	return ErrorCode(static_cast<int>(futureErrC), GetFutureCategory());
}

inline CPPDEVTK_BASE_API ErrorCondition MakeErrorCondition(future_errc_t futureErrC) CPPDEVTK_NOEXCEPT {
	return ErrorCondition(static_cast<int>(futureErrC), GetFutureCategory());
}


}	// namespace future_errc


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_FUTURE_ERRC_HPP_INCLUDED_
