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


void ConditionVariableAny::NotifyOne() CPPDEVTK_NOEXCEPT {
	UniqueLock<Mutex> uniqueLock(*pMtx_, deferLock);
	try {
		uniqueLock.Lock();
	}
	catch (const exception& exc) {
		SuppressUnusedWarning(exc);
		CPPDEVTK_LOG_INFO("failed to lock; impredictable scheduling behaviour; exc: " << Exception::GetDetailedInfo(exc));
	}
	catch (...) {
		CPPDEVTK_LOG_INFO("failed to lock; impredictable scheduling behaviour; (caught unknown exception)");
	}
	
	conditionVariable_.NotifyOne();
}

void ConditionVariableAny::NotifyAll() CPPDEVTK_NOEXCEPT {
	UniqueLock<Mutex> uniqueLock(*pMtx_, deferLock);
	try {
		uniqueLock.Lock();
	}
	catch (const exception& exc) {
		SuppressUnusedWarning(exc);
		CPPDEVTK_LOG_INFO("failed to lock; impredictable scheduling behaviour; exc: " << Exception::GetDetailedInfo(exc));
	}
	catch (...) {
		CPPDEVTK_LOG_INFO("failed to lock; impredictable scheduling behaviour; (caught unknown exception)");
	}
	
	conditionVariable_.NotifyAll();
}


}	// namespace base
}	// namespace cppdevtk
