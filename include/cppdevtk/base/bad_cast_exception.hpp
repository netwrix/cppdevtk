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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/typeinfo.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_BAD_CAST_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_BAD_CAST_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BadCastException

#define CPPDEVTK_BAD_CAST_EXCEPTION()	\
	::cppdevtk::base::BadCastException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_CAST_EXCEPTION(excName)	\
	::cppdevtk::base::BadCastException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 18.5.2 Class bad_cast
class CPPDEVTK_BASE_API BadCastException: public Exception {
public:
	explicit BadCastException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadCastException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadCastException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual QtException* clone() const;
#	else
	virtual BadCastException* clone() const;
#	endif
	
	void Swap(BadCastException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadCastException* DoClone() const;
#	endif
	
	void SwapOwnData(BadCastException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadCastException& x, BadCastException& y) CPPDEVTK_NOEXCEPT;


/// @}	// std_exceptions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline BadCastException::BadCastException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadCastException::~BadCastException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadCastException> BadCastException::Clone() const {
	return ::std::auto_ptr<BadCastException>(dynamic_cast<BadCastException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline QtException* BadCastException::clone() const {
#else
inline BadCastException* BadCastException::clone() const {
#endif
	return Clone().release();
}

inline void BadCastException::Swap(BadCastException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadCastException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadCastException::DoClone() const {
#else
inline BadCastException* BadCastException::DoClone() const {
#endif
	return new BadCastException(*this);
}

inline void BadCastException::SwapOwnData(BadCastException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadCastException& x, BadCastException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_BAD_CAST_EXCEPTION_HPP_INCLUDED_
