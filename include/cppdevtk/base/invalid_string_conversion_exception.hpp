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


#ifndef CPPDEVTK_BASE_INVALID_STRING_CONVERSION_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_BASE_INVALID_STRING_CONVERSION_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include "stdexcept.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InvalidStringConversionException
// NOTE: Does not inherit virtual because we don't think that it is needed (we can change it).

#define CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION(from, to)	\
	::cppdevtk::base::InvalidStringConversionException(CPPDEVTK_SOURCE_CODE_INFO(), (from), (to))

#define CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION_W_CAUSE(from, to, cause)	\
	::cppdevtk::base::InvalidStringConversionException(CPPDEVTK_SOURCE_CODE_INFO(), (from), (to), (cause))

#define CPPDEVTK_MAKE_INVALID_STRING_CONVERSION_EXCEPTION(excName, from, to)	\
	::cppdevtk::base::InvalidStringConversionException excName(CPPDEVTK_SOURCE_CODE_INFO(), (from), (to))

#define CPPDEVTK_MAKE_INVALID_STRING_CONVERSION_EXCEPTION_W_CAUSE(excName, from, to, cause)	\
	::cppdevtk::base::InvalidStringConversionException excName(CPPDEVTK_SOURCE_CODE_INFO(), (from), (to), (cause))


class CPPDEVTK_BASE_API InvalidStringConversionException: public RuntimeException {
public:
	InvalidStringConversionException(const SourceCodeInfo& throwPoint, const QString& from, const QString& to);
	InvalidStringConversionException(const SourceCodeInfo& throwPoint, const QString& from, const QString& to,
			const Exception& cause);
	
	virtual ~InvalidStringConversionException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<InvalidStringConversionException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual InvalidStringConversionException* clone() const;
#	endif
	
	void Swap(InvalidStringConversionException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual InvalidStringConversionException* DoClone() const;
#	endif
	
	void SwapOwnData(InvalidStringConversionException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(InvalidStringConversionException& x, InvalidStringConversionException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

#define CPPDEVTK_DETAIL_INVALID_STRING_CONVERSION_EXCEPTION_FMT_MSG "Invalid string conversion from '%1' to '%2'"

inline InvalidStringConversionException::InvalidStringConversionException(const SourceCodeInfo& throwPoint, const QString& from,
		const QString& to):
		Exception(throwPoint),
		RuntimeException(throwPoint, QString(CPPDEVTK_DETAIL_INVALID_STRING_CONVERSION_EXCEPTION_FMT_MSG).arg(from, to)) {}

inline InvalidStringConversionException::InvalidStringConversionException(const SourceCodeInfo& throwPoint, const QString& from,
		const QString& to, const Exception& cause):
		Exception(throwPoint, cause),
		RuntimeException(throwPoint, QString(CPPDEVTK_DETAIL_INVALID_STRING_CONVERSION_EXCEPTION_FMT_MSG).arg(from, to),
		cause) {}

inline InvalidStringConversionException::~InvalidStringConversionException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<InvalidStringConversionException> InvalidStringConversionException::Clone() const {
	return ::std::auto_ptr<InvalidStringConversionException>(
			dynamic_cast<InvalidStringConversionException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* InvalidStringConversionException::clone() const {
#else
inline InvalidStringConversionException* InvalidStringConversionException::clone() const {
#endif
	return Clone().release();
}

inline void InvalidStringConversionException::Swap(InvalidStringConversionException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void InvalidStringConversionException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* InvalidStringConversionException::DoClone() const {
#else
inline InvalidStringConversionException* InvalidStringConversionException::DoClone() const {
#endif
	return new InvalidStringConversionException(*this);
}

inline void InvalidStringConversionException::SwapOwnData(InvalidStringConversionException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(InvalidStringConversionException& x, InvalidStringConversionException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_INVALID_STRING_CONVERSION_EXCEPTION_HPP_INCLUDED_
