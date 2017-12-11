/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
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


#include <cppdevtk/util/dynamic_library.hpp>
#include <cppdevtk/util/dynamic_loader.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>

#include <cstddef>


namespace cppdevtk {
namespace util {


DynamicLibrary::DynamicLibrary(const QString& dynLibName): NonCopyable(), handle_(DynamicLoader::Load(dynLibName)),
		autoUnload_(true) {}

DynamicLibrary::~DynamicLibrary() {
	CPPDEVTK_ASSERT(handle_ != 0);
	
	if (autoUnload_) {
		try {
			DynamicLoader::Unload(handle_);
		}
		catch (const ::std::exception& exc) {
			CPPDEVTK_LOG_WARN("unload failed: " << base::Exception::GetDetailedInfo(exc));
			CPPDEVTK_ASSERT(0 && "unload failed");
			base::SuppressUnusedWarning(exc);
		}
	}
}


}	// namespace util
}	// namespace cppdevtk
