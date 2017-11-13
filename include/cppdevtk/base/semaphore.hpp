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


#ifndef CPPDEVTK_BASE_SEMAPHORE_HPP_INCLUDED_
#define CPPDEVTK_BASE_SEMAPHORE_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"

#if (CPPDEVTK_HAVE_PTHREADS)
#include <semaphore.h>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#include <windows.h>
#elif (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
#include "mutex.hpp"
#include "condition_variable.hpp"
#else
#include <QtCore/QSemaphore>
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
class CPPDEVTK_BASE_API Semaphore: private NonCopyable {
public:
	Semaphore(::std::size_t cnt = 1);
	~Semaphore() CPPDEVTK_NOEXCEPT;
	
	
	void Notify();	///< \sa POSIX sem_post()
	
	void Wait();	///< \sa POSIX sem_wait()
	bool TryWait();	///< \sa POSIX sem_trywait()
	
#	if (!(CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_PLATFORM_MACOSX))	// sem_timedwait() not available on Mac OS X & iOS
	/// \arg relTime Relative timeout, in milliseconds.
	/// \sa POSIX sem_timedwait()
	bool WaitFor(int relTime);
	bool WaitUntil(::std::time_t absTime);	///< \sa POSIX sem_timedwait()
#	endif
private:
#	if (CPPDEVTK_HAVE_PTHREADS)
	sem_t semaphore_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
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

#if (!(CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_PLATFORM_MACOSX))

inline bool Semaphore::WaitUntil(::std::time_t absTime) {
	::std::time_t currTime = ::std::time(NULL);
	::std::time_t seconds = ::std::difftime(absTime, currTime);
	
	return WaitFor(seconds * 1000);
}

#endif


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_SEMAPHORE_HPP_INCLUDED_
