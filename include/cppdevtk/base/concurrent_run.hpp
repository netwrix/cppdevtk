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
///
/// \brief Improved \c QtConcurrent::run()
/// \details
///	- \c QThreadPool has the main disadvantage that \c start() does not return a \c QFuture
/// (also it is not policy based (scheduling and size policies for ex) and does not offer more control
/// (active and pending tasks for ex)).
/// - \c QtConcurrent::run() has the main disadvantages that the returned future is not cancelable
/// and does not support priorities.
/// - Our \c CancelableTaskExecutor supports cancelable future and priorities.
/// \note If we'll have time we'll implement our own thread pool.
/// \sa \c QThreadPool and \c QtConcurrent::run()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_BASE_CONCURRENT_RUN_HPP_INCLUDED_
#define CPPDEVTK_BASE_CONCURRENT_RUN_HPP_INCLUDED_


#include "config.hpp"
#include "cancelable.hpp"
#include "non_copyable.hpp"
#include "logger.hpp"
#include "task_canceled_exception.hpp"
#include "cassert.hpp"

#include <QtCore/QFutureInterface>
#include <QtCore/QRunnable>
#include <QtCore/QFuture>
#include <QtCore/QThreadPool>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QException>
#include <QtCore/QUnhandledException>
#else
#include <QtCore/QtCore>
#endif

#include <new>
#include <memory>


// NOTE: Qt bug #6799 and #54831

// DISCLAIMER: the "this->" used in implementation are not because I like Python but this is the simplest way to avoid error:
// "there are no arguments to 'x' that depend on a template parameter, so a declaration of 'x' must be available"
// Other solutions require more typing...


namespace cppdevtk {
namespace base {
namespace concurrent {


/// This is more an adapter so it can be used in some existing projects
class CPPDEVTK_BASE_API FutureInterfaceCancelable: public Cancelable {
public:
	explicit FutureInterfaceCancelable(QFutureInterfaceBase& futureInterfaceBase);
	
	virtual void Cancel();
	virtual bool IsCanceled() const;
private:
	QFutureInterfaceBase& futureInterfaceBase_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract base class for cancelable tasks. Users should provide concrete implementations.
template <typename TResult>
class CancelableTask {
public:
	virtual ~CancelableTask();
	
	virtual TResult Run(::std::auto_ptr<FutureInterfaceCancelable> pCancelable) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Execute cancelable task in a separate thread.
class CPPDEVTK_BASE_API CancelableTaskExecutor {
public:
	template <typename TResult>
	static QFuture<TResult> Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask, int priority = 0);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	template <typename TResult>
	static QFuture<TResult> Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask, QThreadPool& threadPool,
			int priority = 0);
#	endif
};


namespace detail {


template <typename TResult>
class StartAndRunCancelableTaskBase: public QFutureInterface<TResult>, public QRunnable, private NonCopyable {
public:
	typedef CancelableTask<TResult> CancelableTaskType;
	
	
	QFuture<TResult> Start();
protected:
	StartAndRunCancelableTaskBase(::std::auto_ptr<CancelableTaskType> pCancelableTask, int priority);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	StartAndRunCancelableTaskBase(::std::auto_ptr<CancelableTaskType> pCancelableTask, QThreadPool& threadPool, int priority);
#	endif
	
	virtual ~StartAndRunCancelableTaskBase();
	
	void ReportTaskCanceledException(const TaskCanceledException& exc);
	
	
	::std::auto_ptr<CancelableTaskType> pCancelableTask_;
private:
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	QThreadPool& threadPool_;
#	endif
	const int kPriority_;
};


template <typename TResult>
class StartAndRunCancelableTask: public StartAndRunCancelableTaskBase<TResult> {
public:
	StartAndRunCancelableTask(
			::std::auto_ptr<typename StartAndRunCancelableTask<TResult>::CancelableTaskType> pCancelableTask, int priority);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	StartAndRunCancelableTask(
			::std::auto_ptr<typename StartAndRunCancelableTask<TResult>::CancelableTaskType> pCancelableTask,
			QThreadPool& threadPool, int priority);
#	endif
	
	virtual void run();	// QRunnable::run()
};


template <>
class CPPDEVTK_BASE_API StartAndRunCancelableTask<void>: public StartAndRunCancelableTaskBase<void> {
public:
	StartAndRunCancelableTask(
			::std::auto_ptr<typename StartAndRunCancelableTask<void>::CancelableTaskType> pCancelableTask, int priority);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	StartAndRunCancelableTask(
			::std::auto_ptr<typename StartAndRunCancelableTask<void>::CancelableTaskType> pCancelableTask,
			QThreadPool& threadPool, int priority);
#	endif
	
	virtual void run();
};


}	// namespace detail




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

inline FutureInterfaceCancelable::FutureInterfaceCancelable(QFutureInterfaceBase& futureInterfaceBase): Cancelable(),
		futureInterfaceBase_(futureInterfaceBase) {}

inline void FutureInterfaceCancelable::Cancel() {
	futureInterfaceBase_.cancel();
}

inline bool FutureInterfaceCancelable::IsCanceled() const {
	return futureInterfaceBase_.isCanceled();
}


template <typename TResult>
inline CancelableTask<TResult>::~CancelableTask() {}


template <typename TResult>
inline QFuture<TResult> CancelableTaskExecutor::Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask,
		int priority) {
	// no memory leak because StartAndRunCancelableTask::QRunnable::autoDelete() so it will be deleted by QThreadPool
	return (new ::cppdevtk::base::concurrent::detail::StartAndRunCancelableTask<TResult>(pCancelableTask, priority))->Start();
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

template <typename TResult>
inline QFuture<TResult> CancelableTaskExecutor::Execute(::std::auto_ptr<CancelableTask<TResult> > pCancelableTask,
		QThreadPool& threadPool, int priority) {
	// no memory leak because StartAndRunCancelableTask::QRunnable::autoDelete() so it will be deleted by QThreadPool
	return (new ::cppdevtk::base::concurrent::detail::StartAndRunCancelableTask<TResult>(pCancelableTask, threadPool,
			priority))->Start();
}

#endif


namespace detail {


template <typename TResult>
inline QFuture<TResult> StartAndRunCancelableTaskBase<TResult>::Start() {
	this->reportStarted();
	
	QFuture<TResult> retValue = this->future();
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	threadPool_.start(this, kPriority_);
#	else
	QThreadPool::globalInstance()->start(this, kPriority_);
#	endif
	
	return retValue;
}

template <typename TResult>
inline StartAndRunCancelableTaskBase<TResult>::StartAndRunCancelableTaskBase(
		::std::auto_ptr<CancelableTaskType> pCancelableTask, int priority): QFutureInterface<TResult>(), QRunnable(),
		NonCopyable(), pCancelableTask_(pCancelableTask),
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		threadPool_(*QThreadPool::globalInstance()),
#		endif
		kPriority_(priority) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	this->setThreadPool(&threadPool_);
#	endif
	this->setRunnable(this);
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

template <typename TResult>
inline StartAndRunCancelableTaskBase<TResult>::StartAndRunCancelableTaskBase(
		::std::auto_ptr<CancelableTaskType> pCancelableTask, QThreadPool& threadPool, int priority):
		QFutureInterface<TResult>(), QRunnable(), NonCopyable(), pCancelableTask_(pCancelableTask), threadPool_(threadPool),
		kPriority_(priority) {
	this->setThreadPool(&threadPool_);
	this->setRunnable(this);
}

#endif

template <typename TResult>
inline StartAndRunCancelableTaskBase<TResult>::~StartAndRunCancelableTaskBase() {}

template <typename TResult>
inline void StartAndRunCancelableTaskBase<TResult>::ReportTaskCanceledException(const TaskCanceledException& exc) {
	QMutexLocker locker(this->mutex());
	this->exceptionStore().setException(exc);
}


template <typename TResult>
inline StartAndRunCancelableTask<TResult>::StartAndRunCancelableTask(
		::std::auto_ptr<typename StartAndRunCancelableTask<TResult>::CancelableTaskType> pCancelableTask, int priority):
		StartAndRunCancelableTaskBase<TResult>(pCancelableTask, priority) {}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

template <typename TResult>
inline StartAndRunCancelableTask<TResult>::StartAndRunCancelableTask(
		::std::auto_ptr<typename StartAndRunCancelableTask<TResult>::CancelableTaskType> pCancelableTask,
		QThreadPool& threadPool, int priority): StartAndRunCancelableTaskBase<TResult>(pCancelableTask, threadPool,
		priority) {}

#endif

template <typename TResult>
void StartAndRunCancelableTask<TResult>::run() {
	if (this->isCanceled()) {
		this->ReportTaskCanceledException(CPPDEVTK_TASK_CANCELED_EXCEPTION("task was canceled when started running"));
		this->reportFinished();
		
		return;
	}
	
	TResult result;
	try {
		result = this->pCancelableTask_->Run(::std::auto_ptr<FutureInterfaceCancelable>(new FutureInterfaceCancelable(*this)));
	}
	catch (const TaskCanceledException& exc) {
		//CPPDEVTK_LOG_INFO("TaskCanceledException: " << Exception::GetDetailedInfo(exc));
		CPPDEVTK_ASSERT(this->isCanceled());
		this->ReportTaskCanceledException(exc);
		this->reportFinished();
		return;
	}
	catch (const QtException& exc) {
		//CPPDEVTK_LOG_ERROR("task failed; QtException: " << Exception::GetDetailedInfo(exc));
		this->reportException(exc);
		this->reportFinished();
		return;
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_ERROR("task failed; ::std::exception: " << Exception::GetDetailedInfo(exc));
		this->reportException(QtUnhandledException());
		this->reportFinished();
		return;
	}
	catch (...) {
		CPPDEVTK_LOG_ERROR("task failed; unknown exception");
		this->reportException(QtUnhandledException());
		this->reportFinished();
		return;
	}
	
	this->reportResult(result);
	this->reportFinished();
}


inline StartAndRunCancelableTask<void>::StartAndRunCancelableTask(
		::std::auto_ptr<typename StartAndRunCancelableTask<void>::CancelableTaskType> pCancelableTask, int priority):
		StartAndRunCancelableTaskBase<void>(pCancelableTask, priority) {}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

inline StartAndRunCancelableTask<void>::StartAndRunCancelableTask(
		::std::auto_ptr<typename StartAndRunCancelableTask<void>::CancelableTaskType> pCancelableTask,
		QThreadPool& threadPool, int priority): StartAndRunCancelableTaskBase<void>(pCancelableTask, threadPool,
		priority) {}

#endif


}	// namespace detail
}	// namespace concurrent
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CONCURRENT_RUN_HPP_INCLUDED_
