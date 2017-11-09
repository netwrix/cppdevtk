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


#ifndef CPPDEVTK_BASE_MEMORY_HPP_INCLUDED_
#define CPPDEVTK_BASE_MEMORY_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"
#include "unused.hpp"

#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


#define CPPDEVTK_BAD_WEAK_PTR_EXCEPTION()	\
	::cppdevtk::base::BadWeakPtrException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_WEAK_PTR_EXCEPTION(excName)	\
	::cppdevtk::base::BadWeakPtrException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 20.7.2.1 Class bad_weak_ptr
class CPPDEVTK_BASE_API BadWeakPtrException: public Exception {
public:
	explicit BadWeakPtrException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadWeakPtrException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadWeakPtrException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual BadWeakPtrException* clone() const;
#	endif
	
	void Swap(BadWeakPtrException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadWeakPtrException* DoClone() const;
#	endif
	
	void SwapOwnData(BadWeakPtrException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadWeakPtrException& x, BadWeakPtrException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline BadWeakPtrException::BadWeakPtrException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadWeakPtrException::~BadWeakPtrException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadWeakPtrException> BadWeakPtrException::Clone() const {
	return ::std::auto_ptr<BadWeakPtrException>(dynamic_cast<BadWeakPtrException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* BadWeakPtrException::clone() const {
#else
inline BadWeakPtrException* BadWeakPtrException::clone() const {
#endif
	return Clone().release();
}

inline void BadWeakPtrException::Swap(BadWeakPtrException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadWeakPtrException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadWeakPtrException::DoClone() const {
#else
inline BadWeakPtrException* BadWeakPtrException::DoClone() const {
#endif
	return new BadWeakPtrException(*this);
}

inline void BadWeakPtrException::SwapOwnData(BadWeakPtrException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadWeakPtrException& x, BadWeakPtrException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_MEMORY_HPP_INCLUDED_
