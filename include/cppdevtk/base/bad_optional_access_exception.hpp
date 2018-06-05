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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_OPTIONAL_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/optional.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_BAD_OPTIONAL_ACCESS_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_BAD_OPTIONAL_ACCESS_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BadOptionalAccessException

#define CPPDEVTK_BAD_OPTIONAL_ACCESS_EXCEPTION()	\
	::cppdevtk::base::BadOptionalAccessException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_OPTIONAL_ACCESS_EXCEPTION(excName)	\
	::cppdevtk::base::BadOptionalAccessException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 17, class bad_optional_access
class CPPDEVTK_BASE_API BadOptionalAccessException: public Exception {
public:
	explicit BadOptionalAccessException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadOptionalAccessException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadOptionalAccessException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual QtException* clone() const;
#	else
	virtual BadOptionalAccessException* clone() const;
#	endif
	
	void Swap(BadOptionalAccessException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadOptionalAccessException* DoClone() const;
#	endif
	
	void SwapOwnData(BadOptionalAccessException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadOptionalAccessException& x, BadOptionalAccessException& y) CPPDEVTK_NOEXCEPT;


/// @}	// std_exceptions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline BadOptionalAccessException::BadOptionalAccessException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadOptionalAccessException::~BadOptionalAccessException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadOptionalAccessException> BadOptionalAccessException::Clone() const {
	return ::std::auto_ptr<BadOptionalAccessException>(dynamic_cast<BadOptionalAccessException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline QtException* BadOptionalAccessException::clone() const {
#else
inline BadOptionalAccessException* BadOptionalAccessException::clone() const {
#endif
	return Clone().release();
}

inline void BadOptionalAccessException::Swap(BadOptionalAccessException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadOptionalAccessException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadOptionalAccessException::DoClone() const {
#else
inline BadOptionalAccessException* BadOptionalAccessException::DoClone() const {
#endif
	return new BadOptionalAccessException(*this);
}

inline void BadOptionalAccessException::SwapOwnData(BadOptionalAccessException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadOptionalAccessException& x, BadOptionalAccessException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_BAD_OPTIONAL_ACCESS_EXCEPTION_HPP_INCLUDED_
