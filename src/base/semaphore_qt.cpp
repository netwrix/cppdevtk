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


#include <cppdevtk/base/semaphore.hpp>


#if (!(CPPDEVTK_HAVE_POSIX_UNNAMED_SEMAPHORE || CPPDEVTK_PLATFORM_WINDOWS || (CPPDEVTK_HAVE_CPP11_MUTEX && CPPDEVTK_HAVE_CPP11_CONDITION_VARIABLE))	\
		&& !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)


namespace cppdevtk {
namespace base {


Semaphore::Semaphore(::std::size_t cnt): NonCopyable(), semaphore_(cnt) {}

Semaphore::~Semaphore() CPPDEVTK_NOEXCEPT {}

void Semaphore::Notify() {
	semaphore_.release();
}

void Semaphore::Wait() {
	semaphore_.acquire();
}

bool Semaphore::TryWait() {
	return semaphore_.tryAcquire();
}

bool Semaphore::WaitFor(int relTime) {
	if (relTime <= 0) {
		return TryWait();
	}
	
	return semaphore_.tryAcquire(1, relTime);
}


}	// namespace base
}	// namespace cppdevtk


#endif
