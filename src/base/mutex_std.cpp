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


#if (CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS)


#include <cstddef>


using ::std::time_t;
using ::std::time;
using ::std::difftime;


namespace cppdevtk {
namespace base {


Mutex::Mutex(): NonCopyable(), mutex_() {}

Mutex::~Mutex() {}

void Mutex::Lock() {
	mutex_.lock();
}

bool Mutex::TryLock() {
	return mutex_.try_lock();
}

void Mutex::Unlock() {
	mutex_.unlock();
}

Mutex::NativeHandleType Mutex::GetNativeHandle() {
	return mutex_.native_handle();
}


RecursiveMutex::RecursiveMutex(): NonCopyable(), mutex_() {}

RecursiveMutex::~RecursiveMutex() {}

void RecursiveMutex::Lock() {
	mutex_.lock();
}

bool RecursiveMutex::TryLock() {
	return mutex_.try_lock();
}

void RecursiveMutex::Unlock() {
	mutex_.unlock();
}

RecursiveMutex::NativeHandleType RecursiveMutex::GetNativeHandle() {
	return mutex_.native_handle();
}


#if (!CPPDEVTK_PLATFORM_ANDROID)

TimedMutex::TimedMutex(): NonCopyable(), mutex_() {}

TimedMutex::~TimedMutex() {}

void TimedMutex::Lock() {
	mutex_.lock();
}

bool TimedMutex::TryLock() {
	return mutex_.try_lock();
}

bool TimedMutex::TryLockFor(int relTime) {
	return mutex_.try_lock_for(::std::chrono::milliseconds(relTime));
}

bool TimedMutex::TryLockUntil(::std::time_t absTime) {
	return mutex_.try_lock_until(::std::chrono::system_clock::from_time_t(absTime));
}

void TimedMutex::Unlock() {
	mutex_.unlock();
}

#if (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_MACOSX)

TimedMutex::NativeHandleType TimedMutex::GetNativeHandle() {
	return mutex_.native_handle();
}

#endif


RecursiveTimedMutex::RecursiveTimedMutex(): NonCopyable(), mutex_() {}

RecursiveTimedMutex::~RecursiveTimedMutex() {}

void RecursiveTimedMutex::Lock() {
	mutex_.lock();
}

bool RecursiveTimedMutex::TryLock() {
	return mutex_.try_lock();
}

bool RecursiveTimedMutex::TryLockFor(int relTime) {
	return mutex_.try_lock_for(::std::chrono::milliseconds(relTime));
}

bool RecursiveTimedMutex::TryLockUntil(::std::time_t absTime) {
	return mutex_.try_lock_until(::std::chrono::system_clock::from_time_t(absTime));
}

void RecursiveTimedMutex::Unlock() {
	mutex_.unlock();
}

#if (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_MACOSX)

RecursiveTimedMutex::NativeHandleType RecursiveTimedMutex::GetNativeHandle() {
	return mutex_.native_handle();
}

#endif


#endif	// !CPPDEVTK_PLATFORM_ANDROID


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_HAVE_CPP11_MUTEX && !CPPDEVTK_ENABLE_DOT_NET_WORKAROUNDS
