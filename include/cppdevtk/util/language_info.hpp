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


#ifndef CPPDEVTK_UTIL_LANGUAGE_INFO_HPP_INCLUDED_
#define CPPDEVTK_UTIL_LANGUAGE_INFO_HPP_INCLUDED_


#include "config.hpp"

#include <QtCore/QLocale>
#include <QtCore/QString>


namespace cppdevtk {
namespace util {


class CPPDEVTK_UTIL_API LanguageInfo {
public:
	LanguageInfo();	// C language info
	LanguageInfo(const QLocale& locale, const QString& nativeName);	///< \pre !nativeName.isEmpty()
	
	QLocale GetLocale() const;
	QString GetNativeName() const;
	
	void Set(const QLocale& locale, const QString& nativeName);	///< \pre !nativeName.isEmpty()
private:
	QLocale locale_;
	QString nativeName_;
};


CPPDEVTK_UTIL_API bool operator==(const LanguageInfo& lhs, const LanguageInfo& rhs);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline LanguageInfo::LanguageInfo(): locale_(QLocale::c()), nativeName_(locale_.name()) {}

inline QLocale LanguageInfo::GetLocale() const {
	return locale_;
}

inline QString LanguageInfo::GetNativeName() const {
	return nativeName_;
}

inline CPPDEVTK_UTIL_API bool operator==(const LanguageInfo& lhs, const LanguageInfo& rhs) {
	return lhs.GetLocale() == rhs.GetLocale();
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_LANGUAGE_INFO_HPP_INCLUDED_
