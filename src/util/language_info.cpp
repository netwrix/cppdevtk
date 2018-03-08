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


#include <cppdevtk/util/language_info.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/stdexcept.hpp>


namespace cppdevtk {
namespace util {


LanguageInfo::LanguageInfo(const QString& name, const QString& nativeName) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(name.isEmpty(), "name");
	CPPDEVTK_DBC_CHECK_ARGUMENT(((name.length() == 2) || ((name.length() == 5) && (name[2] == '_'))), "name: invalid format");
	
	Set(QLocale(name), nativeName);
}

void LanguageInfo::Set(const QString& name, const QString& nativeName) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(name.isEmpty(), "name");
	CPPDEVTK_DBC_CHECK_ARGUMENT(((name.length() == 2) || ((name.length() == 5) && (name[2] == '_'))), "name: invalid format");
	
	Set(QLocale(name), nativeName);
}

void LanguageInfo::Set(const QLocale& locale, const QString& nativeName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(((locale.name() != QLocale::c().name()) || (nativeName == QLocale::c().name())),
			"nativeName: incorrect for C locale");
	CPPDEVTK_DBC_CHECK_ARGUMENT(((locale.name() != GetCodeName())
			|| (nativeName == GetCodeNativeName())), "nativeName: incorrect for en_US locale");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(nativeName.isEmpty(), "nativeName");
	
	// make sure that Qt not changed
	CPPDEVTK_ASSERT(QLocale("").name() == QLocale::c().name());
	CPPDEVTK_ASSERT(QLocale("en").name() == GetCodeName());
	CPPDEVTK_ASSERT(QLocale("en_US").name() == GetCodeName());
	
	// NOTE: do not global qualify because moc will generate bad code
	if (QMetaType::type("cppdevtk::util::LanguageInfo")
#			if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
			== QMetaType::UnknownType) {
#			else
			== 0) {
#			endif
		if (qRegisterMetaType< ::cppdevtk::util::LanguageInfo>("cppdevtk::util::LanguageInfo")
#				if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
				== QMetaType::UnknownType) {
#				else
				== 0) {
#				endif
			throw CPPDEVTK_RUNTIME_EXCEPTION("failed to register metatype cppdevtk::util::LanguageInfo");
		}
	}
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	if (!QMetaType::hasRegisteredComparators< ::cppdevtk::util::LanguageInfo>()) {
		if (!QMetaType::registerComparators< ::cppdevtk::util::LanguageInfo>()) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("failed to register comparators for metatype cppdevtk::util::LanguageInfo");
		}
	}
#	endif
	
	name_ = locale.name();
	nativeName_ = nativeName;
}

bool LanguageInfo::operator==(const LanguageInfo& other) const CPPDEVTK_NOEXCEPT {
	CPPDEVTK_ASSERT(((name_ == other.name_) && (nativeName_ == other.nativeName_))
			|| ((name_ != other.name_) && (nativeName_ != other.nativeName_)));
	
	return name_ == other.name_;
}

QString LanguageInfo::ToString() const {
	return QString("name: %1; nativeName: %2").arg(name_, nativeName_);
}


}	// namespace util
}	// namespace cppdevtk
