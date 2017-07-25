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


#include <cppdevtk/base/mutex.hpp>


#if (CPPDEVTK_HAVE_PTHREADS)


#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/lock_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/logger.hpp>

#include <cstddef>
#include <ctime>


#if (CPPDEVTK_PLATFORM_MACOSX)

#include <sys/time.h>


#ifdef CLOCK_REALTIME
#	error "CLOCK_REALTIME defined"
#else
#	define CLOCK_REALTIME 1
#endif
#ifdef CLOCK_MONOTONIC
#	error "CLOCK_MONOTONIC defined"
#else
#	define CLOCK_MONOTONIC 2
#endif


typedef int clockid_t;


int clock_gettime(clockid_t clk_id, struct timespec* tp);

#endif	// (CPPDEVTK_PLATFORM_MACOSX)

#if (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)

int pthread_mutex_timedlock(pthread_mutex_t* mutex, const struct timespec* abstime);

#endif


using ::std::time_t;
using ::std::time;
using ::std::difftime;


// NOTE:
// - C++ 11 std, 30.4.1 Mutex requirements: Construction and destruction of an object of a mutex type need not be thread-safe;
// other synchronization should be used to ensure that mutex objects are initialized and visible to other threads.
// - C++ 11 std, 30.4.1.2.1 Class mutex: A program may deadlock if the thread that owns a mutex object calls lock() on that object.
// If the implementation can detect the deadlock, a resource_deadlock_would_occur error condition may be observed.
// This is why we use PTHREAD_MUTEX_NORMAL and not PTHREAD_MUTEX_DEFAULT.


namespace cppdevtk {
namespace base {


namespace detail {


static void InitializeMutex(pthread_mutex_t& mutex, int mutexType);
static bool RelTimeToTimeSpec(int relTime, timespec& timeSpec);


}	// namespace detail


#if (!(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS))


Mutex::Mutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_NORMAL);
}

Mutex::~Mutex() {
	int retCode = pthread_mutex_destroy(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_LOG_WARN("failed to destroy mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
	}
}

void Mutex::Lock() {
	int retCode = pthread_mutex_lock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXC();
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to lock mutex");
	}
}

bool Mutex::TryLock() {
	int retCode = pthread_mutex_trylock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			CPPDEVTK_LOG_ERROR("failed to try lock mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

void Mutex::Unlock() {
	int retCode = pthread_mutex_unlock(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_ASSERT(retCode != EPERM);
#		if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
		CPPDEVTK_LOG_WARN("failed to unlock mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
#		else
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to unlock mutex");
#		endif
	}
}

Mutex::NativeHandleType Mutex::GetNativeHandle() {
	return &mutex_;
}


#endif	// !(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS)


ErrorCheckingMutex::ErrorCheckingMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_ERRORCHECK);
}

ErrorCheckingMutex::~ErrorCheckingMutex() {
	int retCode = pthread_mutex_destroy(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_LOG_WARN("failed to destroy error checking mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
	}
}

void ErrorCheckingMutex::Lock() {
	int retCode = pthread_mutex_lock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXC();
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to lock error checking mutex");
	}
}

bool ErrorCheckingMutex::TryLock() {
	int retCode = pthread_mutex_trylock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			CPPDEVTK_LOG_ERROR("failed to try lock error checking mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

void ErrorCheckingMutex::Unlock() {
	int retCode = pthread_mutex_unlock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EPERM:
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_ERROR("failed to unlock error checking mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock error checking mutex (EPERM: the current thread does not own the mutex)");
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode),
					"failed to unlock error checking mutex: the current thread does not own the mutex");
#			endif
		default:
			CPPDEVTK_ASSERT(retCode != EINTR);
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_WARN("failed to unlock error checking mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to unlock error checking mutex");
#			endif
	}
}

ErrorCheckingMutex::NativeHandleType ErrorCheckingMutex::GetNativeHandle() {
	return &mutex_;
}


#if (!(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS))


RecursiveMutex::RecursiveMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_RECURSIVE);
}

RecursiveMutex::~RecursiveMutex() {
	int retCode = pthread_mutex_destroy(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_LOG_WARN("failed to destroy recursive mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
	}
}

void RecursiveMutex::Lock() {
	int retCode = pthread_mutex_lock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EAGAIN:
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode),
					"failed to lock recursive mutex: the maximum number of recursive locks for mutex has been exceeded");
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EDEADLK);
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to lock recursive mutex");
	}
}

bool RecursiveMutex::TryLock() {
	int retCode = pthread_mutex_trylock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case EAGAIN:
			CPPDEVTK_LOG_ERROR("failed to lock recursive mutex (EAGAIN): "
					"the maximum number of recursive locks for mutex has been exceeded");
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EBUSY);
			CPPDEVTK_LOG_ERROR("failed to try lock recursive mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

void RecursiveMutex::Unlock() {
	int retCode = pthread_mutex_unlock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EPERM:
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_ERROR("failed to unlock recursive mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock recursive mutex (EPERM: the current thread does not own the mutex)");
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode),
					"failed to unlock recursive mutex: the current thread does not own the mutex");
#			endif
		default:
			CPPDEVTK_ASSERT(retCode != EINTR);
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_WARN("failed to unlock recursive mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to unlock recursive mutex");
#			endif
	}
}

RecursiveMutex::NativeHandleType RecursiveMutex::GetNativeHandle() {
	return &mutex_;
}


#endif	// !(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS)


#if (!(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS) || CPPDEVTK_PLATFORM_ANDROID)


TimedMutex::TimedMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_NORMAL);
}

TimedMutex::~TimedMutex() {
	int retCode = pthread_mutex_destroy(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_LOG_WARN("failed to destroy timed mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
	}
}

void TimedMutex::Lock() {
	int retCode = pthread_mutex_lock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXC();
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to lock timed mutex");
	}
}

bool TimedMutex::TryLock() {
	int retCode = pthread_mutex_trylock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			CPPDEVTK_LOG_ERROR("failed to try lock timed mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

bool TimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	timespec timeSpec;
	if (!detail::RelTimeToTimeSpec(relTime, timeSpec)) {
		return false;
	}
	
	int retCode = pthread_mutex_timedlock(&mutex_, &timeSpec);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case ETIMEDOUT:
			return false;
		case EDEADLK:
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' timed mutex: EDEADLK");
			CPPDEVTK_ASSERT(0 && "failed to 'try lock for' timed mutex: EDEADLK");
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

bool TimedMutex::TryLockUntil(::std::time_t absTime) {
	time_t currTime = time(NULL);
	if (currTime == (time_t)-1) {
		CPPDEVTK_LOG_ERROR("failed to get time; error code: " << MakeSystemErrorCode(errno).ToString());
		return false;
	}
	
	time_t seconds = difftime(absTime, currTime);
	if (seconds <= 0) {
		return TryLock();
	}
	
	return TryLockFor(seconds * 1000);
}

void TimedMutex::Unlock() {
	int retCode = pthread_mutex_unlock(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_ASSERT(retCode != EPERM);
#		if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
		CPPDEVTK_LOG_WARN("failed to unlock timed mutex; error code: " << MakeSystemErrorCode(retCode).ToString());
#		else
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to unlock timed mutex");
#		endif
	}
}

TimedMutex::NativeHandleType TimedMutex::GetNativeHandle() {
	return &mutex_;
}

#endif	// (!(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS) || CPPDEVTK_PLATFORM_ANDROID)


ErrorCheckingTimedMutex::ErrorCheckingTimedMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_ERRORCHECK);
}

ErrorCheckingTimedMutex::~ErrorCheckingTimedMutex() {
	int retCode = pthread_mutex_destroy(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_LOG_WARN("failed to destroy error checking timed mutex; error code: "
				<< MakeSystemErrorCode(retCode).ToString());
	}
}

void ErrorCheckingTimedMutex::Lock() {
	int retCode = pthread_mutex_lock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EDEADLK:
			throw CPPDEVTK_DEADLOCK_EXC();
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to lock error checking timed mutex");
	}
}

bool ErrorCheckingTimedMutex::TryLock() {
	int retCode = pthread_mutex_trylock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case EBUSY:
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			CPPDEVTK_LOG_ERROR("failed to try lock error checking timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

bool ErrorCheckingTimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	timespec timeSpec;
	if (!detail::RelTimeToTimeSpec(relTime, timeSpec)) {
		return false;
	}
	
	int retCode = pthread_mutex_timedlock(&mutex_, &timeSpec);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case ETIMEDOUT:
			return false;
		case EDEADLK:
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' error checking timed mutex: EDEADLK");
			CPPDEVTK_ASSERT(0 && "failed to 'try lock for' error checking timed mutex: EDEADLK");
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EAGAIN);
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' error checking timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

bool ErrorCheckingTimedMutex::TryLockUntil(::std::time_t absTime) {
	time_t currTime = time(NULL);
	if (currTime == (time_t)-1) {
		CPPDEVTK_LOG_ERROR("failed to get time; error code: " << MakeSystemErrorCode(errno).ToString());
		return false;
	}
	
	time_t seconds = difftime(absTime, currTime);
	if (seconds <= 0) {
		return TryLock();
	}
	
	return TryLockFor(seconds * 1000);
}

void ErrorCheckingTimedMutex::Unlock() {
	int retCode = pthread_mutex_unlock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EPERM:
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_ERROR("failed to unlock error checking timed mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock error checking timed mutex (EPERM: the current thread does not own the mutex)");
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode),
					"failed to unlock error checking timed mutex: the current thread does not own the mutex");
#			endif
		default:
			CPPDEVTK_ASSERT(retCode != EINTR);
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_WARN("failed to unlock error checking timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to unlock error checking timed mutex");
#			endif
	}
}

ErrorCheckingTimedMutex::NativeHandleType ErrorCheckingTimedMutex::GetNativeHandle() {
	return &mutex_;
}


#if (!(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS) || CPPDEVTK_PLATFORM_ANDROID)


RecursiveTimedMutex::RecursiveTimedMutex(): NonCopyable(), mutex_() {
	detail::InitializeMutex(mutex_, PTHREAD_MUTEX_RECURSIVE);
}

RecursiveTimedMutex::~RecursiveTimedMutex() {
	int retCode = pthread_mutex_destroy(&mutex_);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_LOG_WARN("failed to destroy recursive timed mutex; error code: "
				<< MakeSystemErrorCode(retCode).ToString());
	}
}

void RecursiveTimedMutex::Lock() {
	int retCode = pthread_mutex_lock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EAGAIN:
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode),
					"failed to lock recursive timed mutex: the maximum number of recursive locks for mutex has been exceeded");
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EDEADLK);
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to lock recursive timed mutex");
	}
}

bool RecursiveTimedMutex::TryLock() {
	int retCode = pthread_mutex_trylock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case EAGAIN:
			CPPDEVTK_LOG_ERROR("failed to lock recursive timed mutex (EAGAIN): "
					"the maximum number of recursive locks for mutex has been exceeded");
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_ASSERT(retCode != EBUSY);
			CPPDEVTK_LOG_ERROR("failed to try lock recursive timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

bool RecursiveTimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	timespec timeSpec;
	if (!detail::RelTimeToTimeSpec(relTime, timeSpec)) {
		return false;
	}
	
	int retCode = pthread_mutex_timedlock(&mutex_, &timeSpec);
	switch (retCode) {
		case ESUCCESS:
			return true;
		case ETIMEDOUT:
			return false;
		case EDEADLK:
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' recursive timed mutex: EDEADLK");
			CPPDEVTK_ASSERT(0 && "failed to 'try lock for' recursive timed mutex: EDEADLK");
			return false;
		case EAGAIN:
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' recursive timed mutex (EAGAIN): "
					"the maximum number of recursive locks for mutex has been exceeded");
			return false;
		default:
#			if (CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
			CPPDEVTK_ASSERT(retCode != EOWNERDEAD);
			CPPDEVTK_ASSERT(retCode != ENOTRECOVERABLE);
#			endif
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_ASSERT(retCode != EINVAL);
			CPPDEVTK_LOG_ERROR("failed to 'try lock for' recursive timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
	}
}

bool RecursiveTimedMutex::TryLockUntil(::std::time_t absTime) {
	time_t currTime = time(NULL);
	if (currTime == (time_t)-1) {
		CPPDEVTK_LOG_ERROR("failed to get time; error code: " << MakeSystemErrorCode(errno).ToString());
		return false;
	}
	
	time_t seconds = difftime(absTime, currTime);
	if (seconds <= 0) {
		return TryLock();
	}
	
	return TryLockFor(seconds * 1000);
}

void RecursiveTimedMutex::Unlock() {
	int retCode = pthread_mutex_unlock(&mutex_);
	switch (retCode) {
		case ESUCCESS:
			break;
		case EPERM:
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_ERROR("failed to unlock recursive timed mutex (EPERM: the current thread does not own the mutex)");
			CPPDEVTK_ASSERT(0 && "failed to unlock recursive timed mutex (EPERM: the current thread does not own the mutex)");
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode),
					"failed to unlock recursive timed mutex: the current thread does not own the mutex");
#			endif
		default:
			CPPDEVTK_ASSERT(retCode != EINTR);
#			if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
			CPPDEVTK_LOG_WARN("failed to unlock recursive timed mutex; error code: "
					<< MakeSystemErrorCode(retCode).ToString());
			break;
#			else
			throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to unlock recursive timed mutex");
#			endif
	}
}

RecursiveTimedMutex::NativeHandleType RecursiveTimedMutex::GetNativeHandle() {
	return &mutex_;
}


#endif	// (!(CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS) || CPPDEVTK_PLATFORM_ANDROID)


namespace detail {


void InitializeMutex(pthread_mutex_t& mutex, int mutexType) {
	pthread_mutexattr_t mutexAttr;
	int retCode = pthread_mutexattr_init(&mutexAttr);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to initialize mutex attribute");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&mutexAttr)) {
		int retCode = pthread_mutexattr_destroy(&mutexAttr);
		if (retCode != ESUCCESS) {
			CPPDEVTK_ASSERT(retCode != EINTR);
			CPPDEVTK_LOG_WARN("failed to destroy mutex attribute; error code: " << MakeSystemErrorCode(retCode).ToString());
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
#	if (!CPPDEVTK_HAVE_PTHREAD_NONROBUST_MUTEX)
	retCode = pthread_mutexattr_setrobust(&mutexAttr, PTHREAD_MUTEX_ROBUST);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_ASSERT(retCode != EINVAL);
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to set mutex attribute robust");
	}
#	endif
	
	retCode = pthread_mutexattr_settype(&mutexAttr, mutexType);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_ASSERT(retCode != EINVAL);
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to set mutex attribute type");
	}
	
	retCode = pthread_mutex_init(&mutex, &mutexAttr);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EINTR);
		CPPDEVTK_ASSERT(retCode != EINVAL);
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(MakeSystemErrorCode(retCode), "failed to initialize mutex");
	}
}

bool RelTimeToTimeSpec(int relTime, timespec& timeSpec) {
	int retCode = clock_gettime(CLOCK_REALTIME, &timeSpec);
	if (retCode != ESUCCESS) {
		CPPDEVTK_ASSERT(retCode != EFAULT);
		CPPDEVTK_ASSERT(retCode != EINVAL);
		CPPDEVTK_LOG_ERROR("failed to get time; error code: " << MakeSystemErrorCode(retCode).ToString());
		return false;
	}
	
	timeSpec.tv_sec += relTime / 1000;
	timeSpec.tv_nsec += (relTime % 1000) * 1000000;
	if (timeSpec.tv_nsec >= 1000000000) {
		timeSpec.tv_sec += timeSpec.tv_nsec / 1000000000;
		timeSpec.tv_nsec = timeSpec.tv_nsec % 1000000000;
	}
	
	return true;
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_PLATFORM_MACOSX)

int clock_gettime(clockid_t clk_id, struct timespec* tp) {
	int retCode = -1;
	
	if (clk_id != CLOCK_REALTIME) {
		errno = EINVAL;
		return retCode;
	}
	
	struct timeval tv = {0, 0};
	retCode = gettimeofday(&tv, NULL);
	if (retCode != ESUCCESS) {
		return retCode;
	}
	
	tp->tv_sec  = tv.tv_sec;
	tp->tv_nsec = tv.tv_usec * 1000;
	
	return retCode;
}

#endif	// (CPPDEVTK_PLATFORM_MACOSX)

#if (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)

int pthread_mutex_timedlock(pthread_mutex_t* mutex, const struct timespec* abstime) {
	int retCode = ESUCCESS;
	
	struct timespec nanoSlept;
	nanoSlept.tv_sec = 0;
	nanoSlept.tv_nsec = 0;
	
	while ((retCode = pthread_mutex_trylock(mutex)) == EBUSY) {
		const long kMaxTvNSec = 999999999;
		
		if (/* (abstime->tv_sec < 0) || */ ((abstime->tv_nsec < 0) || (abstime->tv_nsec > kMaxTvNSec))) {
			retCode = EINVAL;
			break;
		}
		
		timespec toNanoSleep;
		toNanoSleep.tv_sec = 0;
		toNanoSleep.tv_nsec = 10000000;
		
		if (nanoSlept.tv_nsec + toNanoSleep.tv_nsec > kMaxTvNSec) {
			nanoSlept.tv_sec += 1;
			nanoSlept.tv_nsec = (nanoSlept.tv_nsec + toNanoSleep.tv_nsec) - kMaxTvNSec;
		}
		else {
			nanoSlept.tv_nsec += toNanoSleep.tv_nsec;
		}
		if ((nanoSlept.tv_sec > abstime->tv_sec) ||
				((nanoSlept.tv_sec == abstime->tv_sec) && (nanoSlept.tv_nsec >= abstime->tv_nsec))) {
			retCode = ETIMEDOUT;
			break;
		}
		
		int nanoSleepRetCode = ESUCCESS;
		do {
			nanoSleepRetCode = nanosleep(&toNanoSleep, &toNanoSleep);
			CPPDEVTK_ASSERT((nanoSleepRetCode == ESUCCESS) || ((nanoSleepRetCode == -1) && (errno != EINVAL)));
		}
		while (nanoSleepRetCode != ESUCCESS);
	}
	
	return retCode;
}

#endif	// (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)


#endif	// CPPDEVTK_HAVE_PTHREADS
