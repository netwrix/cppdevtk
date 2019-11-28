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


#ifndef CPPDEVTK_BASE_LOCKABLES_HPP_INCLUDED_
#define CPPDEVTK_BASE_LOCKABLES_HPP_INCLUDED_


#include "config.hpp"
#include "mutex.hpp"
#include "locks.hpp"
#include "locking_levels.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"

#include <ctime>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup lockable Lockable classes
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.2.5.2 BasicLockable requirements
template <class TMutex, template <class> class TLockingLevel>
class BasicLockable: private TLockingLevel<TMutex> {
public:
	typedef TMutex MutexType;
	typedef TLockingLevel<TMutex> LockingLevelType;
	
	
	void Lock() const;
	void Unlock() const;
protected:
	BasicLockable();
	/* virtual */ ~BasicLockable();	///< \remark Not virtual.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.2.5.3 Lockable requirements
template <class TMutex, template <class> class TLockingLevel>
class Lockable: private TLockingLevel<TMutex> {
public:
	typedef TMutex MutexType;
	typedef TLockingLevel<TMutex> LockingLevelType;
	
	
	void Lock() const;
	bool TryLock() const;
	void Unlock() const;
protected:
	Lockable();
	/* virtual */ ~Lockable();	///< \remark Not virtual.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.2.5.4 TimedLockable requirements
template <class TMutex, template <class> class TLockingLevel>
class TimedLockable: private TLockingLevel<TMutex> {
public:
	typedef TMutex MutexType;
	typedef TLockingLevel<TMutex> LockingLevelType;
	
	
	void Lock() const;
	bool TryLock() const;
	
	/// Attempts to obtain ownership within the relative timeout.
	/// \param relTime Relative timeout, in milliseconds.
	/// If it is <= 0, attempts to obtain ownership without blocking (as if by calling TryLock()).
	bool TryLockFor(int relTime) const;
	
	/// Attempts to obtain ownership.
	/// \param absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
	/// If it has already passed, attempts to obtain ownership without blocking (as if by calling TryLock()).
	bool TryLockUntil(::std::time_t absTime) const;
	
	void Unlock() const;
protected:
	TimedLockable();
	/* virtual */ ~TimedLockable();	///< \remark Not virtual.
};




typedef BasicLockable<Mutex, ObjectLevelLocking> ObjectLevelBasicLockable;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef BasicLockable<ErrorCheckingMutex, ObjectLevelLocking> ErrorCheckingObjectLevelBasicLockable;
#endif
typedef BasicLockable<RecursiveMutex, ClassLevelLocking> ClassLevelBasicLockable;

typedef Lockable<Mutex, ObjectLevelLocking> ObjectLevelLockable;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef Lockable<ErrorCheckingMutex, ObjectLevelLocking> ErrorCheckingObjectLevelLockable;
#endif
typedef Lockable<RecursiveMutex, ClassLevelLocking> ClassLevelLockable;

typedef TimedLockable<TimedMutex, ObjectLevelLocking> ObjectLevelTimedLockable;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef TimedLockable<ErrorCheckingTimedMutex, ObjectLevelLocking> ErrorCheckingObjectLevelTimedLockable;
#endif
typedef TimedLockable<RecursiveTimedMutex, ClassLevelLocking> ClassLevelTimedLockable;


typedef BasicLockable<NullMutex, ObjectLevelLocking> NullObjectLevelBasicLockable;
typedef BasicLockable<NullRecursiveMutex, ClassLevelLocking> NullClassLevelBasicLockable;

typedef Lockable<NullMutex, ObjectLevelLocking> NullObjectLevelLockable;
typedef Lockable<NullRecursiveMutex, ClassLevelLocking> NullClassLevelLockable;

typedef TimedLockable<NullTimedMutex, ObjectLevelLocking> NullObjectLevelTimedLockable;
typedef TimedLockable<NullRecursiveTimedMutex, ClassLevelLocking> NullClassLevelTimedLockable;


#if (CPPDEVTK_HAVE_PTHREADS && !defined(NDEBUG))
typedef ErrorCheckingObjectLevelBasicLockable DefaultObjectLevelBasicLockable;
typedef ErrorCheckingObjectLevelLockable DefaultObjectLevelLockable;
typedef ErrorCheckingObjectLevelTimedLockable DefaultObjectLevelTimedLockable;
#else
typedef ObjectLevelBasicLockable DefaultObjectLevelBasicLockable;
typedef ObjectLevelLockable DefaultObjectLevelLockable;
typedef ObjectLevelTimedLockable DefaultObjectLevelTimedLockable;
#endif




// guards

typedef LockGuard<ObjectLevelBasicLockable> ObjectLevelBasicLockableLockGuard;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef LockGuard<ErrorCheckingObjectLevelBasicLockable> ErrorCheckingObjectLevelBasicLockableLockGuard;
#endif
typedef LockGuard<ClassLevelBasicLockable> ClassLevelBasicLockableLockGuard;

typedef LockGuard<ObjectLevelLockable> ObjectLevelLockableLockGuard;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef LockGuard<ErrorCheckingObjectLevelLockable> ErrorCheckingObjectLevelLockableLockGuard;
#endif
typedef LockGuard<ClassLevelLockable> ClassLevelLockableLockGuard;

typedef LockGuard<ObjectLevelTimedLockable> ObjectLevelTimedLockableLockGuard;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef LockGuard<ErrorCheckingObjectLevelTimedLockable> ErrorCheckingObjectLevelTimedLockableLockGuard;
#endif
typedef LockGuard<ClassLevelTimedLockable> ClassLevelTimedLockableLockGuard;


typedef LockGuard<NullObjectLevelBasicLockable> NullObjectLevelBasicLockableLockGuard;
typedef LockGuard<NullClassLevelBasicLockable> NullClassLevelBasicLockableLockGuard;

typedef LockGuard<NullObjectLevelLockable> NullObjectLevelLockableLockGuard;
typedef LockGuard<NullClassLevelLockable> NullClassLevelLockableLockGuard;

typedef LockGuard<NullObjectLevelTimedLockable> NullObjectLevelTimedLockableLockGuard;
typedef LockGuard<NullClassLevelTimedLockable> NullClassLevelTimedLockableLockGuard;

#if (CPPDEVTK_HAVE_PTHREADS && !defined(NDEBUG))
typedef ErrorCheckingObjectLevelBasicLockableLockGuard DefaultObjectLevelBasicLockableLockGuard;
typedef ErrorCheckingObjectLevelLockableLockGuard DefaultObjectLevelLockableLockGuard;
typedef ErrorCheckingObjectLevelTimedLockableLockGuard DefaultObjectLevelTimedLockableLockGuard;
#else
typedef ObjectLevelBasicLockableLockGuard DefaultObjectLevelBasicLockableLockGuard;
typedef ObjectLevelLockableLockGuard DefaultObjectLevelLockableLockGuard;
typedef ObjectLevelTimedLockableLockGuard DefaultObjectLevelTimedLockableLockGuard;
#endif


typedef UniqueLock<ObjectLevelBasicLockable> ObjectLevelBasicLockableUniqueLock;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef UniqueLock<ErrorCheckingObjectLevelBasicLockable> ErrorCheckingObjectLevelBasicLockableUniqueLock;
#endif
typedef UniqueLock<ClassLevelBasicLockable> ClassLevelBasicLockableUniqueLock;

typedef UniqueLock<ObjectLevelLockable> ObjectLevelLockableUniqueLock;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef UniqueLock<ErrorCheckingObjectLevelLockable> ErrorCheckingObjectLevelLockableUniqueLock;
#endif
typedef UniqueLock<ClassLevelLockable> ClassLevelLockableUniqueLock;

typedef UniqueLock<ObjectLevelTimedLockable> ObjectLevelTimedLockableUniqueLock;
#if (CPPDEVTK_HAVE_PTHREADS)
typedef UniqueLock<ErrorCheckingObjectLevelTimedLockable> ErrorCheckingObjectLevelTimedLockableUniqueLock;
#endif
typedef UniqueLock<ClassLevelTimedLockable> ClassLevelTimedLockableUniqueLock;


typedef UniqueLock<NullObjectLevelBasicLockable> NullObjectLevelBasicLockableUniqueLock;
typedef UniqueLock<NullClassLevelBasicLockable> NullClassLevelBasicLockableUniqueLock;

typedef UniqueLock<NullObjectLevelLockable> NullObjectLevelLockableUniqueLock;
typedef UniqueLock<NullClassLevelLockable> NullClassLevelLockableUniqueLock;

typedef UniqueLock<NullObjectLevelTimedLockable> NullObjectLevelTimedLockableUniqueLock;
typedef UniqueLock<NullClassLevelTimedLockable> NullClassLevelTimedLockableUniqueLock;

#if (CPPDEVTK_HAVE_PTHREADS && !defined(NDEBUG))
typedef ErrorCheckingObjectLevelBasicLockableUniqueLock DefaultObjectLevelBasicLockableUniqueLock;
typedef ErrorCheckingObjectLevelLockableUniqueLock DefaultObjectLevelLockableUniqueLock;
typedef ErrorCheckingObjectLevelTimedLockableUniqueLock DefaultObjectLevelTimedLockableUniqueLock;
#else
typedef ObjectLevelBasicLockableUniqueLock DefaultObjectLevelBasicLockableUniqueLock;
typedef ObjectLevelLockableUniqueLock DefaultObjectLevelLockableUniqueLock;
typedef ObjectLevelTimedLockableUniqueLock DefaultObjectLevelTimedLockableUniqueLock;
#endif


/// @}	// lockable
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

template <class TMutex, template <class> class TLockingLevel>
inline void BasicLockable<TMutex, TLockingLevel>::Lock() const {
	LockingLevelType::GetMutexRef().Lock();
}

template <class TMutex, template <class> class TLockingLevel>
inline void BasicLockable<TMutex, TLockingLevel>::Unlock() const {
	LockingLevelType::GetMutexRef().Unlock();
}

template <class TMutex, template <class> class TLockingLevel>
inline BasicLockable<TMutex, TLockingLevel>::BasicLockable(): LockingLevelType() {}

template <class TMutex, template <class> class TLockingLevel>
inline BasicLockable<TMutex, TLockingLevel>::~BasicLockable() {}


template <class TMutex, template <class> class TLockingLevel>
inline void Lockable<TMutex, TLockingLevel>::Lock() const {
	LockingLevelType::GetMutexRef().Lock();
}

template <class TMutex, template <class> class TLockingLevel>
inline bool Lockable<TMutex, TLockingLevel>::TryLock() const {
	return LockingLevelType::GetMutexRef().TryLock();
}

template <class TMutex, template <class> class TLockingLevel>
inline void Lockable<TMutex, TLockingLevel>::Unlock() const {
	LockingLevelType::GetMutexRef().Unlock();
}

template <class TMutex, template <class> class TLockingLevel>
inline Lockable<TMutex, TLockingLevel>::Lockable(): LockingLevelType() {}

template <class TMutex, template <class> class TLockingLevel>
inline Lockable<TMutex, TLockingLevel>::~Lockable() {}


template <class TMutex, template <class> class TLockingLevel>
inline void TimedLockable<TMutex, TLockingLevel>::Lock() const {
	LockingLevelType::GetMutexRef().Lock();
}

template <class TMutex, template <class> class TLockingLevel>
inline bool TimedLockable<TMutex, TLockingLevel>::TryLock() const {
	return LockingLevelType::GetMutexRef().TryLock();
}

template <class TMutex, template <class> class TLockingLevel>
inline bool TimedLockable<TMutex, TLockingLevel>::TryLockFor(int relTime) const {
	return LockingLevelType::GetMutexRef().TryLockFor(relTime);
}

template <class TMutex, template <class> class TLockingLevel>
inline bool TimedLockable<TMutex, TLockingLevel>::TryLockUntil(::std::time_t absTime) const {
	return LockingLevelType::GetMutexRef().TryLockUntil(absTime);
}

template <class TMutex, template <class> class TLockingLevel>
inline void TimedLockable<TMutex, TLockingLevel>::Unlock() const {
	LockingLevelType::GetMutexRef().Unlock();
}

template <class TMutex, template <class> class TLockingLevel>
inline TimedLockable<TMutex, TLockingLevel>::TimedLockable(): LockingLevelType() {}

template <class TMutex, template <class> class TLockingLevel>
inline TimedLockable<TMutex, TLockingLevel>::~TimedLockable() {}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation declaration.

#ifndef CPPDEVTK_BASE_LOCKABLES_CPP

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicLockable<Mutex, ObjectLevelLocking>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicLockable<ErrorCheckingMutex, ObjectLevelLocking>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicLockable<RecursiveMutex, ClassLevelLocking>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API Lockable<Mutex, ObjectLevelLocking>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API Lockable<ErrorCheckingMutex, ObjectLevelLocking>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API Lockable<RecursiveMutex, ClassLevelLocking>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API TimedLockable<TimedMutex, ObjectLevelLocking>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API TimedLockable<ErrorCheckingTimedMutex, ObjectLevelLocking>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API TimedLockable<RecursiveTimedMutex, ClassLevelLocking>;


CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicLockable<NullMutex, ObjectLevelLocking>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicLockable<NullRecursiveMutex, ClassLevelLocking>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API Lockable<NullMutex, ObjectLevelLocking>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API Lockable<NullRecursiveMutex, ClassLevelLocking>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API TimedLockable<NullTimedMutex, ObjectLevelLocking>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API TimedLockable<NullRecursiveTimedMutex, ClassLevelLocking>;


CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ObjectLevelBasicLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingObjectLevelBasicLockable>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ClassLevelBasicLockable>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ObjectLevelLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingObjectLevelLockable>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ClassLevelLockable>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ObjectLevelTimedLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingObjectLevelTimedLockable>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<ClassLevelTimedLockable>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<NullObjectLevelBasicLockable>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<NullClassLevelBasicLockable>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<NullObjectLevelLockable>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<NullClassLevelLockable>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<NullObjectLevelTimedLockable>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API LockGuard<NullClassLevelTimedLockable>;


CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<ObjectLevelTimedLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<ErrorCheckingObjectLevelTimedLockable>;
#endif
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<ClassLevelTimedLockable>;

CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<NullObjectLevelTimedLockable>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API UniqueLock<NullClassLevelTimedLockable>;

#endif	// CPPDEVTK_BASE_LOCKABLES_CPP


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_LOCKABLES_HPP_INCLUDED_
