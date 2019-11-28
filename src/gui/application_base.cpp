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


#include <cppdevtk/gui/application_base.hpp>
#include <cppdevtk/gui/application.hpp>
#if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
#include <cppdevtk/gui/single_application.hpp>
#endif
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/gui/set_stylesheet_from_file.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/util/get_user_name.hpp>
#include <cppdevtk/util/language_info.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/system_exception.hpp>

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif


using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::Exception;
using ::std::exception;


namespace cppdevtk {
namespace gui {


ApplicationBase::ApplicationBase(): CoreApplicationBase(),
#		if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
		qtsolQtCopyDialogTranslator_(),
#		endif
		guiTranslator_() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	QApplication::translate("language_native_name", "English");	// to generate translation
}

ApplicationBase::~ApplicationBase() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

QWidget* ApplicationBase::GetDefaultWindow() const {
	QWidget* pDefaultWindow = NULL;
	
	pDefaultWindow = QApplication::activeWindow();
#	if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
	if (pDefaultWindow == NULL) {
		const SingleApplication* pSingleApplication = dynamic_cast<const SingleApplication*>(this);
		if (pSingleApplication != NULL) {
			pDefaultWindow = pSingleApplication->activationWindow();
		}
	}
#	endif
	
	if (pDefaultWindow == NULL) {
		const QWidgetList kTopLevelWidgetList = QApplication::topLevelWidgets();
		if (!kTopLevelWidgetList.isEmpty()) {
			pDefaultWindow = kTopLevelWidgetList[0];
		}
	}
	
	return pDefaultWindow;
}

bool ApplicationBase::SetStyleSheetFromFile(const QString& path, const QString& completeBaseName,
		bool append, const QString& suffix) {
	return ::cppdevtk::gui::SetStyleSheetFromFile(dynamic_cast<QApplication&>(*this), path, completeBaseName, append,
			suffix);
}

bool ApplicationBase::SetStyleSheetFromFileCross(const QString& path, const QString& completeBaseName,
		bool append, const QString& suffix) {
	return ::cppdevtk::gui::SetStyleSheetFromFileCross(dynamic_cast<QApplication&>(*this), path, completeBaseName, append,
			suffix);
}

bool ApplicationBase::OpenLocalizedFileInDefaultApp(const QString& fileNamePrefix, const QString& fileExt,
		const QLocale& locale) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(util::IsValidPath(fileNamePrefix), "fileNamePrefix must be valid");
	//CPPDEVTK_DBC_CHECK_ARGUMENT(fileNamePrefix.startsWith(":/"), "fileNamePrefix must be a Qt resource");
	
	const QString kLocalizedFileName = util::GetLocalizedFileName(fileNamePrefix, fileExt, locale);
	if (kLocalizedFileName.isEmpty()) {
		CPPDEVTK_LOG_ERROR("failed to get localizedFileName for fileNamePrefix: " << fileNamePrefix
				<< "; fileExt: " << fileExt << "; locale: " << locale.name());
		return false;
	}
	
	QString fileToOpen = kLocalizedFileName;
	
	if (kLocalizedFileName.startsWith(":/")) {	// Qt resource file; launch from temp
		QString tmpDstPath = QDir::tempPath();
		if (!tmpDstPath.endsWith('/')) {
			tmpDstPath += '/';
		}
		
		QString userName = util::GetUserName();
		if (userName.isEmpty()) {
			userName = "anonymous";
		}
		
		tmpDstPath += (userName + '/');
		
		ErrorCode errorCode;
		if (!util::MakePath(tmpDstPath, false, errorCode)) {
			CPPDEVTK_LOG_ERROR("failed to make path: '" << tmpDstPath << "'\nerror code: " << errorCode.ToString());
			return false;
		}
		
		const QString kTmpDstFileName = tmpDstPath + QFileInfo(kLocalizedFileName).fileName();
		if (!util::DeleteFile(kTmpDstFileName, false, errorCode)) {
			CPPDEVTK_LOG_ERROR("failed to delete file: '" << kTmpDstFileName << "'\nerror code: " << errorCode.ToString());
			return false;
		}
		
		if (!QFile::copy(kLocalizedFileName, kTmpDstFileName)) {
			CPPDEVTK_LOG_ERROR("failed to copy file '" << kLocalizedFileName << "' to: " << kTmpDstFileName);
			return false;
		}
		
		fileToOpen = kTmpDstFileName;
	}
	
	const QUrl kUrl = QUrl::fromLocalFile(fileToOpen);
	if (!QDesktopServices::openUrl(kUrl)) {
		CPPDEVTK_LOG_ERROR("failed to open file '" << fileToOpen << "' in default app");
		return false;
	}
	
	return true;
}

bool ApplicationBase::notify(QObject* pReceiver, QEvent* pEvent) {
	bool retCode = false;
	
	QApplication* pQApplication = dynamic_cast<QApplication*>(this);
	if (pQApplication != NULL) {
		try {
			retCode = pQApplication->QApplication::notify(pReceiver, pEvent);
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

bool ApplicationBase::NotifyThrowHandler(const ::std::exception* pExc) {
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
	
	if (GetNotifyThrowAction() == ntaRethrow) {
		CPPDEVTK_LOG_ERROR("rethrowing...");
		throw;
	}
	
	if (GetNotifyThrowAction() == ntaQuit) {
		if (pExc != NULL) {
			QString text = tr("An %1 exception was thrown from an event handler!!!\nMessage: %2").arg(
					"::std::exception", pExc->what());
			MessageBox::Critical(GetDefaultWindow(), tr("Fatal Error"), text, *pExc);
		}
		else {
			QString text = tr("An unknown exception was thrown from an event handler!!!");
			MessageBox::Critical(GetDefaultWindow(), tr("Fatal Error"), text);
		}
		
		CPPDEVTK_LOG_FATAL("quitting...");
		QApplication::quit();
	}
	else {
		CPPDEVTK_LOG_WARN("ignoring (return false)...");
	}
	
	return false;
}

bool ApplicationBase::SetupTranslators() {
	if (!guiTranslator_.isEmpty()) {
		QApplication::removeTranslator(&guiTranslator_);
	}
#	if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
	if (!qtsolQtCopyDialogTranslator_.isEmpty()) {
		QApplication::removeTranslator(&qtsolQtCopyDialogTranslator_);
	}
#	endif
	
	bool retCode = CoreApplicationBase::SetupTranslators();
	
	const QString kCurrentLanguageInfoName = GetCurrentLanguageInfo().GetName();
	if (kCurrentLanguageInfoName == ::cppdevtk::util::LanguageInfo::GetCodeName()) {
		return retCode;
	}
	
#	if (CPPDEVTK_ENABLE_QT_SOLUTIONS)
	if (qtsolQtCopyDialogTranslator_.load((QString("tr_") + kCurrentLanguageInfoName),
			":/cppdevtk/QtSolutions/QtCopyDialog/res/tr")) {
		QApplication::installTranslator(&qtsolQtCopyDialogTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load qtsolQtCopyDialogTranslator_");
		retCode = false;
	}
#	endif
	if (guiTranslator_.load((QString("tr_") + kCurrentLanguageInfoName), ":/cppdevtk/gui/res/tr")) {
		QApplication::installTranslator(&guiTranslator_);
	}
	else {
		CPPDEVTK_LOG_ERROR("failed to load guiTranslator_");
		retCode = false;
	}
	
	return retCode;
}


}	// namespace gui
}	// namespace cppdevtk
