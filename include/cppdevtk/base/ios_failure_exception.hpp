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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_IOS_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/ios.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_IOS_FAILURE_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_IOS_FAILURE_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "system_exception.hpp"
#include "unused.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


#define CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA(whatArg)	\
	::cppdevtk::base::IosFailureException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA_EC(whatArg, errorCode)	\
	::cppdevtk::base::IosFailureException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (errorCode))


#define CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA_CAUSE(whatArg, cause)	\
	::cppdevtk::base::IosFailureException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA_EC_CAUSE(whatArg, errorCode, cause)	\
	::cppdevtk::base::IosFailureException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (errorCode), (cause))


#define CPPDEVTK_MAKE_IOS_FAILURE_EXCEPTION_W_WA(excName, whatArg)	\
	::cppdevtk::base::IosFailureException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_IOS_FAILURE_EXCEPTION_W_WA_EC(excName, whatArg, errorCode)	\
	::cppdevtk::base::IosFailureException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (errorCode))


#define CPPDEVTK_MAKE_IOS_FAILURE_EXCEPTION_W_WA_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::IosFailureException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_IOS_FAILURE_EXCEPTION_W_WA_EC_CAUSE(excName, whatArg, errorCode, cause)	\
	::cppdevtk::base::IosFailureException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (errorCode), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11 std, 27.5.3.1.1 Class ios_base::failure
class CPPDEVTK_BASE_API IosFailureException: public virtual SystemException {
public:
	IosFailureException(const SourceCodeInfo& throwPoint, const char* whatArg,
			const ErrorCode& errorCode = MakeErrorCode(::cppdevtk::base::errc::io_error));
	IosFailureException(const SourceCodeInfo& throwPoint, const QString& whatArg,
			const ErrorCode& errorCode = MakeErrorCode(::cppdevtk::base::errc::io_error));
	
	IosFailureException(const SourceCodeInfo& throwPoint, const char* whatArg, const ErrorCode& errorCode,
			const Exception& cause);
	IosFailureException(const SourceCodeInfo& throwPoint, const QString& whatArg, const ErrorCode& errorCode,
			const Exception& cause);
	
	virtual ~IosFailureException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<IosFailureException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual IosFailureException* clone() const;
#	endif
	
	void Swap(IosFailureException& other) CPPDEVTK_NOEXCEPT;
	
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual IosFailureException* DoClone() const;
#	endif
	
	void SwapOwnData(IosFailureException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(IosFailureException& x, IosFailureException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline IosFailureException::IosFailureException(const SourceCodeInfo& throwPoint, const char* whatArg,
		const ErrorCode& errorCode): Exception(throwPoint), RuntimeException(throwPoint, whatArg),
		SystemException(throwPoint, errorCode, whatArg) {}

inline IosFailureException::IosFailureException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const ErrorCode& errorCode): Exception(throwPoint), RuntimeException(throwPoint, whatArg),
		SystemException(throwPoint, errorCode, whatArg) {}

inline IosFailureException::IosFailureException(const SourceCodeInfo& throwPoint, const char* whatArg,
		const ErrorCode& errorCode, const Exception& cause): Exception(throwPoint), RuntimeException(throwPoint, whatArg),
		SystemException(throwPoint, errorCode, whatArg, cause) {}

inline IosFailureException::IosFailureException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const ErrorCode& errorCode, const Exception& cause): Exception(throwPoint), RuntimeException(throwPoint, whatArg),
		SystemException(throwPoint, errorCode, whatArg, cause) {}

inline IosFailureException::~IosFailureException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<IosFailureException> IosFailureException::Clone() const {
	return ::std::auto_ptr<IosFailureException>(dynamic_cast<IosFailureException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* IosFailureException::clone() const {
#else
inline IosFailureException* IosFailureException::clone() const {
#endif
	return Clone().release();
}

inline void IosFailureException::Swap(IosFailureException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		SystemException::Swap(other);
		SwapOwnData(other);
	}
}

inline void IosFailureException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* IosFailureException::DoClone() const {
#else
inline IosFailureException* IosFailureException::DoClone() const {
#endif
	return new IosFailureException(*this);
}

inline void IosFailureException::SwapOwnData(IosFailureException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(IosFailureException& x, IosFailureException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_IOS_FAILURE_EXCEPTION_HPP_INCLUDED_
