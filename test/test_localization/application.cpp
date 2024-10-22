/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#include "application.hpp"

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/util/language_info.hpp>

#include <QtCore/QMetaType>


namespace cppdevtk {
namespace test_localization {


Application::Application(int& argc, char** argv):
#		if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
		::cppdevtk::gui::SingleApplication(argc, argv),
#		else
		::cppdevtk::gui::Application(argc, argv),
#		endif
		translator_() {
	SetStyleSheetFromFileCross(":/cppdevtk/test_localization/res/qss", "application");
	
	translate("language_native_name", "English");	// to generate translation
	SetPreferSystemLocale(true);
	SetCurrentLanguageInfo(GetDefaultLanguageInfo());
#	ifndef NDEBUG
	qMetaTypeId< ::cppdevtk::util::LanguageInfo>();	// compile time
#	endif
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("cppdevtk::util::LanguageInfo") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("cppdevtk::util::LanguageInfo") != 0);
#	endif
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	CPPDEVTK_ASSERT(QMetaType::hasRegisteredComparators< ::cppdevtk::util::LanguageInfo>());
#	endif
}

bool Application::SetupTranslators() {
	if (!translator_.isEmpty()) {
		removeTranslator(&translator_);
	}
	
#	if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
	bool retCode = SingleApplication::SetupTranslators();
#	else
	bool retCode = Application::SetupTranslators();
#	endif
	
	const QString kCurrentLanguageInfoName = GetCurrentLanguageInfo().GetName();
	if (kCurrentLanguageInfoName == ::cppdevtk::util::LanguageInfo::GetCodeName()) {
		return retCode;
	}
	
	QString qmPath;
	QString qmNamePrefix;
	GetQmInfo(qmPath, qmNamePrefix);
	CPPDEVTK_ASSERT(!qmPath.isEmpty());
	CPPDEVTK_ASSERT(!qmNamePrefix.isEmpty());
	
	if (translator_.load((qmNamePrefix + kCurrentLanguageInfoName), qmPath)) {
		installTranslator(&translator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load translator_");
		retCode = false;
	}
	
	return retCode;
}


}	// namespace test_localization
}	// namespace cppdevtk
