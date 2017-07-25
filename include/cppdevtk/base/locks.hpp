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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_MUTEX_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/mutex.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_LOCKS_HPP_INCLUDED_
#define CPPDEVTK_BASE_LOCKS_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "cassert.hpp"
#include "dbc.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"
#if (!CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
#include "system_exception.hpp"
#include "logger.hpp"
#include "unused.hpp"
#endif

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
struct CPPDEVTK_BASE_API DeferLock {};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Try to acquire ownership of the mutex without blocking.
/// \sa C++ 11, 30.4.2 Locks
struct CPPDEVTK_BASE_API TryToLock {};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Assume the calling thread has already obtained mutex ownership and manage it.
/// \sa C++ 11, 30.4.2 Locks
struct CPPDEVTK_BASE_API AdoptLock {};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.2.1 Class template lock_guard
template <class TMutex>
class LockGuard: private NonCopyable {
public:
	typedef TMutex MutexType;
	
	
	explicit LockGuard(MutexType& mutex);
	LockGuard(MutexType& mutex, AdoptLock);
	~LockGuard();
private:
	MutexType& mutex_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.2.2 Class template unique_lock
template <class TMutex>
class UniqueLock: private NonCopyable {
public:
	typedef TMutex MutexType;
	typedef void (UniqueLock::*BoolType)();
	
	
	UniqueLock();
	explicit UniqueLock(MutexType& mutex);
	UniqueLock(MutexType& mutex, DeferLock);
	UniqueLock(MutexType& mutex, TryToLock);
	UniqueLock(MutexType& mutex, AdoptLock);
	UniqueLock(MutexType& mutex, int relTime);
	UniqueLock(MutexType& mutex, ::std::time_t absTime);
	~UniqueLock();
	
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
	
	MutexType* Release();
	
	bool OwnsLock() const;
	operator BoolType() const;
	bool operator!() const;
	
	MutexType* GetMutex() const;
	
	void Swap(UniqueLock& other);
private:
	MutexType* pMutex_;
	bool ownsLock_;
};


template <class TMutex>
void swap(UniqueLock<TMutex>& x, UniqueLock<TMutex>& y);


/// @}	// lock
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

template <class TMutex>
inline LockGuard<TMutex>::LockGuard(MutexType& mutex): NonCopyable(), mutex_(mutex) {
	mutex_.Lock();
}

template <class TMutex>
inline LockGuard<TMutex>::LockGuard(MutexType& mutex, AdoptLock): NonCopyable(), mutex_(mutex) {}

template <class TMutex>
#if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
inline
#endif
LockGuard<TMutex>::~LockGuard() {
#	if (!CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
	try {
#	endif
		mutex_.Unlock();
#	if (!CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
	}
	catch (const SystemException& exc) {
		CPPDEVTK_LOG_WARN("failed to unlock mutex: " << exc.ToString());
		SuppressUnusedWarning(exc);
	}
#	endif
}


template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(): NonCopyable(), pMutex_(NULL), ownsLock_(false) {}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex): NonCopyable(), pMutex_(&mutex), ownsLock_(false) {
	Lock();
}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, DeferLock): NonCopyable(),
		pMutex_(&mutex), ownsLock_(false) {}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, TryToLock): NonCopyable(),
		pMutex_(&mutex), ownsLock_(false) {
	TryLock();
}

template <class TMutex>
inline UniqueLock<TMutex>::UniqueLock(MutexType& mutex, AdoptLock): NonCopyable(),
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
#if (CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
inline
#endif
UniqueLock<TMutex>::~UniqueLock() {
	if (OwnsLock()) {
#		if (!CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
		try {
#		endif
			Unlock();
#		if (!CPPDEVTK_MUTEX_HAVE_NOTHROW_UNLOCK)
		}
		catch (const SystemException& exc) {
			CPPDEVTK_LOG_WARN("failed to unlock mutex: " << exc.ToString());
			SuppressUnusedWarning(exc);
		}
#		endif
	}
}

template <class TMutex>
inline void UniqueLock<TMutex>::Lock() {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXC_WA("lock owns lock");
	}
	
	pMutex_->Lock();
	ownsLock_ = true;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::TryLock() {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXC_WA("lock owns lock");
	}
		
	ownsLock_ = pMutex_->TryLock();
	return ownsLock_;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::TryLockFor(int relTime) {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXC_WA("lock owns lock");
	}
	
	ownsLock_ = pMutex_->TryLockFor(relTime);
	return ownsLock_;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::TryLockUntil(::std::time_t absTime) {
	if (pMutex_ == NULL) {
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "pMutex_ is NULL");
	}
	if (ownsLock_) {
		throw CPPDEVTK_DEADLOCK_EXC_WA("lock owns lock");
	}
	
	ownsLock_ = pMutex_->TryLockUntil(absTime);
	return ownsLock_;
}

template <class TMutex>
inline void UniqueLock<TMutex>::Unlock() {
	if (!ownsLock_) {
		throw CPPDEVTK_LOCK_EXC_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "does not owns lock");
	}
	if (pMutex_ == NULL) {
		throw CPPDEVTK_SYS_EXC_W_EC_WA(
				MakeErrorCode(::cppdevtk::base::errc::operation_not_permitted), "internal error: pMutex_ is NULL");
	}
	
	pMutex_->Unlock();
	ownsLock_ = false;
}

template <class TMutex>
inline typename UniqueLock<TMutex>::MutexType* UniqueLock<TMutex>::Release() {
	MutexType* pRetValue = pMutex_;
	
	pMutex_ = NULL;
	ownsLock_ = false;
	
	return pRetValue;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::OwnsLock() const {
	return ownsLock_;
}

template <class TMutex>
inline UniqueLock<TMutex>::operator BoolType() const {
	return OwnsLock() ? &UniqueLock::Lock : NULL;
}

template <class TMutex>
inline bool UniqueLock<TMutex>::operator!() const {
	return !OwnsLock();
}

template <class TMutex>
inline typename UniqueLock<TMutex>::MutexType* UniqueLock<TMutex>::GetMutex() const {
	return pMutex_;
}

template <class TMutex>
inline void UniqueLock<TMutex>::Swap(UniqueLock& other) {
	using ::std::swap;
	
	if (this != &other) {
		swap(pMutex_, other.pMutex_);
		swap(ownsLock_, other.ownsLock_);
	}
}


template <class TMutex>
inline void swap(UniqueLock<TMutex>& x, UniqueLock<TMutex>& y) {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_LOCKS_HPP_INCLUDED_
