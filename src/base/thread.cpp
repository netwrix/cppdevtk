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
#include <cppdevtk/base/thread_data.hpp>
#include <cppdevtk/base/thread_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/mutex.hpp>
#include <cppdevtk/base/condition_variable.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include "thread_local_data_ptr.hpp"

#include <exception>


namespace cppdevtk {
namespace base {


using this_thread::InterruptionPoint;
using this_thread::DisableInterruptionGuard;
using this_thread::detail::pThreadLocalData;
using ::std::bad_exception;
using ::std::exception;
using ::std::terminate;


Thread::Thread(const Attributes& attributes): NonCopyable(), kAttributes_(attributes),
		pData_(new Data(MainFunctionType(CPPDEVTK_TR1_NS::bind(&Thread::Main, this)), kAttributes_.GetDetached())) {}

Thread::Thread(const MainFunctionType& mainFunction, const Attributes& attributes): NonCopyable(), kAttributes_(attributes),
		pData_(new Data(mainFunction, kAttributes_.GetDetached())) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(mainFunction, "mainFunction must have a target");
}

Thread::~Thread() CPPDEVTK_NOEXCEPT {
	try {
		if (IsJoinable()) {
#			if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			
			RequestInterruption();
			
			try {
				DisableInterruptionGuard disableInterruptionGuard;
				MainFunctionType::result_type retCode = EXIT_FAILURE;
				CPPDEVTK_LOG_TRACE("thread destructor: joining...");
				Join(retCode);
				CPPDEVTK_LOG_DEBUG("thread destructor: joined; child thread retCode: " << retCode);
			}
			catch (const ThreadInterruptedException& exc) {
				CPPDEVTK_LOG_DEBUG("thread destructor: child thread interrupted: " << Exception::GetDetailedInfo(exc));
			}
			catch (const exception& exc) {
				if (IsJoinable()) {
					CPPDEVTK_LOG_FATAL("thread destructor: join threw exception: " << Exception::GetDetailedInfo(exc));
					terminate();
				}
				else {
					CPPDEVTK_LOG_INFO("thread destructor: join threw exception: " << Exception::GetDetailedInfo(exc));
				}
			}
			catch (...) {
				if (IsJoinable()) {
					CPPDEVTK_LOG_FATAL("thread destructor: join threw unknown exception");
					terminate();
				}
				else {
					CPPDEVTK_LOG_INFO("thread destructor: join threw unknown exception");
				}
			}
			
#			else	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			CPPDEVTK_LOG_FATAL("thread destructor: interruption disabled and thread is joinable");
			terminate();
#			endif
		}
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_FATAL("thread destructor: caught exception: " << Exception::GetDetailedInfo(exc));
		terminate();
	}
	catch (...) {
		CPPDEVTK_LOG_FATAL("thread destructor: caught unknown exception");
		terminate();
	}
}

Thread::NativeHandleType Thread::GetNativeHandle() {
	return pData_ ? pData_->GetNativeHandle() : 0;
}

void Thread::Start() {
	if (!pData_) {
		return;
	}
	
	DataPtr pTmpData = pData_;
	
	if (pTmpData->GetStartInfoRef().IsStarted()) {
		return;
	}
	
	Create();
	
	// ensure that data ptr is shared in child thread (this is done before SetStartAndNotifyOne())
	// so it can be safely deleted in parent thread when no longer needed
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	DisableInterruptionGuard disableInterruptionGuard;
#	endif
	pTmpData->GetStartInfoRef().WaitToStart();	// FIXME: problems if throws. (retry and) terminate?
	
	if (kAttributes_.GetDetached()) {
		pData_.reset();
		CPPDEVTK_ASSERT(!IsJoinable());
	}
	else {
		CPPDEVTK_ASSERT(IsJoinable());
	}
}

void Thread::Join(MainFunctionType::result_type& retCode) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	if (GetId() == this_thread::GetId()) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("thread can not join itself");
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionPoint();
#	endif
	
	CPPDEVTK_ASSERT(pData_);
	DataPtr pTmpData = pData_;
	
	pTmpData->GetJoinedOrDetachedInfoRef().WaitToJoin();
	
	pData_.reset();
	CPPDEVTK_ASSERT(!IsJoinable());
	
	if (pTmpData->GetExceptionPtr()) {
		RethrowException(pTmpData->GetExceptionPtr());
	}
#	if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
	if (pTmpData->GetThrowStdBadException()) {
		throw bad_exception();
	}
#	endif
	
	retCode = pTmpData->GetRetCode();
}

bool Thread::TryJoin(ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	if (GetId() == this_thread::GetId()) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("thread can not join itself");
	}
	
	CPPDEVTK_ASSERT(pData_);
	DataPtr pTmpData = pData_;
	
	
	if (!pTmpData->GetJoinedOrDetachedInfoRef().IsJoinedOrDetached()) {
		return false;
	}
	
	pData_.reset();
	CPPDEVTK_ASSERT(!IsJoinable());
	
	exceptionPtr = GetTryJoinExceptionPtr(pTmpData);
	
	if (!exceptionPtr) {
		retCode = pTmpData->GetRetCode();
	}
	
	return true;
}

bool Thread::TryJoinFor(int relTime, ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	if (GetId() == this_thread::GetId()) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("thread can not join itself");
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionPoint();
#	endif
	
	CPPDEVTK_ASSERT(pData_);
	DataPtr pTmpData = pData_;
	
	if (!pTmpData->GetJoinedOrDetachedInfoRef().WaitToJoinFor(relTime)) {
		return false;
	}
	
	pData_.reset();
	CPPDEVTK_ASSERT(!IsJoinable());
	
	exceptionPtr = GetTryJoinExceptionPtr(pTmpData);
	
	if (!exceptionPtr) {
		retCode = pTmpData->GetRetCode();
	}
	
	return true;
}

bool Thread::TryJoinUntil(::std::time_t absTime, ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	if (GetId() == this_thread::GetId()) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("thread can not join itself");
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionPoint();
#	endif
	
	CPPDEVTK_ASSERT(pData_);
	DataPtr pTmpData = pData_;
	
	if (!pTmpData->GetJoinedOrDetachedInfoRef().WaitToJoinUntil(absTime)) {
		return false;
	}
	
	pData_.reset();
	CPPDEVTK_ASSERT(!IsJoinable());
	
	exceptionPtr = GetTryJoinExceptionPtr(pTmpData);
	
	if (!exceptionPtr) {
		retCode = pTmpData->GetRetCode();
	}
	
	return true;
}

void Thread::Detach() {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	CPPDEVTK_ASSERT(pData_);
	DataPtr pTmpData = pData_;
	
	pTmpData->GetJoinedOrDetachedInfoRef().SetDetach();
	
	pData_.reset();
	CPPDEVTK_ASSERT(!IsJoinable());
}

#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

void Thread::RequestInterruption() {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	CPPDEVTK_ASSERT(pData_);
	pData_->GetInterruptionInfoRef().SetInterruptionRequestedAndIfIntPtThenWaitingCondVarNotifyAll(true);
}

bool Thread::IsInterruptionRequested() const {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	CPPDEVTK_ASSERT(pData_);
	return pData_->GetInterruptionInfoRef().IsInterruptionRequested();
}

#endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

void Thread::Swap(Thread& other) CPPDEVTK_NOEXCEPT {
	pData_.swap(other.pData_);
}

ExceptionPtr Thread::GetTryJoinExceptionPtr(DataPtr pData) {
	CPPDEVTK_ASSERT(pData);
	
	ExceptionPtr exceptionPtr = pData->GetExceptionPtr();
	
#	if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
	if (!exceptionPtr) {
		if (pData->GetThrowStdBadException()) {
			exceptionPtr = MakeExceptionPtr(bad_exception());
			if (!exceptionPtr) {
				CPPDEVTK_LOG_FATAL("failed to propagate exception from child to parent thread");
				terminate();
			}
		}
	}
	else {
		CPPDEVTK_ASSERT(pData->GetThrowStdBadException());
	}
#	endif
	
	return exceptionPtr;
}

#if (CPPDEVTK_PLATFORM_UNIX)
void* Thread::Run(void* pVoidData)
#elif (CPPDEVTK_PLATFORM_WINDOWS)
unsigned __stdcall Thread::Run(void* pVoidData)
#endif
		try {
	CPPDEVTK_ASSERT(pVoidData != NULL);
	CPPDEVTK_ASSERT(pThreadLocalData == NULL);
	
	DataPtr pData = static_cast<Data*>(pVoidData)->shared_from_this();
	pData->GetStartInfoRef().SetStartAndNotifyOne();
	
	pThreadLocalData = pData.get();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pThreadLocalData)) {
		pThreadLocalData = NULL;
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
#	if (CPPDEVTK_PLATFORM_UNIX)
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	// POSIX does not specify for pthread_setcancelstate() that oldstate may be NULL
	int oldCancelState = PTHREAD_CANCEL_ENABLE;
	CPPDEVTK_VERIFY(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldCancelState) == ESUCCESS);
#	endif
#	endif
	
	try {
		pData->SetRetCode(pData->GetMainFunctionRef()());
	}
	catch (...) {
		pData->SetExceptionPtr(CurrentException());
#		if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
		pData->SetThrowStdBadException(true);
#		endif
	}
	
	if (!pData->GetJoinedOrDetachedInfoRef().IfIsNotJoinedOrDetachedThenSetJoinAndNotifyOne()) {
		// detached; if exceptionPtr then terminate()
		const ExceptionPtr kExceptionPtr = pData->GetExceptionPtr();
		if (kExceptionPtr) {
			try {
				RethrowException(kExceptionPtr);
			}
#			if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			catch (const ThreadInterruptedException&) {
				CPPDEVTK_LOG_INFO("detached thread interrupted");
			}
#			endif
			catch (const exception& exc) {
				CPPDEVTK_LOG_FATAL("detached thread threw exception: " << Exception::GetDetailedInfo(exc));
				terminate();
			}
			catch (...) {
				CPPDEVTK_LOG_FATAL("detached thread threw unknown exception");
				terminate();
			}
		}
#		if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
		else {
			if (pData->GetThrowStdBadException()) {
				CPPDEVTK_LOG_FATAL("detached thread threw bad_exception");
				terminate();
			}
		}
#		endif
	}
	
	return 0;
}
catch (const exception& exc) {
	CPPDEVTK_LOG_FATAL("thread Run() C routine threw: " << Exception::GetDetailedInfo(exc));
	terminate();
	return 0;
}
catch (...) {
	CPPDEVTK_LOG_FATAL("thread Run() C routine threw unknown exception");
	terminate();
	return 0;
}


CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, Thread::Id threadId) {
	return os << threadId.nativeId_;
}


namespace this_thread {


#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

CPPDEVTK_BASE_API bool IsInterruptionEnabled() {
	return (pThreadLocalData != NULL) ? pThreadLocalData->GetInterruptionInfoRef().IsInterruptionEnabled() : false;
}

CPPDEVTK_BASE_API bool IsInterruptionRequested() {
	return (pThreadLocalData != NULL) ? pThreadLocalData->GetInterruptionInfoRef().IsInterruptionRequested() : false;
}

CPPDEVTK_BASE_API void InterruptionPoint() {
	if (IsInterruptionEnabled()) {
		CPPDEVTK_ASSERT(pThreadLocalData != NULL);
		pThreadLocalData->GetInterruptionInfoRef().InterruptionRequest();
	}
}


DisableInterruptionGuard::DisableInterruptionGuard(): NonCopyable(),
		kInterruptionWasEnabled_(IsInterruptionEnabled()) {
	if (kInterruptionWasEnabled_) {
		if (pThreadLocalData != NULL) {
			pThreadLocalData->GetInterruptionInfoRef().SetInterruptionEnabled(false);
		}
	}
}

DisableInterruptionGuard::~DisableInterruptionGuard() {
	if (pThreadLocalData != NULL) {
		pThreadLocalData->GetInterruptionInfoRef().SetInterruptionEnabled(kInterruptionWasEnabled_);
	}
}


RestoreInterruptionGuard::RestoreInterruptionGuard(const DisableInterruptionGuard& disableInterruptionGuard): NonCopyable() {
	if (disableInterruptionGuard.kInterruptionWasEnabled_) {
		if (pThreadLocalData != NULL) {
			pThreadLocalData->GetInterruptionInfoRef().SetInterruptionEnabled(true);
		}
	}
}

RestoreInterruptionGuard::~RestoreInterruptionGuard() {
	if (pThreadLocalData != NULL) {
		pThreadLocalData->GetInterruptionInfoRef().SetInterruptionEnabled(false);
	}
}

#endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)


namespace detail {


/* CPPDEVTK_BASE_API */ void SetInterruptionWaitingConditionVariable(ConditionVariable* value) {
	if (pThreadLocalData != NULL) {
		pThreadLocalData->GetInterruptionInfoRef().SetWaitingConditionVariable(value);
	}
}


}	// namespace detail


}	// namespace this_thread


// NOTE: do not inline: nativeId_ and pThread_ are not exported!

Thread::Id::Id() CPPDEVTK_NOEXCEPT: nativeId_(0) {}

QString Thread::Id::ToString() const {
	return QString::number(nativeId_);
}

bool Thread::Id::operator<(const Id& other) const CPPDEVTK_NOEXCEPT {
	return nativeId_ < other.nativeId_;
}

Thread::Id::Id(NativeIdType nativeId) CPPDEVTK_NOEXCEPT: nativeId_(nativeId) {}


}	// namespace base
}	// namespace cppdevtk
