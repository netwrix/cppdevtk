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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_FUTURE_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/future.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_FUTURE_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_FUTURE_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "stdexcept.hpp"
#include "system_exception.hpp"
#include "unused.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


#define CPPDEVTK_FUTURE_EXCEPTION(errorCode)	\
	::cppdevtk::base::FutureException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))


#define CPPDEVTK_MAKE_FUTURE_EXCEPTION(excName, errorCode)	\
	::cppdevtk::base::FutureException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 30.6.3 Class future_error
class CPPDEVTK_BASE_API FutureException: public virtual LogicException {
public:
	FutureException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode);	
	FutureException(const FutureException& other) CPPDEVTK_NOEXCEPT;
	
	virtual ~FutureException() CPPDEVTK_NOEXCEPT;
	
	FutureException& operator=(const FutureException& other) CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<FutureException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual FutureException* clone() const;
#	endif
	
	void Swap(FutureException& other) CPPDEVTK_NOEXCEPT;
	
	const ErrorCode& ErrorCodeRef() const CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual FutureException* DoClone() const;
#	endif
	
	virtual QString DoOwnWhat() const;
	
	void SwapOwnData(FutureException& other) CPPDEVTK_NOEXCEPT;
private:
	ErrorCode errorCode_;
};


CPPDEVTK_BASE_API void swap(FutureException& x, FutureException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline FutureException::FutureException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode):
		Exception(throwPoint), LogicException(throwPoint, ""), errorCode_(errorCode) {}

inline FutureException::FutureException(const FutureException& other) CPPDEVTK_NOEXCEPT: Exception(other), LogicException(other),
		errorCode_(other.errorCode_) {}

inline FutureException::~FutureException() CPPDEVTK_NOEXCEPT {}

inline FutureException& FutureException::operator=(const FutureException& other) CPPDEVTK_NOEXCEPT {
	FutureException tmp(other);
	Swap(tmp);
	return *this;
}

inline ::std::auto_ptr<FutureException> FutureException::Clone() const {
	return ::std::auto_ptr<FutureException>(dynamic_cast<FutureException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* FutureException::clone() const {
#else
inline FutureException* FutureException::clone() const {
#endif
	return Clone().release();
}

inline void FutureException::Swap(FutureException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline const ErrorCode& FutureException::ErrorCodeRef() const CPPDEVTK_NOEXCEPT {
	return errorCode_;
}

inline void FutureException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* FutureException::DoClone() const {
#else
inline FutureException* FutureException::DoClone() const {
#endif
	return new FutureException(*this);
}

inline void FutureException::SwapOwnData(FutureException& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	
	swap(errorCode_, other.errorCode_);
}


inline CPPDEVTK_BASE_API void swap(FutureException& x, FutureException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_FUTURE_EXCEPTION_HPP_INCLUDED_
