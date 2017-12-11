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


#include "stack_walker_wrapper_win_msvc.hpp"

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>


namespace cppdevtk {
namespace base {
namespace detail {


// NOTE: not inlined so we can skip stack frames in StackTrace::ToString()
bool StackWalkerWrapper::CaptureCallStack() {
	stackFrames_.clear();
	
	if (maxSize_ == 0) {
		return true;
	}
	
	return ShowCallstack();
}

void StackWalkerWrapper::OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName) {
	Q_ASSERT(szSearchPath != NULL);
	Q_ASSERT(szUserName != NULL);
	
	CPPDEVTK_LOG_TRACE("symSearchPath: " << szSearchPath << "; symOptions: " << symOptions << "; userName: " << szUserName);
	
	SuppressUnusedWarning(szSearchPath);
	SuppressUnusedWarning(symOptions);
	SuppressUnusedWarning(szUserName);
}

void StackWalkerWrapper::OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType,
		LPCSTR pdbName, ULONGLONG fileVersion) {
	Q_ASSERT(img != NULL);
	Q_ASSERT(mod != NULL);
	Q_ASSERT(symType != NULL);
	Q_ASSERT(pdbName != NULL);
	
	CPPDEVTK_LOG_TRACE(img << ':' << mod << " (" << (LPVOID)baseAddr << ") size: " << size << " (result: " << result
			<< "), SymType: " << symType << ", PDB: " << pdbName);
	if (fileVersion != 0) {
		DWORD v4 = (DWORD) (fileVersion & 0xFFFF);
		DWORD v3 = (DWORD) ((fileVersion>>16) & 0xFFFF);
		DWORD v2 = (DWORD) ((fileVersion>>32) & 0xFFFF);
		DWORD v1 = (DWORD) ((fileVersion>>48) & 0xFFFF);
		
		CPPDEVTK_LOG_TRACE("fileVersion: " << v1 << '.' << v2 << '.' << v3 << '.' << v4);
		
		SuppressUnusedWarning(v4);
		SuppressUnusedWarning(v3);
		SuppressUnusedWarning(v2);
		SuppressUnusedWarning(v1);
	}
	
	SuppressUnusedWarning(pdbName);
	SuppressUnusedWarning(symType);
	SuppressUnusedWarning(result);
	SuppressUnusedWarning(size);
	SuppressUnusedWarning(baseAddr);
	SuppressUnusedWarning(mod);
	SuppressUnusedWarning(img);
}

void StackWalkerWrapper::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) {
	if (eType == lastEntry) {
		return;
	}
	
	if (stackFrames_.size() >= maxSize_) {
		return;
	}
	
	StackFrame stackFrame;
	
	if (entry.moduleName[0] != '\0') {
		stackFrame.SetModuleName(entry.moduleName);
	}
	
	if (entry.undFullName[0] != '\0') {
		stackFrame.SetFunctionName(entry.undFullName);
	}
	else {
		if (entry.undName[0] != '\0') {
			stackFrame.SetFunctionName(entry.undName);
		}
		else {
			if (entry.name[0] != '\0') {
				stackFrame.SetFunctionName(entry.name);
			}
		}
	}
	
	stackFrame.SetOffset(entry.offsetFromSmybol);
	
	stackFrame.SetReturnAddress(entry.offset);
	
	if (!stackFrame.IsEmpty()) {
		stackFrames_.push_back(stackFrame);
	}
	else {
		CPPDEVTK_LOG_ERROR("all stack frame components are empty");
	}
}

void StackWalkerWrapper::OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) {
	Q_ASSERT(szFuncName != NULL);
	
	// makes log unreadable and is not so important, so trace instead of error...
	CPPDEVTK_LOG_TRACE(szFuncName << " failed; errorCode: " << gle << "; address: " << (LPVOID)addr);
	
	SuppressUnusedWarning(szFuncName);
	SuppressUnusedWarning(gle);
	SuppressUnusedWarning(addr);
}

void StackWalkerWrapper::OnOutput(LPCSTR buffer) {
	SuppressUnusedWarning(buffer);
}

StackWalkerWrapper::StackWalkerWrapper(): Singleton<StackWalkerWrapper>(), ObjectLevelBasicLockable(),
		StackWalker(RetrieveSymbol | RetrieveModuleInfo | SymBuildPath
#		if (CPPDEVTK_STACK_TRACE_ENABLE_MS_SYM_SRV)
		| SymUseSymSrv
#		endif
		), maxSize_(60), stackFrames_() {}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk
