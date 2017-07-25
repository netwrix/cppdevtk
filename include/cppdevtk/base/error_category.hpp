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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_SYSTEM_EXCEPTION_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/system_exception.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_ERROR_CATEGORY_HPP_INCLUDED_
#define CPPDEVTK_BASE_ERROR_CATEGORY_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "stringizable.hpp"
#include "error_condition.hpp"
#include "error_code.hpp"

#include <QtCore/QString>
#include <QtCore/QLocale>

#include CPPDEVTK_TR1_HEADER(functional)


#undef GetMessage


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup error_category ErrorCategory
/// \ingroup system_exception
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa C++11, 19.5.1 Class error_category
class CPPDEVTK_BASE_API ErrorCategory: private NonCopyable, public QStringizable {
public:
	ErrorCategory();	// needed for default initialization of an object of const type
	virtual ~ErrorCategory();
	
	virtual QString GetName() const = 0;
	virtual QString GetMessage(int errVal, const QLocale& locale = QLocale::c()) const = 0;
	
	virtual ErrorCondition GetDefaultErrorCondition(int errVal) const;
	
	virtual bool IsEquivalent(int code, const ErrorCondition& condition) const;
	virtual bool IsEquivalent(const ErrorCode& code, int condition) const;
	
	bool operator==(const ErrorCategory& other) const;
	bool operator!=(const ErrorCategory& other) const;
	bool operator<(const ErrorCategory& other) const;
	
	virtual QString ToString() const;	///< Same as GetName()
};


CPPDEVTK_BASE_API const ErrorCategory& GetGenericCategory();
CPPDEVTK_BASE_API const ErrorCategory& GetSystemCategory();


class CPPDEVTK_BASE_API GenericErrorCategory: public ErrorCategory {
public:
	GenericErrorCategory();	// needed for default initialization of an object of const type
	virtual QString GetName() const;
	virtual QString GetMessage(int errVal, const QLocale& locale = QLocale::c()) const;
};


class CPPDEVTK_BASE_API SystemErrorCategory: public ErrorCategory {
public:
	SystemErrorCategory();	// needed for default initialization of an object of const type
	virtual QString GetName() const;
	virtual QString GetMessage(int errVal, const QLocale& locale = QLocale::c()) const;
	
	// NOTE: C++11 std:
	// If the argument ev corresponds to a POSIX errno value posv, the function shall return
	// error_condition(posv, generic_category()).
	// Otherwise, the function shall return error_condition(ev, system_category()).
	virtual ErrorCondition GetDefaultErrorCondition(int errVal) const;
};


/// @}	// error_category
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline ErrorCategory::ErrorCategory(): NonCopyable(), QStringizable() {}

inline ErrorCategory::~ErrorCategory() {}

inline ErrorCondition ErrorCategory::GetDefaultErrorCondition(int errVal) const {
	return ErrorCondition(errVal, *this);
}

inline bool ErrorCategory::IsEquivalent(int code, const ErrorCondition& condition) const {
	return GetDefaultErrorCondition(code) == condition;
}

inline bool ErrorCategory::IsEquivalent(const ErrorCode& code, int condition) const {
	return (*this == code.GetCategory()) && (code.GetValue() == condition);
}

inline bool ErrorCategory::operator==(const ErrorCategory& other) const {
	return this == &other;
}

inline bool ErrorCategory::operator!=(const ErrorCategory& other) const {
	return this != &other;
}

inline bool ErrorCategory::operator<(const ErrorCategory& other) const {
	return ::std::less<const ErrorCategory*>()(this, &other);
}

inline QString ErrorCategory::ToString() const {
	return GetName();
}


inline GenericErrorCategory::GenericErrorCategory(): ErrorCategory() {}

inline QString GenericErrorCategory::GetName() const {
	return QString("generic");
}


inline SystemErrorCategory::SystemErrorCategory(): ErrorCategory() {}

inline QString SystemErrorCategory::GetName() const {
	return QString("system");
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_ERROR_CATEGORY_HPP_INCLUDED_
