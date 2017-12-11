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


// NOTE: Do not use CPPDEVTK_ASSERT() because it use StackTrace; use assert() instead


#include <cppdevtk/base/execinfo_unx.h>
#if (!CPPDEVTK_COMPILER_GCC)
#	error "This file is gcc specific!!!"
#endif
#if (!CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is Android specific!!!"
#endif


#if (!CPPDEVTK_HAVE_BACKTRACE)

#include <cppdevtk/base/unused.h>
#include <cppdevtk/base/assert.h>
#include <cppdevtk/base/stdint.h>

#include <stddef.h>

#include <unwind.h>


typedef struct BacktraceState {
	void** buf;
	int bufSize;
	int idx;
} BacktraceState;


// Callback for _Unwind_Backtrace(). The backtrace stops immediately if the callback returns any value
// other than _URC_NO_REASON.
static _Unwind_Reason_Code UnwindTraceFn(struct _Unwind_Context* pUnwindContext, void* pUnwindTraceArg);


CPPDEVTK_BASE_API int backtrace(void** buffer, int size) {
	assert(size >= 0);
	if (size == 0) {
		return 0;
	}
	assert(buffer != NULL);
	
	BacktraceState backtraceState = {buffer, size, 0};
	// _Unwind_Backtrace() returns _URC_END_OF_STACK when the backtrace stopped due to reaching the end of the call-chain
	// or _URC_FATAL_PHASE1_ERROR if it stops for any other reason.
	// We ignore ret code and return what we got.
	_Unwind_Backtrace(UnwindTraceFn, &backtraceState);
	return backtraceState.idx;
}

_Unwind_Reason_Code UnwindTraceFn(struct _Unwind_Context* pUnwindContext, void* pUnwindTraceArg) {
	BacktraceState* pBacktraceState = NULL;
	
	assert(pUnwindContext != NULL);
	assert(pUnwindTraceArg != NULL);
	
	pBacktraceState = (BacktraceState*)pUnwindTraceArg;
	if (pBacktraceState->idx == pBacktraceState->bufSize) {
		return _URC_END_OF_STACK;
	}
	assert(pBacktraceState->idx < pBacktraceState->bufSize);
	
	void* pIP = (void*)_Unwind_GetIP(pUnwindContext);
	if (pIP == NULL) {
		return _URC_END_OF_STACK;
	}
	
	pBacktraceState->buf[pBacktraceState->idx++] = pIP;
	
	return _URC_NO_REASON;
}

#endif	// (!CPPDEVTK_HAVE_BACKTRACE)
