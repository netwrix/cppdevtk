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


#include <cppdevtk/util/exception_to_errno.hpp>

#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/task_canceled_exception.hpp>
#include <cppdevtk/base/dbc_exceptions.hpp>
#include <cppdevtk/base/ios.hpp>
#include <cppdevtk/util/filesystem_exception.hpp>
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>

#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>

#include <new>


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API void ExceptionToErrno() {
	using namespace ::cppdevtk::base;
	
	
	// TODO: keep updated
	try {
		throw;
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_ERROR("caught exception (converting to errno): " << Exception::GetDetailedInfo(exc));
		SuppressUnusedWarning(exc);
		
		try {
			throw;
		}
		catch (const concurrent::TaskCanceledException&) {
			errno = ECANCELED;
		}
		catch (const NoSuchFileOrDirectoryException&) {
			errno = ENOENT;
		}
		catch (const IosFailureException&) {
			errno = EIO;
		}
		catch (const RuntimeException&) {
			errno = ENODATA;
		}
		catch (const InvalidArgumentException&) {
			errno = EINVAL;
		}
		catch (const LogicException&) {
			errno = ENODATA;
		}
		catch (const Exception&) {
			errno = ENODATA;
		}
		
		catch (const ::std::ios_base::failure&) {
			errno = EIO;
		}
		catch (const ::std::runtime_error&) {
			errno = ENODATA;
		}
		catch (const ::std::invalid_argument&) {
			errno = EINVAL;
		}
		catch (const ::std::logic_error&) {
			errno = ENODATA;
		}
		catch (const ::std::bad_alloc&) {
			errno = ENOMEM;
		}
		catch (const ::std::exception&) {
			errno = ENODATA;
		}
	}
	catch (...) {
		CPPDEVTK_LOG_ERROR("caught unknown exception (converting to errno)");
		errno = ENODATA;
	}
}


}	// namespace util
}	// namespace cppdevtk
