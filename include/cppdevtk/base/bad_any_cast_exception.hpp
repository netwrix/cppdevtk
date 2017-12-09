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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_ANY_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/any.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_BAD_ANY_CAST_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_BAD_ANY_CAST_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "typeinfo.hpp"
#include "unused.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


#define CPPDEVTK_BAD_ANY_CAST_EXCEPTION()	\
	::cppdevtk::base::BadAnyCastException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_ANY_CAST_EXCEPTION(excName)	\
	::cppdevtk::base::BadAnyCastException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Exception thrown by \c Any or \c AnyCast
class CPPDEVTK_BASE_API BadAnyCastException: public BadCastException {
public:
	explicit BadAnyCastException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadAnyCastException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadAnyCastException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual BadAnyCastException* clone() const;
#	endif
	
	void Swap(BadAnyCastException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadAnyCastException* DoClone() const;
#	endif
	
	void SwapOwnData(BadAnyCastException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadAnyCastException& x, BadAnyCastException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline BadAnyCastException::BadAnyCastException(const SourceCodeInfo& throwPoint): BadCastException(throwPoint) {}

inline BadAnyCastException::~BadAnyCastException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadAnyCastException> BadAnyCastException::Clone() const {
	return ::std::auto_ptr<BadAnyCastException>(dynamic_cast<BadAnyCastException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* BadAnyCastException::clone() const {
#else
inline BadAnyCastException* BadAnyCastException::clone() const {
#endif
	return Clone().release();
}

inline void BadAnyCastException::Swap(BadAnyCastException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		BadCastException::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadAnyCastException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadAnyCastException::DoClone() const {
#else
inline BadAnyCastException* BadAnyCastException::DoClone() const {
#endif
	return new BadAnyCastException(*this);
}

inline void BadAnyCastException::SwapOwnData(BadAnyCastException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadAnyCastException& x, BadAnyCastException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_BAD_ANY_CAST_EXCEPTION_HPP_INCLUDED_
