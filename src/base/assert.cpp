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


#include <cppdevtk/base/assert.h>
#include <cppdevtk/base/stack_trace.hpp>
#include <cppdevtk/base/string_conv.hpp>

#include <QtCore/QtGlobal>

#include <cstddef>


CPPDEVTK_BASE_API void CppDevTkAssert(const char* assertion, const char* file, int line, const char* function) {
	Q_ASSERT(assertion != NULL);
	Q_ASSERT(file != NULL);
	Q_ASSERT(function != NULL);
	
	try {
		qFatal("Assertion '%s' failed!!!\nfile: %s\nline: %d\nfunction: %s\nstack trace: %s", assertion, file, line, function,
				::cppdevtk::base::Q2A(::cppdevtk::base::StackTrace(true, 200).ToString()).c_str());
	}
	catch (...) {
		qFatal("Assertion '%s' failed!!!\nfile: %s\nline: %d\nfunction: %s", assertion, file, line, function);
	}
}
