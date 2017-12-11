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


// NOTE: Do not use CPPDEVTK_ASSERT() because it use StackTrace; use Q_ASSERT() instead


#include <cppdevtk/base/stack_frame.hpp>
#include <cppdevtk/base/dbc.hpp>


namespace cppdevtk {
namespace base {


StackFrame::StackFrame(const char* moduleName, char* functionName,
		::cppdevtk::base::dword64 offset, ::cppdevtk::base::dword64 returnAddress): QStringizable(),
		moduleName_(), functionName_(), offset_(offset), returnAddress_(returnAddress) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(moduleName);
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(functionName);
	
	moduleName_ = moduleName;
	functionName_ = functionName;
}

void StackFrame::SetModuleName(const char* moduleName) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(moduleName);
	
	moduleName_ = moduleName;
}

void StackFrame::SetFunctionName(const char* functionName) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(functionName);
	
	functionName_ = functionName;
}


}	// namespace base
}	// namespace cppdevtk
