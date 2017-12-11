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


#ifndef CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_
#define CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_


#include "config.hpp"
#include "bad_cast_exception.hpp"
#include "stringizable.hpp"

#include <typeinfo>
#include <cstddef>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Wrapper class around std::type_info with value semantics (public copy constructor and assignment operator)
/// and seamless comparisons.
/// \sa <a href="http://loki-lib.sourceforge.net/html/a00553.html">Loki::TypeInfo</a>
class CPPDEVTK_BASE_API TypeInfo: public QStringizable {
public:
	TypeInfo() CPPDEVTK_NOEXCEPT;	///< \note Needed for containers.
	TypeInfo(const ::std::type_info& stdTypeInfo) CPPDEVTK_NOEXCEPT;	///< \note Non-explicit (conversion ctor).
	
	bool Before(const TypeInfo& other) const CPPDEVTK_NOEXCEPT;
	
	const char* Name() const CPPDEVTK_NOEXCEPT;
	virtual QString ToString() const;	///< Same as \c Name()
	
	const ::std::type_info& Peek() const CPPDEVTK_NOEXCEPT;
	
	void Swap(TypeInfo& other) CPPDEVTK_NOEXCEPT;
private:
	const ::std::type_info* pStdTypeInfo_;
};


CPPDEVTK_BASE_API bool operator==(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator<(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator>(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;

CPPDEVTK_BASE_API void swap(TypeInfo& x, TypeInfo& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline TypeInfo::TypeInfo() CPPDEVTK_NOEXCEPT: QStringizable(), pStdTypeInfo_(NULL) {
	class NullTypeInfo {};
	pStdTypeInfo_ = &typeid(NullTypeInfo);
}

inline TypeInfo::TypeInfo(const ::std::type_info& stdTypeInfo) CPPDEVTK_NOEXCEPT: QStringizable(),
		pStdTypeInfo_(&stdTypeInfo) {}

inline bool TypeInfo::Before(const TypeInfo& other) const CPPDEVTK_NOEXCEPT {
#if (CPPDEVTK_COMPILER_MSVC)
	// As usually for Ms standards are made to be broken (::std::type_info::before() returns int...)
	// Please see: https://msdn.microsoft.com/en-us/library/70ky2y6k%28v=vs.90%29.aspx
	return (pStdTypeInfo_->before(*(other.pStdTypeInfo_)) != 0);
#else
	return pStdTypeInfo_->before(*(other.pStdTypeInfo_));
#endif
}

inline const char* TypeInfo::Name() const CPPDEVTK_NOEXCEPT {
	return pStdTypeInfo_->name();
}

inline QString TypeInfo::ToString() const {
	return Name();
}

inline const ::std::type_info& TypeInfo::Peek() const CPPDEVTK_NOEXCEPT {
	return *pStdTypeInfo_;
}

inline void TypeInfo::Swap(TypeInfo& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	swap(pStdTypeInfo_, other.pStdTypeInfo_);
}


inline CPPDEVTK_BASE_API bool operator==(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT {
	return (lhs.Peek() == rhs.Peek());
}

inline CPPDEVTK_BASE_API bool operator<(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT {
	return lhs.Before(rhs);
}

inline CPPDEVTK_BASE_API bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}

inline CPPDEVTK_BASE_API bool operator>(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT {
	return rhs < lhs;
}

inline CPPDEVTK_BASE_API bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT {
	return !(rhs < lhs);
}

inline CPPDEVTK_BASE_API bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs < rhs);
}


inline CPPDEVTK_BASE_API void swap(TypeInfo& x, TypeInfo& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_
