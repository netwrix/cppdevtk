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


#ifndef CPPDEVTK_BASE_STACK_FRAME_HPP_INCLUDED_
#define CPPDEVTK_BASE_STACK_FRAME_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif

#include "stringizable.hpp"
#include "architecture_types.hpp"
#include "string_utils.hpp"


namespace cppdevtk {
namespace base {


class CPPDEVTK_BASE_API StackFrame: public QStringizable {
public:
	StackFrame();
	StackFrame(const char* moduleName, char* functionName,
			::cppdevtk::base::dword64 offset, ::cppdevtk::base::dword64 returnAddress);
	StackFrame(const QString& moduleName, const QString& functionName,
			::cppdevtk::base::dword64 offset, ::cppdevtk::base::dword64 returnAddress);
	
	bool IsEmpty() const;
	void Clear();
	
	void SetModuleName(const char* moduleName);
	void SetModuleName(const QString& moduleName);
	QString GetModuleName() const;
	
	void SetFunctionName(const char* functionName);
	void SetFunctionName(const QString& functionName);
	QString GetFunctionName() const;
	
	void SetOffset(::cppdevtk::base::dword64 offset);
	::cppdevtk::base::dword64 GetOffset() const;
	
	void SetReturnAddress(::cppdevtk::base::dword64 returnAddress);
	::cppdevtk::base::dword64 GetReturnAddress() const;
	
	virtual QString ToString() const;
private:
	QString moduleName_;
	QString functionName_;
	::cppdevtk::base::dword64 offset_;
	::cppdevtk::base::dword64 returnAddress_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline StackFrame::StackFrame(): QStringizable(),
		moduleName_(), functionName_(), offset_(0), returnAddress_(0) {}

inline StackFrame::StackFrame(const QString& moduleName, const QString& functionName,
		::cppdevtk::base::dword64 offset, ::cppdevtk::base::dword64 returnAddress): QStringizable(),
		moduleName_(moduleName), functionName_(functionName), offset_(offset), returnAddress_(returnAddress) {}

inline bool StackFrame::IsEmpty() const {
	return (moduleName_.isEmpty() && functionName_.isEmpty() && (offset_ == 0) && (returnAddress_ == 0));
}

inline void StackFrame::Clear() {
	moduleName_.clear();
	functionName_.clear();
	offset_ = 0;
	returnAddress_ = 0;
}

inline void StackFrame::SetModuleName(const QString& moduleName) {
	moduleName_ = moduleName;
}

inline QString StackFrame::GetModuleName() const {
	return moduleName_;
}

inline void StackFrame::SetFunctionName(const QString& functionName) {
	functionName_ = functionName;
}

inline QString StackFrame::GetFunctionName() const {
	return functionName_;
}

inline void StackFrame::SetOffset(::cppdevtk::base::dword64 offset) {
	offset_ = offset;
}

inline ::cppdevtk::base::dword64 StackFrame::GetOffset() const {
	return offset_;
}

inline void StackFrame::SetReturnAddress(::cppdevtk::base::dword64 returnAddress) {
	returnAddress_ = returnAddress;
}

inline ::cppdevtk::base::dword64 StackFrame::GetReturnAddress() const {
	return returnAddress_;
}

inline QString StackFrame::ToString() const {
	return QString("(%1 + %2) [%3] %4").arg((functionName_.isEmpty() ? "unknown_function" : functionName_),
			NumToHexStr(offset_), NumToHexStr(returnAddress_), (moduleName_.isEmpty() ? "unknown_module" : moduleName_));
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_STACK_FRAME_HPP_INCLUDED_
