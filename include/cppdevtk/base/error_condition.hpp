/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_BASE_ERROR_CONDITION_HPP_INCLUDED_
#define CPPDEVTK_BASE_ERROR_CONDITION_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif

#include "cerrno.hpp"
#include "stringizable.hpp"
#include "type_traits.hpp"
#include "string_conv.hpp"
#include "tostream.hpp"

#include <QtCore/QTextStream>
#include <QtCore/QString>

#include <cstddef>
#include CPPDEVTK_TR1_HEADER(type_traits)


#undef GetMessage


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup error_condition ErrorCondition
/// \ingroup system_exception
/// @{


namespace errc {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Generic error conditions.
/// \sa C++11, 19.5 System error support, enum class errc
enum errc_t {
	success = ESUCCESS,	///< \note Not in C++11 std
	address_family_not_supported = EAFNOSUPPORT,
	address_in_use = EADDRINUSE,
	address_not_available = EADDRNOTAVAIL,
	already_connected = EISCONN,
	argument_list_too_long = E2BIG,
	argument_out_of_domain = EDOM,
	bad_address = EFAULT,
	bad_file_descriptor = EBADF,
	bad_message = EBADMSG,
	broken_pipe = EPIPE,
	connection_aborted = ECONNABORTED,
	connection_already_in_progress = EALREADY,
	connection_refused = ECONNREFUSED,
	connection_reset = ECONNRESET,
	cross_device_link = EXDEV,
	destination_address_required = EDESTADDRREQ,
	device_or_resource_busy = EBUSY,
	directory_not_empty = ENOTEMPTY,
	executable_format_error = ENOEXEC,
	file_exists = EEXIST,
	file_too_large = EFBIG,
	filename_too_long = ENAMETOOLONG,
	function_not_supported = ENOSYS,
	host_unreachable = EHOSTUNREACH,
	identifier_removed = EIDRM,
	illegal_byte_sequence = EILSEQ,
	inappropriate_io_control_operation = ENOTTY,
	interrupted = EINTR,
	invalid_argument = EINVAL,
	invalid_seek = ESPIPE,
	io_error = EIO,
	is_a_directory = EISDIR,
	message_size = EMSGSIZE,
	network_down = ENETDOWN,
	network_reset = ENETRESET,
	network_unreachable = ENETUNREACH,
	no_buffer_space = ENOBUFS,
	no_child_process = ECHILD,
	no_link = ENOLINK,
	no_lock_available = ENOLCK,
	no_message_available = ENODATA,
	no_message = ENOMSG,
	no_protocol_option = ENOPROTOOPT,
	no_space_on_device = ENOSPC,
	no_stream_resources = ENOSR,
	no_such_device_or_address = ENXIO,
	no_such_device = ENODEV,
	no_such_file_or_directory = ENOENT,
	no_such_process = ESRCH,
	not_a_directory = ENOTDIR,
	not_a_socket = ENOTSOCK,
	not_a_stream = ENOSTR,
	not_connected = ENOTCONN,
	not_enough_memory = ENOMEM,
	not_supported = ENOTSUP,
	operation_canceled = ECANCELED,
	operation_in_progress = EINPROGRESS,
	operation_not_permitted = EPERM,
	operation_not_supported = EOPNOTSUPP,
	operation_would_block = EWOULDBLOCK,
	owner_dead = EOWNERDEAD,
	permission_denied = EACCES,
	protocol_error = EPROTO,
	protocol_not_supported = EPROTONOSUPPORT,
	read_only_file_system = EROFS,
	resource_deadlock_would_occur = EDEADLK,
	resource_unavailable_try_again = EAGAIN,
	result_out_of_range = ERANGE,
	state_not_recoverable = ENOTRECOVERABLE,
	stream_timeout = ETIME,
	text_file_busy = ETXTBSY,
	timed_out = ETIMEDOUT,
	too_many_files_open_in_system = ENFILE,
	too_many_files_open = EMFILE,
	too_many_links = EMLINK,
	too_many_symbolic_link_levels = ELOOP,
	value_too_large = EOVERFLOW,
	wrong_protocol_type = EPROTOTYPE
};


}	// namespace errc


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++11, 19.5 System error support
template <typename T>
struct IsErrorConditionEnum: public CPPDEVTK_TR1_NS::false_type {};

template <>
struct CPPDEVTK_BASE_API IsErrorConditionEnum<errc::errc_t>: public CPPDEVTK_TR1_NS::true_type {};


class ErrorCategory;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note Usage: ErrorCondition(errno, GenericCategoryRef())
/// \sa C++11, 19.5.3 Class error_condition
class CPPDEVTK_BASE_API ErrorCondition: public QStringizable {
public:
	typedef void (*UnspecifiedBoolType)();
	
	
	ErrorCondition() CPPDEVTK_NOEXCEPT;
	ErrorCondition(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT;
	
	template <typename ErrorConditionEnum>
	ErrorCondition(ErrorConditionEnum errorConditionEnum,
			typename EnableIf<IsErrorConditionEnum<ErrorConditionEnum>::value>::Type* = NULL) CPPDEVTK_NOEXCEPT;
	
	void Assign(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT;
	
	template <typename ErrorConditionEnum>
	typename EnableIf<IsErrorConditionEnum<ErrorConditionEnum>::value, ErrorConditionEnum>::Type& operator=(
			ErrorConditionEnum errorConditionEnum) CPPDEVTK_NOEXCEPT;
	
	void Clear() CPPDEVTK_NOEXCEPT;
	
	int GetValue() const CPPDEVTK_NOEXCEPT;
	const ErrorCategory& CategoryRef() const CPPDEVTK_NOEXCEPT;
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


namespace errc {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++11, 19.5.3.5 Class error_condition non-member functions
CPPDEVTK_BASE_API ErrorCondition MakeErrorCondition(errc_t errCond) CPPDEVTK_NOEXCEPT;


}	// namespace errc


CPPDEVTK_BASE_API ErrorCondition MakeGenericErrorCondition(int value) CPPDEVTK_NOEXCEPT;	///< \note Not in std.


/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator==(const ErrorCondition& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.4 Comparison operators
CPPDEVTK_BASE_API bool operator!=(const ErrorCondition& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT;
/// \sa C++11, 19.5.3.5 non-member functions
CPPDEVTK_BASE_API bool operator<(const ErrorCondition& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT;

::std::ostream& operator<<(::std::ostream& os, const ErrorCondition& errorCondition);	///< \remark Non-std extension
::std::wostream& operator<<(::std::wostream& os, const ErrorCondition& errorCondition);	///< \remark Non-std extension
CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, const ErrorCondition& errorCondition);	///< \remark Non-std extension


/// @}	// error_condition
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline ErrorCondition::ErrorCondition(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT: value_(value),
		pErrorCategory_(&errorCategory) {}

template <typename ErrorConditionEnum>
inline ErrorCondition::ErrorCondition(ErrorConditionEnum errorConditionEnum,
		typename EnableIf<IsErrorConditionEnum<ErrorConditionEnum>::value>::Type*) CPPDEVTK_NOEXCEPT {
	*this = MakeErrorCondition(errorConditionEnum);
}

inline void ErrorCondition::Assign(int value, const ErrorCategory& errorCategory) CPPDEVTK_NOEXCEPT {
	value_ = value;
	pErrorCategory_ = &errorCategory;
}

template <typename ErrorConditionEnum>
inline typename EnableIf<IsErrorConditionEnum<ErrorConditionEnum>::value, ErrorConditionEnum>::Type& ErrorCondition::operator=(
		ErrorConditionEnum errorConditionEnum) CPPDEVTK_NOEXCEPT {
	*this = MakeErrorCondition(errorConditionEnum);
	return *this;
}

inline int ErrorCondition::GetValue() const CPPDEVTK_NOEXCEPT {
	return value_;
}

inline ErrorCondition::operator UnspecifiedBoolType() const CPPDEVTK_NOEXCEPT {
	return (value_ == errc::success) ? NULL : &UnspecifiedBoolTrue;
}

inline bool ErrorCondition::operator!() const CPPDEVTK_NOEXCEPT {
	return value_ == errc::success;
}

inline void ErrorCondition::UnspecifiedBoolTrue() CPPDEVTK_NOEXCEPT {}


inline CPPDEVTK_BASE_API bool operator!=(const ErrorCondition& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT {
	return !(lhs == rhs);
}


inline ::std::ostream& operator<<(::std::ostream& os, const ErrorCondition& errorCondition) {
	os << Q2A(errorCondition.ToString());
    return os;
}

inline ::std::wostream& operator<<(::std::wostream& os, const ErrorCondition& errorCondition) {
	os << Q2W(errorCondition.ToString());
    return os;
}

inline CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, const ErrorCondition& errorCondition) {
	os << errorCondition.ToString();
    return os;
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_ERROR_CONDITION_HPP_INCLUDED_
