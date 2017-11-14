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


#include "time_utils.hpp"
#include <cppdevtk/base/condition_variable.hpp>


#if (CPPDEVTK_HAVE_PTHREADS)


#include <cppdevtk/base/lock_exception.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/verify.h>

#include <cstddef>
#include <new>
#include <exception>


using ::std::terminate;


namespace cppdevtk {
namespace base {


ConditionVariable::ConditionVariable(): NonCopyable(), conditionVariable_() {
	const int kRetCode = pthread_cond_init(&conditionVariable_, NULL);
	switch (kRetCode) {
		case ESUCCESS:
			break;
		/*
		case ENOMEM:
			throw ::std::bad_alloc();
		*/
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to initialize condition variable");
	}
}

ConditionVariable::~ConditionVariable() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_cond_destroy(&conditionVariable_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy condition variable; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY) && "logic error: attempt to destroy the condition variable while it is referenced");
		CPPDEVTK_ASSERT(0 && "pthread_cond_destroy() failed with undocumented error code");
	}
}


void ConditionVariable::NotifyOne() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_cond_signal(&conditionVariable_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_FATAL("condition variable failed to notify one; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		terminate();
	}
}

void ConditionVariable::NotifyAll() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_cond_broadcast(&conditionVariable_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_FATAL("condition variable failed to notify all; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		terminate();
	}
}


void ConditionVariable::Wait(UniqueLock<Mutex>& uniqueLock) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(uniqueLock.OwnsLock(), "uniqueLock must own mutex");
	
	const int kRetCode = pthread_cond_wait(&conditionVariable_, uniqueLock.GetMutex()->GetNativeHandle());
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "condition variable failed to wait");
	}
}

cv_status::cv_status_t ConditionVariable::WaitFor(UniqueLock<Mutex>& uniqueLock, int relTime) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(uniqueLock.OwnsLock(), "uniqueLock must own mutex");
	
	timespec absTime;
	if (!detail::RelTimeToAbsTime(relTime, absTime)) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "condition variable failed to convert time");
	}
	
	const int kRetCode = pthread_cond_timedwait(&conditionVariable_, uniqueLock.GetMutex()->GetNativeHandle(), &absTime);
	switch (kRetCode) {
		case ESUCCESS:
			return ::cppdevtk::base::cv_status::no_timeout;
		case ETIMEDOUT:
			return ::cppdevtk::base::cv_status::timeout;
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "condition variable failed to timedwait");
	}
}


}	// namespace base
}	// namespace cppdevtk


#endif	// (CPPDEVTK_HAVE_PTHREADS)
