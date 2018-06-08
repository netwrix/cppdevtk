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


// NOTE: Android does not have timed_mutex and recursive_timed_mutex so even if CPPDEVTK_HAVE_CPP11_MUTEX,
// pthread or Qt based mutex will be used


#ifndef CPPDEVTK_BASE_MUTEX_HPP_INCLUDED_
#define CPPDEVTK_BASE_MUTEX_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "locks.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"
#include "generic_locking_algorithms.hpp"
#include "unused.hpp"

#if (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))
#include <pthread.h>
#endif
#if (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_HAVE_CPP11_MUTEX)
#include <mutex>
#endif
#if (!(CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_MUTEX))
#include <QtCore/QMutex>
#endif

#include <ctime>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup mutex Mutex classes
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.1.2.1 Class mutex
class CPPDEVTK_BASE_API Mutex: private NonCopyable {
#if	(!CPPDEVTK_HAVE_PTHREADS)
	friend class ConditionVariable;
#	endif
public:
#	if (CPPDEVTK_HAVE_PTHREADS)
	typedef pthread_mutex_t* NativeHandleType;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX)
	typedef ::std::mutex::native_handle_type NativeHandleType;
#	endif
	
	
	Mutex();
	~Mutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock();
	void Unlock();
	
#	if (CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_MUTEX)
	NativeHandleType GetNativeHandle();
#	endif
private:
#	if (CPPDEVTK_HAVE_PTHREADS)
	pthread_mutex_t mutex_;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX)
	::std::mutex mutex_;
#	else
	QMutex mutex_;
#	endif
};


#if (CPPDEVTK_HAVE_PTHREADS)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ 11 std.
class CPPDEVTK_BASE_API ErrorCheckingMutex: private NonCopyable {
public:
	typedef pthread_mutex_t* NativeHandleType;
	
	
	ErrorCheckingMutex();
	~ErrorCheckingMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock();
	void Unlock();
	
	NativeHandleType GetNativeHandle();
private:
	pthread_mutex_t mutex_;
};

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.1.2.2 Class recursive_mutex
class CPPDEVTK_BASE_API RecursiveMutex: private NonCopyable {
public:
#	if (CPPDEVTK_HAVE_PTHREADS)
	typedef pthread_mutex_t* NativeHandleType;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX)
	typedef ::std::recursive_mutex::native_handle_type NativeHandleType;
#	endif
	
	
	RecursiveMutex();
	~RecursiveMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock() CPPDEVTK_NOEXCEPT;
	void Unlock();
	
#	if (CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_MUTEX)
	NativeHandleType GetNativeHandle();
#	endif
private:
#	if (CPPDEVTK_HAVE_PTHREADS)
	pthread_mutex_t mutex_;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX)
	::std::recursive_mutex mutex_;
#	else
	QMutex mutex_;
#	endif
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.1.3.1 Class timed_mutex
class CPPDEVTK_BASE_API TimedMutex: private NonCopyable {
public:
#	if (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))
	typedef pthread_mutex_t* NativeHandleType;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX	\
			&& (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_ANDROID && !CPPDEVTK_PLATFORM_MACOSX))
	typedef ::std::timed_mutex::native_handle_type NativeHandleType;
#	endif
	
	
	TimedMutex();
	~TimedMutex() CPPDEVTK_NOEXCEPT;
	
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
	
#	if ((CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))	\
			|| (CPPDEVTK_HAVE_CPP11_MUTEX && (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_ANDROID && !CPPDEVTK_PLATFORM_MACOSX)))
	NativeHandleType GetNativeHandle();
#	endif
private:
#	if (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))
	pthread_mutex_t mutex_;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_PLATFORM_ANDROID)
	::std::timed_mutex mutex_;
#	else
	QMutex mutex_;
#	endif
};


#if (CPPDEVTK_HAVE_PTHREADS)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ 11 std.
class CPPDEVTK_BASE_API ErrorCheckingTimedMutex: private NonCopyable {
public:
	typedef pthread_mutex_t* NativeHandleType;
	
	
	ErrorCheckingTimedMutex();
	~ErrorCheckingTimedMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock();
	bool TryLockFor(int relTime);
	bool TryLockUntil(::std::time_t absTime);
	void Unlock();
	
	NativeHandleType GetNativeHandle();
private:
	pthread_mutex_t mutex_;
};

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.4.1.3.2 Class recursive_timed_mutex
class CPPDEVTK_BASE_API RecursiveTimedMutex: private NonCopyable {
public:
#	if (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))
	typedef pthread_mutex_t* NativeHandleType;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX	\
			&& (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_ANDROID && !CPPDEVTK_PLATFORM_MACOSX))
	typedef ::std::recursive_timed_mutex::native_handle_type NativeHandleType;
#	endif
	
	
	RecursiveTimedMutex();
	~RecursiveTimedMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock() CPPDEVTK_NOEXCEPT;
	
	/// Attempts to obtain ownership of the mutex within the relative timeout.
	/// \arg relTime Relative timeout, in milliseconds.
	/// If it is <= 0, attempts to obtain ownership without blocking (as if by calling TryLock()).
	bool TryLockFor(int relTime);
	
	/// Attempts to obtain ownership of the mutex.
	/// \arg absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
	/// If it has already passed, attempts to obtain ownership without blocking (as if by calling TryLock()).
	bool TryLockUntil(::std::time_t absTime);
	
	void Unlock();
	
#	if ((CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))	\
			|| (CPPDEVTK_HAVE_CPP11_MUTEX && (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_ANDROID && !CPPDEVTK_PLATFORM_MACOSX)))
	NativeHandleType GetNativeHandle();
#	endif
private:
#	if (CPPDEVTK_HAVE_PTHREADS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_PLATFORM_ANDROID))
	pthread_mutex_t mutex_;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_PLATFORM_ANDROID)
	::std::recursive_timed_mutex mutex_;
#	else
	QMutex mutex_;
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ std.
class CPPDEVTK_BASE_API NullMutex: private NonCopyable {
public:
	NullMutex();
	~NullMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock();
	void Unlock();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ std.
class CPPDEVTK_BASE_API NullRecursiveMutex: private NonCopyable {
public:
	NullRecursiveMutex();
	~NullRecursiveMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock() CPPDEVTK_NOEXCEPT;
	void Unlock();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ std.
class CPPDEVTK_BASE_API NullTimedMutex: private NonCopyable {
public:
	NullTimedMutex();
	~NullTimedMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock();
	bool TryLockFor(int relTime);
	bool TryLockUntil(::std::time_t absTime);
	void Unlock();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ std.
class CPPDEVTK_BASE_API NullRecursiveTimedMutex: private NonCopyable {
public:
	NullRecursiveTimedMutex();
	~NullRecursiveTimedMutex() CPPDEVTK_NOEXCEPT;
	
	void Lock();
	bool TryLock() CPPDEVTK_NOEXCEPT;
	bool TryLockFor(int relTime);
	bool TryLockUntil(::std::time_t absTime);
	void Unlock();
};


#if (CPPDEVTK_HAVE_PTHREADS && !defined(NDEBUG))
typedef ErrorCheckingMutex DefaultMutex;
typedef ErrorCheckingTimedMutex DefaultTimedMutex;
#else
typedef Mutex DefaultMutex;
typedef TimedMutex DefaultTimedMutex;
#endif


typedef LockGuard<Mutex> NonRecursiveLockGuard;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef LockGuard<ErrorCheckingMutex> ErrorCheckingLockGuard;
#endif
typedef LockGuard<RecursiveMutex> RecursiveLockGuard;

typedef UniqueLock<TimedMutex> TimedUniqueLock;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef UniqueLock<ErrorCheckingTimedMutex> ErrorCheckingTimedUniqueLock;
#endif
typedef UniqueLock<RecursiveTimedMutex> RecursiveTimedUniqueLock;

typedef LockGuard<DefaultMutex> DefaultLockGuard;
typedef UniqueLock<DefaultTimedMutex> DefaultUniqueLock;


/// @}	// mutex
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline NullMutex::NullMutex(): NonCopyable() {}

inline NullMutex::~NullMutex() CPPDEVTK_NOEXCEPT {}

inline void NullMutex::Lock() {}

inline bool NullMutex::TryLock() {
	return true;
}

inline void NullMutex::Unlock() {}


inline NullRecursiveMutex::NullRecursiveMutex(): NonCopyable() {}

inline NullRecursiveMutex::~NullRecursiveMutex() CPPDEVTK_NOEXCEPT {}

inline void NullRecursiveMutex::Lock() {}

inline bool NullRecursiveMutex::TryLock() CPPDEVTK_NOEXCEPT {
	return true;
}

inline void NullRecursiveMutex::Unlock() {}


inline NullTimedMutex::NullTimedMutex(): NonCopyable() {}

inline NullTimedMutex::~NullTimedMutex() CPPDEVTK_NOEXCEPT {}

inline void NullTimedMutex::Lock() {}

inline bool NullTimedMutex::TryLock() {
	return true;
}

inline bool NullTimedMutex::TryLockFor(int relTime) {
	SuppressUnusedWarning(relTime);
	
	return true;
}

inline bool NullTimedMutex::TryLockUntil(::std::time_t absTime) {
	SuppressUnusedWarning(absTime);
	
	return true;
}

inline void NullTimedMutex::Unlock() {}


inline NullRecursiveTimedMutex::NullRecursiveTimedMutex(): NonCopyable() {}

inline NullRecursiveTimedMutex::~NullRecursiveTimedMutex() CPPDEVTK_NOEXCEPT {}

inline void NullRecursiveTimedMutex::Lock() {}

inline bool NullRecursiveTimedMutex::TryLock() CPPDEVTK_NOEXCEPT {
	return true;
}

inline bool NullRecursiveTimedMutex::TryLockFor(int relTime) {
	SuppressUnusedWarning(relTime);
	
	return true;
}

inline bool NullRecursiveTimedMutex::TryLockUntil(::std::time_t absTime) {
	SuppressUnusedWarning(absTime);
	
	return true;
}

inline void NullRecursiveTimedMutex::Unlock() {}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation.

#if (CPPDEVTK_ENABLE_TMPL_EXPL_INST)

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<Mutex>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingMutex>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<RecursiveMutex>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<TimedMutex>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<ErrorCheckingTimedMutex>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<RecursiveTimedMutex>;

#endif


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_MUTEX_HPP_INCLUDED_
