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
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


#include <cppdevtk/base/thread_data.hpp>
#include <cppdevtk/base/thread_exception.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include "thread_local_data_ptr.hpp"

#include <windows.h>
#include <process.h>


namespace cppdevtk {
namespace base {


using this_thread::detail::pThreadLocalData;


Thread::Attributes::Attributes(): detached_(false), stackSize_(0) {}

Thread::Attributes::~Attributes() CPPDEVTK_NOEXCEPT {}

void Thread::Attributes::SetDetached(bool value) {
	detached_ = value;
}

bool Thread::Attributes::GetDetached() const {
	return detached_;
}

void Thread::Attributes::SetStackSize(::std::size_t value) {
	stackSize_ = value;
}

::std::size_t Thread::Attributes::GetStackSize() const {
	return stackSize_;
}


Thread::Id Thread::GetId() const CPPDEVTK_NOEXCEPT {
	return pData_ ? pData_->GetNativeId() : Id();
}

unsigned int Thread::GetHardwareConcurrency() CPPDEVTK_NOEXCEPT {
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	CPPDEVTK_ASSERT(systemInfo.dwNumberOfProcessors > 0);
	return systemInfo.dwNumberOfProcessors;
}

void Thread::Create() {
	CPPDEVTK_ASSERT(pData_);
	CPPDEVTK_ASSERT(pData_->GetNativeHandle() == NULL);
	CPPDEVTK_ASSERT(pData_->GetNativeId() == 0);
	CPPDEVTK_ASSERT(!pData_->GetStartInfoRef().IsStarted());
	
	detail::ThreadData::NativeIdType nativeId = 0;
	NativeHandleType nativeHandle = _beginthreadex(NULL, (unsigned)kAttributes_.GetStackSize(), &Thread::Run, pData_.get(),
			STACK_SIZE_PARAM_IS_A_RESERVATION, &nativeId);
	if (nativeHandle == NULL) {
		throw CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to create thread");
	}
	
	pData_->SetNativeHandle(nativeHandle);
	pData_->SetNativeId(nativeId);
}


namespace this_thread {


CPPDEVTK_BASE_API Thread::Id GetId() CPPDEVTK_NOEXCEPT {
	return ::GetCurrentThreadId();
}


CPPDEVTK_BASE_API void Yield() /* CPPDEVTK_NOEXCEPT */ {
	::Sleep(0);
}

CPPDEVTK_BASE_API void SleepFor(int relTime) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((relTime >= 0), "relTime must be >= 0");
	
	const DWORD kAdjustedRelTime = (relTime >= 0) ? relTime : 0;
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	using this_thread::InterruptionPoint;
	
	
	InterruptionPoint();
	
	if (kAdjustedRelTime == 0) {
		::Sleep(kAdjustedRelTime);
		InterruptionPoint();
		return;
	}
	
	DWORD slept = 0;
	do {
		::Sleep(CPPDEVTK_CHECK_INTERRUPT_REL_TIME);
		InterruptionPoint();
		slept += CPPDEVTK_CHECK_INTERRUPT_REL_TIME;
	}
	while ((slept + CPPDEVTK_CHECK_INTERRUPT_REL_TIME) < kAdjustedRelTime);
	
#	else	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	::Sleep(kAdjustedRelTime);
#	endif
}


}	// namespace this_thread


bool Thread::Id::operator==(const Id& other) const CPPDEVTK_NOEXCEPT {
	return nativeId_ == other.nativeId_;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)
