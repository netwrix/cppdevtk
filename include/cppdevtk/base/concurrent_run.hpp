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

#include <QtCore/QFutureInterfaceBase>
#include <QtCore/QFutureInterface>
#include <QtCore/QFuture>
#include <QtCore/QRunnable>
#include <QtCore/QThreadPool>
#include <QtCore/QtGlobal>
#include <QtCore/QMutexLocker>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QException>
#include <QtCore/QUnhandledException>
#else
#include <QtCore/QtCore>
#endif

#include <new>
#include <memory>


// DISCLAIMER: the "this->" used in implementation are not because I'm a Python fan, but this is the simplest way to avoid error:
// "there are no arguments to 'x' that depend on a template parameter, so a declaration of 'x' must be available"
// Other solutions require more typing...


#define CPPDEVTK_EXECUTOR_TEST 0


namespace cppdevtk {
namespace base {
namespace concurrent {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note This is more an adapter so it can be used in some existing projects
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
/// \attention
/// - returned future cannot be paused/resumed/queried for progress
/// - if Qt < 5.6.2 then may be affected by Qt bug #54831
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
	
	void ReportStarted();
	void ReportCanceled(const TaskCanceledException& exc);
	void ReportException(const QtException& exc);
	void ReportUnhandledException();
	void ReportFinished();
	
	
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
protected:
	void ReportResult(const TResult& result);
	
#	if (QT_VERSION < QT_VERSION_CHECK(5, 9, 0))
	QtPrivate::ResultStore<TResult>& ResultStore();
#	endif
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
	ReportStarted();
	
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
#if (CPPDEVTK_EXECUTOR_TEST)
CPPDEVTK_NO_INLINE
#else
inline
#endif
void StartAndRunCancelableTaskBase<TResult>::ReportStarted() {
#	if (CPPDEVTK_EXECUTOR_TEST)
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Started));
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Running));
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Canceled));
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Finished));
	CPPDEVTK_ASSERT(!this->exceptionStore().hasException());
#	endif
	
	this->reportStarted();
}

template <typename TResult>
#if (CPPDEVTK_EXECUTOR_TEST)
CPPDEVTK_NO_INLINE
#else
inline
#endif
void StartAndRunCancelableTaskBase<TResult>::ReportCanceled(const TaskCanceledException& exc) {
	QMutexLocker locker(this->mutex());
	
#	if (CPPDEVTK_EXECUTOR_TEST)
	CPPDEVTK_ASSERT(this->queryState(QFutureInterfaceBase::Canceled));
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Finished));
	CPPDEVTK_ASSERT(!this->exceptionStore().hasException());
#	endif
	
	this->exceptionStore().setException(exc);
}

template <typename TResult>
#if (CPPDEVTK_EXECUTOR_TEST)
CPPDEVTK_NO_INLINE
#else
inline
#endif
void StartAndRunCancelableTaskBase<TResult>::ReportException(const QtException& exc) {
	QMutexLocker locker(this->mutex());
	
#	if (CPPDEVTK_EXECUTOR_TEST)
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Finished));
	CPPDEVTK_ASSERT(!this->exceptionStore().hasException());
#	endif
	
	this->exceptionStore().setException(exc);
	
	if (!this->queryState(QFutureInterfaceBase::Canceled)) {
		// not atomic but there is nothing we can do (QFutureInterfaceBase::d is private)...
		locker.unlock();
		this->reportException(exc);
	}
}

template <typename TResult>
inline void StartAndRunCancelableTaskBase<TResult>::ReportUnhandledException() {
	ReportException(QtUnhandledException());	// QtUnhandledException ctor can not throw
}

template <typename TResult>
#if (CPPDEVTK_EXECUTOR_TEST)
CPPDEVTK_NO_INLINE
#else
inline
#endif
void StartAndRunCancelableTaskBase<TResult>::ReportFinished() {
#	if (CPPDEVTK_EXECUTOR_TEST)
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Finished));
#	endif
	
	this->reportFinished();
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
		this->ReportCanceled(CPPDEVTK_TASK_CANCELED_EXCEPTION_W_WA("task was canceled when started running"));
	}
	else {
		try {
			TResult result;
			
			try {
				result = this->pCancelableTask_->Run(::std::auto_ptr<FutureInterfaceCancelable>(new FutureInterfaceCancelable(*this)));
			}
			catch (const TaskCanceledException& exc) {
				//CPPDEVTK_LOG_INFO("TaskCanceledException: " << Exception::GetDetailedInfo(exc));
				this->ReportCanceled(exc);
				this->ReportFinished();
				return;
			}
			catch (const QtException& exc) {
				//CPPDEVTK_LOG_ERROR("task failed; QtException: " << Exception::GetDetailedInfo(exc));
				this->ReportException(exc);
				this->ReportFinished();
				return;
			}
			catch (const ::std::exception& exc) {
				CPPDEVTK_LOG_ERROR("task failed; unhandled ::std::exception: " << Exception::GetDetailedInfo(exc));
				this->ReportUnhandledException();
				this->ReportFinished();
				return;
			}
			catch (...) {
				CPPDEVTK_LOG_ERROR("task failed; unhandled unknown exception");
				this->ReportUnhandledException();
				this->ReportFinished();
				return;
			}
			
			this->ReportResult(result);
		}
		catch (const QtException& exc) {
			//CPPDEVTK_LOG_ERROR("task failed; QtException: " << Exception::GetDetailedInfo(exc));
			this->ReportException(exc);
			this->ReportFinished();
			return;
		}
		catch (const ::std::exception& exc) {
			CPPDEVTK_LOG_ERROR("task failed; unhandled ::std::exception: " << Exception::GetDetailedInfo(exc));
			this->ReportUnhandledException();
			this->ReportFinished();
			return;
		}
		catch (...) {
			CPPDEVTK_LOG_ERROR("task failed; unhandled unknown exception");
			this->ReportUnhandledException();
			this->ReportFinished();
			return;
		}
	}
	
	this->ReportFinished();
}

template <typename TResult>
#if (CPPDEVTK_EXECUTOR_TEST)
CPPDEVTK_NO_INLINE
#else
inline
#endif
void StartAndRunCancelableTask<TResult>::ReportResult(const TResult& result) {
	QMutexLocker locker(this->mutex());
	
#	if (CPPDEVTK_EXECUTOR_TEST)
	CPPDEVTK_ASSERT(!this->queryState(QFutureInterfaceBase::Finished));
	CPPDEVTK_ASSERT(!this->exceptionStore().hasException());
#	endif
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
	QtPrivate::ResultStoreBase& store = this->resultStoreBase();
#	else
	QtPrivate::ResultStore<TResult>& store = this->ResultStore();
#	endif
	
	if (store.filterMode()) {
		const int resultCountBefore = store.count();
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
		store.addResult<TResult>(0, &result);
#		else
		store.addResult(0, &result);
#		endif
		// Fix for Qt bug #6799
		//this->reportResultsReady(resultCountBefore, resultCountBefore + store.count());
		this->reportResultsReady(resultCountBefore, (resultCountBefore + 1));
	}
	else {
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
		const int insertIndex = store.addResult<TResult>(0, &result);
#		else
		const int insertIndex = store.addResult(0, &result);
#		endif
		this->reportResultsReady(insertIndex, (insertIndex + 1));
	}
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 9, 0))

template <typename TResult>
inline QtPrivate::ResultStore<TResult>& StartAndRunCancelableTask<TResult>::ResultStore() {
	return static_cast<QtPrivate::ResultStore<TResult>&>(this->resultStoreBase());
}

#endif

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
