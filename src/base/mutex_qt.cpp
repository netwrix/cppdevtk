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


#if (!(CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_MUTEX))


#include <cstddef>


using ::std::time_t;
using ::std::time;
using ::std::difftime;


namespace cppdevtk {
namespace base {


Mutex::Mutex(): NonCopyable(), mutex_(QMutex::NonRecursive) {}

Mutex::~Mutex() {}

void Mutex::Lock() {
	mutex_.lock();
}

bool Mutex::TryLock() {
	return mutex_.tryLock();
}

void Mutex::Unlock() {
	mutex_.unlock();
}


RecursiveMutex::RecursiveMutex(): NonCopyable(), mutex_(QMutex::Recursive) {}

RecursiveMutex::~RecursiveMutex() {}

void RecursiveMutex::Lock() {
	mutex_.lock();
}

bool RecursiveMutex::TryLock() CPPDEVTK_NOEXCEPT {
	return mutex_.tryLock();
}

void RecursiveMutex::Unlock() {
	mutex_.unlock();
}


TimedMutex::TimedMutex(): NonCopyable(), mutex_(QMutex::NonRecursive) {}

TimedMutex::~TimedMutex() {}

void TimedMutex::Lock() {
	mutex_.lock();
}

bool TimedMutex::TryLock() {
	return mutex_.tryLock();
}

bool TimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	return mutex_.tryLock(relTime);
}

bool TimedMutex::TryLockUntil(::std::time_t absTime) {
	time_t currTime = time(NULL);
	time_t seconds = difftime(absTime, currTime);
	if (seconds <= 0) {
		return TryLock();
	}
	
	return mutex_.tryLock(seconds * 1000);
}

void TimedMutex::Unlock() {
	mutex_.unlock();
}


RecursiveTimedMutex::RecursiveTimedMutex(): NonCopyable(), mutex_(QMutex::Recursive) {}

RecursiveTimedMutex::~RecursiveTimedMutex() {}

void RecursiveTimedMutex::Lock() {
	mutex_.lock();
}

bool RecursiveTimedMutex::TryLock() CPPDEVTK_NOEXCEPT {
	return mutex_.tryLock();
}

bool RecursiveTimedMutex::TryLockFor(int relTime) {
	if (relTime <= 0) {
		return TryLock();
	}
	
	return mutex_.tryLock(relTime);
}

bool RecursiveTimedMutex::TryLockUntil(::std::time_t absTime) {
	time_t currTime = time(NULL);
	time_t seconds = difftime(absTime, currTime);
	if (seconds <= 0) {
		return TryLock();
	}
	
	return mutex_.tryLock(seconds * 1000);
}

void RecursiveTimedMutex::Unlock() {
	mutex_.unlock();
}


}	// namespace base
}	// namespace cppdevtk


#endif	// (!(CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_MUTEX))
