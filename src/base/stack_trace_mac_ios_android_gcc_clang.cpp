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
#if (!(CPPDEVTK_COMPILER_GCC || CPPDEVTK_COMPILER_CLANG))
#	error "This file is GCC/Clang specific!!!"
#endif
#if (!(CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID))
#	error "This file is Mac OS X / Android specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID && !CPPDEVTK_COMPILER_GCC)
#	error "Android requires GCC!!!"
#endif

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/demangle.hpp>

#include <QtCore/QStringList>

#include <cstddef>


namespace cppdevtk {
namespace base {


bool StackTrace::ParseSymbolicAddressRepresentation(/* const */ char* pSymAddrRep, StackFrame& stackFrame) {
	Q_ASSERT(pSymAddrRep != NULL);
	
	if (*pSymAddrRep == '\0') {
		CPPDEVTK_LOG_ERROR("pSymAddrRep is empty");
		return false;
	}
	
	//CPPDEVTK_LOG_DEBUG("pSymAddrRep: " << pSymAddrRep);
	
	// backtrace_symbols() format:
	// #define _BACKTRACE_FORMAT "%-4d%-35s 0x%08x %s + %u"
	// snprintf(buf, size, _BACKTRACE_FORMAT, frame, image, addr, symbol, (addr - info->dli_saddr));
	// if unknown image is "???"
	
	// backtrace_symbols() format examples:
	/*
	0   cppdevtk_test_base_debug            0x00008cb5 _ZN8cppdevtk4base10StackTrace9DoCaptureEv + 155
	3   cppdevtk_test_base_debug            0x00003e08 TestStackTraceCHelper + 280
	5   cppdevtk_test_base_debug            0x00004049 main + 115
	6   cppdevtk_test_base_debug            0x0000367e tart + 258
	8   ???                                 0x00000001 0x0 + 1
	9 AppKit       0x00007fff8708b226 -[NSApplication _nextEventMatchingEventMask:untilDate:inMode:dequeue:] + 454
	*/
		
	const QStringList kSymAddrRepParts = QString(pSymAddrRep).split(' ', QString::SkipEmptyParts);
	if ((kSymAddrRepParts.size() != 6) && (kSymAddrRepParts.size() != 7)) {
		//CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: not 6 or 7 components but " << kSymAddrRepParts.size());
		Q_ASSERT(0 && "backtrace_symbols() format changed: not 6 or 7 components");
		return false;
	}
	if (((kSymAddrRepParts.size() == 6) && (kSymAddrRepParts[4] != "+"))
			|| ((kSymAddrRepParts.size() == 7) && (kSymAddrRepParts[5] != "+"))) {
		//CPPDEVTK_LOG_ERROR("backtrace_symbols() format changed: component 4(5) is not + but " << kSymAddrRepParts[4]);
		Q_ASSERT(0 && "backtrace_symbols() format changed: component 4(5) is not +");
		return false;
	}
	
	QString moduleName = kSymAddrRepParts[1];
	if (moduleName == "???") {
		moduleName.clear();
	}
	
	const QString kStrReturnAddress = kSymAddrRepParts[2];
	::cppdevtk::base::dword64 returnAddress = 0;
	if (!kStrReturnAddress.isEmpty()) {
		bool returnAddressConvOk = false;
		returnAddress = kStrReturnAddress.toULongLong(&returnAddressConvOk, 16);
		if (!returnAddressConvOk) {
			CPPDEVTK_LOG_ERROR("kStrReturnAddress.toULongLong() failed; kStrReturnAddress: " << kStrReturnAddress);
			Q_ASSERT(0 && "kStrReturnAddress.toULongLong() failed");
			return false;
		}
	}
	
	QString functionName = kSymAddrRepParts[3];
	if (kSymAddrRepParts.size() == 7) {
		functionName += " " + kSymAddrRepParts[4];
	}
	QString demangledFunctionName = Demangle(functionName);
	if (demangledFunctionName.isEmpty()) {
		demangledFunctionName = functionName;
	}
	
	const QString kStrOffset =  (kSymAddrRepParts.size() == 6) ? kSymAddrRepParts[5] : kSymAddrRepParts[6];
	::cppdevtk::base::dword64 offset = 0;
	if (!kStrOffset.isEmpty()) {
		bool offsetConvOk = false;
		offset = kStrOffset.toULongLong(&offsetConvOk, 10);
		if (!offsetConvOk) {
			CPPDEVTK_LOG_ERROR("kStrOffset.toULongLong() failed; kStrOffset: " << kStrOffset);
			Q_ASSERT(0 && "kStrOffset.toULongLong() failed");
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
