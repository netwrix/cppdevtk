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


#ifndef CPPDEVTK_BASE_DBC_EXCEPTIONS_HPP_INCLUDED_
#define CPPDEVTK_BASE_DBC_EXCEPTIONS_HPP_INCLUDED_


#include "config.hpp"
#include "stdexcept.hpp"
#include "cerrno.hpp"

#include <QtCore/QLocale>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup dbc_exceptions Design By Contract Exceptions
/// \ingroup dbc
/// \ingroup exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NullPointerException
// NOTE: derived from LogicException not RuntimeException (not for SIGSEGV, STATUS_ACCESS_VIOLATION)

#define CPPDEVTK_NULL_POINTER_EXCEPTION()	\
	::cppdevtk::base::NullPointerException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_NULL_POINTER_EXCEPTION_W_CAUSE(cause)	\
	::cppdevtk::base::NullPointerException(CPPDEVTK_SOURCE_CODE_INFO(), (cause))

#define CPPDEVTK_MAKE_NULL_POINTER_EXCEPTION(excName)	\
	::cppdevtk::base::NullPointerException excName(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_NULL_POINTER_EXCEPTION_W_CAUSE(excName, cause)	\
	::cppdevtk::base::NullPointerException excName(CPPDEVTK_SOURCE_CODE_INFO(), (cause))


class CPPDEVTK_BASE_API NullPointerException: public virtual LogicException {
public:
	explicit NullPointerException(const SourceCodeInfo& throwPoint);
	NullPointerException(const SourceCodeInfo& throwPoint, const Exception& cause);
	
	virtual ~NullPointerException() throw();
	
	::std::auto_ptr<NullPointerException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual NullPointerException* clone() const;
#	endif
	
	void Swap(NullPointerException& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual NullPointerException* DoClone() const;
#	endif
	
	void SwapOwnData(NullPointerException& other);
};


CPPDEVTK_BASE_API void swap(NullPointerException& x, NullPointerException& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InvalidStateException
// NOTE: Does not inherit virtual because we don't think that it is needed (we can change it).

#define CPPDEVTK_INVALID_STATE_EXCEPTION(expectedState, actualState, requestedOperation)	\
	::cppdevtk::base::InvalidStateException(CPPDEVTK_SOURCE_CODE_INFO(),	\
			(expectedState), (actualState), (requestedOperation))

#define CPPDEVTK_INVALID_STATE_EXCEPTION_W_CAUSE(expectedState, actualState, requestedOperation, cause)	\
	::cppdevtk::base::InvalidStateException(CPPDEVTK_SOURCE_CODE_INFO(),	\
			(expectedState), (actualState), (requestedOperation), (cause))

#define CPPDEVTK_MAKE_INVALID_STATE_EXCEPTION(excName, expectedState, actualState, requestedOperation)	\
	::cppdevtk::base::InvalidStateException excName(CPPDEVTK_SOURCE_CODE_INFO(),	\
			(expectedState), (actualState), (requestedOperation))

#define CPPDEVTK_MAKE_INVALID_STATE_EXCEPTION_W_CAUSE(excName, expectedState, actualState, requestedOperation, cause)	\
	::cppdevtk::base::InvalidStateException excName(CPPDEVTK_SOURCE_CODE_INFO(),	\
			(expectedState), (actualState), (requestedOperation), (cause))


class CPPDEVTK_BASE_API InvalidStateException: public LogicException {
public:
	InvalidStateException(const SourceCodeInfo& throwPoint, const QString& expectedState, const QString& actualState,
			const QString& requestedOperation);
	InvalidStateException(const SourceCodeInfo& throwPoint, const QString& expectedState, const QString& actualState,
			const QString& requestedOperation, const Exception& cause);
	
	virtual ~InvalidStateException() throw();
	
	::std::auto_ptr<InvalidStateException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual InvalidStateException* clone() const;
#	endif
	
	void Swap(InvalidStateException& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual InvalidStateException* DoClone() const;
#	endif
	
	void SwapOwnData(InvalidStateException& other);
};


CPPDEVTK_BASE_API void swap(InvalidStateException& x, InvalidStateException& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EmptyArgumentException
// NOTE: Does not inherit virtual because we don't think that it is needed (we can change it).

#define CPPDEVTK_EMPTY_ARGUMENT_EXCEPTION(argName)	\
	::cppdevtk::base::EmptyArgumentException(CPPDEVTK_SOURCE_CODE_INFO(), (argName))

#define CPPDEVTK_EMPTY_ARGUMENT_EXCEPTION_W_CAUSE(argName, cause)	\
	::cppdevtk::base::EmptyArgumentException(CPPDEVTK_SOURCE_CODE_INFO(), (argName), (cause))

#define CPPDEVTK_MAKE_EMPTY_ARGUMENT_EXCEPTION(excName, argName)	\
	::cppdevtk::base::EmptyArgumentException excName(CPPDEVTK_SOURCE_CODE_INFO(), (argName))

#define CPPDEVTK_MAKE_EMPTY_ARGUMENT_EXCEPTION_W_CAUSE(excName, argName, cause)	\
	::cppdevtk::base::EmptyArgumentException excName(CPPDEVTK_SOURCE_CODE_INFO(), (argName), (cause))


class CPPDEVTK_BASE_API EmptyArgumentException: public InvalidArgumentException {
public:
	EmptyArgumentException(const SourceCodeInfo& throwPoint, const QString& argName);
	EmptyArgumentException(const SourceCodeInfo& throwPoint, const QString& argName, const Exception& cause);
	
	virtual ~EmptyArgumentException() throw();
	
	::std::auto_ptr<EmptyArgumentException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual EmptyArgumentException* clone() const;
#	endif
	
	void Swap(EmptyArgumentException& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual EmptyArgumentException* DoClone() const;
#	endif
	
	void SwapOwnData(EmptyArgumentException& other);
};


CPPDEVTK_BASE_API void swap(EmptyArgumentException& x, EmptyArgumentException& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NullArgumentException

#define CPPDEVTK_NULL_ARGUMENT_EXCEPTION(argName)	\
	::cppdevtk::base::NullArgumentException(CPPDEVTK_SOURCE_CODE_INFO(), (argName))

#define CPPDEVTK_NULL_ARGUMENT_EXCEPTION_W_CAUSE(argName, cause)	\
	::cppdevtk::base::NullArgumentException(CPPDEVTK_SOURCE_CODE_INFO(), (argName), (cause))

#define CPPDEVTK_MAKE_NULL_ARGUMENT_EXCEPTION(excName, argName)	\
	::cppdevtk::base::NullArgumentException excName(CPPDEVTK_SOURCE_CODE_INFO(), (argName))

#define CPPDEVTK_MAKE_NULL_ARGUMENT_EXCEPTION_W_CAUSE(excName, argName, cause)	\
	::cppdevtk::base::NullArgumentException excName(CPPDEVTK_SOURCE_CODE_INFO(), (argName), (cause))


class CPPDEVTK_BASE_API NullArgumentException: public virtual LogicException,
		public NullPointerException,
		public InvalidArgumentException {
public:
	NullArgumentException(const SourceCodeInfo& throwPoint, const QString& argName);
	NullArgumentException(const SourceCodeInfo& throwPoint, const QString& argName, const Exception& cause);
	
	virtual ~NullArgumentException() throw();
	
	::std::auto_ptr<NullArgumentException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual NullArgumentException* clone() const;
#	endif
	
	void Swap(NullArgumentException& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual NullArgumentException* DoClone() const;
#	endif
	
	void SwapOwnData(NullArgumentException& other);
};


CPPDEVTK_BASE_API void swap(NullArgumentException& x, NullArgumentException& y);


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

#define CPPDEVTK_DETAIL_NULL_POINTER_EXCEPTION_MSG "Attempt to dereference a NULL pointer detected"

inline NullPointerException::NullPointerException(const SourceCodeInfo& throwPoint):
		Exception(throwPoint), LogicException(throwPoint, CPPDEVTK_DETAIL_NULL_POINTER_EXCEPTION_MSG) {}

inline NullPointerException::NullPointerException(const SourceCodeInfo& throwPoint, const Exception& cause):
		Exception(throwPoint, cause), LogicException(throwPoint, CPPDEVTK_DETAIL_NULL_POINTER_EXCEPTION_MSG, cause) {}

inline NullPointerException::~NullPointerException() throw() {}

inline ::std::auto_ptr<NullPointerException> NullPointerException::Clone() const {
	return ::std::auto_ptr<NullPointerException>(dynamic_cast<NullPointerException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* NullPointerException::clone() const {
#else
inline NullPointerException* NullPointerException::clone() const {
#endif
	return Clone().release();
}

inline void NullPointerException::Swap(NullPointerException& other) {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline void NullPointerException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* NullPointerException::DoClone() const {
#else
inline NullPointerException* NullPointerException::DoClone() const {
#endif
	return new NullPointerException(*this);
}

inline void NullPointerException::SwapOwnData(NullPointerException& other) {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(NullPointerException& x, NullPointerException& y) {
	x.Swap(y);
}




#define CPPDEVTK_DETAIL_INVALID_STATE_EXCEPTION_FMT_MSG "Invalid state for requested operation: expectedState: %1;"	\
		" actualState: %2; requestedOperation: %3"

inline InvalidStateException::InvalidStateException(const SourceCodeInfo& throwPoint, const QString& expectedState,
		const QString& actualState, const QString& requestedOperation):
		Exception(throwPoint), LogicException(throwPoint, QString(CPPDEVTK_DETAIL_INVALID_STATE_EXCEPTION_FMT_MSG).arg(
		expectedState, actualState, requestedOperation)) {}

inline InvalidStateException::InvalidStateException(const SourceCodeInfo& throwPoint, const QString& expectedState,
		const QString& actualState, const QString& requestedOperation, const Exception& cause):
		Exception(throwPoint, cause),
		LogicException(throwPoint, QString(CPPDEVTK_DETAIL_INVALID_STATE_EXCEPTION_FMT_MSG).arg(
		expectedState, actualState, requestedOperation), cause) {}

inline InvalidStateException::~InvalidStateException() throw() {}

inline ::std::auto_ptr<InvalidStateException> InvalidStateException::Clone() const {
	return ::std::auto_ptr<InvalidStateException>(dynamic_cast<InvalidStateException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* InvalidStateException::clone() const {
#else
inline InvalidStateException* InvalidStateException::clone() const {
#endif
	return Clone().release();
}

inline void InvalidStateException::Swap(InvalidStateException& other) {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline void InvalidStateException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* InvalidStateException::DoClone() const {
#else
inline InvalidStateException* InvalidStateException::DoClone() const {
#endif
	return new InvalidStateException(*this);
}

inline void InvalidStateException::SwapOwnData(InvalidStateException& other) {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(InvalidStateException& x, InvalidStateException& y) {
	x.Swap(y);
}




#define CPPDEVTK_DETAIL_EMPTY_ARGUMENT_EXCEPTION_FMT_MSG "Argument %1 is empty"

inline EmptyArgumentException::EmptyArgumentException(const SourceCodeInfo& throwPoint, const QString& argName):
		Exception(throwPoint), LogicException(throwPoint, QString(CPPDEVTK_DETAIL_EMPTY_ARGUMENT_EXCEPTION_FMT_MSG).arg(
		argName)), InvalidArgumentException(throwPoint, "") {}

inline EmptyArgumentException::EmptyArgumentException(const SourceCodeInfo& throwPoint, const QString& argName,
		const Exception& cause): Exception(throwPoint, cause),
		LogicException(throwPoint, QString(CPPDEVTK_DETAIL_EMPTY_ARGUMENT_EXCEPTION_FMT_MSG).arg(argName), cause),
		InvalidArgumentException(throwPoint, "", cause) {}

inline EmptyArgumentException::~EmptyArgumentException() throw() {}

inline ::std::auto_ptr<EmptyArgumentException> EmptyArgumentException::Clone() const {
	return ::std::auto_ptr<EmptyArgumentException>(
			dynamic_cast<EmptyArgumentException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* EmptyArgumentException::clone() const {
#else
inline EmptyArgumentException* EmptyArgumentException::clone() const {
#endif
	return Clone().release();
}

inline void EmptyArgumentException::Swap(EmptyArgumentException& other) {
	if (this != &other) {
		InvalidArgumentException::Swap(other);
		SwapOwnData(other);
	}
}

inline void EmptyArgumentException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* EmptyArgumentException::DoClone() const {
#else
inline EmptyArgumentException* EmptyArgumentException::DoClone() const {
#endif
	return new EmptyArgumentException(*this);
}

inline void EmptyArgumentException::SwapOwnData(EmptyArgumentException& other) {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(EmptyArgumentException& x, EmptyArgumentException& y) {
	x.Swap(y);
}




#define CPPDEVTK_DETAIL_NULL_ARGUMENT_EXCEPTION_FMT_MSG "Argument %1 is NULL"

inline NullArgumentException::NullArgumentException(const SourceCodeInfo& throwPoint, const QString& argName):
		Exception(throwPoint), LogicException(throwPoint, QString(CPPDEVTK_DETAIL_NULL_ARGUMENT_EXCEPTION_FMT_MSG).arg(
		argName)), NullPointerException(throwPoint), InvalidArgumentException(throwPoint, "") {}

inline NullArgumentException::NullArgumentException(const SourceCodeInfo& throwPoint, const QString& argName,
		const Exception& cause): Exception(throwPoint, cause),
		LogicException(throwPoint, QString(CPPDEVTK_DETAIL_NULL_ARGUMENT_EXCEPTION_FMT_MSG).arg(argName), cause),
		NullPointerException(throwPoint, cause), InvalidArgumentException(throwPoint, "", cause) {}

inline NullArgumentException::~NullArgumentException() throw() {}

inline ::std::auto_ptr<NullArgumentException> NullArgumentException::Clone() const {
	return ::std::auto_ptr<NullArgumentException>(
			dynamic_cast<NullArgumentException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* NullArgumentException::clone() const {
#else
inline NullArgumentException* NullArgumentException::clone() const {
#endif
	return Clone().release();
}

inline void NullArgumentException::Swap(NullArgumentException& other) {
	if (this != &other) {
		LogicException::Swap(other);
		NullPointerException::SwapOwnData(other);
		InvalidArgumentException::SwapOwnData(other);
		SwapOwnData(other);
	}
}

inline void NullArgumentException::DoThrow() const {
#	if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
	// buggy msvc: C4673: throwing 'type' the following types will not be considered at the catch site
#	pragma warning(disable: 4673)
#	endif
	
	throw *this;
	
#	if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(default: 4673)
#	endif
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* NullArgumentException::DoClone() const {
#else
inline NullArgumentException* NullArgumentException::DoClone() const {
#endif
	return new NullArgumentException(*this);
}

inline void NullArgumentException::SwapOwnData(NullArgumentException& other) {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(NullArgumentException& x, NullArgumentException& y) {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_DBC_EXCEPTIONS_HPP_INCLUDED_
