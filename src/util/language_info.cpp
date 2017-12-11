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


namespace cppdevtk {
namespace util {


LanguageInfo::LanguageInfo(const QLocale& locale, const QString& nativeName):
		locale_(locale), nativeName_(nativeName) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(nativeName.isEmpty(), nativeName);
}

void LanguageInfo::Set(const QLocale& locale, const QString& nativeName) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(nativeName.isEmpty(), nativeName);
	
	locale_ = locale;
	nativeName_ = nativeName;
}


}	// namespace util
}	// namespace cppdevtk
