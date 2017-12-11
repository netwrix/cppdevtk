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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_MUTEX_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/mutex.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_LOCKS_HPP_INCLUDED_
#define CPPDEVTK_BASE_LOCKS_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"
#include "cassert.hpp"
#include "dbc.hpp"
#include "logger.hpp"
#include "unused.hpp"

#include <ctime>
#include <cstddef>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup lock Lock classes
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Do not acquire ownership of the mutex.
/// \sa C++ 11, 30.4.2 Locks
struct CPPDEVTK_BASE_API DeferLockT {};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Try to acquire ownership of the mutex without blocking.
/// \sa C++ 11, 30.4.2 Locks
struct CPPDEVTK_BASE_API TryToLockT {};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Assume the calling thread has already obtained mutex ownership and manage it.
/// \sa C++ 11, 30.4.2 Locks
struct CPPDEVTK_BASE_API AdoptLockT {};


static const DeferLockT deferLock = {};
static const TryToLockT tryToLock = {};
static const AdoptLockT adoptLock = {};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.2.1 Class template lock_guard
template <class TMutex>
class LockGuard: private NonCopyable {
public:
	typedef TMutex MutexType;
	
	
	explicit LockGuard(MutexType& mutex);
	LockGuard(MutexType& mutex, AdoptLockT);
	~LockGuard() CPPDEVTK_NOEXCEPT;
private:
	MutexType& mutex_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.2.2 Class template unique_lock
template <class TMutex>
class UniqueLock: private NonCopyable {
#if (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_HAVE_CPP11_MUTEX)
	friend class ConditionVariable;
#endif
public:
	typedef TMutex MutexType;
	typedef void (UniqueLock::*BoolType)();
	
	
	UniqueLock() CPPDEVTK_NOEXCEPT;
	explicit UniqueLock(MutexType& mutex);
	UniqueLock(MutexType& mutex, DeferLockT) CPPDEVTK_NOEXCEPT;
	UniqueLock(MutexType& mutex, TryToLockT);
	UniqueLock(MutexType& mutex, AdoptLockT);
	UniqueLock(MutexType& mutex, int relTime);
	UniqueLock(MutexType& mutex, ::std::time_t absTime);
	~UniqueLock() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock();
	
	/// Attempts to obtain ownership of the mutex within the relative timeout.
	/// \arg relTime Relative timeout, in milliseconds.
	/// If it is <= 0, attempts to obtain ownership without blocking (as if by calling TryLock()).
	bool TryLockFor(int relTime);
	
	/// Attempts to obtain ownership of the mutex.
	/// \arg absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
	/// If it has already passed, attempts to obtain ownership without blocking (as if by calling TryLock()).
	bool TryLockUntil(::std::time_t absTime);
	
	void Unlock();
	
	MutexType* Release() CPPDEVTK_NOEXCEPT;
	
	bool OwnsLock() const CPPDEVTK_NOEXCEPT;
	operator BoolType() const CPPDEVTK_NOEXCEPT;
	bool operator!() const CPPDEVTK_NOEXCEPT;
	
	MutexType* GetMutex() const CPPDEVTK_NOEXCEPT;
	
	void Swap(UniqueLock& other) CPPDEVTK_NOEXCEPT;
private:
	MutexType* pMutex_;
	bool ownsLock_;
};


template <class TMutex>
void swap(UniqueLock<TMutex>& x, UniqueLock<TMutex>& y) CPPDEVTK_NOEXCEPT;


/// @}	// lock
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

template <class TMutex>
inline LockGuard<TMutex>::LockGuard(MutexType& mutex): NonCopyable(), mutex_(mutex) {
	mutex_.Lock();
}

template <class TMutex>
inline LockGuard<TMutex>::LockGuard(MutexType& mutex, AdoptLockT): NonCopyable(), mutex_(mutex) {}

template <class TMutex>
inline LockGuard<TMutex>::~LockGuard() CPPDEVTK_NOEXCEPT {
	mutex_.Unlock();
}


template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock() CPPDEVTK_NOEXCEPT: NonCopyable(), pMutex_(NULL), ownsLock_(false) {}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex): NonCopyable(), pMutex_(&mutex), ownsLock_(false) {
	Lock();
}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, DeferLockT) CPPDEVTK_NOEXCEPT: NonCopyable(),
		pMutex_(&mutex), ownsLock_(false) {}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, TryToLockT): NonCopyable(),
		pMutex_(&mutex), ownsLock_(false) {
	TryLock();
}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, AdoptLockT): NonCopyable(),
		pMutex_(&mutex), ownsLock_(true) {}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, int relTime): NonCopyable(),
		pMutex_(&mutex), ownsLock_(false) {
	TryLockFor(relTime);
}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, ::std::time_t absTime): NonCopyable(),
		pMutex_(&mutex), ownsLock_(false) {
	TryLockUntil(absTime);
}

template <class TMutex>
UniqueLock<TMutex>::~UniqueLock() CPPDEVTK_NOEXCEPT {
	if (OwnsLock()) {
		Unlock();
	}
}

template <class TMutex>
inline void UniqueLock<TMutex>::Lock() {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("lock owns lock");
	}
	
	pMutex_->Lock();
	ownsLock_ = true;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::TryLock() {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("lock owns lock");
	}
		
	ownsLock_ = pMutex_->TryLock();
	return ownsLock_;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::TryLockFor(int relTime) {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("lock owns lock");
	}
	
	ownsLock_ = pMutex_->TryLockFor(relTime);
	return ownsLock_;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::TryLockUntil(::std::time_t absTime) {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("lock owns lock");
	}
	
	ownsLock_ = pMutex_->TryLockUntil(absTime);
	return ownsLock_;
}

template <class TMutex>
inline void UniqueLock<TMutex>::Unlock() {
	if (!ownsLock_) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "does not owns lock");
	}
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "internal error: pMutex_ is NULL");
	}
	
	pMutex_->Unlock();
	ownsLock_ = false;
}

template <class TMutex>
inline typename UniqueLock<TMutex>::MutexType* UniqueLock<TMutex>::Release() CPPDEVTK_NOEXCEPT {
	MutexType* pRetValue = pMutex_;
	
	pMutex_ = NULL;
	ownsLock_ = false;
	
	return pRetValue;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::OwnsLock() const CPPDEVTK_NOEXCEPT {
	return ownsLock_;
}

template <class TMutex>
inline UniqueLock<TMutex>::operator BoolType() const CPPDEVTK_NOEXCEPT {
	return OwnsLock() ? &UniqueLock::Lock : NULL;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::operator!() const CPPDEVTK_NOEXCEPT {
	return !OwnsLock();
}

template <class TMutex>
inline typename UniqueLock<TMutex>::MutexType* UniqueLock<TMutex>::GetMutex() const CPPDEVTK_NOEXCEPT {
	return pMutex_;
}

template <class TMutex>
inline void UniqueLock<TMutex>::Swap(UniqueLock& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	
	if (this != &other) {
		swap(pMutex_, other.pMutex_);
		swap(ownsLock_, other.ownsLock_);
	}
}


template <class TMutex>
inline void swap(UniqueLock<TMutex>& x, UniqueLock<TMutex>& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_LOCKS_HPP_INCLUDED_
