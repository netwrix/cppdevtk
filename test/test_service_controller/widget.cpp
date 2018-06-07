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


#include "widget.hpp"
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/info_tr.hpp>
#include <cppdevtk/gui/application.hpp>

#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>


namespace cppdevtk {
namespace test_service_controller {


using ::cppdevtk::util::ServiceController;


Widget::Widget(QWidget* pParent): QWidget(pParent), WidgetBase(), Ui::Widget(), serviceController_("CppDevTk Test Service") {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	setupUi(this);
	
	SetStyleSheetFromFileCross(":/cppdevtk/test_service_controller/res/qss", "widget");
	setWindowIcon(QIcon(":/cppdevtk/test_service_controller/res/ico/application.ico"));
	
	adjustSize();
	
	MakeConnections();
}

Widget::~Widget() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

void Widget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}

void Widget::Status() {
	pPlainTextEditOutput_->appendPlainText("\nStatus:");
	
	const bool kIsInstalled = serviceController_.isInstalled();
	pPlainTextEditOutput_->appendPlainText(QString("The service %1 is %2 and %3").arg(serviceController_.serviceName(),
			(kIsInstalled ? "installed" : "not installed"), (serviceController_.isRunning() ? "running" : "not running")));
	if (kIsInstalled) {
		pPlainTextEditOutput_->appendPlainText(QString("serviceFilePath: %1").arg(serviceController_.serviceFilePath()));
		pPlainTextEditOutput_->appendPlainText(QString("serviceDescription: %1").arg(serviceController_.serviceDescription()));
		pPlainTextEditOutput_->appendPlainText(QString("startupType: %1").arg(
				((serviceController_.startupType() == ServiceController::AutoStartup) ? "AutoStartup" : "ManualStartup")));
	}
}

void Widget::Install() {
	const QString kServicePath = gui::Application::GetAppDirPath() + "cppdevtk_test_service";
	CPPDEVTK_LOG_DEBUG("kServicePath: " << kServicePath);
	const bool kInstalled = ServiceController::install(kServicePath, pLineEditAccount_->text(), pLineEditPassword_->text());
	pPlainTextEditOutput_->appendPlainText(QString("\nInstall: %1").arg(kInstalled));
}

void Widget::Uninstall() {
	const bool kUninstalled = serviceController_.uninstall();
	pPlainTextEditOutput_->appendPlainText(QString("\nUninstall: %1").arg(kUninstalled));
}

void Widget::Start() {
	const bool kStarted = serviceController_.start(pLineEditArgs_->text().split(' ', QString::SkipEmptyParts));
	pPlainTextEditOutput_->appendPlainText(QString("\nStart: %1").arg(kStarted));
}

void Widget::Stop() {
	const bool kStopped = serviceController_.stop();
	pPlainTextEditOutput_->appendPlainText(QString("\nStop: %1").arg(kStopped));
}

void Widget::Pause() {
	const bool kPaused = serviceController_.pause();
	pPlainTextEditOutput_->appendPlainText(QString("\nPause: %1").arg(kPaused));
}

void Widget::Resume() {
	const bool kResumed = serviceController_.resume();
	pPlainTextEditOutput_->appendPlainText(QString("\nResume: %1").arg(kResumed));
}

void Widget::SendCommand() {
	const bool kCommandSended = serviceController_.sendCommand(pSpinBoxCommandId_->value());
	pPlainTextEditOutput_->appendPlainText(QString("\nSendCommand: %1").arg(kCommandSended));
}

void Widget::ReloadConfiguration() {
	const bool kConfigReloaded = serviceController_.reloadConfig();
	pPlainTextEditOutput_->appendPlainText(QString("\nReloadConfiguration: %1").arg(kConfigReloaded));
}

void Widget::HttpGet() {
	const bool kHttpGet = QDesktopServices::openUrl(QUrl("http://localhost:8080"));
	pPlainTextEditOutput_->appendPlainText(QString("\nHttpGet: %1").arg(kHttpGet));
}

void Widget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pPushButtonStatus_, SIGNAL(clicked(bool)), SLOT(Status())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonInstall_, SIGNAL(clicked(bool)), SLOT(Install())));
	CPPDEVTK_VERIFY(connect(pPushButtonUninstall_, SIGNAL(clicked(bool)), SLOT(Uninstall())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonStart_, SIGNAL(clicked(bool)), SLOT(Start())));
	CPPDEVTK_VERIFY(connect(pPushButtonStop_, SIGNAL(clicked(bool)), SLOT(Stop())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonPause_, SIGNAL(clicked(bool)), SLOT(Pause())));
	CPPDEVTK_VERIFY(connect(pPushButtonResume_, SIGNAL(clicked(bool)), SLOT(Resume())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonSendCommand_, SIGNAL(clicked(bool)), SLOT(SendCommand())));
	CPPDEVTK_VERIFY(connect(pPushButtonReloadConfiguration_, SIGNAL(clicked(bool)), SLOT(ReloadConfiguration())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonHttpGet_, SIGNAL(clicked(bool)), SLOT(HttpGet())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonClear_, SIGNAL(clicked(bool)), pPlainTextEditOutput_, SLOT(clear())));
}


}	// namespace test_service_controller
}	// namespace cppdevtk
