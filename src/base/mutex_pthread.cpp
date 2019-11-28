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


// NOTE:
// - C++11 std, 30.4.1 Mutex requirements, unlock() is nothrow and requires that the calling thread shall own the mutex.
// - C++ 11 std, 30.4.1 Mutex requirements: Construction and destruction of an object of a mutex type need not be thread-safe;
// other synchronization should be used to ensure that mutex objects are initialized and visible to other threads.
// - C++ 11 std, 30.4.1.2.1 Class mutex: A program may deadlock if the thread that owns a mutex object calls lock() on that object.
// If the implementation can detect the deadlock, a resource_deadlock_would_occur error condition may be observed.
// This is why we use PTHREAD_MUTEX_NORMAL and not PTHREAD_MUTEX_DEFAULT.


#include <cppdevtk/base/mutex.hpp>


#if (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))


#include "pthread_ext.h"
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/lock_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/time_utils.hpp>

#include <cstddef>
#include <ctime>
#include <exception>
#include <new>


using ::std::time_t;
using ::std::time;
using ::std::difftime;
using ::std::terminate;


namespace cppdevtk {
namespace base {


namespace detail {


static void InitializeMutex(pthread_mutex_t& mutex, int mutexType);


}	// namespace detail


#if (CPPDEVTK_HAVE_PTHREADS)


Mutex::Mutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_NORMAL);
}

Mutex::~Mutex() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_destroy(&mutex_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY) && "logic error: attempt to destroy mutex while it is locked or referenced");
		CPPDEVTK_ASSERT(0 && "pthread_mutex_destroy() failed with undocumented error code for mutex");
	}
}

void Mutex::Lock() {
	const int kRetCode = pthread_mutex_lock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when lock mutex");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock mutex");
	}
}

bool Mutex::TryLock() {
	const int kRetCode = pthread_mutex_trylock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock' mutex");
	}
}

void Mutex::Unlock() {
	const int kRetCode = pthread_mutex_unlock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EPERM:
			CPPDEVTK_LOG_ERROR("failed to unlock mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock mutex (EPERM: the current thread does not own the mutex)");
			//throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode),
			//		"failed to unlock mutex (EPERM: the current thread does not own the mutex)");
			return;
		default:
			CPPDEVTK_LOG_ERROR("failed to unlock mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			CPPDEVTK_ASSERT(0 && "pthread_mutex_unlock() failed with undocumented error code for mutex");
			return;
	}
}

Mutex::NativeHandleType Mutex::GetNativeHandle() {
	return &mutex_;
}


ErrorCheckingMutex::ErrorCheckingMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_ERRORCHECK);
}

ErrorCheckingMutex::~ErrorCheckingMutex() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_destroy(&mutex_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy error checking mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY) && "logic error: attempt to destroy error checking mutex while it is locked or referenced");
		CPPDEVTK_ASSERT(0 && "pthread_mutex_destroy() failed with undocumented error code for error checking mutex");
	}
}

void ErrorCheckingMutex::Lock() {
	const int kRetCode = pthread_mutex_lock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when lock error checking mutex");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock error checking mutex");
	}
}

bool ErrorCheckingMutex::TryLock() {
	const int kRetCode = pthread_mutex_trylock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock' error checking mutex");
	}
}

void ErrorCheckingMutex::Unlock() {
	const int kRetCode = pthread_mutex_unlock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EPERM:
			CPPDEVTK_LOG_ERROR("failed to unlock error checking mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock error checking mutex (EPERM: the current thread does not own the mutex)");
			//throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode),
			//		"failed to unlock error checking mutex (EPERM: the current thread does not own the mutex)");
			return;
		default:
			CPPDEVTK_LOG_ERROR("failed to unlock error checking mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			CPPDEVTK_ASSERT(0 && "pthread_mutex_unlock() failed with undocumented error code for error checking mutex");
			return;
	}
}

ErrorCheckingMutex::NativeHandleType ErrorCheckingMutex::GetNativeHandle() {
	return &mutex_;
}


RecursiveMutex::RecursiveMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_RECURSIVE);
}

RecursiveMutex::~RecursiveMutex() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_destroy(&mutex_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy recursive mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY) && "logic error: attempt to destroy recursive mutex while it is locked or referenced");
		CPPDEVTK_ASSERT(0 && "pthread_mutex_destroy() failed with undocumented error code for recursive mutex");
	}
}

void RecursiveMutex::Lock() {
	const int kRetCode = pthread_mutex_lock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EAGAIN:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock recursive mutex "
					"(EAGAIN: the maximum number of recursive locks for mutex has been exceeded)");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock recursive mutex");
	}
}

bool RecursiveMutex::TryLock() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_trylock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		case EAGAIN:
			CPPDEVTK_LOG_ERROR("failed to 'try lock' recursive mutex "
					"(EAGAIN: the maximum number of recursive locks for mutex has been exceeded)");
			return false;
		default:
			CPPDEVTK_LOG_ERROR("failed to 'try lock' recursive mutex; error code: "
					<< MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			return false;
	}
}

void RecursiveMutex::Unlock() {
	const int kRetCode = pthread_mutex_unlock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EPERM:
			CPPDEVTK_LOG_ERROR("failed to unlock recursive mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock recursive mutex (EPERM: the current thread does not own the mutex)");
			//throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode),
			//		"failed to unlock recursive mutex (EPERM: the current thread does not own the mutex)");
			return;
		default:
			CPPDEVTK_LOG_ERROR("failed to unlock recursive mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			CPPDEVTK_ASSERT(0 && "pthread_mutex_unlock() failed with undocumented error code for recursive mutex");
			return;
	}
}

RecursiveMutex::NativeHandleType RecursiveMutex::GetNativeHandle() {
	return &mutex_;
}


#endif	// (CPPDEVTK_HAVE_PTHREADS)


TimedMutex::TimedMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_NORMAL);
}

TimedMutex::~TimedMutex() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_destroy(&mutex_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy timed mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY) && "logic error: attempt to destroy timed mutex while it is locked or referenced");
		CPPDEVTK_ASSERT(0 && "pthread_mutex_destroy() failed with undocumented error code for timed mutex");
	}
}

void TimedMutex::Lock() {
	const int kRetCode = pthread_mutex_lock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when lock timed mutex");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock timed mutex");
	}
}

bool TimedMutex::TryLock() {
	const int kRetCode = pthread_mutex_trylock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock' timed mutex");
	}
}

bool TimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	const timespec kAbsTime = RelTimeToAbsTime(relTime);
	const int kRetCode = pthread_mutex_timedlock(&mutex_, &kAbsTime);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case ETIMEDOUT:
			return false;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when 'try lock for' timed mutex");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock for' timed mutex");
	}
}

bool TimedMutex::TryLockUntil(::std::time_t absTime) {
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = difftime(absTime, kCurrTime);
	
	return (kSeconds <= 0) ? TryLock() : TryLockFor(kSeconds * 1000);
}

void TimedMutex::Unlock() {
	const int kRetCode = pthread_mutex_unlock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EPERM:
			CPPDEVTK_LOG_ERROR("failed to unlock timed mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock timed mutex (EPERM: the current thread does not own the mutex)");
			//throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode),
			//		"failed to unlock timed mutex (EPERM: the current thread does not own the mutex)");
			return;
		default:
			CPPDEVTK_LOG_ERROR("failed to unlock timed mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			CPPDEVTK_ASSERT(0 && "pthread_mutex_unlock() failed with undocumented error code for timed mutex");
			return;
	}
}

TimedMutex::NativeHandleType TimedMutex::GetNativeHandle() {
	return &mutex_;
}


#if (CPPDEVTK_HAVE_PTHREADS)


ErrorCheckingTimedMutex::ErrorCheckingTimedMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_ERRORCHECK);
}

ErrorCheckingTimedMutex::~ErrorCheckingTimedMutex() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_destroy(&mutex_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy error checking timed mutex; error code: "
				<< MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY)
				&& "logic error: attempt to destroy error checking timed mutex while it is locked or referenced");
		CPPDEVTK_ASSERT(0 && "pthread_mutex_destroy() failed with undocumented error code for error checking timed mutex");
	}
}

void ErrorCheckingTimedMutex::Lock() {
	const int kRetCode = pthread_mutex_lock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when lock error checking timed mutex");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock error checking timed mutex");
	}
}

bool ErrorCheckingTimedMutex::TryLock() {
	const int kRetCode = pthread_mutex_trylock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock' error checking timed mutex");
	}
}

bool ErrorCheckingTimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	const timespec kAbsTime = RelTimeToAbsTime(relTime);
	const int kRetCode = pthread_mutex_timedlock(&mutex_, &kAbsTime);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case ETIMEDOUT:
			return false;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("deadlock when 'try lock for' error checking timed mutex");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock for' error checking timed mutex");
	}
}

bool ErrorCheckingTimedMutex::TryLockUntil(::std::time_t absTime) {
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = difftime(absTime, kCurrTime);
	
	return (kSeconds <= 0) ? TryLock() : TryLockFor(kSeconds * 1000);
}

void ErrorCheckingTimedMutex::Unlock() {
	const int kRetCode = pthread_mutex_unlock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EPERM:
			CPPDEVTK_LOG_ERROR("failed to unlock error checking timed mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock error checking timed mutex (EPERM: the current thread does not own the mutex)");
			//throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode),
			//		"failed to unlock error checking timed mutex (EPERM: the current thread does not own the mutex)");
			return;
		default:
			CPPDEVTK_LOG_ERROR("failed to unlock error checking timed mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			CPPDEVTK_ASSERT(0 && "pthread_mutex_unlock() failed with undocumented error code for error checking timed mutex");
			return;
	}
}

ErrorCheckingTimedMutex::NativeHandleType ErrorCheckingTimedMutex::GetNativeHandle() {
	return &mutex_;
}


#endif	// (CPPDEVTK_HAVE_PTHREADS)


RecursiveTimedMutex::RecursiveTimedMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_RECURSIVE);
}

RecursiveTimedMutex::~RecursiveTimedMutex() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_destroy(&mutex_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("failed to destroy recursive timed mutex; error code: "
				<< MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(kRetCode != EINTR);
		CPPDEVTK_ASSERT(kRetCode != EINVAL);
		CPPDEVTK_ASSERT((kRetCode != EBUSY)
				&& "logic error: attempt to destroy recursive timed mutex while it is locked or referenced");
		CPPDEVTK_ASSERT(0 && "pthread_mutex_destroy() failed with undocumented error code for recursive timed mutex");
	}
}

void RecursiveTimedMutex::Lock() {
	const int kRetCode = pthread_mutex_lock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EAGAIN:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock recursive timed mutex "
					"(EAGAIN: the maximum number of recursive locks for mutex has been exceeded)");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to lock recursive timed mutex");
	}
}

bool RecursiveTimedMutex::TryLock() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_mutex_trylock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		case EAGAIN:
			CPPDEVTK_LOG_ERROR("failed to 'try lock' recursive timed mutex "
					"(EAGAIN: the maximum number of recursive locks for mutex has been exceeded)");
			return false;
		default:
			CPPDEVTK_LOG_ERROR("failed to 'try lock' recursive timed mutex; error code: "
					<< MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			return false;
	}
}

bool RecursiveTimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	const timespec kAbsTime = RelTimeToAbsTime(relTime);
	const int kRetCode = pthread_mutex_timedlock(&mutex_, &kAbsTime);
	switch (kRetCode) {
		case ESUCCESS:
			return true;
		case ETIMEDOUT:
			return false;
		case EAGAIN:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock for' recursive timed mutex "
					"(EAGAIN: the maximum number of recursive locks for mutex has been exceeded");
		default:
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to 'try lock for' recursive timed mutex");
	}
}

bool RecursiveTimedMutex::TryLockUntil(::std::time_t absTime) {
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = difftime(absTime, kCurrTime);
	
	return (kSeconds <= 0) ? TryLock() : TryLockFor(kSeconds * 1000);
}

void RecursiveTimedMutex::Unlock() {
	const int kRetCode = pthread_mutex_unlock(&mutex_);
	switch (kRetCode) {
		case ESUCCESS:
			return;
		case EPERM:
			CPPDEVTK_LOG_ERROR("failed to unlock recursive timed mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock recursive timed mutex (EPERM: the current thread does not own the mutex)");
			//throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode),
			//		"failed to unlock recursive timed mutex (EPERM: the current thread does not own the mutex)");
			return;
		default:
			CPPDEVTK_LOG_ERROR("failed to unlock recursive timed mutex; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(kRetCode != EINVAL);
			CPPDEVTK_ASSERT(0 && "pthread_mutex_unlock() failed with undocumented error code for recursive timed mutex");
			return;
	}
}

RecursiveTimedMutex::NativeHandleType RecursiveTimedMutex::GetNativeHandle() {
	return &mutex_;
}


namespace detail {


void InitializeMutex(pthread_mutex_t& mutex, int mutexType) {
	using ::std::bad_alloc;
	
	pthread_mutexattr_t mutexAttr;
	int retCode = pthread_mutexattr_init(&mutexAttr);
	switch (retCode) {
		case ESUCCESS:
			break;
		case ENOMEM:
			throw bad_alloc();
		default:
			CPPDEVTK_ASSERT(retCode != EINTR);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode), "failed to initialize mutex attribute");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&mutexAttr)) {
		const int kRetCode = pthread_mutexattr_destroy(&mutexAttr);
		if (kRetCode != ESUCCESS) {
			CPPDEVTK_LOG_WARN("failed to destroy mutex attribute; error code: " << MakeSystemErrorCode(kRetCode).ToString());
			CPPDEVTK_ASSERT(kRetCode != EINTR);
			CPPDEVTK_ASSERT(0 && "pthread_mutexattr_destroy() failed with with undocumented error code");
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
#	if (!CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
	retCode = pthread_mutexattr_setrobust(&mutexAttr, PTHREAD_MUTEX_ROBUST);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode), "failed to set mutex attribute robust");
	}
#	endif
	
	retCode = pthread_mutexattr_settype(&mutexAttr, mutexType);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode), "failed to set mutex attribute type");
	}
	
	retCode = pthread_mutex_init(&mutex, &mutexAttr);
	switch (retCode) {
		case ESUCCESS:
			return;
		case ENOMEM:
			throw bad_alloc();
		default:
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EBUSY);
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode), "failed to initialize mutex");
	}
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))
