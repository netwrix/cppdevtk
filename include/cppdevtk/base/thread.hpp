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


#ifndef CPPDEVTK_BASE_THREAD_HPP_INCLUDED_
#define CPPDEVTK_BASE_THREAD_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif

#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <windows.h>
#undef Yield
#endif

#include <ctime>


// iOS < 9.0 and official GCC on Mac OS X do not have __thread so Thread class not available on iOS < 9.0
// and Mac OS X < 10.7 (>= 10.7 use clang, OK); maybe implement pthread based workaround in the future...
#if (CPPDEVTK_HAVE_THREAD_STORAGE)


#include "thread_data_types.hpp"
#include "non_copyable.hpp"
#include "stringizable.hpp"
#include "exception.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"
#include "static_assert.hpp"

#include <QtCore/QTextStream>

#include <cstddef>
#include <ostream>
#include CPPDEVTK_TR1_HEADER(memory)


// Please see CPPDEVTK_CHECK_INTERRUPT_REL_TIME performance impact table in cppdevtk/config/platform.hpp
CPPDEVTK_STATIC_ASSERT((1 <= CPPDEVTK_CHECK_INTERRUPT_REL_TIME) && (CPPDEVTK_CHECK_INTERRUPT_REL_TIME <= 999));
#if (CPPDEVTK_PLATFORM_UNIX)
#	if (CPPDEVTK_PLATFORM_LINUX)
#		if (CPPDEVTK_PLATFORM_ANDROID)
			CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_CHECK_INTERRUPT_REL_TIME >= 25), "(severe) performance issue");
#		else
			CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_CHECK_INTERRUPT_REL_TIME >= 25), "(severe) performance issue");
#		endif
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#		if (CPPDEVTK_PLATFORM_IOS)
			CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_CHECK_INTERRUPT_REL_TIME >= 50), "(severe) performance issue");
#		else
			CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_CHECK_INTERRUPT_REL_TIME >= 50), "(severe) performance issue");
#		endif
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
	CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_CHECK_INTERRUPT_REL_TIME >= 100), "(severe) performance issue");
#else
#	error "Unsupported platform!!!"
#endif


#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


namespace cppdevtk {
namespace base {


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


class ConditionVariable;


namespace detail {


class ThreadData;


}	// namespace detail


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Thread class that supports features not present in \c ::std::thread (attributes, start, interruption, propagates exception etc).
/// \attention Despite its name this class is not thread-safe (only reentrant).
/// \note Although there is no parent - child relationship between threads, for convenience we'll name the thread where
/// \c Thread object lives parent thread and the thread it creates child thread.
/// \remarks
/// - TLS is not supported (maybe in the future).
/// - Termination is not supported (dangerous, discouraged etc)
/// - Priorities are not supported (Linux non-real-time scheduling policies do not have priority, nice() require CAP_SYS_NICE
/// (POSIX nice is per-process, Linux is per-thread but maybe this will be considered bug), RT policies require root).
/// \sa
/// - C++11, 30.3.1 Class thread
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1883.pdf">Preliminary Threading Library Proposal for TR2</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2184.html">Thread Launching for C++0X</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2497.html">Multi-threading Library for Standard C++ (Revision 1)</a>
/// - <a http://zthread.sourceforge.net/html/classZThread_1_1Thread.html">ZThread Thread</a>
/// - <a href="http://www.boost.org/doc/libs/1_65_1/doc/html/thread/thread_management.html#thread.thread_management.thread">Boost thread</a>
class CPPDEVTK_BASE_API Thread: private NonCopyable {
public:
	/// \sa 30.3.1.1 Class thread::id
	class Id;	// forward declaration to avoid compile errors due to friend declarations
	
	
	class CPPDEVTK_BASE_API Attributes {
	public:
#		if (CPPDEVTK_PLATFORM_UNIX)
		typedef pthread_attr_t NativeHandleType;
#		endif
		
		
		Attributes();
		~Attributes() CPPDEVTK_NOEXCEPT;
		
		void SetDetached(bool value);	///< \arg value If \c true then detached, joinable otherwise; default if not set is joinable.
		bool GetDetached() const;
		
		void SetStackSize(::std::size_t value);	///< \arg value stack size in bytes.
		::std::size_t GetStackSize() const;
		
#		if (CPPDEVTK_PLATFORM_UNIX)
		NativeHandleType* GetNativeHandlePtr();
		const NativeHandleType* GetNativeHandlePtr() const;
#		endif
	private:
		bool detached_;
#		if (CPPDEVTK_PLATFORM_UNIX)
		pthread_attr_t nativeAttributes_;
#		elif (CPPDEVTK_PLATFORM_WINDOWS)
		::std::size_t stackSize_;
#		endif
	};
	
	
	typedef detail::ThreadNativeHandleType NativeHandleType;
	typedef detail::ThreadMainFunctionType MainFunctionType;
	
	
	/// \name Constructor
	/// \note
	/// - Constructed \c Thread object does not represent a thread of execution.
	/// - It may be tempting to add autoStart but this may lead to the problem that a virtual function, \c Main()
	/// may be called in child thread before the derived object is fully constructed in parent thread.
	///@{
	
	/// Inheritance based constructor; main function is \c Main() that needs to be overriden in derived class.
	explicit Thread(const Attributes& attributes = Attributes());
	
	/// Delegation based constructor.
	/// \pre \a mainFunction must have a target.
	explicit Thread(const MainFunctionType& mainFunction, const Attributes& attributes = Attributes());
	
	///@}
	
	/// \attention
	/// - If \c CPPDEVTK_ENABLE_THREAD_INTERRUPTION and \c IsJoinable() calls \c RequestInterruption() then \c Join().
	/// If fails calls \c ::std::terminate()
	/// - If \c !CPPDEVTK_ENABLE_THREAD_INTERRUPTION and \c IsJoinable() calls \c ::std::terminate()
	/// - Any exception thrown by thread main function and propagated by \c Join() is swallowed.
	/// \note C++11 std, 30.3.1.3 thread destructor: if \c joinable(), calls \c ::std::terminate()
	/// \sa
	/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2802.html">A plea to reconsider detach-on-destruction for thread objects</a>
	/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3636.pdf">~thread Should Join</a>
	/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0206r0.html">Discussion about std::thread and RAII</a>
	/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0660r0.pdf">A Cooperatively Interruptible Joining Thread</a>
	virtual ~Thread() CPPDEVTK_NOEXCEPT;
	
	/// \returns A default constructed \c Id object if this does not represent a thread, otherwise the \c Id of the child thread.
	Id GetId() const CPPDEVTK_NOEXCEPT;
	
	NativeHandleType GetNativeHandle();
	
	/// Create the thread and start executing it. If successful this \c Thread object does represent a thread of execution.
	/// \remark Has no effect if already started.
	void Start();
	void operator()();	///< Same as \c Start()
	
	
	/// \name Join/detach
	///@{
	
	bool IsJoinable() const CPPDEVTK_NOEXCEPT;	///< \returns GetId() != Id()
	
	/// Blocks until the child thread has completed. Exceptions are propagated from child thread to parent thread
	/// (\c ::std::thread calls \c ::std::terminate() in this case).
	/// \pre IsJoinable()
	/// \post !IsJoinable()
	/// \attention Interruption point (if parent thread is interrupted child thread is not joined).
	/// \note If an exception is thrown use \c IsJoinable() to distinguish if it is from parent thread (call failed) or
	/// propagated from child thread.
	void Join(MainFunctionType::result_type& retCode);
	
	/// \attention calls \c ::std::terminate() if fails to propagate child thread exception, if any, in \a exceptionPtr
	/// \remark If child thread exits with exception then \a exceptionPtr is set, otherwise \a retCode is set.
	bool TryJoin(ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode);
	
	/// \arg relTime Relative timeout, in milliseconds. If it is <= 0 calls \c TryJoin()
	/// \attention
	/// - calls \c ::std::terminate() if fails to propagate child thread exception, if any, in \a exceptionPtr
	/// - Interruption point (if parent thread is interrupted child thread is not joined).
	/// \remark If child thread exits with exception then \a exceptionPtr is set, otherwise \a retCode is set.
	bool TryJoinFor(int relTime, ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode);
	
	/// \arg absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC. If it is not in the future calls \c TryJoin()
	/// \attention
	/// - calls \c ::std::terminate() if fails to propagate child thread exception, if any, in \a exceptionPtr
	/// - Interruption point (if parent thread is interrupted child thread is not joined).
	/// \remark If child thread exits with exception then \a exceptionPtr is set, otherwise \a retCode is set.
	bool TryJoinUntil(::std::time_t absTime, ExceptionPtr& exceptionPtr, MainFunctionType::result_type& retCode);
	
	/// The child thread continues execution without the parent thread blocking.
	/// \attention After completion if child thread function throws an exception other than \c ThreadInterruptedException
	/// calls \c ::std::terminate()
	/// \pre IsJoinable()
	/// \post !IsJoinable()
	void Detach();
	
	///@}
	
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	/// \name Interruption
	/// \sa
	/// - <a http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2455.pdf">Thread Cancellation</a>
	/// - <a https://groups.google.com/forum/#!topic/comp.std.c++/Sv5Ss57ByW8/discussion">Why was cooperative thread cancellation removed from C++0x</a>
	/// - <a https://www.justsoftwaresolutions.co.uk/threading/thread-interruption-in-boost-thread-library.html">Thread Interruption in the Boost Thread Library</a>
	///@{
	
	/// \note Interruption points:
	/// - ConditionVariable: Wait(), WaitFor(), WaitUntil()
	/// - ConditionVariableAny: Wait(), WaitFor(), WaitUntil()
	/// - Semaphore: Wait(), WaitFor(), WaitUntil()
	/// - Thread: Join(), TryJoinFor(), TryJoinUntil()
	/// - this_thread: InterruptionPoint(), SleepFor(), SleepUntil()
	/// \sa
	/// - <a href="http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_09">POSIX Cancellation Points</a>
	/// - <a href="http://www.boost.org/doc/libs/1_65_1/doc/html/thread/thread_management.html#thread.thread_management.tutorial.interruption.predefined_interruption_points">Boost Predefined Interruption Points</a>
	/// \pre IsJoinable()
	void RequestInterruption();
	
	bool IsInterruptionRequested() const;	///< \pre IsJoinable()
	
	///@}
	
#	endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	void Swap(Thread& other) CPPDEVTK_NOEXCEPT;
	
	
	/// \returns The number of hardware thread contexts; 0 if fails.
	static unsigned int GetHardwareConcurrency() CPPDEVTK_NOEXCEPT;
protected:
	virtual int Main();	///< If inheritance based thread is used, override in derived class.
private:
	typedef detail::ThreadData Data;
	typedef CPPDEVTK_TR1_NS::shared_ptr<Data> DataPtr;
	
	
	void Create();
	
	
	static ExceptionPtr GetTryJoinExceptionPtr(DataPtr pData);
	
#	if (CPPDEVTK_PLATFORM_UNIX)
	static void* Run(void* pVoidData);
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	static unsigned __stdcall Run(void* pVoidData);
#	endif
	
	
	const Attributes kAttributes_;
	DataPtr pData_;	// null if joined/detached
};


CPPDEVTK_BASE_API void swap(Thread& x, Thread& y) CPPDEVTK_NOEXCEPT;

template <typename TChar>
::std::basic_ostream<TChar>& operator<<(::std::basic_ostream<TChar>& os, Thread::Id threadId);

CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, Thread::Id threadId);

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


namespace this_thread {


#if (CPPDEVTK_HAVE_THREAD_STORAGE)

/// \returns An object of type \c Thread::Id that uniquely identifies the current thread of execution.
/// \remark Threads not created by \c Thread (adopted threads) are supported.
CPPDEVTK_BASE_API Thread::Id GetId() CPPDEVTK_NOEXCEPT;

// TODO
//template<typename Func>
//void AtThreadExit(Func func);

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)

/// \remark Threads not created by \c Thread (adopted threads) are supported.
CPPDEVTK_BASE_API void Yield() CPPDEVTK_NOEXCEPT;


/// \name Sleep
/// \remark Threads not created by \c Thread (adopted threads) are supported.
///@{

/// Blocks the calling thread for the relative timeout specified by \a relTime.
/// \arg relTime In milliseconds.
/// \pre \a relTime >= 0
CPPDEVTK_BASE_API void SleepFor(int relTime);

/// Blocks the calling thread for the absolute timeout specified by \a absTime.
/// \arg absTime The number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
/// \pre \a absTime is not in the past.
CPPDEVTK_BASE_API void SleepUntil(::std::time_t absTime);

///@}


#if (CPPDEVTK_HAVE_THREAD_STORAGE)
#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

/// \name Interruption
/// \attention Threads not created by \c Thread (adopted threads) are not supported (maybe in the future).
/// \remark Because TLS and adopted threads are not supported it works only in \c Thread main function.
///@{

CPPDEVTK_BASE_API bool IsInterruptionEnabled();
CPPDEVTK_BASE_API bool IsInterruptionRequested();

/// If \c IsInterruptionEnabled() and \c IsInterruptionRequested() then clears interruption request and throws \c ThreadInterruptedException
CPPDEVTK_BASE_API void InterruptionPoint();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RAII for disabling interruption.
/// \note Because interruption is restored in destructor it can not be interruption point.
class CPPDEVTK_BASE_API DisableInterruptionGuard: private NonCopyable {
	friend class RestoreInterruptionGuard;
public:
	DisableInterruptionGuard();
	~DisableInterruptionGuard();
private:
	const bool kInterruptionWasEnabled_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RAII for restoring interruption.
/// \note For similarity with \c DisableInterruptionGuard constructor is not interruption point.
class CPPDEVTK_BASE_API RestoreInterruptionGuard: private NonCopyable {
public:
	RestoreInterruptionGuard(const DisableInterruptionGuard& disableInterruptionGuard);
	~RestoreInterruptionGuard();
};

///@}


#endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


}	// namespace this_thread


#if (CPPDEVTK_HAVE_THREAD_STORAGE)


// full declaration here to avoid compile errors due to friend declarations
class CPPDEVTK_BASE_API Thread::Id: public QStringizable {
	friend class Thread;
	
	template <typename TChar>
	friend ::std::basic_ostream<TChar>& operator<<(::std::basic_ostream<TChar>& os, Id id);
	
	friend CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, Id threadId);
	
	friend CPPDEVTK_BASE_API Id this_thread::GetId() CPPDEVTK_NOEXCEPT;
public:
	Id() CPPDEVTK_NOEXCEPT;	///< Constructed object does not represent a thread of execution.
	
	virtual QString ToString() const;
	
	bool operator==(const Id& other) const CPPDEVTK_NOEXCEPT;
	bool operator!=(const Id& other) const CPPDEVTK_NOEXCEPT;
	bool operator<(const Id& other) const CPPDEVTK_NOEXCEPT;
	bool operator<=(const Id& other) const CPPDEVTK_NOEXCEPT;
	bool operator>(const Id& other) const CPPDEVTK_NOEXCEPT;
	bool operator>=(const Id& other) const CPPDEVTK_NOEXCEPT;
private:
	typedef detail::ThreadNativeIdType NativeIdType;
	
	
	Id(NativeIdType nativeId) CPPDEVTK_NOEXCEPT;	///< \remark not explicit (conversion ctor)
	
	
	NativeIdType nativeId_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

inline void Thread::operator()() {
	Start();
}

inline bool Thread::IsJoinable() const CPPDEVTK_NOEXCEPT {
	return GetId() != Id();
}

inline int Thread::Main() {
	return EXIT_SUCCESS;
}


inline CPPDEVTK_BASE_API void swap(Thread& x, Thread& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}

template <typename TChar>
/* inline */ ::std::basic_ostream<TChar>& operator<<(::std::basic_ostream<TChar>& os, Thread::Id threadId) {
	return os << threadId.nativeId_;
}


namespace this_thread {


}	// namespace this_thread


inline bool Thread::Id::operator!=(const Id& other) const CPPDEVTK_NOEXCEPT {
	return !(*this == other);
}

inline bool Thread::Id::operator<=(const Id& other) const CPPDEVTK_NOEXCEPT {
	return !(other < *this);
}

inline bool Thread::Id::operator>(const Id& other) const CPPDEVTK_NOEXCEPT {
	return other < *this;
}

inline bool Thread::Id::operator>=(const Id& other) const CPPDEVTK_NOEXCEPT {
	return !(*this < other);
}

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_HAVE_THREAD_STORAGE)

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


#include "thread_exception.hpp"


#endif	// CPPDEVTK_BASE_THREAD_HPP_INCLUDED_
