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


#include <cppdevtk/util/set_errno_from_exception.hpp>
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>

#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/deadlock_exception.hpp>
#include <cppdevtk/base/task_canceled_exception.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/ios.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>
#if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
#include <system_error>
#endif


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API void SetErrNoFromException() {
	using ::cppdevtk::base::Exception;
	using ::cppdevtk::base::GenericCategoryRef;
	using ::cppdevtk::base::SystemCategoryRef;
#	if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
	using ::std::generic_category;
	using ::std::system_category;
#	endif
	
	
	// TODO: keep updated
	try {
		throw;
	}
	catch (const base::DeadlockException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EDEADLK; caught DeadlockException: "
				<< Exception::GetDetailedInfo(exc));
		errno = EDEADLK;
	}
	catch (const base::concurrent::TaskCanceledException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ECANCELED; caught TaskCanceledException: "
				<< Exception::GetDetailedInfo(exc));
		errno = ECANCELED;
	}
	catch (const util::NoSuchFileOrDirectoryException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ENOENT; caught NoSuchFileOrDirectoryException: "
				<< Exception::GetDetailedInfo(exc));
		errno = ENOENT;
	}
	
	catch (const base::IosFailureException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EIO; caught IosFailureException: "
				<< Exception::GetDetailedInfo(exc));
		errno = EIO;
	}
	catch (const base::SystemException& exc) {
		int errCodeErrNo = ESUCCESS;
		const base::ErrorCode& kErrorCode = exc.ErrorCodeRef();
		if ((kErrorCode.CategoryRef() == GenericCategoryRef())
#				if (CPPDEVTK_PLATFORM_UNIX)
				|| (kErrorCode.CategoryRef() == SystemCategoryRef())
#				endif
				) {
			errCodeErrNo = kErrorCode.GetValue();
		}
		else {
			const base::ErrorCondition kErrorCondition = kErrorCode.GetDefaultErrorCondition();
			if ((kErrorCondition.CategoryRef() == GenericCategoryRef())
#					if (CPPDEVTK_PLATFORM_UNIX)
					|| (kErrorCondition.CategoryRef() == SystemCategoryRef())
#					endif
					) {
				errCodeErrNo = kErrorCondition.GetValue();
			}
		}
		
		const int kErrNo = (errCodeErrNo != ESUCCESS) ? errCodeErrNo : ENODATA;
		CPPDEVTK_LOG_ERROR("setting errno to " << kErrNo << "; caught SystemException: "
				<< Exception::GetDetailedInfo(exc));
		errno = kErrNo;
	}
	catch (const base::RuntimeException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ENODATA; caught RuntimeException: "
				<< Exception::GetDetailedInfo(exc));
		errno = ENODATA;
	}
	catch (const base::InvalidArgumentException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EINVAL; caught InvalidArgumentException: "
				<< Exception::GetDetailedInfo(exc));
		errno = EINVAL;
	}
	catch (const base::LogicException& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EINVAL; caught LogicException: "
				<< Exception::GetDetailedInfo(exc));
		errno = EINVAL;
	}
	catch (const Exception& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ENODATA; caught Exception: "
				<< Exception::GetDetailedInfo(exc));
		errno = ENODATA;
	}
	
	catch (const ::std::ios_base::failure& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EIO; caught ios_base::failure: "
				<< Exception::GetDetailedInfo(exc));
		errno = EIO;
	}
#	if (CPPDEVTK_HAVE_CPP11_SYSTEM_ERROR)
	catch (const ::std::system_error& exc) {
		int errCodeErrNo = ESUCCESS;
		const ::std::error_code& kErrorCode = exc.code();
		if ((kErrorCode.category() == generic_category())
#				if (CPPDEVTK_PLATFORM_UNIX)
				|| (kErrorCode.category() == system_category())
#				endif
				) {
			errCodeErrNo = kErrorCode.value();
		}
		else {
			const ::std::error_condition kErrorCondition = kErrorCode.default_error_condition();
			if ((kErrorCondition.category() == generic_category())
#					if (CPPDEVTK_PLATFORM_UNIX)
					|| (kErrorCondition.category() == system_category())
#					endif
					) {
				errCodeErrNo = kErrorCondition.value();
			}
		}
		
		const int kErrNo = (errCodeErrNo != ESUCCESS) ? errCodeErrNo : ENODATA;
		CPPDEVTK_LOG_ERROR("setting errno to " << kErrNo << "; caught system_error: "
				<< Exception::GetDetailedInfo(exc));
		errno = kErrNo;
	}
#	endif
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ENODATA; caught runtime_error: "
				<< Exception::GetDetailedInfo(exc));
		errno = ENODATA;
	}
	catch (const ::std::invalid_argument& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EINVAL; caught invalid_argument: "
				<< Exception::GetDetailedInfo(exc));
		errno = EINVAL;
	}
	catch (const ::std::logic_error& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to EINVAL; caught logic_error: "
				<< Exception::GetDetailedInfo(exc));
		errno = EINVAL;
	}
	catch (const ::std::bad_alloc& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ENOMEM; caught bad_alloc: "
				<< Exception::GetDetailedInfo(exc));
		errno = ENOMEM;
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_ERROR("setting errno to ENODATA; caught exception: "
				<< Exception::GetDetailedInfo(exc));
		errno = ENODATA;
	}
	
	catch (...) {
		CPPDEVTK_LOG_ERROR("setting errno to ENODATA; caught unknown exception");
		errno = ENODATA;
	}
}


}	// namespace util
}	// namespace cppdevtk
