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
#if (!CPPDEVTK_COMPILER_GCC)
#	error "This file is gcc specific!!!"
#endif
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/name_mangling.hpp>

#include <cstddef>


namespace cppdevtk {
namespace base {


bool StackTrace::ParseSymbolicAddressRepresentation(/* const */ char* pSymAddrRep, StackFrame& stackFrame) {
	Q_ASSERT(pSymAddrRep != NULL);
	
	if (*pSymAddrRep == '\0') {
		CPPDEVTK_LOG_ERROR("pSymAddrRep is empty");
		return false;
	}
	
	//CPPDEVTK_LOG_TRACE("pSymAddrRep: " << pSymAddrRep);
	
	// backtrace_symbols() format examples:
	/*
	./cppdevtk/build/qt5/linux-g++/shared/debug/libcppdevtk_base_qt5.so.0(_ZN8cppdevtk4base10StackTrace7CaptureEv+0x78) [0x7fb5aee1a4a8]
	./cppdevtk_test_base(_ZN8cppdevtk4base10StackTraceC2Ebj+0x80) [0x8053e68]
	./cppdevtk_test_base(main+0x85) [0x80527b9]
	/lib/libc.so.6(__libc_start_main+0xf3) [0xb738d003]
	./cppdevtk_test_base() [0x80526a9]
	./cppdevtk_test_base(TestStackTraceCHelper+0x42) [0x8052adf]
	./cppdevtk_test_base(_Z14TestStackTracev+0x36)
	*/
	
	char* pOpenRoundParanthesis = NULL;
	char* pPlusSign = NULL;
	char* pCloseRoundParanthesis = NULL;
	char* pOpenSquareParanthesis = NULL;
	char* pCloseSquareParanthesis = NULL;
	
	for (char* pCurrent = pSymAddrRep; *pCurrent != '\0'; ++pCurrent) {
		switch (*pCurrent) {
			case '(':
				if (pOpenRoundParanthesis != NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: multiple (");
					Q_ASSERT(0 && "backtrace_symbols() format changed: multiple (");
					return false;
				}
				if ((pPlusSign != NULL) || (pCloseRoundParanthesis != NULL) || (pOpenSquareParanthesis != NULL)
						|| (pCloseSquareParanthesis != NULL)) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: ( is not before + ) [ ]");
					Q_ASSERT(0 && "backtrace_symbols() format changed: ( is not before + ) [ ]");
					return false;
				}
				
				pOpenRoundParanthesis = pCurrent;
				*pOpenRoundParanthesis = '\0';
				break;
			case '+':
				if (pOpenRoundParanthesis == NULL) {
#					if 0
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: + before (");
					Q_ASSERT(0 && "backtrace_symbols() format changed: + before (");
					return false;
#					else
					break;
#					endif
				}
				if (pPlusSign != NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: multiple +");
					Q_ASSERT(0 && "backtrace_symbols() format changed: multiple +");
					return false;
				}
				if ((pCloseRoundParanthesis != NULL) || (pOpenSquareParanthesis != NULL)
						|| (pCloseSquareParanthesis != NULL)) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: + is not before ) [ ]");
					Q_ASSERT(0 && "backtrace_symbols() format changed: + is not before ) [ ]");
					return false;
				}
				
				pPlusSign = pCurrent;
				*pPlusSign = '\0';
				break;
			case ')':
				if (pCloseRoundParanthesis != NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: multiple )");
					Q_ASSERT(0 && "backtrace_symbols() format changed: multiple )");
					return false;
				}
				if (pOpenRoundParanthesis == NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: ) before (");
					Q_ASSERT(0 && "backtrace_symbols() format changed: ) before (");
					return false;
				}
				if ((pOpenSquareParanthesis != NULL) || (pCloseSquareParanthesis != NULL)) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: ) is not before [ ]");
					Q_ASSERT(0 && "backtrace_symbols() format changed: ) is not before [ ]");
					return false;
				}
				
				pCloseRoundParanthesis = pCurrent;
				*pCloseRoundParanthesis = '\0';
				break;
			case '[':
				if (pOpenSquareParanthesis != NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: multiple [");
					Q_ASSERT(0 && "backtrace_symbols() format changed: multiple [");
					return false;
				}
				if (pCloseSquareParanthesis != NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: ] before [");
					Q_ASSERT(0 && "backtrace_symbols() format changed: ] before [");
					return false;
				}
				
				pOpenSquareParanthesis = pCurrent;
				*pOpenSquareParanthesis = '\0';
				break;
			case ']':
				if (pCloseSquareParanthesis != NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: multiple ]");
					Q_ASSERT(0 && "backtrace_symbols() format changed: multiple ]");
					return false;
				}
				if (pOpenSquareParanthesis == NULL) {
					CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: ] before [");
					Q_ASSERT(0 && "backtrace_symbols() format changed: ] before [");
					return false;
				}
				
				pCloseSquareParanthesis = pCurrent;
				*pCloseSquareParanthesis = '\0';
				break;
			default:
				break;
		}
	}
	
	if (pOpenRoundParanthesis != NULL) {
		if (pCloseRoundParanthesis == NULL) {
			CPPDEVTK_LOG_ERROR("missing closing )");
			Q_ASSERT(0 && "missing closing )");
			return false;
		}
	}
	if (pOpenSquareParanthesis != NULL) {
		if (pCloseSquareParanthesis == NULL) {
			CPPDEVTK_LOG_ERROR("missing closing ]");
			Q_ASSERT(0 && "missing closing ]");
			return false;
		}
	}
	
	QString moduleName;
	if (*pSymAddrRep != '\0') {
		moduleName = pSymAddrRep;
		moduleName = moduleName.trimmed();
	}
	
	QString demangledFunctionName;
	if (pOpenRoundParanthesis != NULL) {
		Q_ASSERT(pCloseRoundParanthesis != NULL);
		QString functionName = pOpenRoundParanthesis + 1;
		functionName = functionName.trimmed();
		if (!functionName.isEmpty()) {
			if (IsMangled(functionName)) {
				demangledFunctionName = Demangle(functionName);
			}
			if (demangledFunctionName.isEmpty()) {
				demangledFunctionName = functionName;
			}
		}
	}
	
	QString strOffset;
	if (pPlusSign != NULL) {
		Q_ASSERT(pCloseRoundParanthesis != NULL);
		strOffset = pPlusSign + 1;
		strOffset = strOffset.trimmed();
	}
	::cppdevtk::base::dword64 offset = 0;
	if (!strOffset.isEmpty()) {
		bool offsetConvOk = false;
		offset = strOffset.toULongLong(&offsetConvOk, 16);
		if (!offsetConvOk) {
			CPPDEVTK_LOG_ERROR("strOffset.toULongLong() failed; strOffset: " << strOffset);
			Q_ASSERT(0 && "strOffset.toULongLong() failed");
			return false;
		}
	}
	
	QString strReturnAddress;
	if (pOpenSquareParanthesis != NULL) {
		Q_ASSERT(pCloseSquareParanthesis != NULL);
		strReturnAddress = pOpenSquareParanthesis + 1;
		strReturnAddress = strReturnAddress.trimmed();
	}
	::cppdevtk::base::dword64 returnAddress = 0;
	if (!strReturnAddress.isEmpty()) {
		bool returnAddressConvOk = false;
		returnAddress = strReturnAddress.toULongLong(&returnAddressConvOk, 16);
		if (!returnAddressConvOk) {
			CPPDEVTK_LOG_ERROR("strReturnAddress.toULongLong() failed; strReturnAddress: " << strReturnAddress);
			Q_ASSERT(0 && "strReturnAddress.toULongLong() failed");
			return false;
		}
	}
	
	if (moduleName.isEmpty() && demangledFunctionName.isEmpty() && (offset == 0) && (returnAddress == 0)) {
		CPPDEVTK_LOG_ERROR("all stack frame parsed components are empty");
		return false;
	}
	
	stackFrame.SetModuleName(moduleName);
	stackFrame.SetFunctionName(demangledFunctionName);
	stackFrame.SetOffset(offset);
	stackFrame.SetReturnAddress(returnAddress);
	
	Q_ASSERT(!stackFrame.IsEmpty());
	
	return true;
}


}	// namespace base
}	// namespace cppdevtk
