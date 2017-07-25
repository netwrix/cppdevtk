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
#	error "Do not include directlyv; please include <cppdevtk/base/system_exception.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_ERROR_CODE_LNX_HPP_INCLUDED_
#define CPPDEVTK_BASE_ERROR_CODE_LNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include "cerrno.hpp"


namespace cppdevtk {
namespace base {


namespace sys_err {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Linux error codes.
/// \note Taken from boost 1.60.0, boost\system\linux_error.hpp, enum linux_errno
/// \sa C++11, 19.5 System error support, enum class errc
/// \ingroup error_code
enum sys_err_t {
	success = ESUCCESS,
	advertise_error = EADV,
	bad_exchange = EBADE,
	bad_file_number = EBADFD,
	bad_font_format = EBFONT,
	bad_request_code = EBADRQC,
	bad_request_descriptor = EBADR,
	bad_slot = EBADSLT,
	channel_range = ECHRNG,
	communication_error = ECOMM,
	dot_dot_error = EDOTDOT,
	exchange_full = EXFULL,
	host_down = EHOSTDOWN,
	is_named_file_type = EISNAM,
	key_expired = EKEYEXPIRED,
	key_rejected = EKEYREJECTED,
	key_revoked = EKEYREVOKED,
	level2_halt = EL2HLT,
	level2_no_syncronized = EL2NSYNC,
	level3_halt = EL3HLT,
	level3_reset = EL3RST,
	link_range = ELNRNG,
	medium_type = EMEDIUMTYPE,
	no_anode = ENOANO,
	no_block_device = ENOTBLK,
	no_csi = ENOCSI,
	no_key = ENOKEY,
	no_medium = ENOMEDIUM,
	no_network = ENONET,
	no_package = ENOPKG,
	not_avail = ENAVAIL,
	not_named_file_type = ENOTNAM,
	not_recoverable = ENOTRECOVERABLE,
	not_unique = ENOTUNIQ,
	owner_dead = EOWNERDEAD,
	protocol_no_supported = EPFNOSUPPORT,
	remote_address_changed = EREMCHG,
	remote_io_error = EREMOTEIO,
	remote_object = EREMOTE,
	restart_needed = ERESTART,
	shared_library_access = ELIBACC,
	shared_library_bad = ELIBBAD,
	shared_library_execute = ELIBEXEC,
	shared_library_max = ELIBMAX,
	shared_library_section = ELIBSCN,
	shutdown = ESHUTDOWN,
	socket_type_not_supported = ESOCKTNOSUPPORT,
	srmount_error = ESRMNT,
	stream_pipe_error = ESTRPIPE,
	too_many_references = ETOOMANYREFS,
	too_many_users = EUSERS,
	unattached = EUNATCH,
	unclean = EUCLEAN
};


}	// namespace sys_err


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_ERROR_CODE_LNX_HPP_INCLUDED_
