/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_EXCEPTION_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/exception.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_EXCEPTION_PROPAGATION_HPP_INCLUDED_
#define CPPDEVTK_BASE_EXCEPTION_PROPAGATION_HPP_INCLUDED_


#include "config.hpp"
#include "unused.hpp"

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QException>
#include <QtCore/QUnhandledException>
#else
#include <QtCore/QtCore>
#endif

#if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

#include <stdexcept>
#include <typeinfo>
#include <ios>
#include <new>
#include CPPDEVTK_TR1_HEADER(memory)
#include CPPDEVTK_TR1_HEADER(functional)
#if (CPPDEVTK_HAVE_CPP11_FUTURE)
#include <future>
#endif
#if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
#include <system_error>
#endif

#include <cstddef>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11

#endif	// (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

#include <exception>


#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
typedef QException QtException;
typedef QUnhandledException QtUnhandledException;
#else
typedef QtConcurrent::Exception QtException;
typedef QtConcurrent::UnhandledException QtUnhandledException;
#endif


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup exception_propagation Exception Propagation
/// \ingroup exceptions
/// \sa
/// - C++ 11, 18.8.5 Exception propagation
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2179.html">Language Support for Transporting Exceptions between Threads</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2107.html">Exception Propagation across Threads</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2061.html">Library Exception Propagation Support</a>
/// - <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2229.html">Cloning and Throwing Dynamically Typed Exceptions (Rev 1)</a>
/// @{


#if (CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

typedef ::std::exception_ptr ExceptionPtr;

#else

namespace detail {


typedef QtException PolymorphicExceptionBase;


template <class TStdException>
class PolymorphicException: public PolymorphicExceptionBase {
public:
	typedef CPPDEVTK_TR1_NS::shared_ptr<const TStdException> PtrType;
	
	
	explicit PolymorphicException(const TStdException& stdException);
	
	virtual PolymorphicExceptionBase* clone() const;
	virtual void raise() const;
private:
	PtrType ptr_;
};


typedef PolymorphicException< ::std::invalid_argument> PolymorphicStdInvalidArgument;
typedef PolymorphicException< ::std::domain_error> PolymorphicStdDomainError;
typedef PolymorphicException< ::std::length_error> PolymorphicStdLengthError;
typedef PolymorphicException< ::std::out_of_range> PolymorphicStdOutOfRange;
#if (CPPDEVTK_HAVE_CPP11_FUTURE)
typedef PolymorphicException< ::std::future_error> PolymorphicStdFutureError;
#endif
typedef PolymorphicException< ::std::logic_error> PolymorphicStdLogicError;

typedef PolymorphicException< ::std::overflow_error> PolymorphicStdOverflowError;
typedef PolymorphicException< ::std::underflow_error> PolymorphicStdUnderflowError;
typedef PolymorphicException< ::std::range_error> PolymorphicStdRangeError;
typedef PolymorphicException< ::std::ios_base::failure> PolymorphicStdIosFailure;
#if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
typedef PolymorphicException< ::std::system_error> PolymorphicStdSystemError;
#endif
typedef PolymorphicException< ::std::runtime_error> PolymorphicStdRuntimeError;

typedef PolymorphicException< ::std::bad_typeid> PolymorphicStdBadTypeid;
typedef PolymorphicException< ::std::bad_cast> PolymorphicStdBadCast;
typedef PolymorphicException< CPPDEVTK_TR1_NS::bad_weak_ptr> PolymorphicStdBadWeakPtr;
typedef PolymorphicException< CPPDEVTK_TR1_NS::bad_function_call> PolymorphicStdBadFunctionCall;
typedef PolymorphicException< ::std::bad_exception> PolymorphicStdBadException;
typedef PolymorphicException< ::std::bad_alloc> PolymorphicStdBadAlloc;

typedef PolymorphicException< ::std::exception> PolymorphicStdException;


}	// namespace detail


class ExceptionPtr;
namespace detail {
	ExceptionPtr ThrowableCurrentException();
}


class CPPDEVTK_BASE_API ExceptionPtr {
private:
	friend CPPDEVTK_BASE_API ExceptionPtr CurrentException(bool) CPPDEVTK_NOEXCEPT;
	friend ExceptionPtr detail::ThrowableCurrentException();
	friend CPPDEVTK_BASE_API void RethrowException(const ExceptionPtr& exceptionPtr);
	
	typedef CPPDEVTK_TR1_NS::shared_ptr<const detail::PolymorphicExceptionBase> ImplPtrType;
public:
	typedef void (*UnspecifiedBoolType)();
	
	
	ExceptionPtr() CPPDEVTK_NOEXCEPT;
	~ExceptionPtr() CPPDEVTK_NOEXCEPT;
	ExceptionPtr(const ExceptionPtr& other) CPPDEVTK_NOEXCEPT;
	
	ExceptionPtr& operator=(const ExceptionPtr& other) CPPDEVTK_NOEXCEPT;
	
	bool operator==(const ExceptionPtr& other) const CPPDEVTK_NOEXCEPT;
	bool operator!=(const ExceptionPtr& other) const CPPDEVTK_NOEXCEPT;
	
	operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT;
	bool operator!() const CPPDEVTK_NOEXCEPT;
	
	void Swap(ExceptionPtr& other) CPPDEVTK_NOEXCEPT;
private:
	explicit ExceptionPtr(ImplPtrType pImpl) CPPDEVTK_NOEXCEPT;
	
	static void UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT;
	
	
	ImplPtrType pImpl_;
};


CPPDEVTK_BASE_API void swap(ExceptionPtr& x, ExceptionPtr& y) CPPDEVTK_NOEXCEPT;

#endif	// (CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

/// \param terminateIfFail If fails then if it is \c true then calls \c ::std::terminate() else returns null exception ptr.
/// \attention If \c ::std::make_exception_ptr() is not available then:
/// - Qt based (including CppDevTk) exceptions work as expected.
/// - only exceptions that inherit from \c ::std::exception are supported; other exceptions will be reported as unknown exception.
/// - user defined exceptions, other than Qt, that inherit from std exceptions will be reported as their std exception base
/// (ex: if FooError inherits from \c ::std::runtime_error then \c runtime_error will be reported, not FooError, so slicing...).
/// - it may fail.
template <typename TException>
ExceptionPtr MakeExceptionPtr(TException exc, bool terminateIfFail = true) CPPDEVTK_NOEXCEPT;

/// \param terminateIfFail If fails then if it is \c true then calls \c ::std::terminate() else returns null exception ptr.
/// \attention If \c ::std::current_exception() is not available then:
/// - Qt based (including CppDevTk) exceptions work as expected.
/// - only exceptions that inherit from \c ::std::exception are supported; other exceptions will be reported as unknown exception.
/// - user defined exceptions, other than Qt, that inherit from std exceptions will be reported as their std exception base
/// (ex: if FooError inherits from \c ::std::runtime_error then \c runtime_error will be reported, not FooError, so slicing...).
/// - it will work only if there is a handled exception (called in a catch clause).
/// - it may fail.
CPPDEVTK_BASE_API ExceptionPtr CurrentException(bool terminateIfFail = true) CPPDEVTK_NOEXCEPT;

/// \pre \a exceptionPtr is not null.
CPPDEVTK_BASE_API void RethrowException(const ExceptionPtr& exceptionPtr);

/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

#if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

namespace detail {


template <class TStdException>
inline PolymorphicException<TStdException>::PolymorphicException(const TStdException& stdException):
		ptr_(new TStdException(stdException)) {}

template <class TStdException>
inline PolymorphicExceptionBase* PolymorphicException<TStdException>::clone() const {
	return new PolymorphicException(*this);
}

template <class TStdException>
inline void PolymorphicException<TStdException>::raise() const {
	throw *ptr_;
}


}	// namespace detail


inline ExceptionPtr::ExceptionPtr() CPPDEVTK_NOEXCEPT: pImpl_() {}

inline ExceptionPtr::~ExceptionPtr() CPPDEVTK_NOEXCEPT {}

inline ExceptionPtr::ExceptionPtr(const ExceptionPtr& other) CPPDEVTK_NOEXCEPT: pImpl_(other.pImpl_) {}

inline ExceptionPtr& ExceptionPtr::operator=(const ExceptionPtr& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		pImpl_ = other.pImpl_;
	}
	return *this;
}

inline bool ExceptionPtr::operator==(const ExceptionPtr& other) const CPPDEVTK_NOEXCEPT {
	return pImpl_ == other.pImpl_;
}

inline bool ExceptionPtr::operator!=(const ExceptionPtr& other) const CPPDEVTK_NOEXCEPT {
	return pImpl_ != other.pImpl_;
}

inline ExceptionPtr::operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT {
	return pImpl_ ? &UnspecifiedBoolTrue : NULL;
}

inline bool ExceptionPtr::operator!() const CPPDEVTK_NOEXCEPT {
	return !pImpl_;
}

inline void ExceptionPtr::Swap(ExceptionPtr& other) CPPDEVTK_NOEXCEPT {
	pImpl_.swap(other.pImpl_);
}

inline ExceptionPtr::ExceptionPtr(ImplPtrType pImpl) CPPDEVTK_NOEXCEPT: pImpl_(pImpl) {}

inline void ExceptionPtr::UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT {}


inline CPPDEVTK_BASE_API void swap(ExceptionPtr& x, ExceptionPtr& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}

#endif	// (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

template <typename TException>
inline ExceptionPtr MakeExceptionPtr(TException exc, bool terminateIfFail) CPPDEVTK_NOEXCEPT {
#if (CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)
	SuppressUnusedWarning(terminateIfFail);
#if (!CPPDEVTK_COMPILER_GCC)
	return ::std::make_exception_ptr(exc);
#elif (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 6, 0))
	return ::std::make_exception_ptr(exc);
#else
	return ::std::copy_exception(exc);
#endif
#else
	try {
		throw exc;
	}
	catch (...) {
		return CurrentException(terminateIfFail);
	}
#endif
}

#if (CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)

inline CPPDEVTK_BASE_API ExceptionPtr CurrentException(bool terminateIfFail) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(terminateIfFail);
	return ::std::current_exception();
}

inline CPPDEVTK_BASE_API void RethrowException(const ExceptionPtr& exceptionPtr) {
	::std::rethrow_exception(exceptionPtr);
}

#endif	// (CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_EXCEPTION_PROPAGATION_HPP_INCLUDED_
