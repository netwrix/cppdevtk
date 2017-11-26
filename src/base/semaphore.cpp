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


#include <cppdevtk/base/semaphore.hpp>


#if (!(CPPDEVTK_HAVE_PTHREADS && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)	\
		&& !(CPPDEVTK_PLATFORM_WINDOWS && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)	\
		&& (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE))


#include <cppdevtk/base/cassert.hpp>
#if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
#include <cppdevtk/base/thread.hpp>
#endif


namespace cppdevtk {
namespace base {


Semaphore::Semaphore(::std::size_t cnt): NonCopyable(), cnt_(cnt), mutex_(), conditionVariable_() {}

Semaphore::~Semaphore() CPPDEVTK_NOEXCEPT {}

void Semaphore::Notify() {
	LockGuard<Mutex> lockGuard(mutex_);
	
	++cnt_;
	
	conditionVariable_.NotifyOne();
}

void Semaphore::Wait() {
#	if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif

	UniqueLock<Mutex> uniqueLock(mutex_);
	
#	if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	while (cnt_ == 0) {
		conditionVariable_.Wait(uniqueLock);
	}
	
	--cnt_;
	
#	if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
}

bool Semaphore::TryWait() {
	UniqueLock<Mutex> uniqueLock(mutex_);
	
	if (cnt_ > 0) {
		--cnt_;
		return true;
	}
	
	return false;
}

bool Semaphore::WaitFor(int relTime) {
#	if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	if (relTime <= 0) {
		return TryWait();
	}
	
	UniqueLock<Mutex> uniqueLock(mutex_);
	
#	if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	while (cnt_ == 0) {
		if (conditionVariable_.WaitFor(uniqueLock, relTime) == ::cppdevtk::base::cv_status::timeout) {
			if (cnt_ == 0) {
				return false;
			}
			
			break;
		}
	}
	
	CPPDEVTK_ASSERT(cnt_ > 0);
	--cnt_;
	
#	if (CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
	this_thread::InterruptionPoint();
#	endif
	
	return true;
}


}	// namespace base
}	// namespace cppdevtk


#endif
