/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#include "thread_data.hpp"
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <windows.h>

#include <cstddef>


namespace cppdevtk {
namespace base {
namespace detail {


ThreadData::~ThreadData() {
	if (nativeHandle_ != NULL) {
		// NOTE: Closing a thread handle does not terminate the associated thread or remove the thread object.
		if (!CloseHandle((HANDLE)nativeHandle_)) {
			CPPDEVTK_LOG_WARN("failed to close thread handle; error code: " << GetLastSystemErrorCode().ToString());
			CPPDEVTK_ASSERT(0 && "failed to close thread handle");
		}
	}
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
