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
#include <cppdevtk/gui/screensaver.hpp>
#include <cppdevtk/gui/power_notifier.hpp>
#include <cppdevtk/gui/session_manager.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/info_tr.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>

#include <QtGui/QIcon>
#include <QtCore/QString>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>

#include <stdexcept>


namespace cppdevtk {
namespace test_pc_man {


using ::cppdevtk::gui::ScreenSaver;
using ::cppdevtk::gui::StorageDeviceNotifier;
using ::cppdevtk::gui::PowerNotifier;
using ::cppdevtk::gui::Session;
using ::cppdevtk::gui::SessionManager;
using ::std::exception;
using ::cppdevtk::base::Exception;
using ::std::runtime_error;


Widget::Widget(QWidget* pParent): QWidget(pParent), WidgetBase(), Ui::Widget(), pSession_() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	if (SessionManager::IsSessionManagerServiceRegistered()) {
#	endif
		try {
			pSession_ = SessionManager::GetInstance().GetCurrentProcessSession();
		}
		catch (const runtime_error& exc) {
			pPlainTextEditOutput_->appendPlainText(QString("failed to GetCurrentProcessSession; exc: %1").arg(
					Exception::GetDetailedInfo(exc)));
		}
#	if (CPPDEVTK_PLATFORM_LINUX)
	}
#	endif
	
	setupUi(this);
	CPPDEVTK_ASSERT(pPlainTextEditOutput_->isReadOnly());
	
	SetStyleSheetFromFileCross(":/cppdevtk/test_pc_man/res/qss", "widget");
	setWindowIcon(QIcon(":/cppdevtk/test_pc_man/res/ico/application.ico"));
	
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
		case QEvent::ApplicationActivate:
			//CPPDEVTK_LOG_INFO("got QEvent::ApplicationActivate");
			break;
		case QEvent::WindowActivate:
			//CPPDEVTK_LOG_INFO("got QEvent::WindowActivate");
			break;
		case QEvent::ActivationChange:
			//CPPDEVTK_LOG_INFO("got QEvent::ActivationChange");
			break;
		case QEvent::WindowStateChange:
			//CPPDEVTK_LOG_INFO("got QEvent::WindowStateChange");
			break;
		/*
		case QEvent::UpdateRequest:
			CPPDEVTK_LOG_INFO("got QEvent::UpdateRequest");
			pCapsLockWidget_->Refresh();
			break;
		*/
		default:
			break;
	}
}

void Widget::closeEvent(QCloseEvent* pCloseEvent) {
	using ::cppdevtk::gui::MessageBox;
	
	const MessageBox::StandardButton kRetValue = MessageBox::Question(this, "test", "Do you want to quit?",
			(MessageBox::Yes | MessageBox::No));
	if (kRetValue == MessageBox::Yes) {
		pCloseEvent->accept();
	}
	else {
		pCloseEvent->ignore();
	}
}

void Widget::ScreenSaverActiveChanged(bool isActive) {
	pPlainTextEditOutput_->appendPlainText(QString("screensaver active changed; isActive: %1").arg(isActive));
}

void Widget::ActivateScreenSaver() {
	if (ScreenSaver::GetInstance().SetActive(true)) {
		pPlainTextEditOutput_->appendPlainText("ActivateScreenSaver(): OK");
		
		if (pCheckBoxAutoDeactivateScreenSaver_->isChecked()) {
			QTimer::singleShot(5000, this, SLOT(DeactivateScreenSaver()));
		}
	}
	else {
		pPlainTextEditOutput_->appendPlainText("ActivateScreenSaver(): ERR");
	}
}

void Widget::DeactivateScreenSaver() {
	if (ScreenSaver::GetInstance().SetActive(false)) {
		pPlainTextEditOutput_->appendPlainText("DeactivateScreenSaver(): OK");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("DeactivateScreenSaver(): ERR");
	}
}

void Widget::LockScreenSaver() {
	if (ScreenSaver::GetInstance().Lock()) {
		pPlainTextEditOutput_->appendPlainText("LockScreenSaver(): OK");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("LockScreenSaver(): ERR");
	}
}

void Widget::OnStorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId) {
	QString msg = QString("storage device added; storageDeviceId: %1").arg(
#			if (!CPPDEVTK_PLATFORM_LINUX)
			storageDeviceId
#			else
			storageDeviceId.path()
#			endif
	);
	msg += QString("; storageDeviceName: ") + StorageDeviceNotifier::GetStorageDeviceName(storageDeviceId);
	pPlainTextEditOutput_->appendPlainText(msg);
}

void Widget::OnStorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId) {
	pPlainTextEditOutput_->appendPlainText(QString("storage device removed; storageDeviceId: %1").arg(
#			if (!CPPDEVTK_PLATFORM_LINUX)
			storageDeviceId
#			else
			storageDeviceId.path()
#			endif
	));
}

void Widget::OnStorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId) {
	QString msg = QString("storage device mounted; storageDeviceId: %1").arg(
#			if (!CPPDEVTK_PLATFORM_LINUX)
			storageDeviceId
#			else
			storageDeviceId.path()
#			endif
	);
	const QString kStorageDeviceName = StorageDeviceNotifier::GetStorageDeviceName(storageDeviceId);
	msg += QString("; storageDeviceName: ") + kStorageDeviceName;
#	if (CPPDEVTK_PLATFORM_UNIX)
	const QStringList kStorageDeviceMountPoints = util::GetMountPointsFromDeviceName(kStorageDeviceName);
	CPPDEVTK_ASSERT(!kStorageDeviceMountPoints.isEmpty());
	msg += QString("; storageDeviceMountPoints: ") + kStorageDeviceMountPoints.join(" ");
	
	CPPDEVTK_ASSERT(StorageDeviceNotifier::GetStorageDeviceId(
			util::GetDeviceNameFromMountPoint(kStorageDeviceMountPoints[0])) == storageDeviceId);
#	endif
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	CPPDEVTK_ASSERT(StorageDeviceNotifier::GetStorageDeviceId(kStorageDeviceName) == storageDeviceId);
#	endif
	pPlainTextEditOutput_->appendPlainText(msg);
}

void Widget::OnStorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId) {
	QString msg = QString("storage device unmounted; storageDeviceId: %1").arg(
#			if (!CPPDEVTK_PLATFORM_LINUX)
			storageDeviceId
#			else
			storageDeviceId.path()
#			endif
	);
#	if (CPPDEVTK_PLATFORM_LINUX)
	try {
		msg += QString("; storageDeviceName: ") + StorageDeviceNotifier::GetStorageDeviceName(storageDeviceId);
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_WARN("GetStorageDeviceName() failed for storageDeviceId "
#				if (!CPPDEVTK_PLATFORM_LINUX)
				<< storageDeviceId
#				else
				<< storageDeviceId.path()
#				endif
				<< "; exc: " << base::Exception::GetDetailedInfo(exc));
	}
#	endif
	pPlainTextEditOutput_->appendPlainText(msg);
}

void Widget::OnSleeping() {
	pPlainTextEditOutput_->appendPlainText("sleeping...");
}

void Widget::OnResuming() {
	pPlainTextEditOutput_->appendPlainText("resuming...");
}

void Widget::OnSessionActivated() {
	pPlainTextEditOutput_->appendPlainText("SessionActivated...");
}

void Widget::OnSessionDeactivated() {
	pPlainTextEditOutput_->appendPlainText("SessionDeactivated...");
}

void Widget::OnSessionLocked() {
	pPlainTextEditOutput_->appendPlainText("SessionLocked...");
}

void Widget::OnSessionUnlocked() {
	pPlainTextEditOutput_->appendPlainText("SessionUnlocked...");
}


void Widget::LockSession() {
	if (pSession_.get() == NULL) {
		pPlainTextEditOutput_->appendPlainText("LockSession(): ERR (session not available)");
	}
	
	if (pSession_->Lock()) {
		pPlainTextEditOutput_->appendPlainText("LockSession(): OK");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("LockSession(): ERR");
	}
}

void Widget::GetSessionId() try {
	if (pSession_.get() == NULL) {
		pPlainTextEditOutput_->appendPlainText("GetSessionId(): ERR (session not available)");
	}
	
	pPlainTextEditOutput_->appendPlainText(QString("sessionId: %1").arg(pSession_->GetId()));
}
catch (const runtime_error& exc) {
	pPlainTextEditOutput_->appendPlainText(QString("failed to get sessionId; exc: %1").arg(Exception::GetDetailedInfo(exc)));
}

void Widget::Logout() {
	if (SessionManager::GetInstance().Logout()) {
		pPlainTextEditOutput_->appendPlainText("Logout(): OK");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("Logout(): ERR");
	}
}

void Widget::Shutdown() {
	if (SessionManager::GetInstance().Shutdown()) {
		pPlainTextEditOutput_->appendPlainText("Shutdown(): OK");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("Shutdown(): ERR");
	}
}

void Widget::GetIdleTime() {
	pPlainTextEditOutput_->appendPlainText("Scheduling idle timer 5s");
	QTimer::singleShot(5000, this, SLOT(OnIdleTimerTimeout()));
}

void Widget::OnIdleTimerTimeout() try {
	pPlainTextEditOutput_->appendPlainText(QString("idleTime: %1").arg(
			SessionManager::GetInstance().GetCurrentProcessSession()->GetIdleTime()));
}
catch (const runtime_error& exc) {
	pPlainTextEditOutput_->appendPlainText(QString("failed to get idleTime; exc: %1").arg(Exception::GetDetailedInfo(exc)));
}

void Widget::MakeConnections() {
#	if (CPPDEVTK_PLATFORM_LINUX)
	if (ScreenSaver::IsScreenSaverServiceRegistered()) {
		pPlainTextEditOutput_->appendPlainText("ScreenSaver is supported");
#	endif
		CPPDEVTK_VERIFY(connect(&ScreenSaver::GetInstance(), SIGNAL(ActiveChanged(bool)), SLOT(ScreenSaverActiveChanged(bool))));
		CPPDEVTK_VERIFY(connect(pPushButtonActivateScreenSaver_, SIGNAL(clicked(bool)), SLOT(ActivateScreenSaver())));
		CPPDEVTK_VERIFY(connect(pPushButtonLockScreenSaver_, SIGNAL(clicked(bool)), SLOT(LockScreenSaver())));
#	if (CPPDEVTK_PLATFORM_LINUX)
	}
	else {
		pPlainTextEditOutput_->appendPlainText("ScreenSaver is not supported");
	}
#	endif
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	if (SessionManager::IsSessionManagerServiceRegistered()) {
		pPlainTextEditOutput_->appendPlainText("SessionManager is supported");
#	endif
		if (pSession_.get() != NULL) {
			CPPDEVTK_VERIFY(connect(pSession_.get(), SIGNAL(Activated()), SLOT(OnSessionActivated())));
			CPPDEVTK_VERIFY(connect(pSession_.get(), SIGNAL(Deactivated()), SLOT(OnSessionDeactivated())));
			CPPDEVTK_VERIFY(connect(pSession_.get(), SIGNAL(Locked()), SLOT(OnSessionLocked())));
			CPPDEVTK_VERIFY(connect(pSession_.get(), SIGNAL(Unlocked()), SLOT(OnSessionUnlocked())));
		}
		CPPDEVTK_VERIFY(connect(pPushButtonLockSession_, SIGNAL(clicked(bool)), SLOT(LockSession())));
		CPPDEVTK_VERIFY(connect(pPushButtonGetSessionId_, SIGNAL(clicked(bool)), SLOT(GetSessionId())));
		
		CPPDEVTK_VERIFY(connect(pPushButtonLogout_, SIGNAL(clicked(bool)), SLOT(Logout())));
		CPPDEVTK_VERIFY(connect(pPushButtonShutdown_, SIGNAL(clicked(bool)), SLOT(Shutdown())));
		CPPDEVTK_VERIFY(connect(pPushButtonGetIdleTime_, SIGNAL(clicked(bool)), SLOT(GetIdleTime())));
#	if (CPPDEVTK_PLATFORM_LINUX)
	}
	else {
		pPlainTextEditOutput_->appendPlainText("SessionManager is not supported");
	}
#	endif
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	if (PowerNotifier::IsPowerNotifierServiceRegistered()) {
		pPlainTextEditOutput_->appendPlainText("PowerNotifier is supported");
#	endif
		PowerNotifier& thePowerNotifier = PowerNotifier::GetInstance();
		CPPDEVTK_VERIFY(connect(&thePowerNotifier, SIGNAL(Sleeping()), SLOT(OnSleeping())));
		CPPDEVTK_VERIFY(connect(&thePowerNotifier, SIGNAL(Resuming()), SLOT(OnResuming())));
#	if (CPPDEVTK_PLATFORM_LINUX)
	}
	else {
		pPlainTextEditOutput_->appendPlainText("PowerNotifier is not supported");
	}
#	endif
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	if (StorageDeviceNotifier::IsStorageDeviceNotifierServiceRegistered()) {
		pPlainTextEditOutput_->appendPlainText("StorageDeviceNotifier is supported");
#	endif
		StorageDeviceNotifier& theStorageDeviceNotifier = StorageDeviceNotifier::GetInstance();
		CPPDEVTK_VERIFY(connect(&theStorageDeviceNotifier,
				SIGNAL(StorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
				SLOT(OnStorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
		CPPDEVTK_VERIFY(connect(&theStorageDeviceNotifier,
				SIGNAL(StorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
				SLOT(OnStorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
		CPPDEVTK_VERIFY(connect(&theStorageDeviceNotifier,
				SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
				SLOT(OnStorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
		CPPDEVTK_VERIFY(connect(&theStorageDeviceNotifier,
				SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
				SLOT(OnStorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
#	if (CPPDEVTK_PLATFORM_LINUX)
	}
	else {
		pPlainTextEditOutput_->appendPlainText("StorageDeviceNotifier is not supported");
	}
#	endif
	
	CPPDEVTK_VERIFY(connect(pPushButtonClearOutput_, SIGNAL(clicked(bool)), pPlainTextEditOutput_, SLOT(clear())));
}


}	// namespace test_pc_man
}	// namespace cppdevtk
