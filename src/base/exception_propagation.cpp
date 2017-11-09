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


#include <cppdevtk/base/exception_propagation.hpp>
#include <cppdevtk/base/unknown_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>


namespace cppdevtk {
namespace base {


#if (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)


namespace detail {


ExceptionPtr ThrowableCurrentException() {
	ExceptionPtr exceptionPtr;
	CPPDEVTK_ASSERT(!exceptionPtr);
	
	try {
		throw;
	}
	catch (const detail::PolymorphicExceptionBase& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(exc.clone()));
	}
	
	catch (const ::std::invalid_argument& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdInvalidArgument(exc).clone()));
	}
	catch (const ::std::domain_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdDomainError(exc).clone()));
	}
	catch (const ::std::length_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdLengthError(exc).clone()));
	}
	catch (const ::std::out_of_range& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdOutOfRange(exc).clone()));
	}
	#if (CPPDEVTK_HAVE_CPP11_FUTURE)
	catch (const ::std::future_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdFutureError(exc).clone()));
	}
	#endif
	catch (const ::std::logic_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdLogicError(exc).clone()));
	}
	
	catch (const ::std::overflow_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdOverflowError(exc).clone()));
	}
	catch (const ::std::underflow_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdUnderflowError(exc).clone()));
	}
	catch (const ::std::range_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdRangeError(exc).clone()));
	}
	catch (const ::std::ios_base::failure& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdIosFailure(exc).clone()));
	}
	#if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
	catch (const ::std::system_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdSystemError(exc).clone()));
	}
	#endif
	catch (const ::std::runtime_error& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdRuntimeError(exc).clone()));
	}
	
	catch (const ::std::bad_typeid& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadTypeid(exc).clone()));
	}
	catch (const ::std::bad_cast& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadCast(exc).clone()));
	}
	catch (const ::std::bad_weak_ptr& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadWeakPtr(exc).clone()));
	}
	catch (const ::std::bad_function_call& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadFunctionCall(exc).clone()));
	}
	catch (const ::std::bad_exception& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadException(exc).clone()));
	}
	catch (const ::std::bad_alloc& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadAlloc(exc).clone()));
	}
	
	catch (const ::std::exception& exc) {
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdException(exc).clone()));
	}
	
	catch (...) {
		CPPDEVTK_MAKE_UNKNOWN_EXCEPTION(exc);
		exceptionPtr = ExceptionPtr(ExceptionPtr::ImplPtrType(exc.clone()));
	}
	
	CPPDEVTK_ASSERT(exceptionPtr);
	return exceptionPtr;
}


}	// namespace detail


CPPDEVTK_BASE_API ExceptionPtr CurrentException() CPPDEVTK_NOEXCEPT {
	// NOTE:
	// - If needs to allocate memory and the attempt fails, it returns an exception_ptr object that refers to
	// an instance of bad_alloc.
	// - If the attempt to copy the current exception object throws an exception, the function returns an exception_ptr
	// object that refers to the thrown exception or, if this is not possible, to an instance of bad_exception.
	// - ::std::current_exception() may not fail but our impl may fail and return null exception pointer to indicate failure.
	
	try {
		return detail::ThrowableCurrentException();
	}
	catch (const ::std::bad_alloc& exc) {
		try {
			return ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadAlloc(exc).clone()));
		}
		catch (...) {
			return ExceptionPtr();
		}
	}
	catch (...) {
		try {
			return detail::ThrowableCurrentException();
		}
		catch (...) {
			try {
				return ExceptionPtr(ExceptionPtr::ImplPtrType(detail::PolymorphicStdBadException(
						::std::bad_exception()).clone()));
			}
			catch (...) {
				return ExceptionPtr();
			}
		}
	}
}

CPPDEVTK_BASE_API void RethrowException(ExceptionPtr exceptionPtr) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(exceptionPtr);
	CPPDEVTK_ASSERT(exceptionPtr.pImpl_);
	
	exceptionPtr.pImpl_->raise();
	
	CPPDEVTK_LOG_FATAL("exceptionPtr.pImpl_->raise() did not throw!!!; typeInfoName: "
			<< typeid(*(exceptionPtr.pImpl_)).name());
	CPPDEVTK_ASSERT(0 && "exceptionPtr.pImpl_->raise() did not throw!!!");
	::std::terminate();
}


#endif	// (!CPPDEVTK_HAVE_CPP11_EXCEPTION_PROPAGATION)


}	// namespace base
}	// namespace cppdevtk
