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


#ifndef CPPDEVTK_BASE_SEMAPHORE_HPP_INCLUDED_
#define CPPDEVTK_BASE_SEMAPHORE_HPP_INCLUDED_


#define CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION ((CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)	\
		&& CPPDEVTK_ENABLE_THREAD_INTERRUPTION)


#include "config.hpp"
#include "non_copyable.hpp"
#include "lock_exception.hpp"
#include "time_utils.hpp"

#if (CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
#include <semaphore.h>
#elif (CPPDEVTK_PLATFORM_WINDOWS && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
#include <windows.h>
#elif (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
#include "mutex.hpp"
#include "condition_variable.hpp"
#elif (!CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
#include <QtCore/QSemaphore>
#else
#error "internal error: CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION for Qt based semaphore"
#endif

#include <ctime>
#include <cstddef>


namespace cppdevtk {
namespace base {



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Semaphore synchronization primitive.
/// \details
/// - Take ownership with wait (decrement (lock) the semaphore)
/// - Release ownership with notify (increment (unlock) the semaphore)
/// \attention Interruption is supported only if C++11 mutex and condition variable are available.
class CPPDEVTK_BASE_API Semaphore: private NonCopyable {
public:
	Semaphore(::std::size_t cnt = 1);
	~Semaphore() CPPDEVTK_NOEXCEPT;
	
	
	void Notify();	///< \sa POSIX sem_post()
	
	/// \attention Interruption point
	/// \sa POSIX sem_wait()
	void Wait();
	
	bool TryWait();	///< \sa POSIX sem_trywait()
	
	// sem_timedwait() not available on Mac OS X & iOS
#	if (!((CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION) && CPPDEVTK_PLATFORM_MACOSX))
	/// \param relTime Relative timeout, in milliseconds. If it is <= 0, calls TryWait()
	/// \attention Interruption point
	/// \sa POSIX sem_timedwait()
	bool WaitFor(int relTime);
	
	/// \param absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC. If it is not in the future, calls TryWait()
	/// \attention Interruption point
	/// \sa POSIX sem_timedwait()
	bool WaitUntil(::std::time_t absTime);
#	endif
private:
#	if (CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	sem_t semaphore_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	HANDLE semaphore_;
#	elif (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
	volatile ::std::size_t cnt_;
	Mutex mutex_;
	ConditionVariable conditionVariable_;
#	else
	QSemaphore semaphore_;
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

#if (!((CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION) && CPPDEVTK_PLATFORM_MACOSX))

inline bool Semaphore::WaitUntil(::std::time_t absTime) {
	using ::std::time_t;
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	
	return (kSeconds <= 0) ? TryWait() : WaitFor(kSeconds * 1000);
}

#endif


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_SEMAPHORE_HPP_INCLUDED_
