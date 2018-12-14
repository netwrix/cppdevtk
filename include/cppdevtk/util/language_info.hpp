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


#ifndef CPPDEVTK_UTIL_LANGUAGE_INFO_HPP_INCLUDED_
#define CPPDEVTK_UTIL_LANGUAGE_INFO_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif

#include <cppdevtk/base/stringizable.hpp>

#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QMetaType>


namespace cppdevtk {
namespace util {


class CPPDEVTK_UTIL_API LanguageInfo: public ::cppdevtk::base::QStringizable {
public:
	LanguageInfo();	///< Code language info
	LanguageInfo(const QString& name, const QString& nativeName);
	LanguageInfo(QLocale::Language language, QLocale::Country country, const QString& nativeName);
	LanguageInfo(const QLocale& locale, const QString& nativeName);
	
#	if (CPPDEVTK_COMPILER_CLANG)
	// Latest XCode 9.3.1 Clang triggers lots of [-Wdelete-non-virtual-dtor] that are painful to get rid of...
	// So we provide virtual dtor (also class could have been marked CPPDEVTK_FINAL)
	virtual ~LanguageInfo() {}
#	endif
	
	QString GetName() const;	///< QLocale::name()
	QString GetNativeName() const;
	
	void Set();
	void Set(const QString& name, const QString& nativeName);
	void Set(QLocale::Language language, QLocale::Country country, const QString& nativeName);
	void Set(const QLocale& locale, const QString& nativeName);
	
	bool operator==(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT;
	bool operator!=(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT;
	bool operator<(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT;
	bool operator<=(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT;
	bool operator>(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT;
	bool operator>=(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT;
	
	virtual QString ToString() const;
	
	
	static QString GetCodeName();
	static QString GetCodeNativeName();
	static LanguageInfo GetCodeLanguageInfo();
	static LanguageInfo GetCLanguageInfo();
private:
	static QLocale GetCodeLocale();
	
	
	QString name_;
	QString nativeName_;
};


}	// namespace util
}	// namespace cppdevtk


// NOTE: do not global qualify because moc will generate bad code
Q_DECLARE_METATYPE(cppdevtk::util::LanguageInfo)


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline LanguageInfo::LanguageInfo() {
	Set(GetCodeLocale(), GetCodeNativeName());
}

inline LanguageInfo::LanguageInfo(QLocale::Language language, QLocale::Country country, const QString& nativeName) {
	Set(QLocale(language, country), nativeName);
}

inline LanguageInfo::LanguageInfo(const QLocale& locale, const QString& nativeName) {
	Set(locale, nativeName);
}

inline void LanguageInfo::Set() {
	Set(GetCodeLocale(), GetCodeNativeName());
}

inline void LanguageInfo::Set(QLocale::Language language, QLocale::Country country, const QString& nativeName) {
	Set(QLocale(language, country), nativeName);
}

inline QString LanguageInfo::GetName() const {
	return name_;
}

inline QString LanguageInfo::GetNativeName() const {
	return nativeName_;
}

inline bool LanguageInfo::operator<(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT {
	return nativeName_ < other.nativeName_;
}

inline bool LanguageInfo::operator!=(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT {
	return !(*this == other);
}

inline bool LanguageInfo::operator<=(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT {
	return !(other < *this);
}

inline bool LanguageInfo::operator>(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT {
	return other < *this;
}

inline bool LanguageInfo::operator>=(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT {
	return !(*this < other);
}

inline QString LanguageInfo::GetCodeName() {
	return GetCodeLocale().name();
}

inline QString LanguageInfo::GetCodeNativeName() {
	return "English";
}

inline LanguageInfo LanguageInfo::GetCodeLanguageInfo() {
	return LanguageInfo(GetCodeLocale(), GetCodeNativeName());
}

inline LanguageInfo LanguageInfo::GetCLanguageInfo() {
	return LanguageInfo(QLocale::c(), QLocale::c().name());
}

inline QLocale LanguageInfo::GetCodeLocale() {
	return QLocale(QLocale::English, QLocale::UnitedStates);
}


}	// namespace util
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_UTIL_LANGUAGE_INFO_HPP_INCLUDED_
