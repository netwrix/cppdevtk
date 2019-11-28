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


// NOTE: Do not use CPPDEVTK_ASSERT() because it use StackTrace; use Q_ASSERT() instead


#include <cppdevtk/base/stack_trace.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>

#include <exception>
#include <algorithm>


namespace cppdevtk {
namespace base {


bool StackTrace::Capture() CPPDEVTK_NOEXCEPT {
	Clear();
	
	if (GetMaxSize() == 0) {
		return true;
	}
	
	bool retValue = false;
	
	try {
		retValue = DoCapture();
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_WARN("absorbing std::exception: " << exc.what());
		SuppressUnusedWarning(exc);
		
		Clear();
	}
	catch (...) {
		CPPDEVTK_LOG_WARN("absorbing unknown exception");
		
		Clear();
	}
	
	Q_ASSERT(GetSize() <= GetMaxSize());
	Q_ASSERT(retValue || IsEmpty());
	
	return retValue;
}

StackFrame StackTrace::GetStackFrame(::std::size_t idx) const {
	CPPDEVTK_DBC_CHECK_IN_RANGE(idx < GetSize(), "argument idx is out of range");
	
	return stackFrames_[idx];
}

QString StackTrace::ToString() const {
	const StackFrames::size_type kSize = GetSize();
	const StackFrames::size_type kDesiredNumFramesToSkip =
#			if (CPPDEVTK_COMPILER_GCC)
			1
#			elif (CPPDEVTK_COMPILER_CLANG)
			1
#			elif (CPPDEVTK_COMPILER_MSVC)
			3
#			else
#			error "Unsupported compiler!!!"
#			endif
	;
	const StackFrames::size_type kActualNumFramesToSkip = ::std::min(kDesiredNumFramesToSkip, kSize);
	const StackFrames::size_type kMaxIdx = kSize - kActualNumFramesToSkip;
	
	QString stringized = QString("Stack Trace has %1 frame(s)").arg(kMaxIdx);
	
	if (kMaxIdx > 0) {
		stringized += ": [";
		for (StackFrames::size_type idx = 0; idx < kMaxIdx; ++idx) {
			stringized += QString("\n\tframe #%1: %2").arg(QString::number(idx),
					stackFrames_[idx + kActualNumFramesToSkip].ToString());
		}
		stringized += "\n]";
	}
	
	return stringized;
}


}	// namespace base
}	// namespace cppdevtk
