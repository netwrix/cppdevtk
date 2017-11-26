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
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/thread_data.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include "thread_local_data_ptr.hpp"

#include <cstddef>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif


namespace cppdevtk {
namespace base {


using this_thread::InterruptionPoint;
using this_thread::detail::pThreadLocalData;
using ::std::exception;


void ConditionVariable::Wait(UniqueLock<Mutex>& uniqueLock) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(uniqueLock.OwnsLock(), "uniqueLock must own mutex");
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	WaitFor(uniqueLock, CPPDEVTK_CHECK_INTERRUPT_REL_TIME);
#	else	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	UninterruptibleWait(uniqueLock);
#	endif
}

cv_status::cv_status_t ConditionVariable::WaitFor(UniqueLock<Mutex>& uniqueLock, int relTime) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(uniqueLock.OwnsLock(), "uniqueLock must own mutex");
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	InterruptionPoint();
	
	if (pThreadLocalData != NULL) {
		pThreadLocalData->GetInterruptionInfoRef().SetWaitingConditionVariable(this);
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN(void) {
		if (pThreadLocalData != NULL) {
			pThreadLocalData->GetInterruptionInfoRef().SetWaitingConditionVariable(NULL);
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	InterruptionPoint();
	
	cv_status::cv_status_t retValue = base::cv_status::timeout;
	if (relTime <= 0) {
		retValue = UninterruptibleWaitFor(uniqueLock, relTime);
		InterruptionPoint();
	}
	else {
		int waited = 0;
		do {
			retValue = UninterruptibleWaitFor(uniqueLock, CPPDEVTK_CHECK_INTERRUPT_REL_TIME);
			InterruptionPoint();
			if (retValue == base::cv_status::no_timeout) {
				break;
			}
			waited += CPPDEVTK_CHECK_INTERRUPT_REL_TIME;
		}
		while ((waited + CPPDEVTK_CHECK_INTERRUPT_REL_TIME) < relTime);
	}
	
	return retValue;
	
#	else	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	return UninterruptibleWaitFor(uniqueLock, relTime);
#	endif
}


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
