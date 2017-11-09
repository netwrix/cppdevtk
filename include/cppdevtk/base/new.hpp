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


#ifndef CPPDEVTK_BASE_NEW_HPP_INCLUDED_
#define CPPDEVTK_BASE_NEW_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"
#include "unused.hpp"

#include <new>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


#define CPPDEVTK_BAD_ALLOC_EXCEPTION()	\
	::cppdevtk::base::BadAllocException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_ALLOC_EXCEPTION(excName)	\
	::cppdevtk::base::BadAllocException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 18.4.2.1 Class bad_alloc
class CPPDEVTK_BASE_API BadAllocException: public Exception {
public:
	explicit BadAllocException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadAllocException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadAllocException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual BadAllocException* clone() const;
#	endif
	
	void Swap(BadAllocException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadAllocException* DoClone() const;
#	endif
	
	void SwapOwnData(BadAllocException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadAllocException& x, BadAllocException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline BadAllocException::BadAllocException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadAllocException::~BadAllocException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadAllocException> BadAllocException::Clone() const {
	return ::std::auto_ptr<BadAllocException>(dynamic_cast<BadAllocException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* BadAllocException::clone() const {
#else
inline BadAllocException* BadAllocException::clone() const {
#endif
	return Clone().release();
}

inline void BadAllocException::Swap(BadAllocException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadAllocException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadAllocException::DoClone() const {
#else
inline BadAllocException* BadAllocException::DoClone() const {
#endif
	return new BadAllocException(*this);
}

inline void BadAllocException::SwapOwnData(BadAllocException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadAllocException& x, BadAllocException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_NEW_HPP_INCLUDED_
