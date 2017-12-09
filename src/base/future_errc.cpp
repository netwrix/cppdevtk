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


#include <cppdevtk/base/future.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>


namespace cppdevtk {
namespace base {


namespace detail {


class /* CPPDEVTK_BASE_API */ FutureErrorCategory: public ErrorCategory {
public:
	FutureErrorCategory() CPPDEVTK_NOEXCEPT;	// needed for default initialization of an object of const type
	virtual QString GetName() const;
	virtual QString GetMessage(int errVal, const QLocale& locale = QLocale::c()) const;
};


}	// namespace detail


CPPDEVTK_BASE_API const ErrorCategory& GetFutureCategory() CPPDEVTK_NOEXCEPT {
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	CPPDEVTK_COMPILER_WARNING("local static variable initialization is not thread safe!");
#	endif
	
	static const detail::FutureErrorCategory kFutureErrorCategory;
	
	return kFutureErrorCategory;
}


namespace detail {


FutureErrorCategory::FutureErrorCategory() CPPDEVTK_NOEXCEPT: ErrorCategory() {}

QString FutureErrorCategory::GetName() const {
	return "future";
}

QString FutureErrorCategory::GetMessage(int errVal, const QLocale& locale) const {
	SuppressUnusedWarning(locale);
	
	switch (static_cast<future_errc::future_errc_t>(errVal)) {
		case future_errc::broken_promise:
			return "the promise object with which the future shares its shared state was destroyed before being set a value or an exception";
		case future_errc::promise_already_satisfied:
			return "the promise object was already set a value or exception";
		case future_errc::no_state:
			return "an operation attempted to access the shared state of an object with no shared state";
		default:
			CPPDEVTK_ASSERT(0 && "unknown future_errc_t value");
			return "unknown future_errc_t value";
	}
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
