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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_THREAD_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/thread.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_THREAD_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_THREAD_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "system_exception.hpp"
#include "thread.hpp"
#include "unused.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ThreadException

#define CPPDEVTK_THREAD_EXCEPTION_W_EC(errorCode)	\
	::cppdevtk::base::ThreadException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))

#define CPPDEVTK_THREAD_EXCEPTION_W_EC_WA(errorCode, whatArg)	\
	::cppdevtk::base::ThreadException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg))


#define CPPDEVTK_MAKE_THREAD_EXCEPTION_W_EC(excName, errorCode)	\
	::cppdevtk::base::ThreadException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))

#define CPPDEVTK_MAKE_THREAD_EXCEPTION_W_EC_WA(excName, errorCode, whatArg)	\
	::cppdevtk::base::ThreadException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for thread exceptions.
/// \remark Not in C++ 11 std.
class CPPDEVTK_BASE_API ThreadException: public SystemException {
public:
#	if (CPPDEVTK_HAVE_THREAD_STORAGE)
	typedef Thread::Id ThreadId;
#	endif
	
	
	ThreadException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode);
	ThreadException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const QString& whatArg);
	
	virtual ~ThreadException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<ThreadException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual ThreadException* clone() const;
#	endif
	
	void Swap(ThreadException& other) CPPDEVTK_NOEXCEPT;
	
#	if (CPPDEVTK_HAVE_THREAD_STORAGE)
	ThreadId GetThreadId() const;
#	endif
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual ThreadException* DoClone() const;
#	endif
	
	virtual QString DoOwnWhat() const;
	
	void SwapOwnData(ThreadException& other) CPPDEVTK_NOEXCEPT;
private:
#	if (CPPDEVTK_HAVE_THREAD_STORAGE)
	ThreadId threadId_;
#	endif
};


CPPDEVTK_BASE_API void swap(ThreadException& x, ThreadException& y) CPPDEVTK_NOEXCEPT;




#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ThreadInterruptedException

#define CPPDEVTK_THREAD_INTERRUPTED_EXCEPTION()	\
	::cppdevtk::base::ThreadInterruptedException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_THREAD_INTERRUPTED_EXCEPTION_WA(whatArg)	\
	::cppdevtk::base::ThreadInterruptedException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


#define CPPDEVTK_MAKE_THREAD_INTERRUPTED_EXCEPTION(excName)	\
	::cppdevtk::base::ThreadInterruptedException excName(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_THREAD_INTERRUPTED_EXCEPTION_WA(excName, whatArg)	\
	::cppdevtk::base::ThreadInterruptedException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ 11 std.
class CPPDEVTK_BASE_API ThreadInterruptedException: public ThreadException {
public:
	explicit ThreadInterruptedException(const SourceCodeInfo& throwPoint);
	ThreadInterruptedException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~ThreadInterruptedException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<ThreadInterruptedException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual ThreadInterruptedException* clone() const;
#	endif
	
	void Swap(ThreadInterruptedException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual ThreadInterruptedException* DoClone() const;
#	endif
	
	void SwapOwnData(ThreadInterruptedException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(ThreadInterruptedException& x, ThreadInterruptedException& y) CPPDEVTK_NOEXCEPT;

#endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline ThreadException::ThreadException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode): Exception(throwPoint),
		RuntimeException(throwPoint, ""), SystemException(throwPoint, errorCode)
#		if (CPPDEVTK_HAVE_THREAD_STORAGE)
		, threadId_(this_thread::GetId())
#		endif
		{}

inline ThreadException::ThreadException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg), SystemException(throwPoint, errorCode, whatArg)
#		if (CPPDEVTK_HAVE_THREAD_STORAGE)
		, threadId_(this_thread::GetId())
#		endif
		{}

inline ThreadException::~ThreadException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<ThreadException> ThreadException::Clone() const {
	return ::std::auto_ptr<ThreadException>(dynamic_cast<ThreadException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* ThreadException::clone() const {
#else
inline ThreadException* ThreadException::clone() const {
#endif
	return Clone().release();
}

inline void ThreadException::Swap(ThreadException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		SystemException::Swap(other);
		SwapOwnData(other);
	}
}

#if (CPPDEVTK_HAVE_THREAD_STORAGE)

inline ThreadException::ThreadId ThreadException::GetThreadId() const {
	return threadId_;
}

#endif

inline void ThreadException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* ThreadException::DoClone() const {
#else
inline ThreadException* ThreadException::DoClone() const {
#endif
	return new ThreadException(*this);
}

inline void ThreadException::SwapOwnData(ThreadException& other) CPPDEVTK_NOEXCEPT {
#	if (CPPDEVTK_HAVE_THREAD_STORAGE)
	using ::std::swap;
	
	swap(threadId_, other.threadId_);
#	else
	SuppressUnusedWarning(other);
#	endif
}


inline CPPDEVTK_BASE_API void swap(ThreadException& x, ThreadException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

inline ThreadInterruptedException::ThreadInterruptedException(const SourceCodeInfo& throwPoint): Exception(throwPoint),
		RuntimeException(throwPoint, ""), //SystemException(throwPoint, MakeErrorCode(::cppdevtk::base::errc::interrupted)),
		ThreadException(throwPoint, MakeErrorCode(::cppdevtk::base::errc::interrupted)) {}

inline ThreadInterruptedException::ThreadInterruptedException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg),
		//SystemException(throwPoint, MakeErrorCode(::cppdevtk::base::errc::interrupted), whatArg),
		ThreadException(throwPoint, MakeErrorCode(::cppdevtk::base::errc::interrupted), whatArg) {}

inline ThreadInterruptedException::~ThreadInterruptedException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<ThreadInterruptedException> ThreadInterruptedException::Clone() const {
	return ::std::auto_ptr<ThreadInterruptedException>(dynamic_cast<ThreadInterruptedException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* ThreadInterruptedException::clone() const {
#else
inline ThreadInterruptedException* ThreadInterruptedException::clone() const {
#endif
	return Clone().release();
}

inline void ThreadInterruptedException::Swap(ThreadInterruptedException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		ThreadException::Swap(other);
		SwapOwnData(other);
	}
}

inline void ThreadInterruptedException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* ThreadInterruptedException::DoClone() const {
#else
inline ThreadInterruptedException* ThreadInterruptedException::DoClone() const {
#endif
	return new ThreadInterruptedException(*this);
}

inline void ThreadInterruptedException::SwapOwnData(ThreadInterruptedException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(ThreadInterruptedException& x, ThreadInterruptedException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}

#endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_THREAD_EXCEPTION_HPP_INCLUDED_
