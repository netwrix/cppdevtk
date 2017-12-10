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


#ifndef CPPDEVTK_BASE_CONDITION_VARIABLE_HPP_INCLUDED_
#define CPPDEVTK_BASE_CONDITION_VARIABLE_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "mutex.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"
#include "time_utils.hpp"
#include "dbc.hpp"

#if (CPPDEVTK_HAVE_PTHREADS)
#include <pthread.h>
#elif (CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
#include <condition_variable>
#else
#include <QtCore/QWaitCondition>
#endif

#include <ctime>
#include <new>
#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {


namespace cv_status {


/// \sa C++ 11, 30.5 Condition variables, Header condition_variable synopsis
enum cv_status_t {
	no_timeout,
	timeout
};


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.5.1 Class condition_variable
class CPPDEVTK_BASE_API ConditionVariable: private NonCopyable {
public:
#	if (CPPDEVTK_HAVE_PTHREADS)
	typedef pthread_cond_t* NativeHandleType;
#	elif (CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
	typedef ::std::condition_variable::native_handle_type NativeHandleType;
#	endif
	
#	if (CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
	NativeHandleType GetNativeHandle();
#	endif
	
	
	ConditionVariable();
	~ConditionVariable() CPPDEVTK_NOEXCEPT;
	
	
	void NotifyOne() CPPDEVTK_NOEXCEPT;
	void NotifyAll() CPPDEVTK_NOEXCEPT;
	
	
	void Wait(UniqueLock<Mutex>& uniqueLock);	///< \attention Interruption point
	
	template <class Predicate>
	void Wait(UniqueLock<Mutex>& uniqueLock, Predicate predicate);	///< \attention Interruption point
	
	
	/// \arg relTime Relative timeout, in milliseconds.
	/// \attention Interruption point
	cv_status::cv_status_t WaitFor(UniqueLock<Mutex>& uniqueLock, int relTime);
	
	/// \arg relTime Relative timeout, in milliseconds.
	/// \attention Interruption point
	template <class Predicate>
	bool WaitFor(UniqueLock<Mutex>& uniqueLock, int relTime, Predicate predicate);
	
	
	/// \arg absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
	/// \attention Interruption point
	cv_status::cv_status_t WaitUntil(UniqueLock<Mutex>& uniqueLock, ::std::time_t absTime);
	
	/// \arg absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
	/// \attention Interruption point
	template <class Predicate>
	bool WaitUntil(UniqueLock<Mutex>& uniqueLock, ::std::time_t absTime, Predicate predicate);
private:
	void DoWait(UniqueLock<Mutex>& uniqueLock);
	cv_status::cv_status_t DoWaitFor(UniqueLock<Mutex>& uniqueLock, int relTime);
	
	
#	if (CPPDEVTK_HAVE_PTHREADS)
	pthread_cond_t conditionVariable_;
#	elif (CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
	::std::condition_variable conditionVariable_;
#	else
	QWaitCondition conditionVariable_;
#	endif
};


// TODO
//CPPDEVTK_BASE_API void NotifyAllAtThreadExit(ConditionVariable& condVar, UniqueLock<Mutex> uniqueLock);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.5.2 Class ConditionVariableAny
class CPPDEVTK_BASE_API ConditionVariableAny: private NonCopyable {
public:
	ConditionVariableAny();
	~ConditionVariableAny() CPPDEVTK_NOEXCEPT;
	
	void NotifyOne() CPPDEVTK_NOEXCEPT;
	void NotifyAll() CPPDEVTK_NOEXCEPT;
	
	template <class Lock>
	void Wait(Lock& lock);	///< \attention Interruption point
	
	template <class Lock, class Predicate>
	void Wait(Lock& lock, Predicate predicate);	///< \attention Interruption point
	
	template <class Lock>
	cv_status::cv_status_t WaitFor(Lock& lock, int relTime);	///< \attention Interruption point
	
	template <class Lock, class Predicate>
	bool WaitFor(Lock& lock, int relTime, Predicate predicate);	///< \attention Interruption point
	
	template <class Lock>
	cv_status::cv_status_t WaitUntil(Lock& lock, ::std::time_t absTime);	///< \attention Interruption point
	
	template <class Lock, class Predicate>
	bool WaitUntil(Lock& lock, ::std::time_t absTime, Predicate predicate);	///< \attention Interruption point
private:
	CPPDEVTK_TR1_NS::shared_ptr<Mutex> pMtx_;
	ConditionVariable conditionVariable_;
};




}	// namespace base
}	// namespace cppdevtk


#include "thread.hpp"


namespace cppdevtk {
namespace base {




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.


template <class Predicate>
void ConditionVariable::Wait(UniqueLock<Mutex>& uniqueLock, Predicate predicate) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(uniqueLock.OwnsLock(), "uniqueLock must own mutex");
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	while (!predicate()) {
		Wait(uniqueLock);
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
}

template <class Predicate>
bool ConditionVariable::WaitFor(UniqueLock<Mutex>& uniqueLock, int relTime, Predicate predicate) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(uniqueLock.OwnsLock(), "uniqueLock must own mutex");
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	using this_thread::InterruptionPoint;
	
	InterruptionPoint();
#	endif
	
	while (!predicate()) {
		if (WaitFor(uniqueLock, relTime) == ::cppdevtk::base::cv_status::timeout) {
#			if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			const bool kRetValue = predicate();
			InterruptionPoint();
			return kRetValue;
#			else
			return predicate();
#			endif
		}
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionPoint();
#	endif
	
	return true;
}


inline cv_status::cv_status_t ConditionVariable::WaitUntil(UniqueLock<Mutex>& uniqueLock, ::std::time_t absTime) {
	using ::std::time_t;
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	
	return WaitFor(uniqueLock, (kSeconds * 1000));
}

template <class Predicate>
inline bool ConditionVariable::WaitUntil(UniqueLock<Mutex>& uniqueLock, ::std::time_t absTime, Predicate predicate) {
	using ::std::time_t;
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	
	return WaitFor(uniqueLock, (kSeconds * 1000), predicate);
}




inline ConditionVariableAny::ConditionVariableAny(): NonCopyable(), pMtx_(new Mutex()), conditionVariable_() {}

inline ConditionVariableAny::~ConditionVariableAny() CPPDEVTK_NOEXCEPT {}


template <class Lock>
void ConditionVariableAny::Wait(Lock& lock) {
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	CPPDEVTK_TR1_NS::shared_ptr<Mutex> pMtx = pMtx_;
	UniqueLock<Mutex> uniqueLock(*pMtx);
	
	lock.Unlock();
	
	try {
		conditionVariable_.Wait(uniqueLock);
	}
	catch (...) {
		if (uniqueLock.OwnsLock()) {
			try {
				uniqueLock.Unlock();
			}
			catch (...) {
				lock.Lock();
				throw;
			}
			
			lock.Lock();
			throw;
		}
		
		lock.Lock();
		throw;
	}
	
	try {
		uniqueLock.Unlock();
	}
	catch (...) {
		lock.Lock();
		throw;
	}
	
	lock.Lock();
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
}

template <class Lock, class Predicate>
inline void ConditionVariableAny::Wait(Lock& lock, Predicate predicate) {
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	while (!predicate()) {
		Wait(lock);
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
}


template <class Lock>
cv_status::cv_status_t ConditionVariableAny::WaitFor(Lock& lock, int relTime) {
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	CPPDEVTK_TR1_NS::shared_ptr<Mutex> pMtx = pMtx_;
	UniqueLock<Mutex> uniqueLock(*pMtx);
	
	lock.Unlock();
	
	try {
		conditionVariable_.WaitFor(uniqueLock, relTime);
	}
	catch (...) {
		if (uniqueLock.OwnsLock()) {
			try {
				uniqueLock.Unlock();
			}
			catch (...) {
				lock.Lock();
				throw;
			}
			
			lock.Lock();
			throw;
		}
		
		lock.Lock();
		throw;
	}
	
	try {
		uniqueLock.Unlock();
	}
	catch (...) {
		lock.Lock();
		throw;
	}
	
	lock.Lock();
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
}

template <class Lock, class Predicate>
inline bool ConditionVariableAny::WaitFor(Lock& lock, int relTime, Predicate predicate) {
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	using this_thread::InterruptionPoint;
	
	InterruptionPoint();
#	endif
	
	while (!predicate()) {
		if (WaitFor(lock, relTime) == ::cppdevtk::base::cv_status::timeout) {
#			if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			const bool kRetValue = predicate();
			InterruptionPoint();
			return kRetValue;
#			else
			return predicate();
#			endif
		}
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionPoint();
#	endif
	
	return true;
}


template <class Lock>
inline cv_status::cv_status_t ConditionVariableAny::WaitUntil(Lock& lock, ::std::time_t absTime) {
	using ::std::time_t;
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	
	return WaitFor(lock, (kSeconds * 1000));
}

template <class Lock, class Predicate>
inline bool ConditionVariableAny::WaitUntil(Lock& lock, ::std::time_t absTime, Predicate predicate) {
	using ::std::time_t;
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	
	return WaitFor(lock, (kSeconds * 1000), predicate);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CONDITION_VARIABLE_HPP_INCLUDED_
