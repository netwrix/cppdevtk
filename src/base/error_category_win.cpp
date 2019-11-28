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


#include <cppdevtk/base/config.hpp>
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
#	pragma warning(disable: 4996)	// C4996: 'func' : this function or variable may be unsafe (_sys_nerr)
#	pragma warning(disable: 4456)	// C4456: declaration of 'var' hides previous local declaration (kErrNo)
#endif

#include <cppdevtk/base/error_category.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/base/locale_to_lang_id_win.hpp>
#include <cppdevtk/base/tchar.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/string_utils.hpp>
#include <cppdevtk/base/unused.hpp>

#include <windows.h>
#include <tchar.h>


namespace cppdevtk {
namespace base {


QString GenericErrorCategory::GetMessage(int errVal, const QLocale& locale) const {
	CPPDEVTK_ASSERT(CPPDEVTK_EFIRST > _sys_nerr);
	
	const int kOrigErrNo = errno;
	
	SuppressUnusedWarning(locale);
	
	QString message;
	
	if ((CPPDEVTK_EFIRST <= errVal) && (errVal <= CPPDEVTK_ELAST)) {
		switch (errVal) {
			case EAFNOSUPPORT:
				message = "address family not supported";
				break;
			case EADDRINUSE:
				message = "address in use";
				break;
			case EADDRNOTAVAIL:
				message = "address not available";
				break;
			case EISCONN:
				message = "already connected";
				break;
			case EBADMSG:
				message = "bad message";
				break;
			case ECONNABORTED:
				message = "connection aborted";
				break;
			case EALREADY:
				message = "connection already in progress";
				break;
			case ECONNREFUSED:
				message = "connection refused";
				break;
			case ECONNRESET:
				message = "connection reset";
				break;
			case EDESTADDRREQ:
				message = "destination address required";
				break;
			case EHOSTUNREACH:
				message = "host unreachable";
				break;
			case EIDRM:
				message = "identifier removed";
				break;
			case EMSGSIZE:
				message = "message size";
				break;
			case ENETDOWN:
				message = "network down";
				break;
			case ENETRESET:
				message = "network reset";
				break;
			case ENETUNREACH:
				message = "network unreachable";
				break;
			case ENOBUFS:
				message = "no buffer space";
				break;
			case ENOLINK:
				message = "no link";
				break;
			case ENODATA:
				message = "no message available";
				break;
			case ENOMSG:
				message = "no message";
				break;
			case ENOPROTOOPT:
				message = "no protocol option";
				break;
			case ENOSR:
				message = "no stream resources";
				break;
			case ENOTSOCK:
				message = "not a socket";
				break;
			case ENOSTR:
				message = "not a stream";
				break;
			case ENOTCONN:
				message = "not connected";
				break;
			case ENOTSUP:
				message = "not supported";
				break;
			case ECANCELED:
				message = "operation canceled";
				break;
			case EINPROGRESS:
				message = "operation in progress";
				break;
			case EOPNOTSUPP:
				message = "operation not supported";
				break;
			case EWOULDBLOCK:
				message = "operation would block";
				break;
			case EOWNERDEAD:
				message = "owner dead";
				break;
			case EPROTO:
				message = "protocol error";
				break;
			case EPROTONOSUPPORT:
				message = "protocol not supported";
				break;
			case ENOTRECOVERABLE:
				message = "state not recoverable";
				break;
			case ETIME:
				message = "stream timeout";
				break;
			case ETXTBSY:
				message = "text file busy";
				break;
			case ETIMEDOUT:
				message = "timed out";
				break;
			case ELOOP:
				message = "too many synbolic link levels";
				break;
			case EOVERFLOW:
				message = "value too large";
				break;
			case EPROTOTYPE:
				message = "wrong protocol type";
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
	
	message = QString("Failed to get message for generic error: %1").arg(errVal);
	
	tchar buf[1024] = { CPPDEVTK_T('\0') };
	
	errno = ESUCCESS;
	
	const errno_t kRetCode = _tcserror_s(buf, (sizeof(buf)/sizeof(buf[0])), errVal);
	if (kRetCode == ESUCCESS) {
		message = CPPDEVTK_T2Q(buf);
	}
	else {
		const int kErrNo = errno;
		CPPDEVTK_LOG_ERROR("_tcserror_s() failed; it returned: " << kRetCode << "; errno: " << kErrNo);
		SuppressUnusedWarning(kErrNo);
	}
	
	errno = kOrigErrNo;
	
	return message;
}


QString SystemErrorCategory::GetMessage(int errVal, const QLocale& locale) const {
	const DWORD kLastError = ::GetLastError();
	
	QString sysErrMsg = QString("Failed to get message for system error: %1").arg(errVal);
	
	const WORD kLangId = LocaleToLangId(locale);
	//CPPDEVTK_LOG_TRACE("locale.language(): " << locale.language() << "; langId: " << NumToHexStr(langId));
	
	LPTSTR pTSysErrMsg = NULL;
	
	::SetLastError(ERROR_SUCCESS);
	
	DWORD retCode = FormatMessage(
			(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS),
			NULL, errVal, kLangId, (LPTSTR)&pTSysErrMsg, 0, NULL);
	if (retCode != 0) {
		CPPDEVTK_ASSERT(pTSysErrMsg != NULL);
		sysErrMsg = CPPDEVTK_T2Q(pTSysErrMsg);
	}
	else {
		DWORD lastError = ::GetLastError();
		if ((lastError == ERROR_RESOURCE_LANG_NOT_FOUND)
				|| (lastError == ERROR_MUI_FILE_NOT_FOUND)
				|| (lastError == ERROR_MUI_FILE_NOT_LOADED)) {
			CPPDEVTK_LOG_WARN("FormatMessage() failed; retrying with langId 0"
					<< "; errVal: " << errVal << "; locale.name(): " << locale.name() << "; lastError: " << lastError);
			
			if (pTSysErrMsg != NULL) {
				LocalFree((HLOCAL)pTSysErrMsg);
				pTSysErrMsg = NULL;
			}
			
			::SetLastError(ERROR_SUCCESS);
			
			retCode = FormatMessage(
					(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS),
					NULL, errVal, 0, (LPTSTR)&pTSysErrMsg, 0, NULL);
			if (retCode != 0) {
				CPPDEVTK_ASSERT(pTSysErrMsg != NULL);
				sysErrMsg = CPPDEVTK_T2Q(pTSysErrMsg);
			}
			else {
				lastError = ::GetLastError();
				CPPDEVTK_LOG_ERROR("FormatMessage() failed with langId 0; give up..."
						<< "; errVal: " << errVal << "; locale.name(): " << locale.name() << "; lastError: " << lastError);
			}
		}
		else {
			CPPDEVTK_LOG_ERROR("FormatMessage() failed"
					<< "; errVal: " << errVal << "; locale.name(): " << locale.name() << "; lastError: " << lastError);
		}
	}
	
	if (pTSysErrMsg != NULL) {
		LocalFree((HLOCAL)pTSysErrMsg);
	}
	
	::SetLastError(kLastError);
	
	return sysErrMsg;
}

ErrorCondition SystemErrorCategory::GetDefaultErrorCondition(int errVal) const CPPDEVTK_NOEXCEPT {
	using namespace ::cppdevtk::base::errc;

	switch (errVal) {
		case ERROR_SUCCESS: return MakeErrorCondition(success);
		
		// following cases are taken from boost 1.60.0
		case ERROR_ACCESS_DENIED: return MakeErrorCondition(permission_denied);
		case ERROR_ALREADY_EXISTS: return MakeErrorCondition(file_exists);
		case ERROR_BAD_UNIT: return MakeErrorCondition(no_such_device);
		case ERROR_BUFFER_OVERFLOW: return MakeErrorCondition(filename_too_long);
		case ERROR_BUSY: return MakeErrorCondition(device_or_resource_busy);
		case ERROR_BUSY_DRIVE: return MakeErrorCondition(device_or_resource_busy);
		case ERROR_CANNOT_MAKE: return MakeErrorCondition(permission_denied);
		case ERROR_CANTOPEN: return MakeErrorCondition(io_error);
		case ERROR_CANTREAD: return MakeErrorCondition(io_error);
		case ERROR_CANTWRITE: return MakeErrorCondition(io_error);
		case ERROR_CURRENT_DIRECTORY: return MakeErrorCondition(permission_denied);
		case ERROR_DEV_NOT_EXIST: return MakeErrorCondition(no_such_device);
		case ERROR_DEVICE_IN_USE: return MakeErrorCondition(device_or_resource_busy);
		case ERROR_DIR_NOT_EMPTY: return MakeErrorCondition(directory_not_empty);
		case ERROR_DIRECTORY: return MakeErrorCondition(invalid_argument); // WinError.h: "The directory name is invalid"
		case ERROR_DISK_FULL: return MakeErrorCondition(no_space_on_device);
		case ERROR_FILE_EXISTS: return MakeErrorCondition(file_exists);
		case ERROR_FILE_NOT_FOUND: return MakeErrorCondition(no_such_file_or_directory);
		case ERROR_HANDLE_DISK_FULL: return MakeErrorCondition(no_space_on_device);
		case ERROR_INVALID_ACCESS: return MakeErrorCondition(permission_denied);
		case ERROR_INVALID_DRIVE: return MakeErrorCondition(no_such_device);
		case ERROR_INVALID_FUNCTION: return MakeErrorCondition(function_not_supported);
		case ERROR_INVALID_HANDLE: return MakeErrorCondition(invalid_argument);
		case ERROR_INVALID_NAME: return MakeErrorCondition(invalid_argument);
		case ERROR_LOCK_VIOLATION: return MakeErrorCondition(no_lock_available);
		case ERROR_LOCKED: return MakeErrorCondition(no_lock_available);
		case ERROR_NEGATIVE_SEEK: return MakeErrorCondition(invalid_argument);
		case ERROR_NOACCESS: return MakeErrorCondition(permission_denied);
		case ERROR_NOT_ENOUGH_MEMORY: return MakeErrorCondition(not_enough_memory);
		case ERROR_NOT_READY: return MakeErrorCondition(resource_unavailable_try_again);
		case ERROR_NOT_SAME_DEVICE: return MakeErrorCondition(cross_device_link);
		case ERROR_OPEN_FAILED: return MakeErrorCondition(io_error);
		case ERROR_OPEN_FILES: return MakeErrorCondition(device_or_resource_busy);
		case ERROR_OPERATION_ABORTED: return MakeErrorCondition(operation_canceled);
		case ERROR_OUTOFMEMORY: return MakeErrorCondition(not_enough_memory);
		case ERROR_PATH_NOT_FOUND: return MakeErrorCondition(no_such_file_or_directory);
		case ERROR_READ_FAULT: return MakeErrorCondition(io_error);
		case ERROR_RETRY: return MakeErrorCondition(resource_unavailable_try_again);
		case ERROR_SEEK: return MakeErrorCondition(io_error);
		case ERROR_SHARING_VIOLATION: return MakeErrorCondition(permission_denied);
		case ERROR_TOO_MANY_OPEN_FILES: return MakeErrorCondition(too_many_files_open);
		case ERROR_WRITE_FAULT: return MakeErrorCondition(io_error);
		case ERROR_WRITE_PROTECT: return MakeErrorCondition(permission_denied);
		case WSAEACCES: return MakeErrorCondition(permission_denied);
		case WSAEADDRINUSE: return MakeErrorCondition(address_in_use);
		case WSAEADDRNOTAVAIL: return MakeErrorCondition(address_not_available);
		case WSAEAFNOSUPPORT: return MakeErrorCondition(address_family_not_supported);
		case WSAEALREADY: return MakeErrorCondition(connection_already_in_progress);
		case WSAEBADF: return MakeErrorCondition(bad_file_descriptor);
		case WSAECONNABORTED: return MakeErrorCondition(connection_aborted);
		case WSAECONNREFUSED: return MakeErrorCondition(connection_refused);
		case WSAECONNRESET: return MakeErrorCondition(connection_reset);
		case WSAEDESTADDRREQ: return MakeErrorCondition(destination_address_required);
		case WSAEFAULT: return MakeErrorCondition(bad_address);
		case WSAEHOSTUNREACH: return MakeErrorCondition(host_unreachable);
		case WSAEINPROGRESS: return MakeErrorCondition(operation_in_progress);
		case WSAEINTR: return MakeErrorCondition(interrupted);
		case WSAEINVAL: return MakeErrorCondition(invalid_argument);
		case WSAEISCONN: return MakeErrorCondition(already_connected);
		case WSAEMFILE: return MakeErrorCondition(too_many_files_open);
		case WSAEMSGSIZE: return MakeErrorCondition(message_size);
		case WSAENAMETOOLONG: return MakeErrorCondition(filename_too_long);
		case WSAENETDOWN: return MakeErrorCondition(network_down);
		case WSAENETRESET: return MakeErrorCondition(network_reset);
		case WSAENETUNREACH: return MakeErrorCondition(network_unreachable);
		case WSAENOBUFS: return MakeErrorCondition(no_buffer_space);
		case WSAENOPROTOOPT: return MakeErrorCondition(no_protocol_option);
		case WSAENOTCONN: return MakeErrorCondition(not_connected);
		case WSAENOTSOCK: return MakeErrorCondition(not_a_socket);
		case WSAEOPNOTSUPP: return MakeErrorCondition(operation_not_supported);
		case WSAEPROTONOSUPPORT: return MakeErrorCondition(protocol_not_supported);
		case WSAEPROTOTYPE: return MakeErrorCondition(wrong_protocol_type);
		case WSAETIMEDOUT: return MakeErrorCondition(timed_out);
		case WSAEWOULDBLOCK: return MakeErrorCondition(operation_would_block);
		
		default: return ErrorCondition(errVal, SystemCategoryRef());
	}
}


}	// namespace base
}	// namespace cppdevtk
