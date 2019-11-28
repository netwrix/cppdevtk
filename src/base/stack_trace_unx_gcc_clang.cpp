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
#if (!(CPPDEVTK_COMPILER_GCC || CPPDEVTK_COMPILER_CLANG))
#	error "This file is gcc/clang specific!!!"
#endif
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/execinfo_unx.h>

#include <cstddef>
#include <cstdlib>
#include <vector>
#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {


bool StackTrace::DoCapture() {
	typedef void* StackFrameReturnAddress;
	typedef ::std::vector<StackFrameReturnAddress> StackFrameReturnAddresses;
	
	
	Q_ASSERT(IsEmpty());
	Q_ASSERT(GetMaxSize() > 0);
	
	StackFrameReturnAddresses stackFrameReturnAddresses(GetMaxSize());
	const int kNumStackFrameReturnAddresses = backtrace(&stackFrameReturnAddresses[0], stackFrameReturnAddresses.size());
	if (kNumStackFrameReturnAddresses <= 0) {
		CPPDEVTK_LOG_ERROR("backtrace() failed");
		return false;
	}
	if ((StackFrameReturnAddresses::size_type)kNumStackFrameReturnAddresses == stackFrameReturnAddresses.size()) {
		CPPDEVTK_LOG_INFO("backtrace may have been truncated");
	}
	stackFrameReturnAddresses.resize(kNumStackFrameReturnAddresses);
	
	CPPDEVTK_TR1_NS::shared_ptr<char*> pSymbolicAddressesRepresentation(backtrace_symbols(
			&stackFrameReturnAddresses[0], kNumStackFrameReturnAddresses), &::std::free);
	if (pSymbolicAddressesRepresentation.get() == NULL) {
		CPPDEVTK_LOG_ERROR("backtrace_symbols() failed");
		return false;
	}
	
	for (StackFrames::size_type idx = 0; idx < (StackFrameReturnAddresses::size_type)kNumStackFrameReturnAddresses; ++idx) {
		/* const */ char* pSymbolicAddressRepresentation = (pSymbolicAddressesRepresentation.get())[idx];
		Q_ASSERT(pSymbolicAddressRepresentation != NULL);
		StackFrame stackFrame;
		if (ParseSymbolicAddressRepresentation(pSymbolicAddressRepresentation, stackFrame)) {
			Q_ASSERT(!stackFrame.IsEmpty());
			stackFrames_.push_back(stackFrame);
		}
		else {
			// pSymbolicAddressRepresentation may be altered at this point by ParseSymbolicAddressRepresentation()
			// but for speed purposes we do not make a copy...
			CPPDEVTK_LOG_ERROR("ParseSymbolicAddressRepresentation() failed for: " << pSymbolicAddressRepresentation);
		}
	}
	
	Q_ASSERT(GetSize() <= GetMaxSize());
	
	return !stackFrames_.empty();
}


}	// namespace base
}	// namespace cppdevtk
