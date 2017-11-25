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


#if (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_PLATFORM_WINDOWS && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)


#include <cstddef>
#include <cppdevtk/base/cstdint.hpp>
#include <cppdevtk/base/lock_exception.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <climits>


namespace cppdevtk {
namespace base {


Semaphore::Semaphore(::std::size_t cnt): NonCopyable(), semaphore_(CreateSemaphore(NULL, (LONG)cnt, LONG_MAX, NULL)) {
	if (semaphore_ == NULL) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to initialize semaphore");
	}
}

Semaphore::~Semaphore() CPPDEVTK_NOEXCEPT {
	if (!CloseHandle(semaphore_)) {
		CPPDEVTK_LOG_WARN("failed to close semaphore handle; error code: " << GetLastSystemErrorCode().ToString());
		CPPDEVTK_ASSERT(0 && "failed to close semaphore handle");
	}
}

void Semaphore::Notify() {
	if (!ReleaseSemaphore(semaphore_, 1, NULL)) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "semaphore failed to release");
	}
}

void Semaphore::Wait() {
	if (WaitForSingleObject(semaphore_, INFINITE) != WAIT_OBJECT_0) {
		throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "semaphore failed to WaitForSingleObject");
	}
}

bool Semaphore::TryWait() {
	const DWORD kRetCode = WaitForSingleObject(semaphore_, 0);
	switch (kRetCode) {
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		case WAIT_ABANDONED:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(WAIT_ABANDONED),
					"semaphore failed to WaitForSingleObject; error code: WAIT_ABANDONED");
		default:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
					"semaphore failed to WaitForSingleObject");
	}
}

bool Semaphore::WaitFor(int relTime) {
	const DWORD kRetCode = WaitForSingleObject(semaphore_, relTime);
	switch (kRetCode) {
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		case WAIT_ABANDONED:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(MakeSystemErrorCode(WAIT_ABANDONED),
					"semaphore failed to WaitForSingleObject; error code: WAIT_ABANDONED");
		default:
			throw CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
					"semaphore failed to WaitForSingleObject");
	}
}


}	// namespace base
}	// namespace cppdevtk


#endif	// (!CPPDEVTK_HAVE_PTHREADS && CPPDEVTK_PLATFORM_WINDOWS && !CPPDEVTK_ENABLE_SEMAPHORE_INTERRUPTION)
