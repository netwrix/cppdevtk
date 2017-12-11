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
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/util/dynamic_loader_exception.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>

#include <cstddef>


namespace cppdevtk {
namespace util {


DynamicLibrary::SymbolAddress DynamicLibrary::LoadSymbol(const QString& symbolName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(symbolName), "symbolName must be valid");
	
	SymbolAddress symbolAddress = ::GetProcAddress(handle_, base::Q2A(symbolName).c_str());
	if (symbolAddress == NULL) {
		throw CPPDEVTK_DYNAMIC_LOADER_EXCEPTION("failed to load symbol " + symbolName);
	}
	
	return symbolAddress;
}


}	// namespace util
}	// namespace cppdevtk
