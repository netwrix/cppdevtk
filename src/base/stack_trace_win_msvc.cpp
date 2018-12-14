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


// NOTE: Do not use CPPDEVTK_ASSERT() because it use StackTrace; use Q_ASSERT() instead


#include <cppdevtk/base/stack_trace.hpp>
#if (!CPPDEVTK_COMPILER_MSVC)
#	error "This file is msvc specific!!!"
#endif
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include "stack_walker_wrapper_win_msvc.hpp"
#include <cppdevtk/base/thread_shared_obj_locking_ptr.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>


namespace cppdevtk {
namespace base {


bool StackTrace::DoCapture() {
	using detail::StackWalkerWrapper;
	
	Q_ASSERT(IsEmpty());
	Q_ASSERT(GetMaxSize() > 0);
	
	volatile StackWalkerWrapper& theStackWalkerWrapper = StackWalkerWrapper::GetInstance();
	ThreadSharedObjLockingPtr<StackWalkerWrapper, StackWalkerWrapper> pStackWalkerWrapper(
			theStackWalkerWrapper, const_cast<StackWalkerWrapper&>(theStackWalkerWrapper));
	pStackWalkerWrapper->SetMaxSize(GetMaxSize());
	if (!pStackWalkerWrapper->CaptureCallStack()) {
		CPPDEVTK_LOG_ERROR("CaptureCallStack() failed");
		return false;
	}
	
	stackFrames_ = pStackWalkerWrapper->GetStackFrames();
	
	Q_ASSERT(GetSize() <= GetMaxSize());
	
	return true;
}


}	// namespace base
}	// namespace cppdevtk
