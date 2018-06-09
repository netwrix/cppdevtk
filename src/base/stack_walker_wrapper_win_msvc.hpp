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


#ifndef CPPDEVTK_BASE_STACK_WALKER_WRAPPER_MSVC_WIN_HPP_INCLUDED_
#define CPPDEVTK_BASE_STACK_WALKER_WRAPPER_MSVC_WIN_HPP_INCLUDED_


#include <cppdevtk/base/config.hpp>
#if (!CPPDEVTK_COMPILER_MSVC)
#	error "This file is msvc specific!!!"
#endif
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <cppdevtk/base/singletons.hpp>
#include <cppdevtk/base/stack_frame.hpp>
#include <cppdevtk/base/lockables.hpp>
#include "stack_walker_win_msvc.hpp"

#include <vector>
#include <cstddef>


namespace cppdevtk {
namespace base {
namespace detail {


// NOTE: Helper class to adapt StackWalk to our needs and improve performance as documented (please see quote below):
//
// Reusing the StackWalk instance:
// It is no problem to reuse the StackWalk instance, as long as you want to do the stack-walking for the same process.
// If you want to do a lot of stack-walking it is recommended to reuse the instance.
// The reason is simple: if you create a new instance, then the symbol-files must be re-loaded for each instance.
// And this is really time-consuming.
// Also it is not allowed to access the StackWalk functions from different threads (the dbghelp.dll is not thread-safe!).
// Therefore it makes no sense to create more than one instance...
class StackWalkerWrapper: public ::cppdevtk::base::Singleton<StackWalkerWrapper>,
		public ObjectLevelBasicLockable, protected StackWalker {
	friend class ::cppdevtk::base::Singleton<StackWalkerWrapper>;
public:
	bool CaptureCallStack();
	
	::std::vector<StackFrame> GetStackFrames() const;
	
	void SetMaxSize(::std::size_t maxSize);	// default is 60
	::std::size_t GetMaxSize() const;
protected:
	virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	
	virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType,
			LPCSTR pdbName, ULONGLONG fileVersion);
	
	virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry);
	
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
	
	virtual void OnOutput(LPCSTR szText);
private:
	StackWalkerWrapper();
	~StackWalkerWrapper();
	
	
	::std::size_t maxSize_;
	::std::vector<StackFrame> stackFrames_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline ::std::vector<StackFrame> StackWalkerWrapper::GetStackFrames() const {
	return stackFrames_;
}

inline void StackWalkerWrapper::SetMaxSize(::std::size_t maxSize) {
	maxSize_ = maxSize;
}

inline ::std::size_t StackWalkerWrapper::GetMaxSize() const {
	return maxSize_;
}

inline StackWalkerWrapper::~StackWalkerWrapper() {}


}	// namespace detail


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_STACK_WALKER_WRAPPER_MSVC_WIN_HPP_INCLUDED_
