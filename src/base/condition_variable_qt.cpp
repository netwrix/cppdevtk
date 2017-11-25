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


#include <cppdevtk/base/condition_variable.hpp>


#if (!(CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE))


#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/cassert.hpp>


namespace cppdevtk {
namespace base {


ConditionVariable::ConditionVariable(): NonCopyable(), conditionVariable_() {}

ConditionVariable::~ConditionVariable() CPPDEVTK_NOEXCEPT {}

void ConditionVariable::NotifyOne() CPPDEVTK_NOEXCEPT {
	conditionVariable_.wakeOne();
}

void ConditionVariable::NotifyAll() CPPDEVTK_NOEXCEPT {
	conditionVariable_.wakeAll();
}

void ConditionVariable::UninterruptibleWait(UniqueLock<Mutex>& uniqueLock) {
	CPPDEVTK_ASSERT(uniqueLock.OwnsLock());
	
	CPPDEVTK_VERIFY(conditionVariable_.wait(&(uniqueLock.GetMutex()->mutex_)));
}

cv_status::cv_status_t ConditionVariable::UninterruptibleWaitFor(UniqueLock<Mutex>& uniqueLock, int relTime) {
	CPPDEVTK_ASSERT(uniqueLock.OwnsLock());
	
	return conditionVariable_.wait(&(uniqueLock.GetMutex()->mutex_), relTime)
			? ::cppdevtk::base::cv_status::no_timeout : ::cppdevtk::base::cv_status::timeout;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// (!(CPPDEVTK_HAVE_PTHREADS || CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE))
