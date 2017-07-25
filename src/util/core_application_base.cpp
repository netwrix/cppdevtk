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


#include <cppdevtk/util/core_application_base.hpp>
#include <cppdevtk/util/core_application.hpp>
#include <cppdevtk/util/single_core_application.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>


using ::cppdevtk::base::Exception;
using ::std::exception;


namespace cppdevtk {
namespace util {


QString CoreApplicationBase::qmPath_;
QString CoreApplicationBase::qmNamePrefix_;
QTranslator CoreApplicationBase::helperTranslator_;


CoreApplicationBase::CoreApplicationBase(): NonCopyable(), notifyThrowAction_(ntaRethrow),
		isSystemLocalePreferred_(true), currentLanguageInfo_(QLocale(QLocale::English, QLocale::UnitedStates), "English"),
		qtTranslator_(), baseTranslator_(), utilTranslator_() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

CoreApplicationBase::~CoreApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

LanguageInfo CoreApplicationBase::GetDefaultLanguageInfo() const {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(!qmPath_.isEmpty(), "qmPath is empty; call SetQmInfo() first");
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(!qmNamePrefix_.isEmpty(), "qmNamePrefix_ is empty; call SetQmInfo() first");
	
	LanguageInfo defaultLanguageInfo(QLocale(QLocale::English, QLocale::UnitedStates), "English");
	
	if (isSystemLocalePreferred_) {
		const QList<LanguageInfo> kSupportedLanguageInfos = GetSupportedLanguageInfos();
		const QLocale kSystemLocale = QLocale::system();
		for (QList<LanguageInfo>::ConstIterator kIter = kSupportedLanguageInfos.constBegin();
				kIter != kSupportedLanguageInfos.constEnd(); ++kIter) {
			if (kIter->GetLocale() == kSystemLocale) {
				CPPDEVTK_LOG_INFO("system locale " << kSystemLocale.name() << " is supported");
				defaultLanguageInfo = *kIter;
				break;
			}
		}
	}
	
	return defaultLanguageInfo;
}

void CoreApplicationBase::SetCurrentLanguageInfo(const LanguageInfo& languageInfo) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((QCoreApplication::instance() != NULL),
			"create app instance first");
	CPPDEVTK_DBC_CHECK_ARGUMENT((GetSupportedLanguageInfos().contains(languageInfo)),
			"languageInfo is not supported");
	
	currentLanguageInfo_ = languageInfo;
	
	SetupTranslators();
}

void CoreApplicationBase::SetQmInfo(const QString& qmPath, const QString& qmNamePrefix) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(util::IsValidPath(qmPath), "qmPath is not valid");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(qmNamePrefix.isEmpty(), "qmNamePrefix");
	
	qmPath_ = qmPath;
	qmNamePrefix_ = qmNamePrefix;
}

QList<LanguageInfo> CoreApplicationBase::GetSupportedLanguageInfos() {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(!qmPath_.isEmpty(), "qmPath is empty; call SetQmInfo() first");
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(!qmNamePrefix_.isEmpty(), "qmNamePrefix_ is empty; call SetQmInfo() first");
	
	const QLocale kLocaleEnUs(QLocale::English, QLocale::UnitedStates);
	
	QList<LanguageInfo> languageInfos;
	languageInfos.append(LanguageInfo(kLocaleEnUs, "English"));
	
	const QList<QLocale> kSupportedLocales = GetSupportedLocales();
	for (QList<QLocale>::ConstIterator kIter = kSupportedLocales.constBegin(); kIter != kSupportedLocales.constEnd();
			++kIter) {
		if (*kIter == kLocaleEnUs) {	// there is no tr_en_US.qm
			continue;
		}
		
		const QString kQmPath(":/cppdevtk/util/res/tr");
		const QString kQmFileName = QString("tr_") + kIter->name();
		if (!helperTranslator_.load(kQmFileName, kQmPath)) {
			CPPDEVTK_LOG_ERROR("failed to load translator for: kQmFileName: " << kQmFileName << "; kQmPath: " << kQmPath);
			//CPPDEVTK_ASSERT(0 && "failed to load translator");
			
			//CPPDEVTK_ASSERT(!languageInfos.contains(LanguageInfo(*kIter, "English")));
			//languageInfos.append(LanguageInfo(*kIter, "English"));
			
			continue;
		}
		const QString kNativeName = helperTranslator_.translate("language_native_name", "English");
		if (kNativeName.isEmpty()) {
			CPPDEVTK_LOG_ERROR("failed to translate kNativeName for: kQmFileName: " << kQmFileName
					<< "; kQmPath: " << kQmPath);
			//CPPDEVTK_ASSERT(0 && "failed to translate kNativeName");
			
			//CPPDEVTK_ASSERT(!languageInfos.contains(LanguageInfo(*kIter, "English")));
			//languageInfos.append(LanguageInfo(*kIter, "English"));
			
			continue;
		}
		if (kNativeName == "English") {
			CPPDEVTK_LOG_ERROR("kNativeName is not properly translated for: kQmFileName: " << kQmFileName
					<< "; kQmPath: " << kQmPath);
			CPPDEVTK_ASSERT(0 && "kNativeName is not properly translated");
			
			//CPPDEVTK_ASSERT(!languageInfos.contains(LanguageInfo(*kIter, "English")));
			//languageInfos.append(LanguageInfo(*kIter, "English"));
			
			continue;
		}
		
		CPPDEVTK_ASSERT(!languageInfos.contains(LanguageInfo(*kIter, kNativeName)));
		languageInfos.append(LanguageInfo(*kIter, kNativeName));
	}
	
	return languageInfos;
}

QString CoreApplicationBase::GetAppDirPath() {
	QString appDirPath;
	
	QDir appDir(QCoreApplication::applicationDirPath());	// inside bundle on Mac OS X
#	if (CPPDEVTK_PLATFORM_MACOSX)
	if (!(appDir.cdUp() && appDir.cdUp() && appDir.cdUp())) {	// out of bundle
		CPPDEVTK_LOG_ERROR("failed to get out of bundle");
	}
	else {
#	endif
	appDirPath = appDir.path();
#	if (CPPDEVTK_PLATFORM_UNIX)
	appDirPath = QFileInfo(appDirPath).canonicalFilePath();	// dereference symlinks
#	endif
	if (!appDirPath.isEmpty()) {
		if (!appDirPath.endsWith('/')) {
			appDirPath += '/';
		}
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to get appDirPath");
	}
#	if (CPPDEVTK_PLATFORM_MACOSX)
	}
#	endif
	
	return appDirPath;
}

bool CoreApplicationBase::notify(QObject* pReceiver, QEvent* pEvent) {
	bool retCode = false;
	
	QCoreApplication* pQCoreApplication = dynamic_cast<QCoreApplication*>(this);
	if (pQCoreApplication != NULL) {
		try {
			retCode = pQCoreApplication->QCoreApplication::notify(pReceiver, pEvent);
		}
		catch (const ::std::exception& exc) {
			retCode = NotifyThrowHandler(&exc);
		}
		catch (...) {
			retCode = NotifyThrowHandler();
		}
	}
	
	return retCode;
}

bool CoreApplicationBase::NotifyThrowHandler(const ::std::exception* pExc) {
#	if (CPPDEVTK_ENABLE_LOG)
	QString errMsg = "exception: ";
	if (pExc != NULL) {
		errMsg += Exception::GetDetailedInfo(*pExc);
	}
	else {
		errMsg += "unknown exception";
	}
	CPPDEVTK_LOG_ERROR(errMsg);
#	endif	// (CPPDEVTK_ENABLE_LOG)
	
	if (notifyThrowAction_ == ntaRethrow) {
		CPPDEVTK_LOG_ERROR("rethrowing...");
		throw;
	}
	
	if (notifyThrowAction_ == ntaQuit) {
		if (pExc != NULL) {
			base::qcerr << tr("Fatal Error: An %1 exception was thrown from an event handler!!!\nMessage: %2\nDetails: %3").arg(
					"::std::exception", pExc->what(), Exception::GetDetailedInfo(*pExc));
		}
		else {
			base::qcerr << tr("Fatal Error: An unknown exception was thrown from an event handler!!!") << endl;
		}
		
		CPPDEVTK_LOG_FATAL("quitting...");
		QCoreApplication::quit();
	}
	else {
		CPPDEVTK_LOG_WARN("ignoring (return false)...");
	}
	
	return false;
}

bool CoreApplicationBase::SetupTranslators() {
	bool retCode = true;
	
	QCoreApplication::removeTranslator(&utilTranslator_);
	QCoreApplication::removeTranslator(&baseTranslator_);
	QCoreApplication::removeTranslator(&qtTranslator_);
	
	if (GetCurrentLanguageInfo().GetLocale() == QLocale(QLocale::English, QLocale::UnitedStates)) {
		return retCode;
	}
	
	if (qtTranslator_.load((QString("qt_") + currentLanguageInfo_.GetLocale().name()), ":/cppdevtk/util/res/tr")) {
		QCoreApplication::installTranslator(&qtTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load qtTranslator_");
		retCode = false;
	}
	
	const QString kQmNamePrefix("tr_");
	if (baseTranslator_.load((kQmNamePrefix + currentLanguageInfo_.GetLocale().name()), ":/cppdevtk/base/res/tr")) {
		QCoreApplication::installTranslator(&baseTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load baseTranslator_");
		retCode = false;
	}
	if (utilTranslator_.load((kQmNamePrefix + currentLanguageInfo_.GetLocale().name()), ":/cppdevtk/util/res/tr")) {
		QCoreApplication::installTranslator(&utilTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load utilTranslator_");
		retCode = false;
	}
	
	return retCode;
}

QList<QLocale> CoreApplicationBase::GetSupportedLocales() {
	QList<QLocale> supportedLocales;
	supportedLocales.append(QLocale(QLocale::English, QLocale::UnitedStates));
	
	CPPDEVTK_ASSERT(!qmPath_.isEmpty());
	QDir qmDir(qmPath_);
	CPPDEVTK_ASSERT(!qmNamePrefix_.isEmpty());
	QStringList qmNameFilters;
	qmNameFilters.append(qmNamePrefix_ + "??.qm");
	qmNameFilters.append(qmNamePrefix_ + "??_??.qm");
	const QStringList qmEntryList = qmDir.entryList(qmNameFilters,
			(QDir::Files | QDir::NoSymLinks | QDir::Readable | QDir::Hidden | QDir::System));
	for (QStringList::ConstIterator kIter = qmEntryList.constBegin(); kIter != qmEntryList.constEnd(); ++kIter) {
		QString localeName = *kIter;
		localeName.remove(0, qmNamePrefix_.length());
		localeName.chop(3);	// .qm
		if ((localeName.length() != 2) && (localeName.length() != 5)) {
			CPPDEVTK_LOG_WARN("skipping invalid localeName: " << localeName);
			CPPDEVTK_ASSERT(0 && "invalid localeName");
			continue;
		}
		const QLocale kLocale(localeName);
		if (kLocale == QLocale::c()) {
			CPPDEVTK_LOG_WARN("skipping invalid localeName: " << localeName);
			CPPDEVTK_ASSERT(0 && "invalid localeName");
			continue;
		}
		
		CPPDEVTK_ASSERT(!supportedLocales.contains(kLocale));
		supportedLocales.append(kLocale);
	}
	
	return supportedLocales;
}


}	// namespace util
}	// namespace cppdevtk
