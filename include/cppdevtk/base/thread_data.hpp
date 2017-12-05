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


#ifndef CPPDEVTK_BASE_THREAD_DATA_HPP_INCLUDED_
#define CPPDEVTK_BASE_THREAD_DATA_HPP_INCLUDED_


#include "config.hpp"
#include "thread_data_types.hpp"
#include "non_copyable.hpp"
#include "mutex.hpp"
#include "condition_variable.hpp"
#include "exception.hpp"
#include "thread_exception.hpp"

#include <cstddef>
#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {
namespace detail {


class /* CPPDEVTK_BASE_API */ ThreadData: public CPPDEVTK_TR1_NS::enable_shared_from_this<ThreadData>, private NonCopyable {
public:
	class StartInfo: private NonCopyable {
	public:
		StartInfo();
		
		bool IsStarted() const;
		void SetStartAndNotifyOne();
		void WaitToStart();
	private:
		volatile bool started_;
		mutable Mutex startedMtx_;
		ConditionVariable startedCondVar_;
	};
	
	
	class JoinedOrDetachedInfo: private NonCopyable {
	public:
		explicit JoinedOrDetachedInfo(bool detached);
		
		bool IsJoinedOrDetached() const;
		void SetDetach();
		void SetJoinAndNotifyOne();
		bool IfIsNotJoinedOrDetachedThenSetJoinAndNotifyOne();	// atomic test + set; return true IfIsNotJoinedOrDetached
		void WaitToJoin();
		bool WaitToJoinFor(int relTime);
		bool WaitToJoinUntil(::std::time_t absTime);
	private:
		volatile bool joinedOrDetached_;
		mutable Mutex joinedOrDetachedMtx_;
		ConditionVariable joinedOrDetachedCondVar_;
	};
	
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	class InterruptionInfo: private NonCopyable {
	public:
		InterruptionInfo();
		
		bool IsInterruptionEnabled() const;
		void SetInterruptionEnabled(bool value);
		
		void SetWaitingConditionVariable(ConditionVariable* value);
		bool IsInterruptionRequested() const;
		void SetInterruptionRequestedAndIfIntPtThenWaitingCondVarNotifyAll(bool value);
		void InterruptionRequest();
	private:
		bool interruptionEnabled_;	// accessed only in this_thread so no need to synchronize
		ConditionVariable* pWaitingConditionVariable_;
		volatile bool interruptionRequested_;
		mutable DefaultMutex interruptionRequestedMtx_;
	};
#	endif
	
	
	typedef ThreadNativeHandleType NativeHandleType;
	typedef ThreadNativeIdType NativeIdType;
	typedef ThreadMainFunctionType MainFunctionType;
	
	
	ThreadData(const MainFunctionType& mainFunction, bool detached);
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	~ThreadData();
#	endif
	
	void SetNativeHandle(NativeHandleType nativeHandle);
	NativeHandleType GetNativeHandle() const;
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	void SetNativeId(NativeIdType nativeId);
#	endif
	NativeIdType GetNativeId() const;
	
	StartInfo& GetStartInfoRef();
	const StartInfo& GetStartInfoRef() const;
	
	const MainFunctionType& GetMainFunctionRef() const;
	
	JoinedOrDetachedInfo& GetJoinedOrDetachedInfoRef();
	const JoinedOrDetachedInfo& GetJoinedOrDetachedInfoRef() const;
	
	MainFunctionType::result_type GetRetCode() const;
	void SetRetCode(MainFunctionType::result_type value);
	
	ExceptionPtr GetExceptionPtr() const;
	void SetExceptionPtr(const ExceptionPtr& value);
	
#	if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
	bool GetThrowStdBadException() const;
	void SetThrowStdBadException(bool value);
#	endif
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionInfo& GetInterruptionInfoRef();
	const InterruptionInfo& GetInterruptionInfoRef() const;
#	endif
private:
	NativeHandleType nativeHandle_;	// used only by parent thread so no need to synchronize
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	ThreadNativeIdType nativeId_;	// used only by parent thread so no need to synchronize
#	endif
	StartInfo startInfo_;
	MainFunctionType mainFunction_;	// set in parent thread and used in child thread; no need to synchronize
	JoinedOrDetachedInfo joinedOrDetachedInfo_;
	MainFunctionType::result_type retCode_;
	ExceptionPtr exceptionPtr_;
#	if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
	bool throwStdBadException_;	// CurrentException() may fail
#	endif
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	InterruptionInfo interruptionInfo_;
#	endif
};


}	// namespace detail


namespace this_thread {
namespace detail {


#define CPPDEVTK_DETAIL_DISABLE_GET_THREAD_LOCAL_DATA_PTR 1
#if (!CPPDEVTK_DETAIL_DISABLE_GET_THREAD_LOCAL_DATA_PTR)
#if (CPPDEVTK_HAVE_THREAD_STORAGE)
// Returned pointer is non-NULL only during execution of thread main function and must not be freed!
/* CPPDEVTK_BASE_API */ const ::cppdevtk::base::detail::ThreadData* GetThreadLocalDataPtr();
#endif
#endif


}	// namespace detail
}	// namespace this_thread


namespace detail {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline ThreadData::StartInfo::StartInfo(): NonCopyable(), started_ (false), startedMtx_(), startedCondVar_() {}

inline bool ThreadData::StartInfo::IsStarted() const {
	LockGuard<Mutex> lockGuard(startedMtx_);
	return started_;
}

inline void ThreadData::StartInfo::SetStartAndNotifyOne() {
	LockGuard<Mutex> lockGuard(startedMtx_);
	started_ = true;
	startedCondVar_.NotifyOne();
}

inline void ThreadData::StartInfo::WaitToStart() {
	UniqueLock<Mutex> uniqueLock(startedMtx_);
	while (!started_) {
		startedCondVar_.Wait(uniqueLock);
	}
}


inline ThreadData::JoinedOrDetachedInfo::JoinedOrDetachedInfo(bool detached): NonCopyable(), joinedOrDetached_(detached),
		joinedOrDetachedMtx_(), joinedOrDetachedCondVar_() {}

inline bool ThreadData::JoinedOrDetachedInfo::IsJoinedOrDetached() const {
	LockGuard<Mutex> lockGuard(joinedOrDetachedMtx_);
	return joinedOrDetached_;
}

inline void ThreadData::JoinedOrDetachedInfo::SetDetach() {
	LockGuard<Mutex> lockGuard(joinedOrDetachedMtx_);
	joinedOrDetached_ = true;
}

inline void ThreadData::JoinedOrDetachedInfo::SetJoinAndNotifyOne() {
	LockGuard<Mutex> lockGuard(joinedOrDetachedMtx_);
	joinedOrDetached_ = true;
	joinedOrDetachedCondVar_.NotifyOne();
}

inline bool ThreadData::JoinedOrDetachedInfo::IfIsNotJoinedOrDetachedThenSetJoinAndNotifyOne() {
	LockGuard<Mutex> lockGuard(joinedOrDetachedMtx_);
	if (joinedOrDetached_) {
		return false;
	}
	
	joinedOrDetached_ = true;
	joinedOrDetachedCondVar_.NotifyOne();
	return true;
}

inline void ThreadData::JoinedOrDetachedInfo::WaitToJoin() {
	UniqueLock<Mutex> uniqueLock(joinedOrDetachedMtx_);
	while (!joinedOrDetached_) {
		joinedOrDetachedCondVar_.Wait(uniqueLock);
	}
}

inline bool ThreadData::JoinedOrDetachedInfo::WaitToJoinFor(int relTime) {
	UniqueLock<Mutex> uniqueLock(joinedOrDetachedMtx_);
	while (!joinedOrDetached_) {
		if (joinedOrDetachedCondVar_.WaitFor(uniqueLock, relTime) == base::cv_status::timeout) {
			return joinedOrDetached_;
		}
	}
	return true;
}

inline bool ThreadData::JoinedOrDetachedInfo::WaitToJoinUntil(::std::time_t absTime) {
	UniqueLock<Mutex> uniqueLock(joinedOrDetachedMtx_);
	while (!joinedOrDetached_) {
		if (joinedOrDetachedCondVar_.WaitUntil(uniqueLock, absTime) == base::cv_status::timeout) {
			return joinedOrDetached_;
		}
	}
	return true;
}


#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

inline ThreadData::InterruptionInfo::InterruptionInfo(): NonCopyable(), interruptionEnabled_(true),
		pWaitingConditionVariable_(NULL), interruptionRequested_(false), interruptionRequestedMtx_() {}

inline bool ThreadData::InterruptionInfo::IsInterruptionEnabled() const {
	return interruptionEnabled_;
}

inline void ThreadData::InterruptionInfo::SetInterruptionEnabled(bool value) {
	interruptionEnabled_ = value;
}

inline void ThreadData::InterruptionInfo::SetWaitingConditionVariable(ConditionVariable* value) {
	DefaultLockGuard lockGuard(interruptionRequestedMtx_);
	pWaitingConditionVariable_ = value;
}

inline bool ThreadData::InterruptionInfo::IsInterruptionRequested() const {
	DefaultLockGuard lockGuard(interruptionRequestedMtx_);
	return interruptionRequested_;
}

inline void ThreadData::InterruptionInfo::SetInterruptionRequestedAndIfIntPtThenWaitingCondVarNotifyAll(bool value) {
	DefaultLockGuard lockGuard(interruptionRequestedMtx_);
	interruptionRequested_ = value;
	if (interruptionEnabled_ && interruptionRequested_) {
		if (pWaitingConditionVariable_ != NULL) {
			pWaitingConditionVariable_->NotifyAll();
		}
	}
}

inline void ThreadData::InterruptionInfo::InterruptionRequest() {
	DefaultLockGuard lockGuard(interruptionRequestedMtx_);
	if (interruptionRequested_) {
		interruptionRequested_ = false;
		throw CPPDEVTK_THREAD_INTERRUPTED_EXCEPTION();
	}
}

#endif


inline ThreadData::ThreadData(const MainFunctionType& mainFunction, bool detached): CPPDEVTK_TR1_NS::enable_shared_from_this<ThreadData>(), NonCopyable(),
		nativeHandle_(0),
#		if (CPPDEVTK_PLATFORM_WINDOWS)
		nativeId_(0),
#		endif
		startInfo_(), mainFunction_(mainFunction), joinedOrDetachedInfo_(detached), retCode_(EXIT_FAILURE), exceptionPtr_()
#		if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
		, throwStdBadException_(false)
#		endif
#		if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
		, interruptionInfo_()
#		endif
		{}

inline void ThreadData::SetNativeHandle(NativeHandleType nativeHandle) {
	nativeHandle_ = nativeHandle;
}

inline ThreadData::NativeHandleType ThreadData::GetNativeHandle() const {
	return nativeHandle_;
}

#if (CPPDEVTK_PLATFORM_WINDOWS)

inline void ThreadData::SetNativeId(NativeIdType nativeId) {
	nativeId_ = nativeId;
}

#endif

inline ThreadData::NativeIdType ThreadData::GetNativeId() const {
#	if (CPPDEVTK_PLATFORM_UNIX)
	return nativeHandle_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	return nativeId_;
#	endif
}

inline ThreadData::StartInfo& ThreadData::GetStartInfoRef() {
	return startInfo_;
}

inline const ThreadData::StartInfo& ThreadData::GetStartInfoRef() const {
	return startInfo_;
}

inline const ThreadData::MainFunctionType& ThreadData::GetMainFunctionRef() const {
	return mainFunction_;
}

inline ThreadData::JoinedOrDetachedInfo& ThreadData::GetJoinedOrDetachedInfoRef() {
	return joinedOrDetachedInfo_;
}

inline const ThreadData::JoinedOrDetachedInfo& ThreadData::GetJoinedOrDetachedInfoRef() const {
	return joinedOrDetachedInfo_;
}

inline ThreadData::MainFunctionType::result_type ThreadData::GetRetCode() const {
	return retCode_;
}

inline void ThreadData::SetRetCode(MainFunctionType::result_type value) {
	retCode_ = value;
}

inline ExceptionPtr ThreadData::GetExceptionPtr() const {
	return exceptionPtr_;
}

inline void ThreadData::SetExceptionPtr(const ExceptionPtr& value) {
	exceptionPtr_ = value;
}

#if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

inline bool ThreadData::GetThrowStdBadException() const {
	return throwStdBadException_;
}

inline void ThreadData::SetThrowStdBadException(bool value) {
	throwStdBadException_ = value;
}

#endif

#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

inline ThreadData::InterruptionInfo& ThreadData::GetInterruptionInfoRef() {
	return interruptionInfo_;
}

inline const ThreadData::InterruptionInfo& ThreadData::GetInterruptionInfoRef() const {
	return interruptionInfo_;
}

#endif


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_THREAD_DATA_HPP_INCLUDED_
