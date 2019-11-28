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


#ifndef CPPDEVTK_BASE_ERROR_CODE_WIN_HPP_INCLUDED_
#define CPPDEVTK_BASE_ERROR_CODE_WIN_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <windows.h>


namespace cppdevtk {
namespace base {


namespace sys_err {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Windows error codes.
/// \note Taken from boost 1.60.0, boost\system\windows_error.hpp, enum windows_error_code
/// \sa C++11, 19.5 System error support, enum class errc
/// \ingroup error_code
enum sys_err_t {
	success = ERROR_SUCCESS,
	invalid_function = ERROR_INVALID_FUNCTION,
	file_not_found = ERROR_FILE_NOT_FOUND,
	path_not_found = ERROR_PATH_NOT_FOUND,
	too_many_open_files = ERROR_TOO_MANY_OPEN_FILES,
	access_denied = ERROR_ACCESS_DENIED,
	invalid_handle = ERROR_INVALID_HANDLE,
	arena_trashed = ERROR_ARENA_TRASHED,
	not_enough_memory = ERROR_NOT_ENOUGH_MEMORY,
	invalid_block = ERROR_INVALID_BLOCK,
	bad_environment = ERROR_BAD_ENVIRONMENT,
	bad_format = ERROR_BAD_FORMAT,
	invalid_access = ERROR_INVALID_ACCESS,
	outofmemory = ERROR_OUTOFMEMORY,
	invalid_drive = ERROR_INVALID_DRIVE,
	current_directory = ERROR_CURRENT_DIRECTORY,
	not_same_device = ERROR_NOT_SAME_DEVICE,
	no_more_files = ERROR_NO_MORE_FILES,
	write_protect = ERROR_WRITE_PROTECT,
	bad_unit = ERROR_BAD_UNIT,
	not_ready = ERROR_NOT_READY,
	bad_command = ERROR_BAD_COMMAND,
	crc = ERROR_CRC,
	bad_length = ERROR_BAD_LENGTH,
	seek = ERROR_SEEK,
	not_dos_disk = ERROR_NOT_DOS_DISK,
	sector_not_found = ERROR_SECTOR_NOT_FOUND,
	out_of_paper = ERROR_OUT_OF_PAPER,
	write_fault = ERROR_WRITE_FAULT,
	read_fault = ERROR_READ_FAULT,
	gen_failure = ERROR_GEN_FAILURE,
	sharing_violation = ERROR_SHARING_VIOLATION,
	lock_violation = ERROR_LOCK_VIOLATION,
	wrong_disk = ERROR_WRONG_DISK,
	sharing_buffer_exceeded = ERROR_SHARING_BUFFER_EXCEEDED,
	handle_eof = ERROR_HANDLE_EOF,
	handle_disk_full = ERROR_HANDLE_DISK_FULL,
	rem_not_list = ERROR_REM_NOT_LIST,
	dup_name = ERROR_DUP_NAME,
	bad_net_path = ERROR_BAD_NETPATH,
	network_busy = ERROR_NETWORK_BUSY,
	file_exists = ERROR_FILE_EXISTS,
	cannot_make = ERROR_CANNOT_MAKE,
	broken_pipe = ERROR_BROKEN_PIPE,
	open_failed = ERROR_OPEN_FAILED,
	buffer_overflow = ERROR_BUFFER_OVERFLOW,
	disk_full = ERROR_DISK_FULL,
	lock_failed = ERROR_LOCK_FAILED,
	busy = ERROR_BUSY,
	cancel_violation = ERROR_CANCEL_VIOLATION,
	already_exists = ERROR_ALREADY_EXISTS
};


}	// namespace sys_err


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_ERROR_CODE_WIN_HPP_INCLUDED_
