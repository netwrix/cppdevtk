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


#include <cppdevtk/base/condition_variable.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>


namespace cppdevtk {
namespace base {


using ::std::exception;
using ::std::terminate;


// NOTE: violating noexcept calls terminate() not unexpected() as throw()


void ConditionVariableAny::NotifyOne() throw() {	// noexcept in std
	try {
		LockGuard<Mutex> lockGuard(*pMtx_);
		conditionVariable_.NotifyOne();
	}
	catch (const exception& exc) {
		SuppressUnusedWarning(exc);
		CPPDEVTK_LOG_FATAL("condition variable any failed to notify one; exc: " << Exception::GetDetailedInfo(exc));
		terminate();
	}
	catch (...) {
		CPPDEVTK_LOG_FATAL("condition variable any failed to notify one; caught unknown exception");
		terminate();
	}
}

void ConditionVariableAny::NotifyAll() throw() {	// noexcept in std
	try {
		LockGuard<Mutex> lockGuard(*pMtx_);
		conditionVariable_.NotifyAll();
	}
	catch (const exception& exc) {
		SuppressUnusedWarning(exc);
		CPPDEVTK_LOG_FATAL("condition variable any failed to notify all; exc: " << Exception::GetDetailedInfo(exc));
		terminate();
	}
	catch (...) {
		CPPDEVTK_LOG_FATAL("condition variable any failed to notify all; caught unknown exception");
		terminate();
	}
}


}	// namespace base
}	// namespace cppdevtk
