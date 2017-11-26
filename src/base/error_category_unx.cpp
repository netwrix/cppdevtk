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


#include <cppdevtk/base/error_category.hpp>
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <clocale>
#include <cstring>


namespace cppdevtk {
namespace base {
namespace detail {


static bool DoGetMessage(int errVal, const QLocale& locale, QString& message);


}


QString GenericErrorCategory::GetMessage(int errVal, const QLocale& locale) const {
	const int kOrigErrNo = errno;
	
	QString message;
	
	// This is Mac OS X specific (please see cerrno.hpp)
	// TODO: move in mac file
	if ((CPPDEVTK_EFIRST <= errVal) && (errVal <= CPPDEVTK_ELAST)) {
		switch (errVal) {
			case EOWNERDEAD:
				message = "owner dead";
				break;
			case ENOTRECOVERABLE:
				message = "state not recoverable";
				break;
			default:
				CPPDEVTK_ASSERT(message.isEmpty());
				break;
		}
	}
	
	if (!message.isEmpty()) {
		errno = kOrigErrNo;
		
		return message;
	}
	
	//message = QString("Failed to get message for generic error: %1").arg(errVal);
	
	if (!detail::DoGetMessage(errVal, locale, message)) {
		if (locale != QLocale::c()) {
			CPPDEVTK_LOG_INFO("DoGetMessage() failed; retrying with QLocale::c()"
				<< "; errVal: " << errVal << "; locale.name(): " << locale.name());
			if (!detail::DoGetMessage(errVal, QLocale::c(), message)) {
				CPPDEVTK_LOG_ERROR("DoGetMessage() failed with QLocale::c(); give up..."
					<< "; errVal: " << errVal << "; locale.name(): " << locale.name());
			}
		}
		else {
			CPPDEVTK_LOG_ERROR("DoGetMessage() failed"
					<< "; errVal: " << errVal << "; locale.name(): " << locale.name());
		}
	}
	
	errno = kOrigErrNo;
	
	return message;
}


QString SystemErrorCategory::GetMessage(int errVal, const QLocale& locale) const {
	return GetGenericCategory().GetMessage(errVal, locale);
}

ErrorCondition SystemErrorCategory::GetDefaultErrorCondition(int errVal) const CPPDEVTK_NOEXCEPT {
	using namespace ::cppdevtk::base::errc;
	
	switch (errVal) {
		case ESUCCESS: return MakeErrorCondition(success);
		
		// following cases are taken from boost 1.60.0
		case E2BIG: return MakeErrorCondition(argument_list_too_long);
		case EACCES: return MakeErrorCondition(permission_denied);
		case EADDRINUSE: return MakeErrorCondition(address_in_use);
		case EADDRNOTAVAIL: return MakeErrorCondition(address_not_available);
		case EAFNOSUPPORT: return MakeErrorCondition(address_family_not_supported);
		case EAGAIN: return MakeErrorCondition(resource_unavailable_try_again);
#		if (EALREADY != EBUSY)  //  EALREADY and EBUSY are the same on QNX Neutrino
		case EALREADY: return MakeErrorCondition(connection_already_in_progress);
#		endif
		case EBADF: return MakeErrorCondition(bad_file_descriptor);
		case EBADMSG: return MakeErrorCondition(bad_message);
		case EBUSY: return MakeErrorCondition(device_or_resource_busy);
		case ECANCELED: return MakeErrorCondition(operation_canceled);
		case ECHILD: return MakeErrorCondition(no_child_process);
		case ECONNABORTED: return MakeErrorCondition(connection_aborted);
		case ECONNREFUSED: return MakeErrorCondition(connection_refused);
		case ECONNRESET: return MakeErrorCondition(connection_reset);
		case EDEADLK: return MakeErrorCondition(resource_deadlock_would_occur);
		case EDESTADDRREQ: return MakeErrorCondition(destination_address_required);
		case EDOM: return MakeErrorCondition(argument_out_of_domain);
		case EEXIST: return MakeErrorCondition(file_exists);
		case EFAULT: return MakeErrorCondition(bad_address);
		case EFBIG: return MakeErrorCondition(file_too_large);
		case EHOSTUNREACH: return MakeErrorCondition(host_unreachable);
		case EIDRM: return MakeErrorCondition(identifier_removed);
		case EILSEQ: return MakeErrorCondition(illegal_byte_sequence);
		case EINPROGRESS: return MakeErrorCondition(operation_in_progress);
		case EINTR: return MakeErrorCondition(interrupted);
		case EINVAL: return MakeErrorCondition(invalid_argument);
		case EIO: return MakeErrorCondition(io_error);
		case EISCONN: return MakeErrorCondition(already_connected);
		case EISDIR: return MakeErrorCondition(is_a_directory);
		case ELOOP: return MakeErrorCondition(too_many_symbolic_link_levels);
		case EMFILE: return MakeErrorCondition(too_many_files_open);
		case EMLINK: return MakeErrorCondition(too_many_links);
		case EMSGSIZE: return MakeErrorCondition(message_size);
		case ENAMETOOLONG: return MakeErrorCondition(filename_too_long);
		case ENETDOWN: return MakeErrorCondition(network_down);
		case ENETRESET: return MakeErrorCondition(network_reset);
		case ENETUNREACH: return MakeErrorCondition(network_unreachable);
		case ENFILE: return MakeErrorCondition(too_many_files_open_in_system);
		case ENOBUFS: return MakeErrorCondition(no_buffer_space);
		case ENODATA: return MakeErrorCondition(no_message_available);
		case ENODEV: return MakeErrorCondition(no_such_device);
		case ENOENT: return MakeErrorCondition(no_such_file_or_directory);
		case ENOEXEC: return MakeErrorCondition(executable_format_error);
		case ENOLCK: return MakeErrorCondition(no_lock_available);
		case ENOLINK: return MakeErrorCondition(no_link);
		case ENOMEM: return MakeErrorCondition(not_enough_memory);
		case ENOMSG: return MakeErrorCondition(no_message);
		case ENOPROTOOPT: return MakeErrorCondition(no_protocol_option);
		case ENOSPC: return MakeErrorCondition(no_space_on_device);
		case ENOSR: return MakeErrorCondition(no_stream_resources);
		case ENOSTR: return MakeErrorCondition(not_a_stream);
		case ENOSYS: return MakeErrorCondition(function_not_supported);
		case ENOTCONN: return MakeErrorCondition(not_connected);
		case ENOTDIR: return MakeErrorCondition(not_a_directory);
#		if (ENOTEMPTY != EEXIST) // AIX treats ENOTEMPTY and EEXIST as the same value
		case ENOTEMPTY: return MakeErrorCondition(directory_not_empty);
#		endif // ENOTEMPTY != EEXIST
#		if (ENOTRECOVERABLE != ECONNRESET) // the same on some Broadcom chips
		case ENOTRECOVERABLE: return MakeErrorCondition(state_not_recoverable);
#		endif // ENOTRECOVERABLE != ECONNRESET
		case ENOTSOCK: return MakeErrorCondition(not_a_socket);
		case ENOTSUP: return MakeErrorCondition(not_supported);
		case ENOTTY: return MakeErrorCondition(inappropriate_io_control_operation);
		case ENXIO: return MakeErrorCondition(no_such_device_or_address);
#		if (EOPNOTSUPP != ENOTSUP)
		case EOPNOTSUPP: return MakeErrorCondition(operation_not_supported);
#		endif // EOPNOTSUPP != ENOTSUP
		case EOVERFLOW: return MakeErrorCondition(value_too_large);
#		if (EOWNERDEAD != ECONNABORTED) // the same on some Broadcom chips
		case EOWNERDEAD: return MakeErrorCondition(owner_dead);
#		endif // EOWNERDEAD != ECONNABORTED
		case EPERM: return MakeErrorCondition(operation_not_permitted);
		case EPIPE: return MakeErrorCondition(broken_pipe);
		case EPROTO: return MakeErrorCondition(protocol_error);
		case EPROTONOSUPPORT: return MakeErrorCondition(protocol_not_supported);
		case EPROTOTYPE: return MakeErrorCondition(wrong_protocol_type);
		case ERANGE: return MakeErrorCondition(result_out_of_range);
		case EROFS: return MakeErrorCondition(read_only_file_system);
		case ESPIPE: return MakeErrorCondition(invalid_seek);
		case ESRCH: return MakeErrorCondition(no_such_process);
		case ETIME: return MakeErrorCondition(stream_timeout);
		case ETIMEDOUT: return MakeErrorCondition(timed_out);
		case ETXTBSY: return MakeErrorCondition(text_file_busy);
#		if (EAGAIN != EWOULDBLOCK)
		case EWOULDBLOCK: return MakeErrorCondition(operation_would_block);
#		endif // EAGAIN != EWOULDBLOCK
		case EXDEV: return MakeErrorCondition(cross_device_link);
		
		default: return ErrorCondition(errVal, GetSystemCategory());
	}
}


namespace detail {


bool DoGetMessage(int errVal, const QLocale& locale, QString& message) {
	//const int kOrigErrNo = errno;
	
	message = QString("Failed to get message for errno: %1").arg(errVal);
	
	bool retValue = false;
	
	const char* pOldLocale = setlocale(LC_MESSAGES, CPPDEVTK_Q2U(locale.name()).c_str());
	if (pOldLocale == NULL) {
		CPPDEVTK_LOG_INFO("setlocale() failed for: " << locale.name());
	}
	
	char buf[1024] = { '\0' };
	
	errno = ESUCCESS;
	
#	if (!CPPDEVTK_STRERROR_R_CHAR_P)
	
	const int kRetCode = strerror_r(errVal, buf, sizeof(buf));
	if (kRetCode == ESUCCESS) {
		message = CPPDEVTK_U2Q(buf);
		retValue = true;
	}
	else {
		const int kErrNo = errno;
		
#		if 0
		if (kErrNo == EINVAL) {
			CPPDEVTK_LOG_INFO("strerror_r() failed; it returned: " << kRetCode << "; errno: " << kErrNo << " EINVAL");
			
			message = QString("Unknown error %1").arg(errVal);
			retValue = true;
		}
		else {
			CPPDEVTK_LOG_ERROR("strerror_r() failed; it returned: " << kRetCode << "; errno: " << kErrNo
					<< ((kErrNo == ERANGE) ? " ERANGE" : ""));
		}
#		else
		CPPDEVTK_LOG_ERROR("strerror_r() failed; it returned: " << kRetCode << "; errno: " << kErrNo);
#		endif
		
		SuppressUnusedWarning(kErrNo);
	}
	
#	else	// (!CPPDEVTK_STRERROR_R_CHAR_P)
	
	const char* pErrnoMessage = strerror_r(errVal, buf, sizeof(buf));
	if (pErrnoMessage != NULL) {
		message = CPPDEVTK_U2Q(pErrnoMessage);
		retValue = true;
	}
	else {
		const int kErrNo = errno;
		CPPDEVTK_LOG_ERROR("strerror_r() failed; errno: " << kErrNo);
		SuppressUnusedWarning(kErrNo);
	}
	
#	endif	// (!CPPDEVTK_STRERROR_R_CHAR_P)
	
	if (pOldLocale != NULL) {
		if (setlocale(LC_MESSAGES, pOldLocale) == NULL) {
			CPPDEVTK_LOG_INFO("setlocale() failed; failed to restore locale: " << pOldLocale);
		}
	}
	
	//errno = kOrigErrNo;
	
	return retValue;
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
