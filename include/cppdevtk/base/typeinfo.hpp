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

#include <typeinfo>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Wrapper class around std::type_info with value semantics (public copy constructor and assignment operator)
/// and seamless comparisons.
/// \sa <a href="http://loki-lib.sourceforge.net/index.php?n=Main.HomePage">Loki, TypeInfo</a>
class CPPDEVTK_BASE_API TypeInfo {
public:
	TypeInfo() CPPDEVTK_NOEXCEPT;	///< \note Needed for containers.
	TypeInfo(const ::std::type_info& stdTypeInfo) CPPDEVTK_NOEXCEPT;	///< \remark Non-explicit (conversion ctor).
	
	bool Before(const TypeInfo& other) const CPPDEVTK_NOEXCEPT;
	const char* Name() const CPPDEVTK_NOEXCEPT;
	
	const ::std::type_info& Peek() const CPPDEVTK_NOEXCEPT;
private:
	const ::std::type_info* pStdTypeInfo_;
};


CPPDEVTK_BASE_API bool operator==(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator<(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator>(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;
CPPDEVTK_BASE_API bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline TypeInfo::TypeInfo() CPPDEVTK_NOEXCEPT: pStdTypeInfo_(&typeid(*this)) {}

inline TypeInfo::TypeInfo(const ::std::type_info& stdTypeInfo) CPPDEVTK_NOEXCEPT: pStdTypeInfo_(&stdTypeInfo) {}

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

inline const ::std::type_info& TypeInfo::Peek() const CPPDEVTK_NOEXCEPT {
	return *pStdTypeInfo_;
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


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_
