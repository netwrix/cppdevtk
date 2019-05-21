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


#include <cppdevtk/util/throw_exception_from_errno.hpp>
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>

#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/task_canceled_exception.hpp>
#include <cppdevtk/base/ios.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QString>


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API void ThrowExceptionFromErrNo(int errNo) {
	const base::ErrorCode kErrorCode = base::MakeSystemErrorCode(errNo);
	const QString kErrMsg = QString("exception from errno: ") + kErrorCode.ToString();
	
	switch (errNo) {
		case EDEADLK:
			CPPDEVTK_LOG_ERROR("errNo EDEADLK; throwing DeadlockException");
			throw CPPDEVTK_DEADLOCK_EXCEPTION_WA(kErrMsg);
		case ECANCELED:
			CPPDEVTK_LOG_ERROR("errNo ECANCELED; throwing TaskCanceledException");
			throw CPPDEVTK_TASK_CANCELED_EXCEPTION_W_WA(kErrMsg);
		case ENOENT:
			CPPDEVTK_LOG_ERROR("errNo ENOENT; throwing NoSuchFileOrDirectoryException");
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION();
		case EIO:
			CPPDEVTK_LOG_ERROR("errNo EIO; throwing IosFailureException");
			throw CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA(kErrMsg);
		case EINVAL:
			CPPDEVTK_LOG_ERROR("errNo EINVAL; throwing InvalidArgumentException");
			throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION(kErrMsg);
		case ENOMEM:
			CPPDEVTK_LOG_ERROR("errNo ENOMEM; throwing bad_alloc");
			throw ::std::bad_alloc();
		case ENODATA:
			CPPDEVTK_LOG_ERROR("errNo ENODATA; throwing RuntimeException");
			throw CPPDEVTK_RUNTIME_EXCEPTION(kErrMsg);
		default:
			CPPDEVTK_LOG_ERROR("errNo other; throwing SystemException");
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(kErrorCode, "system exception from errno");
	}
}


}	// namespace util
}	// namespace cppdevtk
