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


#include <cppdevtk/base/any.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>

#include <typeinfo>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4702)	// C4702: unreachable code
#endif


namespace cppdevtk {
namespace base {


void Any::Reset() CPPDEVTK_NOEXCEPT {
	using ::std::terminate;
	
	
	if (pTypeErasedValue_ == NULL) {
		return;
	}
	
	try {
		delete pTypeErasedValue_;
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_FATAL("Any::Reset(): destructor of TypeErasedValue (" << typeid(*pTypeErasedValue_).name()
			<< ") threw exception: " << Exception::GetDetailedInfo(exc));
		CPPDEVTK_ASSERT(0 && "Any::Reset(): destructor of TypeErasedValue threw exception");
		SuppressUnusedWarning(exc);
		terminate();
	}
	catch (...) {
		CPPDEVTK_LOG_FATAL("Any::Reset(): destructor of TypeErasedValue (" << typeid(*pTypeErasedValue_).name()
			<< ") threw unknown exception");
		CPPDEVTK_ASSERT(0 && "Any::Reset(): destructor of TypeErasedValue threw unknown exception");
		terminate();
	}
	
	pTypeErasedValue_ = NULL;
}


}	// namespace base
}	// namespace cppdevtk
