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


#ifndef CPPDEVTK_BASE_FUNCTIONAL_HPP_INCLUDED_
#define CPPDEVTK_BASE_FUNCTIONAL_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"
#include "unused.hpp"

#include CPPDEVTK_TR1_HEADER(functional)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


#define CPPDEVTK_BAD_FUNCTION_CALL_EXCEPTION()	\
	::cppdevtk::base::BadFunctionCallException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_FUNCTION_CALL_EXCEPTION(excName)	\
	::cppdevtk::base::BadFunctionCallException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 11, 20.8.11.1 Class bad_function_call
class CPPDEVTK_BASE_API BadFunctionCallException: public Exception {
public:
	explicit BadFunctionCallException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadFunctionCallException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadFunctionCallException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual BadFunctionCallException* clone() const;
#	endif
	
	void Swap(BadFunctionCallException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadFunctionCallException* DoClone() const;
#	endif
	
	void SwapOwnData(BadFunctionCallException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadFunctionCallException& x, BadFunctionCallException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline BadFunctionCallException::BadFunctionCallException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadFunctionCallException::~BadFunctionCallException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadFunctionCallException> BadFunctionCallException::Clone() const {
	return ::std::auto_ptr<BadFunctionCallException>(dynamic_cast<BadFunctionCallException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* BadFunctionCallException::clone() const {
#else
inline BadFunctionCallException* BadFunctionCallException::clone() const {
#endif
	return Clone().release();
}

inline void BadFunctionCallException::Swap(BadFunctionCallException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadFunctionCallException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadFunctionCallException::DoClone() const {
#else
inline BadFunctionCallException* BadFunctionCallException::DoClone() const {
#endif
	return new BadFunctionCallException(*this);
}

inline void BadFunctionCallException::SwapOwnData(BadFunctionCallException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadFunctionCallException& x, BadFunctionCallException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_FUNCTIONAL_HPP_INCLUDED_
