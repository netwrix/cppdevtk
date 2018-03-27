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


#include <cppdevtk/util/core_application_base.hpp>
#include <cppdevtk/util/core_application.hpp>
#include <cppdevtk/util/single_core_application.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QtAlgorithms>


using ::cppdevtk::base::Exception;
using ::std::exception;


namespace cppdevtk {
namespace util {


QString CoreApplicationBase::qmPath_;
QString CoreApplicationBase::qmNamePrefix_;


CoreApplicationBase::CoreApplicationBase(): NonCopyable(), notifyThrowAction_(ntaRethrow),
		isSystemLocalePreferred_(false), currentLanguageInfo_(LanguageInfo::GetCodeLanguageInfo()),
		qtTranslator_(), baseTranslator_(), utilTranslator_(), quitOnTerminationSignals_(false)
#		if (CPPDEVTK_PLATFORM_UNIX)
		, unwatchSigTerm_(false), unwatchSigInt_(false)
#		endif
		{
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

CoreApplicationBase::~CoreApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

LanguageInfo CoreApplicationBase::GetDefaultLanguageInfo() const {
	LanguageInfo defaultLanguageInfo(LanguageInfo::GetCodeLanguageInfo());
	
	if (isSystemLocalePreferred_) {
		bool isSystemLocaleSupported = false;
		
		const QString kSystemLocaleName = QLocale::system().name();
		CPPDEVTK_LOG_INFO("kSystemLocaleName: " << kSystemLocaleName);
		if (kSystemLocaleName == LanguageInfo::GetCodeName()) {
			isSystemLocaleSupported = true;
		}
		else {
			const QList<LanguageInfo> kSupportedLanguageInfos = GetSupportedLanguageInfos();
			for (QList<LanguageInfo>::ConstIterator kIter = kSupportedLanguageInfos.constBegin();
					kIter != kSupportedLanguageInfos.constEnd(); ++kIter) {
				if (kIter->GetName() == kSystemLocaleName) {
					isSystemLocaleSupported = true;
					defaultLanguageInfo = *kIter;
					break;
				}
			}
		}
		
		CPPDEVTK_LOG_INFO("isSystemLocaleSupported: " << isSystemLocaleSupported);
	}
	
	return defaultLanguageInfo;
}

void CoreApplicationBase::SetCurrentLanguageInfo(const LanguageInfo& languageInfo) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((languageInfo != LanguageInfo::GetCLanguageInfo()), "languageInfo: C is not supported");
	CPPDEVTK_DBC_CHECK_ARGUMENT(GetSupportedLanguageInfos().contains(languageInfo),
			"languageInfo is not supported");
	
	currentLanguageInfo_ = languageInfo;
	
	if (!SetupTranslators()) {
		CPPDEVTK_LOG_ERROR("failed to setup translators");
		//CPPDEVTK_ASSERT(0 && "failed to setup translators");
	}
}

void CoreApplicationBase::SetQmInfo(const QString& qmPath, const QString& qmNamePrefix) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(util::IsValidPath(qmPath), "qmPath is not valid");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(qmNamePrefix.isEmpty(), "qmNamePrefix");
	
	qmPath_ = qmPath;
	qmNamePrefix_ = qmNamePrefix;
}

QList<LanguageInfo> CoreApplicationBase::GetSupportedLanguageInfos() {
	QList<LanguageInfo> supportedLanguageInfos;
	
	const QList<QLocale> kSupportedLocales = GetSupportedLocales();
	for (QList<QLocale>::ConstIterator kIter = kSupportedLocales.constBegin(); kIter != kSupportedLocales.constEnd();
			++kIter) {
		if (kIter->name() == LanguageInfo::GetCodeName()) {
			supportedLanguageInfos.append(LanguageInfo::GetCodeLanguageInfo());
			continue;
		}
		
		const QString kQmFileName = qmNamePrefix_ + kIter->name();
		QTranslator translator;
		if (!translator.load(kQmFileName, qmPath_)) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(QString("failed to load translator for: kQmFileName: '%1'; qmPath_: '%2'").arg(
					kQmFileName, qmPath_));
		}
		
		const QString kLanguageNativeName = translator.translate("language_native_name", "English");
		if (kLanguageNativeName.isEmpty()) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(
					QString("translated language_native_name is empty for: kQmFileName: '%1'; qmPath_: '%2'").arg(
					kQmFileName, qmPath_));
		}
		if (kLanguageNativeName == LanguageInfo::GetCodeNativeName()) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(
					QString("language_native_name is not translated for: kQmFileName: '%1'; qmPath_: '%2'").arg(
					kQmFileName, qmPath_));
		}
		
		CPPDEVTK_ASSERT(!supportedLanguageInfos.contains(LanguageInfo(*kIter, kLanguageNativeName)));
		supportedLanguageInfos.append(LanguageInfo(*kIter, kLanguageNativeName));
	}
	
	qSort(supportedLanguageInfos.begin(), supportedLanguageInfos.end());
	
	CPPDEVTK_LOG_INFO("supportedLanguageInfos.count(): " << supportedLanguageInfos.count());
	
	return supportedLanguageInfos;
}

void CoreApplicationBase::SetInfo(const QString& companyName, const QString& companyHomepage,
		const QString& productName, const QString& productVersion, const QString& componentName) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(companyName.isEmpty(), "companyName");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(companyHomepage.isEmpty(), "companyHomepage");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(productName.isEmpty(), "productName");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(productVersion.isEmpty(), "productVersion");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(productName.isEmpty(), "componentName");
	
	QCoreApplication::setOrganizationName(companyName);
	QCoreApplication::setOrganizationDomain(companyHomepage);
	QCoreApplication::setApplicationName(QString("%1.%2").arg(productName, componentName));
	QCoreApplication::setApplicationVersion(productVersion);
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

QString CoreApplicationBase::GetId() {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(!QCoreApplication::organizationName().isEmpty(),
			"organizationName() is empty; call setOrganizationName() first");
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(!QCoreApplication::applicationName().isEmpty(),
			"applicationName() is empty; call setApplicationName() first");
	
	return QString("com.%1.%2").arg(QCoreApplication::organizationName(), QCoreApplication::applicationName());
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
	
	if (!utilTranslator_.isEmpty()) {
		QCoreApplication::removeTranslator(&utilTranslator_);
	}
	if (!baseTranslator_.isEmpty()) {
		QCoreApplication::removeTranslator(&baseTranslator_);
	}
	if (!qtTranslator_.isEmpty()) {
		QCoreApplication::removeTranslator(&qtTranslator_);
	}
	
	if (currentLanguageInfo_ == LanguageInfo::GetCodeLanguageInfo()) {
		return retCode;
	}
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	if (qtTranslator_.load((QString("qtbase_") + currentLanguageInfo_.GetName()), ":/cppdevtk/util/res/tr")) {
#	else
	if (qtTranslator_.load((QString("qt_") + currentLanguageInfo_.GetName()), ":/cppdevtk/util/res/tr")) {
#	endif
		QCoreApplication::installTranslator(&qtTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load qtTranslator_");
		retCode = false;
	}
	
	const QString kQmNamePrefix("tr_");
	if (baseTranslator_.load((kQmNamePrefix + currentLanguageInfo_.GetName()), ":/cppdevtk/base/res/tr")) {
		QCoreApplication::installTranslator(&baseTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load baseTranslator_");
		retCode = false;
	}
	if (utilTranslator_.load((kQmNamePrefix + currentLanguageInfo_.GetName()), ":/cppdevtk/util/res/tr")) {
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
	
	supportedLocales.append(LanguageInfo::GetCodeName());	// there is no tr_en(_US).qm
	
	if (qmPath_.isEmpty()) {
		CPPDEVTK_LOG_DEBUG("qmPath_ is empty");
		return supportedLocales;
	}
	CPPDEVTK_LOG_DEBUG("qmPath_: " << qmPath_);
	QDir qmDir(qmPath_);
	
	QStringList qmNameFilters;
	if (qmNamePrefix_.isEmpty()) {
		CPPDEVTK_LOG_DEBUG("qmNamePrefix_ is empty");
		return supportedLocales;
	}
	CPPDEVTK_LOG_DEBUG("qmNamePrefix_: " << qmNamePrefix_);
	qmNameFilters.append(qmNamePrefix_ + "??.qm");
	qmNameFilters.append(qmNamePrefix_ + "??_??.qm");
	
	const QStringList qmEntryList = qmDir.entryList(qmNameFilters,
			(QDir::Files | QDir::Readable | QDir::Hidden /* | QDir::System */ /* | QDir::NoSymLinks */));
	CPPDEVTK_LOG_DEBUG("qmEntryList.count(): " << qmEntryList.count());
	for (QStringList::ConstIterator kIter = qmEntryList.constBegin(); kIter != qmEntryList.constEnd(); ++kIter) {
		QString localeName = *kIter;
		localeName.remove(0, qmNamePrefix_.length());
		localeName.chop(3);	// .qm
		if (!((localeName.length() == 2) || ((localeName.length() == 5) && (localeName[2] == '_')))) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(QString("qm file '%1': invalid localeName: '%2'").arg(*kIter, localeName));
		}
		
		const QLocale kLocale(localeName);
		if (kLocale.name() == QLocale::c().name()) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(QString("qm file '%1': invalid localeName: '%2'").arg(*kIter, localeName));
		}
		if (kLocale.name() == LanguageInfo::GetCodeName()) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(QString(
					"qm file '%1': invalid localeName (qm file with code language): '%2'").arg(*kIter, localeName));
		}
		
		CPPDEVTK_ASSERT(!supportedLocales.contains(kLocale));
		supportedLocales.append(kLocale);
	}
	
	CPPDEVTK_LOG_INFO("supportedLocales.count(): " << supportedLocales.count());
	
	return supportedLocales;
}


}	// namespace util
}	// namespace cppdevtk
