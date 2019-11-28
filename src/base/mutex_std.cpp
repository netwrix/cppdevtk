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


#include <cppdevtk/base/config.hpp>
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4702)	// C4702: unreachable code
#endif

#include <cppdevtk/base/mutex.hpp>


#if (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_HAVE_CPP11_MUTEX)


#include "throw_lock_exception.hpp"

#include <system_error>
#include <cstddef>


using ::cppdevtk::base::detail::ThrowLockException;
using ::std::system_error;


namespace cppdevtk {
namespace base {


Mutex::Mutex() try: NonCopyable(), mutex_() {
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

Mutex::~Mutex() CPPDEVTK_NOEXCEPT {}

void Mutex::Lock() try {
	mutex_.lock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

bool Mutex::TryLock() try {
	return mutex_.try_lock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return false;
}

void Mutex::Unlock() try {
	mutex_.unlock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

Mutex::NativeHandleType Mutex::GetNativeHandle() try {
	return mutex_.native_handle();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return NULL;
}


RecursiveMutex::RecursiveMutex() try: NonCopyable(), mutex_() {
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

RecursiveMutex::~RecursiveMutex() CPPDEVTK_NOEXCEPT {}

void RecursiveMutex::Lock() try {
	mutex_.lock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

bool RecursiveMutex::TryLock() CPPDEVTK_NOEXCEPT {
	return mutex_.try_lock();
}

void RecursiveMutex::Unlock() try {
	mutex_.unlock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

RecursiveMutex::NativeHandleType RecursiveMutex::GetNativeHandle() try {
	return mutex_.native_handle();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return NULL;
}


#if (!CPPDEVTK_PLATFORM_ANDROID)

TimedMutex::TimedMutex() try: NonCopyable(), mutex_() {
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

TimedMutex::~TimedMutex() CPPDEVTK_NOEXCEPT {}

void TimedMutex::Lock() try {
	mutex_.lock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

bool TimedMutex::TryLock() try {
	return mutex_.try_lock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

bool TimedMutex::TryLockFor(int relTime) try {
	return mutex_.try_lock_for(::std::chrono::milliseconds(relTime));
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return false;
}

bool TimedMutex::TryLockUntil(::std::time_t absTime) try {
	return mutex_.try_lock_until(::std::chrono::system_clock::from_time_t(absTime));
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return false;
}

void TimedMutex::Unlock() try {
	mutex_.unlock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

#if (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_MACOSX)

TimedMutex::NativeHandleType TimedMutex::GetNativeHandle() try {
	return mutex_.native_handle();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

#endif


RecursiveTimedMutex::RecursiveTimedMutex() try: NonCopyable(), mutex_() {
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

RecursiveTimedMutex::~RecursiveTimedMutex() CPPDEVTK_NOEXCEPT {}

void RecursiveTimedMutex::Lock() try {
	mutex_.lock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

bool RecursiveTimedMutex::TryLock() CPPDEVTK_NOEXCEPT {
	return mutex_.try_lock();
}

bool RecursiveTimedMutex::TryLockFor(int relTime) try {
	return mutex_.try_lock_for(::std::chrono::milliseconds(relTime));
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return false;
}

bool RecursiveTimedMutex::TryLockUntil(::std::time_t absTime) try {
	return mutex_.try_lock_until(::std::chrono::system_clock::from_time_t(absTime));
}
catch (const system_error& exc) {
	ThrowLockException(exc);
	return false;
}

void RecursiveTimedMutex::Unlock() try {
	mutex_.unlock();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

#if (!CPPDEVTK_COMPILER_MSVC && !CPPDEVTK_PLATFORM_MACOSX)

RecursiveTimedMutex::NativeHandleType RecursiveTimedMutex::GetNativeHandle() try {
	return mutex_.native_handle();
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

#endif

#endif	// (!CPPDEVTK_PLATFORM_ANDROID)


}	// namespace base
}	// namespace cppdevtk


#endif	// (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_HAVE_CPP11_MUTEX)
