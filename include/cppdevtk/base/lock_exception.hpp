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


#ifndef CPPDEVTK_BASE_LOCK_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_LOCK_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "system_exception.hpp"
#include "unused.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


#define CPPDEVTK_LOCK_EXCEPTION_W_EC(errorCode)	\
	::cppdevtk::base::LockException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))

#define CPPDEVTK_LOCK_EXCEPTION_W_EC_WA(errorCode, whatArg)	\
	::cppdevtk::base::LockException(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg))


#define CPPDEVTK_MAKE_LOCK_EXCEPTION_W_EC(excName, errorCode)	\
	::cppdevtk::base::LockException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode))

#define CPPDEVTK_MAKE_LOCK_EXCEPTION_W_EC_WA(excName, errorCode, whatArg)	\
	::cppdevtk::base::LockException excName(CPPDEVTK_SOURCE_CODE_INFO(), (errorCode), (whatArg))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for locking exceptions (exceptions thrown by synchronization primitives: mutex, lock etc).
/// \remark Not in C++ 11 std.
class CPPDEVTK_BASE_API LockException: public SystemException {
public:
	LockException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode);
	LockException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const QString& whatArg);
	
	virtual ~LockException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<LockException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual LockException* clone() const;
#	endif
	
	void Swap(LockException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual LockException* DoClone() const;
#	endif
	
	void SwapOwnData(LockException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(LockException& x, LockException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline LockException::LockException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode): Exception(throwPoint),
		RuntimeException(throwPoint, ""), SystemException(throwPoint, errorCode) {}

inline LockException::LockException(const SourceCodeInfo& throwPoint, const ErrorCode& errorCode, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg), SystemException(throwPoint, errorCode, whatArg) {}

inline LockException::~LockException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<LockException> LockException::Clone() const {
	return ::std::auto_ptr<LockException>(dynamic_cast<LockException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* LockException::clone() const {
#else
inline LockException* LockException::clone() const {
#endif
	return Clone().release();
}

inline void LockException::Swap(LockException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		SystemException::Swap(other);
		SwapOwnData(other);
	}
}

inline void LockException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* LockException::DoClone() const {
#else
inline LockException* LockException::DoClone() const {
#endif
	return new LockException(*this);
}

inline void LockException::SwapOwnData(LockException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(LockException& x, LockException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_LOCK_EXCEPTION_HPP_INCLUDED_
