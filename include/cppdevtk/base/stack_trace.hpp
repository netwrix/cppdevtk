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


#ifndef CPPDEVTK_BASE_STACK_TRACE_HPP_INCLUDED_
#define CPPDEVTK_BASE_STACK_TRACE_HPP_INCLUDED_


#include "config.hpp"
#include "stringizable.hpp"
#include "stack_frame.hpp"

#include <vector>
#include <cstddef>
#include <algorithm>	// swap(), C++98
#include <utility>	// swap(), C++11


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif


#if (CPPDEVTK_ENABLE_TMPL_EXPL_INST)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API ::std::allocator< ::cppdevtk::base::StackFrame>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API ::std::vector< ::cppdevtk::base::StackFrame>;
#endif


namespace cppdevtk {
namespace base {


/// \attention
/// - On Linux, it is necessary to use the -rdynamic linker option.
/// - On Windows need to link with version.lib and advapi32.lib libraries
/// - compiler generated copy ctor may throw (important when used in exception classes)
class CPPDEVTK_BASE_API StackTrace: public QStringizable {
public:
	typedef ::std::vector<StackFrame> StackFrames;
	
	
	/// \param autoCapture If it is \c true, calls Capture().
	/// \param maxSize maximum number of stack frames to Capture().
	/// \sa SetMaxSize()
	explicit StackTrace(bool autoCapture = true, ::std::size_t maxSize = 200);
	
	StackTrace& operator=(const StackTrace& other);	///< \throw whatever copy ctor throws
	
	void Swap(StackTrace& other) CPPDEVTK_NOEXCEPT;
	
	/// \note
	/// - glibc docs says that: "even complicated programs rather seldom have a nesting level of more than, say, 50
	/// and with 200 possible entries probably all programs should be covered."
	/// - On Windows XP CaptureStackBackTrace() has a limit of 62.
	/// \sa StackTrace()
	void SetMaxSize(::std::size_t maxSize);
	::std::size_t GetMaxSize() const;
	
	bool Capture() CPPDEVTK_NOEXCEPT;
	bool operator()();	///< Functor style (same as Capture())
	
	::std::size_t GetSize() const;
	bool IsEmpty() const;
	
	void Clear();
	
	StackFrame GetStackFrame(::std::size_t idx) const;
	StackFrames GetStackFrames() const;
	
	virtual QString ToString() const;
private:
	bool DoCapture();
	
#	if (CPPDEVTK_PLATFORM_UNIX)
#	if (CPPDEVTK_COMPILER_GCC || CPPDEVTK_COMPILER_CLANG)
	
	static bool ParseSymbolicAddressRepresentation(/* const */ char* pSymAddrRep, StackFrame& stackFrame);
	
#	endif
#	endif
	
	::std::size_t maxSize_;
	StackFrames stackFrames_;
};


CPPDEVTK_BASE_API void swap(StackTrace& x, StackTrace& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline StackTrace::StackTrace(bool autoCapture, ::std::size_t maxSize): QStringizable(), maxSize_(maxSize), stackFrames_() {
	if (autoCapture) {
		Capture();
	}
}

inline StackTrace& StackTrace::operator=(const StackTrace& other) {
	StackTrace tmp(other);
	Swap(tmp);
	return *this;
}

inline void StackTrace::Swap(StackTrace& other) CPPDEVTK_NOEXCEPT {
	using ::std::swap;
	
	if (this != &other) {
		swap(maxSize_, other.maxSize_);
		stackFrames_.swap(other.stackFrames_);
	}
}

inline ::std::size_t StackTrace::GetMaxSize() const {
	return maxSize_;
}

inline void StackTrace::SetMaxSize(::std::size_t maxSize) {
	maxSize_ = maxSize;
}

inline bool StackTrace::operator()() {
	return Capture();
}

inline ::std::size_t StackTrace::GetSize() const {
	return stackFrames_.size();
}

inline bool StackTrace::IsEmpty() const {
	return stackFrames_.empty();
}

inline void StackTrace::Clear() {
	stackFrames_.clear();
}

inline StackTrace::StackFrames StackTrace::GetStackFrames() const {
	return stackFrames_;
}


inline CPPDEVTK_BASE_API void swap(StackTrace& x, StackTrace& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_STACK_TRACE_HPP_INCLUDED_
