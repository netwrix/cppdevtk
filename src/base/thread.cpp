/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include <cppdevtk/base/thread.hpp>
#include <cppdevtk/base/time_utils.hpp>
#include <cppdevtk/base/dbc.hpp>


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


#include <cppdevtk/base/thread_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/mutex.hpp>
#include <cppdevtk/base/condition_variable.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>
#include "thread_local_data_ptr.hpp"
#include "thread_data.hpp"

#if (CPPDEVTK_PLATFORM_MACOSX)
#include <QtCore/QTextStream>
#endif

#include <exception>


#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


namespace cppdevtk {
namespace base {


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


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
				CPPDEVTK_LOG_DEBUG("thread destructor: child thread interrupted: " << exc.What());
				SuppressUnusedWarning(exc);
			}
			catch (const exception& exc) {
				if (IsJoinable()) {
					CPPDEVTK_LOG_FATAL("thread destructor: join threw exception: " << Exception::GetDetailedInfo(exc));
					CPPDEVTK_ASSERT(0 && "thread destructor: join threw exception");
					terminate();
				}
				else {
					CPPDEVTK_LOG_WARN("thread destructor: join threw exception: " << Exception::GetDetailedInfo(exc));
				}
				SuppressUnusedWarning(exc);
			}
			catch (...) {
				if (IsJoinable()) {
					CPPDEVTK_LOG_FATAL("thread destructor: join threw unknown exception");
					CPPDEVTK_ASSERT(0 && "thread destructor: join threw unknown exception");
					terminate();
				}
				else {
					CPPDEVTK_LOG_WARN("thread destructor: join threw unknown exception");
				}
			}
			
#			else	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			CPPDEVTK_LOG_FATAL("thread destructor: interruption disabled and thread is joinable");
			CPPDEVTK_ASSERT(0 && "thread destructor: interruption disabled and thread is joinable");
			terminate();
#			endif
		}
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_FATAL("thread destructor: caught exception: " << Exception::GetDetailedInfo(exc));
		CPPDEVTK_ASSERT(0 && "thread destructor: caught exception");
		SuppressUnusedWarning(exc);
		terminate();
	}
	catch (...) {
		CPPDEVTK_LOG_FATAL("thread destructor: caught unknown exception");
		CPPDEVTK_ASSERT(0 && "thread destructor: caught unknown exception");
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
	CPPDEVTK_LOG_TRACE("waiting for child thread to start...");
	try {
		pTmpData->GetStartInfoRef().WaitToStart();
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_WARN("failed to wait for child thread to start; retrying... exc: " << Exception::GetDetailedInfo(exc));
		try {	// last chance...
			this_thread::SleepFor(CPPDEVTK_CHECK_INTERRUPT_REL_TIME);
			pTmpData->GetStartInfoRef().WaitToStart();
		}
		catch (const exception& exc1) {
			CPPDEVTK_LOG_FATAL("failed to wait for child thread to start; exc1: " << Exception::GetDetailedInfo(exc1));
			CPPDEVTK_ASSERT(0 && "failed to wait for child thread to start");
			SuppressUnusedWarning(exc1);
			terminate();
		}
		SuppressUnusedWarning(exc);
	}
	CPPDEVTK_LOG_TRACE("child thread started");
	
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
	
	if (pTmpData->GetExceptionPtr() != NULL) {
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
	
	if (exceptionPtr == NULL) {
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
	
	if (relTime <= 0) {
		return TryJoin(exceptionPtr, retCode);
	}
	
	CPPDEVTK_ASSERT(pData_);
	DataPtr pTmpData = pData_;
	
	if (!pTmpData->GetJoinedOrDetachedInfoRef().WaitToJoinFor(relTime)) {
		return false;
	}
	
	pData_.reset();
	CPPDEVTK_ASSERT(!IsJoinable());
	
	exceptionPtr = GetTryJoinExceptionPtr(pTmpData);
	
	if (exceptionPtr == NULL) {
		retCode = pTmpData->GetRetCode();
	}
	
	return true;
}

bool Thread::TryJoinUntil(::std::time_t absTime, ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode) {
	using ::std::time_t;
	
	
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(IsJoinable(), "thread must be joinable");
	
	if (GetId() == this_thread::GetId()) {
		throw CPPDEVTK_DEADLOCK_EXCEPTION_WA("thread can not join itself");
	}
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionPoint();
#	endif
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	return (kSeconds <= 0) ? TryJoin(exceptionPtr, retCode) : TryJoinFor(kSeconds, exceptionPtr, retCode);
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
			exceptionPtr = MakeExceptionPtr(bad_exception(), false);
			if (!exceptionPtr) {
				CPPDEVTK_LOG_FATAL("failed to propagate exception from child to parent thread");
				CPPDEVTK_ASSERT(0 && "failed to propagate exception from child to parent thread");
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
	CPPDEVTK_LOG_TRACE("notifying parent thread that child thread started...");
	pData->GetStartInfoRef().SetStartAndNotifyOne();
	CPPDEVTK_LOG_TRACE("parent thread notified that child thread started");
	
	pThreadLocalData = pData.get();
#	if (BOOST_VERSION >= 105000)
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN(void) {
#	else
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN() {
#	endif
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
		const MainFunctionType::result_type kRetCode = pData->GetMainFunctionRef()();
		CPPDEVTK_LOG_DEBUG("thread main function returned: " << kRetCode);
		pData->SetRetCode(kRetCode);
	}
	catch (...) {
		CPPDEVTK_LOG_DEBUG("thread main function threw");
		pData->SetExceptionPtr(CurrentException(false));
#		if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
		pData->SetThrowStdBadException(true);
#		endif
	}
	
	if (!pData->GetJoinedOrDetachedInfoRef().IfIsNotJoinedOrDetachedThenSetJoinAndNotifyOne()) {
		// detached; if exceptionPtr then terminate()
		const ExceptionPtr kExceptionPtr = pData->GetExceptionPtr();
		if (kExceptionPtr != NULL) {
			try {
				RethrowException(kExceptionPtr);
			}
#			if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
			catch (const ThreadInterruptedException& exc) {
				CPPDEVTK_LOG_INFO("detached thread interrupted: " << exc.What());
				SuppressUnusedWarning(exc);
			}
#			endif
			catch (const exception& exc) {
				CPPDEVTK_LOG_FATAL("detached thread threw exception: " << Exception::GetDetailedInfo(exc));
				CPPDEVTK_ASSERT(0 && "detached thread threw exception");
				SuppressUnusedWarning(exc);
				terminate();
			}
			catch (...) {
				CPPDEVTK_LOG_FATAL("detached thread threw unknown exception");
				CPPDEVTK_ASSERT(0 && "detached thread threw unknown exception");
				terminate();
			}
		}
#		if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
		else {
			if (pData->GetThrowStdBadException()) {
				CPPDEVTK_LOG_FATAL("detached thread threw bad_exception");
				CPPDEVTK_ASSERT(0 && "detached thread threw bad_exception");
				terminate();
			}
		}
#		endif
	}
	
	return 0;
}
catch (const exception& exc) {
	CPPDEVTK_LOG_FATAL("thread Run() C routine threw exception: " << Exception::GetDetailedInfo(exc));
	CPPDEVTK_ASSERT(0 && "thread Run() C routine threw exception");
	SuppressUnusedWarning(exc);
	terminate();
	return 0;
}
catch (...) {
	CPPDEVTK_LOG_FATAL("thread Run() C routine threw unknown exception");
	CPPDEVTK_ASSERT(0 && "thread Run() C routine threw  unknown exception");
	terminate();
	return 0;
}


CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, Thread::Id threadId) {
	return os << threadId.nativeId_;
}


#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


namespace this_thread {


CPPDEVTK_BASE_API void SleepUntil(::std::time_t absTime) {
	using ::std::time_t;
	
	const time_t kCurrTime = GetCurrentTime();
	const time_t kSeconds = ::std::difftime(absTime, kCurrTime);
	
	CPPDEVTK_DBC_CHECK_ARGUMENT((kSeconds >= 0), "absTime is in the past");
	
	SleepFor(kSeconds * 1000);
}

#if (CPPDEVTK_HAVE_THREAD_STORAGE)
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
#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


}	// namespace this_thread


#if (CPPDEVTK_HAVE_THREAD_STORAGE)

// NOTE: do not inline: nativeId_ is not exported!

Thread::Id::Id() CPPDEVTK_NOEXCEPT: nativeId_(0) {}

QString Thread::Id::ToString() const {
#	if (CPPDEVTK_PLATFORM_MACOSX)
	QString id;
	QTextStream textStream(&id);
	textStream << nativeId_;
	return id;
#	else
	return QString::number(nativeId_);
#	endif
}

bool Thread::Id::operator<(const Id& other) const CPPDEVTK_NOEXCEPT {
	return nativeId_ < other.nativeId_;
}

Thread::Id::Id(NativeIdType nativeId) CPPDEVTK_NOEXCEPT: nativeId_(nativeId) {}

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_HAVE_THREAD_STORAGE)
#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)
