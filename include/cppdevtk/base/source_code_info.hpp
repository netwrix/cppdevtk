/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_BASE_SOURCE_CODE_INFO_HPP_INCLUDED_
#define CPPDEVTK_BASE_SOURCE_CODE_INFO_HPP_INCLUDED_


#include "config.hpp"
#include "stringizable.hpp"


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif


namespace cppdevtk {
namespace base {


#define CPPDEVTK_SOURCE_CODE_INFO()	\
		::cppdevtk::base::SourceCodeInfo(__FILE__, __LINE__, CPPDEVTK_FUNCTION_LONG_NAME)

#define CPPDEVTK_MAKE_SOURCE_CODE_INFO(name)	\
		::cppdevtk::base::SourceCodeInfo name(__FILE__, __LINE__, CPPDEVTK_FUNCTION_LONG_NAME)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides information about source code: file, line and function.
/// \note Compiler generated copy ctor and copy assignment have nothrow guarantee (important when used in exception classes)
class CPPDEVTK_BASE_API SourceCodeInfo: public QStringizable {
public:
	SourceCodeInfo();
	SourceCodeInfo(const char* file, int line, const char* function);
	SourceCodeInfo(const QString& file, int line, const QString& function);
	
	QString GetFile() const;
	void SetFile(const char* file);
	void SetFile(const QString& file);
	
	int GetLine() const;
	void SetLine(int line);
	
	QString GetFunction() const;
	void SetFunction(const char* function);
	void SetFunction(const QString& function);
	
	virtual QString ToString() const;
private:
	QString file_;
	int line_;
	QString function_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline SourceCodeInfo::SourceCodeInfo(): QStringizable(), file_(), line_(-1), function_() {}

inline SourceCodeInfo::SourceCodeInfo(const QString& file, int line, const QString& function): QStringizable(),
		file_(file), line_(line), function_(function) {}

inline QString SourceCodeInfo::GetFile() const {
	return file_;
}

inline void SourceCodeInfo::SetFile(const QString& file) {
	file_ = file;
}

inline int SourceCodeInfo::GetLine() const {
	return line_;
}

inline void SourceCodeInfo::SetLine(int line) {
	line_ = line;
}

inline QString SourceCodeInfo::GetFunction() const {
	return function_;
}

inline void SourceCodeInfo::SetFunction(const QString& function) {
	function_ = function;
}

inline QString SourceCodeInfo::ToString() const {
	return QString("file: %1; line: %2; function: %3").arg((file_.isEmpty() ? "unknown_file" : file_),
			QString::number(line_), (function_.isEmpty() ? "unknown_function" : function_));
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_SOURCE_CODE_INFO_HPP_INCLUDED_
