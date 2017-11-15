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
#include <cppdevtk/base/semaphore.hpp>


#if (CPPDEVTK_HAVE_PTHREADS)


#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/lock_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>


namespace cppdevtk {
namespace base {


using detail::RelTimeToAbsTime;


Semaphore::Semaphore(::std::size_t cnt): NonCopyable(), semaphore_() {
	const int kRetCode = sem_init(&semaphore_, 0, cnt);
	if (kRetCode != ESUCCESS) {
		const ErrorCode kLastErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINTR);
		CPPDEVTK_ASSERT((kLastErrorCode.GetValue() != EINVAL) && "cnt exceeds SEM_VALUE_MAX");
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != ENOSYS);
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(kLastErrorCode, "failed to initialize semaphore");
	}
}

Semaphore::~Semaphore() CPPDEVTK_NOEXCEPT {
	const int kRetCode = sem_destroy(&semaphore_);
	if (kRetCode != ESUCCESS) {
		const ErrorCode kLastErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_WARN("failed to destroy semaphore; error code: " << kLastErrorCode.ToString());
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINTR);
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINVAL);
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != ENOSYS);
		CPPDEVTK_ASSERT((kLastErrorCode.GetValue() != EBUSY)
				&& "logic error: attempt to destroy semaphore while there are currently processes blocked on it");
		CPPDEVTK_ASSERT(0 && "sem_destroy() failed with undocumented error code");
	}
}

void Semaphore::Notify() {
	const int kRetCode = sem_post(&semaphore_);
	if (kRetCode != ESUCCESS) {
		const ErrorCode kLastErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINTR);
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINVAL);
		CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != ENOSYS);
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(kLastErrorCode, "semaphore failed to post");
	}
}

void Semaphore::Wait() {
	const int kRetCode = TEMP_FAILURE_RETRY(sem_wait(&semaphore_));
	if (kRetCode != ESUCCESS) {
		const ErrorCode kLastErrorCode = GetLastSystemErrorCode();
		switch (kLastErrorCode.GetValue()) {
			case EDEADLK:
				throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when semaphore wait");
			default:
				CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINVAL);
				CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != ENOSYS);
				throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "semaphore failed to wait");
		}
	}
}

bool Semaphore::TryWait() {
	const int kRetCode = TEMP_FAILURE_RETRY(sem_trywait(&semaphore_));
	if (kRetCode == ESUCCESS) {
		return true;
	}
	
	const ErrorCode kLastErrorCode = GetLastSystemErrorCode();
	switch (kLastErrorCode.GetValue()) {
		case EAGAIN:
			break;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when trywait semaphore");
		default:
			CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINVAL);
			CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != ENOSYS);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "semaphore failed to trywait");
	}
	
	return false;
}

#if (!CPPDEVTK_PLATFORM_MACOSX)

bool Semaphore::WaitFor(int relTime) {
	if (relTime <= 0) {
		return TryWait();
	}
	
	timespec absTime;
	if (!RelTimeToAbsTime(relTime, absTime)) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to convert relative time to absolute time");
	}
	
	const int kRetCode = TEMP_FAILURE_RETRY(sem_timedwait(&semaphore_, &absTime));
	if (kRetCode == ESUCCESS) {
		return true;
	}
	
	const ErrorCode kLastErrorCode = GetLastSystemErrorCode();
	switch (kLastErrorCode.GetValue()) {
		case ETIMEDOUT:
			break;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when timedwait semaphore");
		default:
			CPPDEVTK_ASSERT(kLastErrorCode.GetValue() != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "semaphore failed to timedwait");
	}
	
	return false;
}

#endif


}	// namespace base
}	// namespace cppdevtk


#endif	// (CPPDEVTK_HAVE_PTHREADS)
