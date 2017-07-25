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


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_TYPE_INFO_HPP_INCLUDED_
