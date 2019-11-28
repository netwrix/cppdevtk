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


#ifndef CPPDEVTK_BASE_ERROR_CODE_MAC_HPP_INCLUDED_
#define CPPDEVTK_BASE_ERROR_CODE_MAC_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif

#include "cerrno.hpp"


namespace cppdevtk {
namespace base {


namespace sys_err {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Mac OS X error codes.
/// \sa C++11, 19.5 System error support, enum class errc
/// \ingroup error_code
enum sys_err_t {
	success = ESUCCESS,
	host_down = EHOSTDOWN,
	no_block_device = ENOTBLK,
	not_recoverable = ENOTRECOVERABLE,
	owner_dead = EOWNERDEAD,
	protocol_no_supported = EPFNOSUPPORT,
	remote_object = EREMOTE,
	shutdown = ESHUTDOWN,
	socket_type_not_supported = ESOCKTNOSUPPORT,
	too_many_references = ETOOMANYREFS,
	too_many_users = EUSERS
};


}	// namespace sys_err


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_ERROR_CODE_MAC_HPP_INCLUDED_
