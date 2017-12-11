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


#ifndef CPPDEVTK_BASE_STDEXCEPT_HPP_INCLUDED_
#define CPPDEVTK_BASE_STDEXCEPT_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"

#include <stdexcept>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LogicException

#define CPPDEVTK_LOGIC_EXCEPTION(whatArg)	\
	::cppdevtk::base::LogicException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_LOGIC_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::LogicException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_LOGIC_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::LogicException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_LOGIC_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::LogicException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.1 Class logic_error
class CPPDEVTK_BASE_API LogicException: public virtual Exception {
public:
	LogicException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	LogicException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	LogicException(const LogicException& other) CPPDEVTK_NOEXCEPT;
	
	virtual ~LogicException() CPPDEVTK_NOEXCEPT;
	
	LogicException& operator=(const LogicException& other) CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<LogicException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual LogicException* clone() const;
#	endif
	
	void Swap(LogicException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual LogicException* DoClone() const;
#	endif
	
	virtual QString DoOwnWhat() const;
	
	void SwapOwnData(LogicException& other) CPPDEVTK_NOEXCEPT;
	
	
	QString whatArg_;
};


CPPDEVTK_BASE_API void swap(LogicException& x, LogicException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DomainException

#define CPPDEVTK_DOMAIN_EXCEPTION(whatArg)	\
	::cppdevtk::base::DomainException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_DOMAIN_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::DomainException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_DOMAIN_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::DomainException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_DOMAIN_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::DomainException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.2 Class domain_error
class CPPDEVTK_BASE_API DomainException: public virtual LogicException {
public:
	DomainException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	DomainException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~DomainException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<DomainException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual DomainException* clone() const;
#	endif
	
	void Swap(DomainException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual DomainException* DoClone() const;
#	endif
	
	void SwapOwnData(DomainException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(DomainException& x, DomainException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InvalidArgumentException

#define CPPDEVTK_INVALID_ARGUMENT_EXCEPTION(whatArg)	\
	::cppdevtk::base::InvalidArgumentException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_INVALID_ARGUMENT_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::InvalidArgumentException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_INVALID_ARGUMENT_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::InvalidArgumentException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_INVALID_ARGUMENT_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::InvalidArgumentException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.3 Class invalid_argument
class CPPDEVTK_BASE_API InvalidArgumentException: public virtual LogicException {
public:
	InvalidArgumentException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	InvalidArgumentException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~InvalidArgumentException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<InvalidArgumentException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual InvalidArgumentException* clone() const;
#	endif
	
	void Swap(InvalidArgumentException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual InvalidArgumentException* DoClone() const;
#	endif
	
	void SwapOwnData(InvalidArgumentException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(InvalidArgumentException& x, InvalidArgumentException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LengthException

#define CPPDEVTK_LENGTH_EXCEPTION(whatArg)	\
	::cppdevtk::base::LengthException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_LENGTH_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::LengthException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_LENGTH_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::LengthException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_LENGTH_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::LengthException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.4 Class length_error
class CPPDEVTK_BASE_API LengthException: public virtual LogicException {
public:
	LengthException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	LengthException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~LengthException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<LengthException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual LengthException* clone() const;
#	endif
	
	void Swap(LengthException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual LengthException* DoClone() const;
#	endif
	
	void SwapOwnData(LengthException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(LengthException& x, LengthException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OutOfRangeException

#define CPPDEVTK_OUT_OF_RANGE_EXCEPTION(whatArg)	\
	::cppdevtk::base::OutOfRangeException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_OUT_OF_RANGE_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::OutOfRangeException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_OUT_OF_RANGE_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::OutOfRangeException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_OUT_OF_RANGE_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::OutOfRangeException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.5 Class out_of_range
class CPPDEVTK_BASE_API OutOfRangeException: public virtual LogicException {
public:
	OutOfRangeException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	OutOfRangeException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~OutOfRangeException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<OutOfRangeException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual OutOfRangeException* clone() const;
#	endif
	
	void Swap(OutOfRangeException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual OutOfRangeException* DoClone() const;
#	endif
	
	void SwapOwnData(OutOfRangeException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(OutOfRangeException& x, OutOfRangeException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RuntimeException

#define CPPDEVTK_RUNTIME_EXCEPTION(whatArg)	\
	::cppdevtk::base::RuntimeException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_RUNTIME_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::RuntimeException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_RUNTIME_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::RuntimeException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_RUNTIME_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::RuntimeException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.6 Class runtime_error
class CPPDEVTK_BASE_API RuntimeException: public virtual Exception {
public:
	RuntimeException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	RuntimeException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	RuntimeException(const RuntimeException& other) CPPDEVTK_NOEXCEPT;
	
	virtual ~RuntimeException() CPPDEVTK_NOEXCEPT;
	
	RuntimeException& operator=(const RuntimeException& other) CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<RuntimeException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual RuntimeException* clone() const;
#	endif
	
	void Swap(RuntimeException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual RuntimeException* DoClone() const;
#	endif
	
	virtual QString DoOwnWhat() const;
	
	void SwapOwnData(RuntimeException& other) CPPDEVTK_NOEXCEPT;
	
	
	QString whatArg_;
};


CPPDEVTK_BASE_API void swap(RuntimeException& x, RuntimeException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RangeException

#define CPPDEVTK_RANGE_EXCEPTION(whatArg)	\
	::cppdevtk::base::RangeException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_RANGE_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::RangeException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_RANGE_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::RangeException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_RANGE_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::RangeException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.7 Class range_error
class CPPDEVTK_BASE_API RangeException: public virtual RuntimeException {
public:
	RangeException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	RangeException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~RangeException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<RangeException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual RangeException* clone() const;
#	endif
	
	void Swap(RangeException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual RangeException* DoClone() const;
#	endif
	
	void SwapOwnData(RangeException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(RangeException& x, RangeException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OverflowException

#define CPPDEVTK_OVERFLOW_EXCEPTION(whatArg)	\
	::cppdevtk::base::OverflowException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_OVERFLOW_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::OverflowException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_OVERFLOW_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::OverflowException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_OVERFLOW_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::OverflowException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.8 Class overflow_error
class CPPDEVTK_BASE_API OverflowException: public virtual RuntimeException {
public:
	OverflowException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	OverflowException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~OverflowException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<OverflowException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual OverflowException* clone() const;
#	endif
	
	void Swap(OverflowException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual OverflowException* DoClone() const;
#	endif
	
	void SwapOwnData(OverflowException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(OverflowException& x, OverflowException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnderflowException

#define CPPDEVTK_UNDERFLOW_EXCEPTION(whatArg)	\
	::cppdevtk::base::UnderflowException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_UNDERFLOW_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::base::UnderflowException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_UNDERFLOW_EXCEPTION(excName, whatArg)	\
	::cppdevtk::base::UnderflowException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_UNDERFLOW_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::base::UnderflowException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 19.1.9 Class underflow_error
class CPPDEVTK_BASE_API UnderflowException: public virtual RuntimeException {
public:
	UnderflowException(const SourceCodeInfo& throwPoint, const QString& whatArg);
	UnderflowException(const SourceCodeInfo& throwPoint, const QString& whatArg, const Exception& cause);
	
	virtual ~UnderflowException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<UnderflowException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual UnderflowException* clone() const;
#	endif
	
	void Swap(UnderflowException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual UnderflowException* DoClone() const;
#	endif
	
	void SwapOwnData(UnderflowException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(UnderflowException& x, UnderflowException& y) CPPDEVTK_NOEXCEPT;


/// @}	// std_exceptions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


inline LogicException::LogicException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), whatArg_(whatArg) {}

inline LogicException::LogicException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), whatArg_(whatArg) {}

inline LogicException::LogicException(const LogicException& other) CPPDEVTK_NOEXCEPT: Exception(other), whatArg_(other.whatArg_) {}

inline LogicException::~LogicException() CPPDEVTK_NOEXCEPT {}

inline LogicException& LogicException::operator=(const LogicException& other) CPPDEVTK_NOEXCEPT {
	LogicException tmp(other);
	Swap(tmp);
	return *this;
}

inline ::std::auto_ptr<LogicException> LogicException::Clone() const {
	return ::std::auto_ptr<LogicException>(dynamic_cast<LogicException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* LogicException::clone() const {
#else
inline LogicException* LogicException::clone() const {
#endif
	return Clone().release();
}

inline void LogicException::Swap(LogicException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void LogicException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* LogicException::DoClone() const {
#else
inline LogicException* LogicException::DoClone() const {
#endif
	return new LogicException(*this);
}

inline QString LogicException::DoOwnWhat() const {
	return whatArg_;
}

inline void LogicException::SwapOwnData(LogicException& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	
	swap(whatArg_, other.whatArg_);
}


inline CPPDEVTK_BASE_API void swap(LogicException& x, LogicException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline DomainException::DomainException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), LogicException(throwPoint, whatArg) {}

inline DomainException::DomainException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), LogicException(throwPoint, whatArg, cause) {}

inline DomainException::~DomainException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<DomainException> DomainException::Clone() const {
	return ::std::auto_ptr<DomainException>(dynamic_cast<DomainException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* DomainException::clone() const {
#else
inline DomainException* DomainException::clone() const {
#endif
	return Clone().release();
}

inline void DomainException::Swap(DomainException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline void DomainException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* DomainException::DoClone() const {
#else
inline DomainException* DomainException::DoClone() const {
#endif
	return new DomainException(*this);
}

inline void DomainException::SwapOwnData(DomainException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(DomainException& x, DomainException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline InvalidArgumentException::InvalidArgumentException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), LogicException(throwPoint, whatArg) {}

inline InvalidArgumentException::InvalidArgumentException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), LogicException(throwPoint, whatArg, cause) {}

inline InvalidArgumentException::~InvalidArgumentException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<InvalidArgumentException> InvalidArgumentException::Clone() const {
	return ::std::auto_ptr<InvalidArgumentException>(dynamic_cast<InvalidArgumentException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* InvalidArgumentException::clone() const {
#else
inline InvalidArgumentException* InvalidArgumentException::clone() const {
#endif
	return Clone().release();
}

inline void InvalidArgumentException::Swap(InvalidArgumentException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline void InvalidArgumentException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* InvalidArgumentException::DoClone() const {
#else
inline InvalidArgumentException* InvalidArgumentException::DoClone() const {
#endif
	return new InvalidArgumentException(*this);
}

inline void InvalidArgumentException::SwapOwnData(InvalidArgumentException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(InvalidArgumentException& x, InvalidArgumentException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline LengthException::LengthException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), LogicException(throwPoint, whatArg) {}

inline LengthException::LengthException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), LogicException(throwPoint, whatArg, cause) {}

inline LengthException::~LengthException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<LengthException> LengthException::Clone() const {
	return ::std::auto_ptr<LengthException>(dynamic_cast<LengthException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* LengthException::clone() const {
#else
inline LengthException* LengthException::clone() const {
#endif
	return Clone().release();
}

inline void LengthException::Swap(LengthException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline void LengthException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* LengthException::DoClone() const {
#else
inline LengthException* LengthException::DoClone() const {
#endif
	return new LengthException(*this);
}

inline void LengthException::SwapOwnData(LengthException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(LengthException& x, LengthException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline OutOfRangeException::OutOfRangeException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), LogicException(throwPoint, whatArg) {}

inline OutOfRangeException::OutOfRangeException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), LogicException(throwPoint, whatArg, cause) {}

inline OutOfRangeException::~OutOfRangeException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<OutOfRangeException> OutOfRangeException::Clone() const {
	return ::std::auto_ptr<OutOfRangeException>(dynamic_cast<OutOfRangeException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* OutOfRangeException::clone() const {
#else
inline OutOfRangeException* OutOfRangeException::clone() const {
#endif
	return Clone().release();
}

inline void OutOfRangeException::Swap(OutOfRangeException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		LogicException::Swap(other);
		SwapOwnData(other);
	}
}

inline void OutOfRangeException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* OutOfRangeException::DoClone() const {
#else
inline OutOfRangeException* OutOfRangeException::DoClone() const {
#endif
	return new OutOfRangeException(*this);
}

inline void OutOfRangeException::SwapOwnData(OutOfRangeException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(OutOfRangeException& x, OutOfRangeException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline RuntimeException::RuntimeException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), whatArg_(whatArg) {}

inline RuntimeException::RuntimeException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), whatArg_(whatArg) {}

inline RuntimeException::RuntimeException(const RuntimeException& other) CPPDEVTK_NOEXCEPT: Exception(other), whatArg_(other.whatArg_) {}

inline RuntimeException::~RuntimeException() CPPDEVTK_NOEXCEPT {}

inline RuntimeException& RuntimeException::operator=(const RuntimeException& other) CPPDEVTK_NOEXCEPT {
	RuntimeException tmp(other);
	Swap(tmp);
	return *this;
}

inline ::std::auto_ptr<RuntimeException> RuntimeException::Clone() const {
	return ::std::auto_ptr<RuntimeException>(dynamic_cast<RuntimeException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* RuntimeException::clone() const {
#else
inline RuntimeException* RuntimeException::clone() const {
#endif
	return Clone().release();
}

inline void RuntimeException::Swap(RuntimeException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void RuntimeException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* RuntimeException::DoClone() const {
#else
inline RuntimeException* RuntimeException::DoClone() const {
#endif
	return new RuntimeException(*this);
}

inline QString RuntimeException::DoOwnWhat() const {
	return whatArg_;
}

inline void RuntimeException::SwapOwnData(RuntimeException& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	
	swap(whatArg_, other.whatArg_);
}


inline CPPDEVTK_BASE_API void swap(RuntimeException& x, RuntimeException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline RangeException::RangeException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg) {}

inline RangeException::RangeException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause) {}

inline RangeException::~RangeException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<RangeException> RangeException::Clone() const {
	return ::std::auto_ptr<RangeException>(dynamic_cast<RangeException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* RangeException::clone() const {
#else
inline RangeException* RangeException::clone() const {
#endif
	return Clone().release();
}

inline void RangeException::Swap(RangeException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void RangeException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* RangeException::DoClone() const {
#else
inline RangeException* RangeException::DoClone() const {
#endif
	return new RangeException(*this);
}

inline void RangeException::SwapOwnData(RangeException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(RangeException& x, RangeException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline OverflowException::OverflowException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg) {}

inline OverflowException::OverflowException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause) {}

inline OverflowException::~OverflowException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<OverflowException> OverflowException::Clone() const {
	return ::std::auto_ptr<OverflowException>(dynamic_cast<OverflowException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* OverflowException::clone() const {
#else
inline OverflowException* OverflowException::clone() const {
#endif
	return Clone().release();
}

inline void OverflowException::Swap(OverflowException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void OverflowException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* OverflowException::DoClone() const {
#else
inline OverflowException* OverflowException::DoClone() const {
#endif
	return new OverflowException(*this);
}

inline void OverflowException::SwapOwnData(OverflowException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(OverflowException& x, OverflowException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline UnderflowException::UnderflowException(const SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg) {}

inline UnderflowException::UnderflowException(const SourceCodeInfo& throwPoint, const QString& whatArg,
		const Exception& cause): Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause) {}

inline UnderflowException::~UnderflowException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<UnderflowException> UnderflowException::Clone() const {
	return ::std::auto_ptr<UnderflowException>(dynamic_cast<UnderflowException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* UnderflowException::clone() const {
#else
inline UnderflowException* UnderflowException::clone() const {
#endif
	return Clone().release();
}

inline void UnderflowException::Swap(UnderflowException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void UnderflowException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* UnderflowException::DoClone() const {
#else
inline UnderflowException* UnderflowException::DoClone() const {
#endif
	return new UnderflowException(*this);
}

inline void UnderflowException::SwapOwnData(UnderflowException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(UnderflowException& x, UnderflowException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_STDEXCEPT_HPP_INCLUDED_
