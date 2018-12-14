/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#define CPPDEVTK_BASE_LOCKABLES_CPP


#include <cppdevtk/base/lockables.hpp>


namespace cppdevtk {
namespace base {


template class CPPDEVTK_BASE_API BasicLockable<Mutex, ObjectLevelLocking>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API BasicLockable<ErrorCheckingMutex, ObjectLevelLocking>;
#endif
template class CPPDEVTK_BASE_API BasicLockable<RecursiveMutex, ClassLevelLocking>;

template class CPPDEVTK_BASE_API Lockable<Mutex, ObjectLevelLocking>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API Lockable<ErrorCheckingMutex, ObjectLevelLocking>;
#endif
template class CPPDEVTK_BASE_API Lockable<RecursiveMutex, ClassLevelLocking>;

template class CPPDEVTK_BASE_API TimedLockable<TimedMutex, ObjectLevelLocking>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API TimedLockable<ErrorCheckingTimedMutex, ObjectLevelLocking>;
#endif
template class CPPDEVTK_BASE_API TimedLockable<RecursiveTimedMutex, ClassLevelLocking>;

template class CPPDEVTK_BASE_API BasicLockable<NullMutex, ObjectLevelLocking>;
template class CPPDEVTK_BASE_API BasicLockable<NullRecursiveMutex, ClassLevelLocking>;

template class CPPDEVTK_BASE_API Lockable<NullMutex, ObjectLevelLocking>;
template class CPPDEVTK_BASE_API Lockable<NullRecursiveMutex, ClassLevelLocking>;

template class CPPDEVTK_BASE_API TimedLockable<NullTimedMutex, ObjectLevelLocking>;
template class CPPDEVTK_BASE_API TimedLockable<NullRecursiveTimedMutex, ClassLevelLocking>;


template class CPPDEVTK_BASE_API LockGuard<ObjectLevelBasicLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingObjectLevelBasicLockable>;
#endif
template class CPPDEVTK_BASE_API LockGuard<ClassLevelBasicLockable>;

template class CPPDEVTK_BASE_API LockGuard<ObjectLevelLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingObjectLevelLockable>;
#endif
template class CPPDEVTK_BASE_API LockGuard<ClassLevelLockable>;

template class CPPDEVTK_BASE_API LockGuard<ObjectLevelTimedLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingObjectLevelTimedLockable>;
#endif
template class CPPDEVTK_BASE_API LockGuard<ClassLevelTimedLockable>;

template class CPPDEVTK_BASE_API LockGuard<NullObjectLevelBasicLockable>;
template class CPPDEVTK_BASE_API LockGuard<NullClassLevelBasicLockable>;

template class CPPDEVTK_BASE_API LockGuard<NullObjectLevelLockable>;
template class CPPDEVTK_BASE_API LockGuard<NullClassLevelLockable>;

template class CPPDEVTK_BASE_API LockGuard<NullObjectLevelTimedLockable>;
template class CPPDEVTK_BASE_API LockGuard<NullClassLevelTimedLockable>;


template class CPPDEVTK_BASE_API UniqueLock<ObjectLevelTimedLockable>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API UniqueLock<ErrorCheckingObjectLevelTimedLockable>;
#endif
template class CPPDEVTK_BASE_API UniqueLock<ClassLevelTimedLockable>;

template class CPPDEVTK_BASE_API UniqueLock<NullObjectLevelTimedLockable>;
template class CPPDEVTK_BASE_API UniqueLock<NullClassLevelTimedLockable>;


}	// namespace base
}	// namespace cppdevtk
