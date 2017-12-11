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


#ifndef CPPDEVTK_BASE_UNKNOWN_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_UNKNOWN_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"
#include "unused.hpp"

#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


#define CPPDEVTK_UNKNOWN_EXCEPTION()	\
	::cppdevtk::base::UnknownException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_UNKNOWN_EXCEPTION(excName)	\
	::cppdevtk::base::UnknownException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Thrown to indicate that an unknown exception was caught (used by CurrentException())
class CPPDEVTK_BASE_API UnknownException: public Exception {
public:
	explicit UnknownException(const SourceCodeInfo& throwPoint);
	
	virtual ~UnknownException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<UnknownException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual UnknownException* clone() const;
#	endif
	
	void Swap(UnknownException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual UnknownException* DoClone() const;
#	endif
	
	void SwapOwnData(UnknownException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(UnknownException& x, UnknownException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline UnknownException::UnknownException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline UnknownException::~UnknownException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<UnknownException> UnknownException::Clone() const {
	return ::std::auto_ptr<UnknownException>(dynamic_cast<UnknownException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* UnknownException::clone() const {
#else
inline UnknownException* UnknownException::clone() const {
#endif
	return Clone().release();
}

inline void UnknownException::Swap(UnknownException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void UnknownException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* UnknownException::DoClone() const {
#else
inline UnknownException* UnknownException::DoClone() const {
#endif
	return new UnknownException(*this);
}

inline void UnknownException::SwapOwnData(UnknownException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(UnknownException& x, UnknownException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_UNKNOWN_EXCEPTION_HPP_INCLUDED_
