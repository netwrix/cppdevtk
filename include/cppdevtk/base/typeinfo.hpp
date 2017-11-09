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


#ifndef CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_
#define CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_


#include "config.hpp"
#include "exception.hpp"

#include <typeinfo>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Wrapper class around std::type_info with value semantics (public copy constructor and assignment operator)
/// and seamless comparisons.
/// \sa <a href="http://loki-lib.sourceforge.net/index.php?n=Main.HomePage">Loki, TypeInfo</a>
class CPPDEVTK_BASE_API TypeInfo {
public:
	TypeInfo();	///< \note Needed for containers.
	TypeInfo(const ::std::type_info& stdTypeInfo);	///< \remark Non-explicit (conversion ctor).
	
	bool Before(const TypeInfo& other) const;
	const char* Name() const;
	
	const ::std::type_info& Peek() const;
private:
	const ::std::type_info* pStdTypeInfo_;
};


CPPDEVTK_BASE_API bool operator==(const TypeInfo& lhs, const TypeInfo& rhs);
CPPDEVTK_BASE_API bool operator<(const TypeInfo& lhs, const TypeInfo& rhs);
CPPDEVTK_BASE_API bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs);
CPPDEVTK_BASE_API bool operator>(const TypeInfo& lhs, const TypeInfo& rhs);
CPPDEVTK_BASE_API bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs);
CPPDEVTK_BASE_API bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup std_exceptions
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BadTypeIdException

#define CPPDEVTK_BAD_TYPE_ID_EXCEPTION()	\
	::cppdevtk::base::BadTypeIdException(CPPDEVTK_SOURCE_CODE_INFO())

#define CPPDEVTK_MAKE_BAD_TYPE_ID_EXCEPTION(excName)	\
	::cppdevtk::base::BadTypeIdException excName(CPPDEVTK_SOURCE_CODE_INFO())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++ 03, 18.5.3 Class bad_typeid
class CPPDEVTK_BASE_API BadTypeIdException: public Exception {
public:
	explicit BadTypeIdException(const SourceCodeInfo& throwPoint);
	
	virtual ~BadTypeIdException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<BadTypeIdException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual BadTypeIdException* clone() const;
#	endif
	
	void Swap(BadTypeIdException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual Cloneable* DoClone() const;
#	else
	virtual BadTypeIdException* DoClone() const;
#	endif
	
	void SwapOwnData(BadTypeIdException& other) CPPDEVTK_NOEXCEPT;
};


CPPDEVTK_BASE_API void swap(BadTypeIdException& x, BadTypeIdException& y) CPPDEVTK_NOEXCEPT;




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
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
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

inline TypeInfo::TypeInfo(): pStdTypeInfo_(&typeid(*this)) {}

inline TypeInfo::TypeInfo(const ::std::type_info& stdTypeInfo): pStdTypeInfo_(&stdTypeInfo) {}

inline bool TypeInfo::Before(const TypeInfo& other) const {
#if (CPPDEVTK_COMPILER_MSVC)
	// As usually for Ms standards are made to be broken (::std::type_info::before() returns int...)
	// Please see: https://msdn.microsoft.com/en-us/library/70ky2y6k%28v=vs.90%29.aspx
	return (pStdTypeInfo_->before(*(other.pStdTypeInfo_)) != 0);
#else
	return pStdTypeInfo_->before(*(other.pStdTypeInfo_));
#endif
}

inline const char* TypeInfo::Name() const {
	return pStdTypeInfo_->name();
}

inline const ::std::type_info& TypeInfo::Peek() const {
	return *pStdTypeInfo_;
}


inline CPPDEVTK_BASE_API bool operator==(const TypeInfo& lhs, const TypeInfo& rhs) {
	return (lhs.Peek() == rhs.Peek());
}

inline CPPDEVTK_BASE_API bool operator<(const TypeInfo& lhs, const TypeInfo& rhs) {
	return lhs.Before(rhs);
}

inline CPPDEVTK_BASE_API bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) {
	return !(lhs == rhs);
}

inline CPPDEVTK_BASE_API bool operator>(const TypeInfo& lhs, const TypeInfo& rhs) {
	return rhs < lhs;
}

inline CPPDEVTK_BASE_API bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs) {
	return !(rhs < lhs);
}

inline CPPDEVTK_BASE_API bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs) {
	return !(lhs < rhs);
}




inline BadTypeIdException::BadTypeIdException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadTypeIdException::~BadTypeIdException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadTypeIdException> BadTypeIdException::Clone() const {
	return ::std::auto_ptr<BadTypeIdException>(dynamic_cast<BadTypeIdException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* BadTypeIdException::clone() const {
#else
inline BadTypeIdException* BadTypeIdException::clone() const {
#endif
	return Clone().release();
}

inline void BadTypeIdException::Swap(BadTypeIdException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		Exception::Swap(other);
		SwapOwnData(other);
	}
}

inline void BadTypeIdException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline Cloneable* BadTypeIdException::DoClone() const {
#else
inline BadTypeIdException* BadTypeIdException::DoClone() const {
#endif
	return new BadTypeIdException(*this);
}

inline void BadTypeIdException::SwapOwnData(BadTypeIdException& other) CPPDEVTK_NOEXCEPT {
	SuppressUnusedWarning(other);
}


inline CPPDEVTK_BASE_API void swap(BadTypeIdException& x, BadTypeIdException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}




inline BadCastException::BadCastException(const SourceCodeInfo& throwPoint): Exception(throwPoint) {}

inline BadCastException::~BadCastException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<BadCastException> BadCastException::Clone() const {
	return ::std::auto_ptr<BadCastException>(dynamic_cast<BadCastException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* BadCastException::clone() const {
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


#endif	// CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_
