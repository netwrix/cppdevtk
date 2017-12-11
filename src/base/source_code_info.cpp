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


#include <cppdevtk/base/source_code_info.hpp>
#include <cppdevtk/base/dbc.hpp>


namespace cppdevtk {
namespace base {


SourceCodeInfo::SourceCodeInfo(const char* file, int line, const char* function): QStringizable(),
		file_(), line_(line), function_() {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(file);
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(function);
	
	file_ = file;
	function_ = function;
}

void SourceCodeInfo::SetFile(const char* file) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(file);
	
	file_ = file;
}

void SourceCodeInfo::SetFunction(const char* function) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(function);
	
	function_ = function;
}


}	// namespace base
}	// namespace cppdevtk
