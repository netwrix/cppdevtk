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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_SYSTEM_EXCEPTION_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/system_exception.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_ERROR_CODE_HPP_INCLUDED_
#define CPPDEVTK_BASE_ERROR_CODE_HPP_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_UNIX)
#	if (CPPDEVTK_PLATFORM_LINUX)
#		include "error_code_lnx.hpp"
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#		include "error_code_mac.hpp"
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include "error_code_win.hpp"
#else
#	error "Unsupported platform!!!"
#endif
#include "error_condition.hpp"
#include "stringizable.hpp"
#include "type_traits.hpp"
#include "tostream.hpp"
#include "string_conv.hpp"

#include <QtCore/QString>
#include <QtCore/QTextStream>

#include <cstddef>
#include CPPDEVTK_TR1_HEADER(type_traits)


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif


#undef GetMessage


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup error_code ErrorCode
/// \ingroup system_exception
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++11, 19.5 System error support
template <typename T>
struct IsErrorCodeEnum: public CPPDEVTK_TR1_NS::false_type {};

template <>
struct IsErrorCodeEnum<sys_err::sys_err_t>: public CPPDEVTK_TR1_NS::true_type {};


class ErrorCategory;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note Usage:
/// - Unix: ErrorCode(errno, GetSystemCategory())
/// - Windows: ErrorCode(GetLastError(), GetSystemCategory())
/// \sa C++11, 19.5.2 Class error_code
class CPPDEVTK_BASE_API ErrorCode: public QStringizable {
public:
	typedef void (*UnspecifiedBoolType)();
	
	
	ErrorCode() CPPDEVTK_NOEXCEPT;
	ErrorCode(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT;
	
	template <typename ErrorCodeEnum>
	ErrorCode(ErrorCodeEnum errorCodeEnum, typename EnableIf<IsErrorCodeEnum<ErrorCodeEnum>::value>::Type* = NULL) CPPDEVTK_NOEXCEPT;
	
	void Assign(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT;
	
	template <typename ErrorCodeEnum>
	typename EnableIf<IsErrorCodeEnum<ErrorCodeEnum>::value, ErrorCodeEnum>::Type& operator=(ErrorCodeEnum errorCodeEnum) CPPDEVTK_NOEXCEPT;
	
	void Clear() CPPDEVTK_NOEXCEPT;
	
	int GetValue() const CPPDEVTK_NOEXCEPT;
	const ErrorCategory& GetCategory() const CPPDEVTK_NOEXCEPT;
	ErrorCondition GetDefaultErrorCondition() const CPPDEVTK_NOEXCEPT;
	QString GetMessage() const;
	
	operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT;
	bool operator!() const CPPDEVTK_NOEXCEPT;
	
	/// Format: "category: " + category().name() + "; error: " + value() + " (" + message() + ')'
	virtual QString ToString() const;
private:
	static void UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT;
	
	
	int value_;
	const ErrorCategory* pErrorCategory_;
};


namespace sys_err {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++11, 19.5.2.5 non-member functions
CPPDEVTK_BASE_API ErrorCode MakeErrorCode(sys_err_t sysErr) CPPDEVTK_NOEXCEPT;


}	// namespace sys_err


namespace errc {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++11, 19.5.2.5 non-member functions
CPPDEVTK_BASE_API ErrorCode MakeErrorCode(errc_t errCond) CPPDEVTK_NOEXCEPT;


}	// namespace errc


CPPDEVTK_BASE_API ErrorCode MakeSystemErrorCode(int value) CPPDEVTK_NOEXCEPT;	///< \note Not in std.
CPPDEVTK_BASE_API ErrorCode GetLastSystemErrorCode() CPPDEVTK_NOEXCEPT;	///< \note Not in std.
CPPDEVTK_BASE_API void SetLastSystemErrorCode(const ErrorCode& systemErrorCode);	///< \note Not in std.


/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator==(const ErrorCode& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator!=(const ErrorCode& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.2.5 non-member functions
CPPDEVTK_BASE_API bool operator<(const ErrorCode& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT;

/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator==(const ErrorCode& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator==(const ErrorCondition& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator!=(const ErrorCode& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator!=(const ErrorCondition& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT;


::std::ostream& operator<<(::std::ostream& os, const ErrorCode& errorCode);	///< \sa C++11, 19.5.2.5 non-member functions
::std::wostream& operator<<(::std::wostream& os, const ErrorCode& errorCode);	///< \sa C++11, 19.5.2.5 non-member functions
CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, const ErrorCode& errorCode);	///< \remark Non-std extension


// TODO:
/*
// 19.5.5 Hash support
template <class T> struct hash;
template <> struct hash<error_code>;
*/


/// @}	// error_code
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline ErrorCode::ErrorCode(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT: value_(value),
		pErrorCategory_(&errorCategory) {}

template <typename ErrorCodeEnum>
inline ErrorCode::ErrorCode(ErrorCodeEnum errorCodeEnum, typename EnableIf<IsErrorCodeEnum<ErrorCodeEnum>::value>::Type*)
		CPPDEVTK_NOEXCEPT {
	*this = MakeErrorCode(errorCodeEnum);
}

inline void ErrorCode::Assign(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT {
	value_ = value;
	pErrorCategory_ = &errorCategory;
}

template <typename ErrorCodeEnum>
inline typename EnableIf<IsErrorCodeEnum<ErrorCodeEnum>::value, ErrorCodeEnum>::Type& ErrorCode::operator=(
		ErrorCodeEnum errorCodeEnum) CPPDEVTK_NOEXCEPT {
	*this = MakeErrorCode(errorCodeEnum);
	return *this;
}

inline int ErrorCode::GetValue() const CPPDEVTK_NOEXCEPT {
	return value_;
}

inline ErrorCode::operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT {
	return (value_ == ::cppdevtk::base::sys_err::success) ? NULL : &UnspecifiedBoolTrue;
}

inline bool ErrorCode::operator!() const CPPDEVTK_NOEXCEPT {
	return value_ == ::cppdevtk::base::sys_err::success;
}

inline void ErrorCode::UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT {}


inline CPPDEVTK_BASE_API bool operator!=(const ErrorCode& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}


inline CPPDEVTK_BASE_API bool operator!=(const ErrorCode& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}

inline CPPDEVTK_BASE_API bool operator!=(const ErrorCondition& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}


inline ::std::ostream& operator<<(::std::ostream& os, const ErrorCode& errorCode) {
	os << Q2A(errorCode.ToString());
    return os;
}

inline ::std::wostream& operator<<(::std::wostream& os, const ErrorCode& errorCode) {
	os << Q2W(errorCode.ToString());
    return os;
}

inline CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, const ErrorCode& errorCode) {
	os << errorCode.ToString();
    return os;
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_ERROR_CODE_HPP_INCLUDED_
