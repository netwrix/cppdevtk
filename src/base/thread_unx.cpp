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


#include <cppdevtk/base/thread.hpp>
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif

#include <cppdevtk/base/thread_exception.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cerrno.hpp>

#include <sched.h>


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


#include <cppdevtk/base/thread_data.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include "thread_local_data_ptr.hpp"

#if (CPPDEVTK_PLATFORM_LINUX && !CPPDEVTK_PLATFORM_ANDROID)
#include <sys/sysinfo.h>
#elif (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)
#if (((MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5) || CPPDEVTK_PLATFORM_IOS) || CPPDEVTK_PLATFORM_ANDROID)
#include <unistd.h>
#else
#include <sys/types.h>
#include <sys/sysctl.h> 
#endif
#endif


#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


namespace cppdevtk {
namespace base {


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


using this_thread::detail::pThreadLocalData;
using ::std::exception;


Thread::Attributes::Attributes(): detached_(false), nativeAttributes_() {
	int retCode = pthread_attr_init(&nativeAttributes_);
	if (retCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode), "failed to initialize thread attributes");
	}
	
	retCode = pthread_attr_setdetachstate(&nativeAttributes_, PTHREAD_CREATE_DETACHED);
	if (retCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode), "failed to set thread attributes detach state");
	}
}

Thread::Attributes::~Attributes() CPPDEVTK_NOEXCEPT {
	const int kRetCode = pthread_attr_destroy(&nativeAttributes_);
	if (kRetCode != ESUCCESS) {
		CPPDEVTK_LOG_WARN("pthread_attr_destroy() failed; errorCode: " << MakeSystemErrorCode(kRetCode).ToString());
		CPPDEVTK_ASSERT(0 && "pthread_attr_destroy() failed");
	}
}

void Thread::Attributes::SetDetached(bool value) {
	detached_ = value;
}

bool Thread::Attributes::GetDetached() const {
	return detached_;
}

void Thread::Attributes::SetStackSize(::std::size_t value) {
	const int kRetCode = pthread_attr_setstacksize(&nativeAttributes_, value);
	if (kRetCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to set thread attributes stack size");
	}
}

::std::size_t Thread::Attributes::GetStackSize() const {
	size_t stackSize = 0;
	const int kRetCode = pthread_attr_getstacksize(&nativeAttributes_, &stackSize);
	if (kRetCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to get thread attributes stack size");
	}
	return stackSize;
}

Thread::Attributes::NativeHandleType* Thread::Attributes::GetNativeHandlePtr() {
	return &nativeAttributes_;
}

const Thread::Attributes::NativeHandleType* Thread::Attributes:: GetNativeHandlePtr() const {
	return &nativeAttributes_;
}


Thread::Id Thread::GetId() const CPPDEVTK_NOEXCEPT {
	return pData_ ? pData_->GetNativeId() : Id();
}

unsigned int Thread::GetHardwareConcurrency() CPPDEVTK_NOEXCEPT {
#	if (CPPDEVTK_PLATFORM_LINUX && !CPPDEVTK_PLATFORM_ANDROID)
	return get_nprocs();
#	elif (CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID)
	
	// _SC_NPROCESSORS_ONLN missing on Mac OS X 10.4 although present in sysconf() manpage...
#	if (((MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5) || CPPDEVTK_PLATFORM_IOS) || CPPDEVTK_PLATFORM_ANDROID)
	
	long numProcessorsOnline = sysconf(_SC_NPROCESSORS_ONLN);
	if (numProcessorsOnline == -1) {
		CPPDEVTK_LOG_ERROR("sysconf(_SC_NPROCESSORS_ONLN) failed; errorCode: " << GetLastSystemErrorCode().ToString());
		return 0;
	}
	
	CPPDEVTK_ASSERT(numProcessorsOnline > 0);
	return (unsigned int)numProcessorsOnline;
	
#	else
	
	int mib[] = {CTL_HW, HW_AVAILCPU};
	int availCpu = 0;
	size_t availCpuLen = sizeof(availCpu);
	int retCode = sysctl(mib, (sizeof(mib) / sizeof(mib[0])), &availCpu, &availCpuLen, NULL, 0);
	if (retCode != ESUCCESS) {
		CPPDEVTK_LOG_ERROR("sysctl() failed for {CTL_HW, HW_AVAILCPU}; errorCode: " << GetLastSystemErrorCode().ToString());
		CPPDEVTK_ASSERT(errno != ENOMEM);
		return 0;
	}
	
	CPPDEVTK_ASSERT(availCpuLen > 0);
	CPPDEVTK_ASSERT(availCpu > 0);
	return (unsigned int)availCpu;
	
#	endif
#	endif
}

void Thread::Create() {
	CPPDEVTK_ASSERT(pData_);
	CPPDEVTK_ASSERT(pData_->GetNativeHandle() == 0);
	CPPDEVTK_ASSERT(!pData_->GetStartInfoRef().IsStarted());
	
	NativeHandleType nativeHandle = 0;
	const int kRetCode = pthread_create(&nativeHandle, kAttributes_.GetNativeHandlePtr(), &Thread::Run, pData_.get());
	if (kRetCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(MakeSystemErrorCode(kRetCode), "failed to create thread");
	}
	
	pData_->SetNativeHandle(nativeHandle);
}

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


namespace this_thread {


#if (CPPDEVTK_HAVE_THREAD_STORAGE)

CPPDEVTK_BASE_API Thread::Id GetId() CPPDEVTK_NOEXCEPT {
	return pthread_self();
}

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)

CPPDEVTK_BASE_API void Yield() /* CPPDEVTK_NOEXCEPT */ {
	const int kRetCode = sched_yield();
	if (kRetCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "sched_yield() failed");
	}
}

CPPDEVTK_BASE_API void SleepFor(int relTime) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((relTime >= 0), "relTime must be >= 0");
	
	const int kAdjustedRelTime = (relTime >= 0) ? relTime : 0;
	timespec timeSpecRelTime = {0, 0};
	timeSpecRelTime.tv_sec = kAdjustedRelTime / 1000;
	timeSpecRelTime.tv_nsec = (kAdjustedRelTime % 1000) * 1000000;
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	using this_thread::InterruptionPoint;
	
	
	InterruptionPoint();
	
	if (kAdjustedRelTime == 0) {
		const int kRetCode = TEMP_FAILURE_RETRY(nanosleep(&timeSpecRelTime, &timeSpecRelTime));
		if (kRetCode != ESUCCESS) {
			throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "nanosleep() failed");
		}
		InterruptionPoint();
		return;
	}
	
	int slept = 0;
	do {
		timespec toNanoSleep = {0, (CPPDEVTK_CHECK_INTERRUPT_REL_TIME * 1000000)};
		const int kRetCode = TEMP_FAILURE_RETRY(nanosleep(&toNanoSleep, &toNanoSleep));
		if (kRetCode != ESUCCESS) {
			throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "nanosleep() failed");
		}
		InterruptionPoint();
		slept += CPPDEVTK_CHECK_INTERRUPT_REL_TIME;
	}
	while ((slept + CPPDEVTK_CHECK_INTERRUPT_REL_TIME) < kAdjustedRelTime);
	
#	else	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	const int kRetCode = TEMP_FAILURE_RETRY(nanosleep(&timeSpecRelTime, &timeSpecRelTime));
	if (kRetCode != ESUCCESS) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "nanosleep() failed");
	}
#	endif
}


}	// namespace this_thread


#if (CPPDEVTK_HAVE_THREAD_STORAGE)

bool Thread::Id::operator==(const Id& other) const CPPDEVTK_NOEXCEPT {
	return pthread_equal(nativeId_, other.nativeId_);
}

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_HAVE_THREAD_STORAGE)
#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)
