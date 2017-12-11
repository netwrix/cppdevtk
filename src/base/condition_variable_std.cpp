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


#include <cppdevtk/base/condition_variable.hpp>


#if (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)


#include "throw_lock_exception.hpp"
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <system_error>
#include <mutex>
#include <chrono>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif


using ::cppdevtk::base::detail::ThrowLockException;
using ::std::system_error;
using ::std::unique_lock;
using ::std::mutex;
using ::std::adopt_lock;


namespace cppdevtk {
namespace base {


ConditionVariable::ConditionVariable() try: NonCopyable(), conditionVariable_() {
}
catch (const system_error& exc) {
	ThrowLockException(exc);
}

ConditionVariable::~ConditionVariable() CPPDEVTK_NOEXCEPT {}

void ConditionVariable::NotifyOne() CPPDEVTK_NOEXCEPT {
	conditionVariable_.notify_one();
}

void ConditionVariable::NotifyAll() CPPDEVTK_NOEXCEPT {
	conditionVariable_.notify_all();
}

void ConditionVariable::DoWait(UniqueLock<Mutex>& uniqueLock) {
	CPPDEVTK_ASSERT(uniqueLock.OwnsLock());
	
	{
		unique_lock<mutex> stdUniqueLock(uniqueLock.GetMutex()->mutex_, adopt_lock);
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&stdUniqueLock)) {
			stdUniqueLock.release();
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		try {
			conditionVariable_.wait(stdUniqueLock);
		}
		catch (const system_error& exc) {
			uniqueLock.ownsLock_ = stdUniqueLock.owns_lock();
			ThrowLockException(exc);
		}
		catch (...) {
			uniqueLock.ownsLock_ = stdUniqueLock.owns_lock();
			throw;
		}
	}
}

cv_status::cv_status_t ConditionVariable::DoWaitFor(UniqueLock<Mutex>& uniqueLock, int relTime) {
	CPPDEVTK_ASSERT(uniqueLock.OwnsLock());
	
	::std::cv_status stdCvStatus = ::std::cv_status::timeout;
	{
		unique_lock<mutex> stdUniqueLock(uniqueLock.GetMutex()->mutex_, adopt_lock);
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&stdUniqueLock)) {
			stdUniqueLock.release();
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		try {
			stdCvStatus = conditionVariable_.wait_for(stdUniqueLock, ::std::chrono::milliseconds(relTime));
		}
		catch (const system_error& exc) {
			uniqueLock.ownsLock_ = stdUniqueLock.owns_lock();
			ThrowLockException(exc);
		}
		catch (...) {
			uniqueLock.ownsLock_ = stdUniqueLock.owns_lock();
			throw;
		}
	}
	return (stdCvStatus == ::std::cv_status::no_timeout)
			? ::cppdevtk::base::cv_status::no_timeout : ::cppdevtk::base::cv_status::timeout;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE)
