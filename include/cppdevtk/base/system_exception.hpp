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


#ifndef CPPDEVTK_BASE_SYSTEM_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_SYSTEM_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "stdexcept.hpp"
#include "error_condition.hpp"
#include "error_code.hpp"
#include "error_category.hpp"

#if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
#	include <system_error>
#endif


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup system_exception System Exception related
/// System Exception related
/// \ingroup std_exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EC(errorCode)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(errorCode, whatArg)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg))

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EV(errVal, errorCategory)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory))

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EV_WA(errVal, errorCategory, whatArg)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory), (whatArg))


#define CPPDEVTK_SYSTEM_EXCEPTION_W_EC_CAUSE(errorCode, cause)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (cause))

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA_CAUSE(errorCode, whatArg, cause)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg), (cause))

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EV_CAUSE(errVal, errorCategory, cause)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory), (cause))

#define CPPDEVTK_SYSTEM_EXCEPTION_W_EV_WA_CAUSE(errVal, errorCategory, whatArg, cause)	\
	::cppdevtk::base::SystemException(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory), (whatArg), (cause))


#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EC(excName, errorCode)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))

#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EC_WA(excName, errorCode, whatArg)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg))

#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EV(excName, errVal, errorCategory)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory))

#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EV_WA(excName, errVal, errorCategory, whatArg)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory), (whatArg))


#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EC_CAUSE(excName, errorCode, cause)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (cause))

#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EC_WA_CAUSE(excName, errorCode, whatArg, cause)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg), (cause))

#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EV_CAUSE(excName, errVal, errorCategory, cause)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory), (cause))

#define CPPDEVTK_MAKE_SYSTEM_EXCEPTION_W_EV_WA_CAUSE(excName, errVal, errorCategory, whatArg, cause)	\
	::cppdevtk::base::SystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errVal), (errorCategory), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11 std, 19.5.6 Class system_error
class CPPDEVTK_BASE_API SystemException: public virtual RuntimeException {
public:
	SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode);
	SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const char* whatArg);
	SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const QString& whatArg);
	
	SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory);
	SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory, const char* whatArg);
	SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory, const QString& whatArg);
	
	SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const Exception& cause);
	SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const char* whatArg, const Exception& cause);
	SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const QString& whatArg, const Exception& cause);
	
	SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory,
			const Exception& cause);
	SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory, const char* whatArg,
			const Exception& cause);
	SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory, const QString& whatArg,
			const Exception& cause);
	
	SystemException(const SystemException& other) CPPDEVTK_NOEXCEPT;
	
	virtual ~SystemException() CPPDEVTK_NOEXCEPT;
	
	SystemException& operator=(const SystemException& other) CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<SystemException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual QtException* clone() const;
#	else
	virtual SystemException* clone() const;
#	endif
	
	void Swap(SystemException& other) CPPDEVTK_NOEXCEPT;
	
	const ErrorCode& ErrorCodeRef() const CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual SystemException* DoClone() const;
#	endif
	
	virtual QString DoOwnWhat() const;
	
	void SwapOwnData(SystemException& other) CPPDEVTK_NOEXCEPT;
private:
	ErrorCode errorCode_;
};


CPPDEVTK_BASE_API void swap(SystemException& x, SystemException& y) CPPDEVTK_NOEXCEPT;


/// @}	// system_exception
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode):
		Exception(throwPoint), RuntimeException(throwPoint, ""), errorCode_(errorCode) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode,
		const char* whatArg): Exception(throwPoint), RuntimeException(throwPoint, whatArg), errorCode_(errorCode) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode,
		const QString& whatArg): Exception(throwPoint), RuntimeException(throwPoint, whatArg), errorCode_(errorCode) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory):
		Exception(throwPoint), RuntimeException(throwPoint,""), errorCode_(errVal, errorCategory) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory,
		const char* whatArg): Exception(throwPoint), RuntimeException(throwPoint, whatArg), errorCode_(errVal, errorCategory) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory,
		const QString& whatArg): Exception(throwPoint), RuntimeException(throwPoint, whatArg), errorCode_(errVal, errorCategory) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode,
		const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, "", cause),
		errorCode_(errorCode) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode,
		const char* whatArg, const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause),
		errorCode_(errorCode) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode,
		const QString& whatArg, const Exception& cause): Exception(throwPoint, cause),
		RuntimeException(throwPoint, whatArg, cause), errorCode_(errorCode) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory,
		const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint,"", cause),
		errorCode_(errVal, errorCategory) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory,
		const char* whatArg, const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause),
		errorCode_(errVal, errorCategory) {}

inline SystemException::SystemException(const SourceCodeInfo& throwPoint, int errVal, const ErrorCategory& errorCategory,
		const QString& whatArg, const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause),
		errorCode_(errVal, errorCategory) {}

inline SystemException::SystemException(const SystemException& other) CPPDEVTK_NOEXCEPT: Exception(other), RuntimeException(other),
		errorCode_(other.errorCode_) {}

inline SystemException::~SystemException() CPPDEVTK_NOEXCEPT {}

inline SystemException& SystemException::operator=(const SystemException& other) CPPDEVTK_NOEXCEPT {
	SystemException tmp(other);
	Swap(tmp);
	return *this;
}

inline ::std::auto_ptr<SystemException> SystemException::Clone() const {
	return ::std::auto_ptr<SystemException>(dynamic_cast<SystemException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline QtException* SystemException::clone() const {
#else
inline SystemException* SystemException::clone() const {
#endif
	return Clone().release();
}

inline void SystemException::Swap(SystemException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline const ErrorCode& SystemException::ErrorCodeRef() const CPPDEVTK_NOEXCEPT {
	return errorCode_;
}

inline void SystemException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* SystemException::DoClone() const {
#else
inline SystemException* SystemException::DoClone() const {
#endif
	return new SystemException(*this);
}

inline void SystemException::SwapOwnData(SystemException& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	
	swap(errorCode_, other.errorCode_);
}


inline CPPDEVTK_BASE_API void swap(SystemException& x, SystemException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_SYSTEM_EXCEPTION_HPP_INCLUDED_
