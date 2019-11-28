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


#ifndef CPPDEVTK_BASE_DEADLOCK_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_DEADLOCK_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "lock_exception.hpp"
#include "unused.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


#define CPPDEVTK_DEADLOCK_EXCEPTION()	\
	::cppdevtk::base::DeadlockException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_DEADLOCK_EXCEPTION_WA(whatArg)	\
	::cppdevtk::base::DeadlockException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


#define CPPDEVTK_MAKE_DEADLOCK_EXCEPTION(excName)	\
	::cppdevtk::base::DeadlockException excName(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_DEADLOCK_EXCEPTION_WA(excName, whatArg)	\
	::cppdevtk::base::DeadlockException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \remark Not in C++ 11 std.
class CPPDEVTK_BASE_API DeadlockException: public LockException {
public:
	explicit DeadlockException(const SourceCodeInfo& throwPoint);
	DeadlockException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~DeadlockException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<DeadlockException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual QtException* clone() const;
#	else
	virtual DeadlockException* clone() const;
#	endif
	
	void Swap(DeadlockException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual DeadlockException* DoClone() const;
#	endif
	
	void SwapOwnData(DeadlockException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(DeadlockException& x, DeadlockException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline DeadlockException::DeadlockException(const SourceCodeInfo& throwPoint): Exception(throwPoint),
		RuntimeException(throwPoint, "deadlock"), LockException(throwPoint,
		MakeErrorCode(::cppdevtk::base::errc::resource_deadlock_would_occur)) {}

inline DeadlockException::DeadlockException(const SourceCodeInfo& throwPoint, const QString& whatArg): Exception(throwPoint),
		RuntimeException(throwPoint, whatArg), LockException(throwPoint,
		MakeErrorCode(::cppdevtk::base::errc::resource_deadlock_would_occur), whatArg) {}

inline DeadlockException::~DeadlockException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<DeadlockException> DeadlockException::Clone() const {
	return ::std::auto_ptr<DeadlockException>(dynamic_cast<DeadlockException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline QtException* DeadlockException::clone() const {
#else
inline DeadlockException* DeadlockException::clone() const {
#endif
	return Clone().release();
}

inline void DeadlockException::Swap(DeadlockException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		LockException::Swap(other);
		SwapOwnData(other);
	}
}

inline void DeadlockException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* DeadlockException::DoClone() const {
#else
inline DeadlockException* DeadlockException::DoClone() const {
#endif
	return new DeadlockException(*this);
}

inline void DeadlockException::SwapOwnData(DeadlockException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(DeadlockException& x, DeadlockException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_DEADLOCK_EXCEPTION_HPP_INCLUDED_
