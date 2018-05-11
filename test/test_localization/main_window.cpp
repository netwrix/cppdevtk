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


#include "main_window.hpp"
#include "info_tr.hpp"

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/gui/message_box.hpp>

#include <QtGui/QIcon>
#include <QtGui/QKeySequence>
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtGui/QShowEvent>


using ::cppdevtk::gui::MessageBox;
using ::cppdevtk::util::LanguageInfo;


namespace cppdevtk {
namespace test_localization {


MainWindow::MainWindow(QWidget* pParent): QMainWindow(pParent), WidgetBase(), Ui::MainWindow() {
	SetupUi();
	
	SetupStrings();
	
	//pLanguageWidget_->SetBold(true);
	//pLanguageWidget_->SetMaxVisibleItems(4);
	
	PopulateLanguages();
	
	MakeConnections();
	
	SetCurrentLanguage();
	
	adjustSize();
}

void MainWindow::changeEvent(QEvent* pEvent) {
	CPPDEVTK_ASSERT(pEvent != NULL);
	
	QMainWindow::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			SetupStrings();
			break;
		default:
			break;
	}
}

void MainWindow::showEvent(QShowEvent* pShowEvent) {
	static bool isShownFirstTime = true;
	
	
	QMainWindow::showEvent(pShowEvent);
	
	if (isShownFirstTime) {
		isShownFirstTime = false;
		
		static_cast<Application*>(QApplication::instance())->setActivationWindow(this);
	}
}

void MainWindow::ChangeLanguage(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	Application* pApplication = static_cast<Application*>(QApplication::instance());
	const LanguageInfo kRealLanguageInfo = (languageInfo != LanguageInfo::GetCLanguageInfo())
			? languageInfo : pApplication->GetDefaultLanguageInfo();
	CPPDEVTK_LOG_INFO("changing language to: " << kRealLanguageInfo.ToString());
	pApplication->SetCurrentLanguageInfo(kRealLanguageInfo);
	if (sender() == pMenuLanguage_) {
		pLanguageWidget_->SetCurrentLanguageInfo(kRealLanguageInfo);
	}
	else {
		CPPDEVTK_ASSERT(sender() == pLanguageWidget_);
		pMenuLanguage_->SetCheckedLanguageInfo(kRealLanguageInfo);
	}
}

void MainWindow::About() {
	const QString kTitle = tr("About %1").arg(Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_SHORT_NAME_TR));
	const QString kText = tr("%1 version %2.\n%3").arg(
			Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_SHORT_NAME_TR),
			CPPDEVTK_VERSION_STRING,
			Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_COMPANY_COPYRIGHT_TR));
	MessageBox::about(this, kTitle, kText);
}

void MainWindow::Homepage() {
	const QUrl kUrl(CPPDEVTK_HOMEPAGE, QUrl::TolerantMode);
	if (!QDesktopServices::openUrl(kUrl)) {
		MessageBox::Critical(this, Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_SHORT_NAME_TR),
				tr("Failed to open product homepage"), QString("URL: %1").arg(kUrl.toString()));
	}
}

void MainWindow::Support() {
	const QUrl kUrl(CPPDEVTK_ONLINE_SUPPORT, QUrl::TolerantMode);
	if (!QDesktopServices::openUrl(kUrl)) {
		MessageBox::Critical(this, Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_SHORT_NAME_TR),
				tr("Failed to open online support"), QString("URL: %1").arg(kUrl.toString()));
	}
}

void MainWindow::Email() {
	const QString kSubject = tr("%1 info").arg(CPPDEVTK_SHORT_NAME);
	const QString kBody = tr("Please write your message here!");
	const QString kEmail = QString("mailto:%1?subject=%2&body=%3").arg(CPPDEVTK_COMPANY_EMAIL, kSubject, kBody);
	
	const QUrl kUrl(kEmail, QUrl::TolerantMode);
	if (!QDesktopServices::openUrl(kUrl)) {
		MessageBox::Critical(this, Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_SHORT_NAME_TR),
				tr("Failed to open default E-Mail client"), QString("URL: %1").arg(kUrl.toString()));
	}
}

void MainWindow::SetupUi() {
	setupUi(this);
	
	SetStyleSheetFromFileCross(":/cppdevtk/test_localization/res/qss", "main_window");
	
	setWindowIcon(QIcon(":/cppdevtk/test_localization/res/ico/application.ico"));
}

void MainWindow::SetupStrings() {
	setWindowTitle(Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_LONG_NAME_TR));
	
	pActionAbout_->setText(tr("&About %1").arg(
			Application::translate(CPPDEVTK_TEST_LOCALIZATION_TR_CTX, CPPDEVTK_TEST_LOCALIZATION_SHORT_NAME_TR)));
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)	// Exit has no shortcut on Windows
	pActionExit_->setShortcut(QKeySequence(tr("Ctrl+Q")));
#	else
	pActionExit_->setShortcut(QKeySequence(QKeySequence::Quit));
#	endif
}

void MainWindow::PopulateLanguages() {
	const QList<LanguageInfo> kSupportedLanguageInfos = Application::GetSupportedLanguageInfos();
	
	pMenuLanguage_->AddLanguageInfos(kSupportedLanguageInfos);
	CPPDEVTK_ASSERT(pMenuLanguage_->GetCount() == kSupportedLanguageInfos.count());
	
	pLanguageWidget_->AddLanguageInfos(kSupportedLanguageInfos);
	CPPDEVTK_ASSERT(pLanguageWidget_->GetCount() == kSupportedLanguageInfos.count());
}

void MainWindow::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pActionExit_, SIGNAL(triggered()), SLOT(Exit())));
	
	CPPDEVTK_VERIFY(connect(pMenuLanguage_, SIGNAL(Triggered(const::cppdevtk::util::LanguageInfo&)),
			SLOT(ChangeLanguage(const::cppdevtk::util::LanguageInfo&))));	
	
	CPPDEVTK_VERIFY(connect(pActionAbout_, SIGNAL(triggered()), SLOT(About())));
	CPPDEVTK_VERIFY(connect(pActionAboutQt_, SIGNAL(triggered()), SLOT(AboutQt())));
	CPPDEVTK_VERIFY(connect(pActionHomepage_, SIGNAL(triggered()), SLOT(Homepage())));
	CPPDEVTK_VERIFY(connect(pActionSupport_, SIGNAL(triggered()), SLOT(Support())));
	CPPDEVTK_VERIFY(connect(pActionEmail_, SIGNAL(triggered()), SLOT(Email())));
	
	CPPDEVTK_VERIFY(connect(pLanguageWidget_, SIGNAL(CurrentLanguageInfoChanged(const::cppdevtk::util::LanguageInfo&)),
			SLOT(ChangeLanguage(const::cppdevtk::util::LanguageInfo&))));
}

void MainWindow::SetCurrentLanguage() {
	const LanguageInfo kCurrentLanguageInfo = static_cast<Application*>(QApplication::instance())->GetCurrentLanguageInfo();
	CPPDEVTK_LOG_INFO("setting language to: " << kCurrentLanguageInfo.ToString());
	pMenuLanguage_->SetCheckedLanguageInfo(kCurrentLanguageInfo);
	pLanguageWidget_->SetCurrentLanguageInfo(kCurrentLanguageInfo);
}


}	// namespace test_localization
}	// namespace cppdevtk
