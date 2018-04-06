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
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/gui/message_box.hpp>

#include <QtCore/QString>
#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>


namespace cppdevtk {
namespace test_pc_man {


using ::std::exception;
using ::cppdevtk::base::Exception;


Widget::Widget(QWidget* pParent): QWidget(pParent), WidgetBase(), Ui::Widget(),
		theScreenSaver_(gui::ScreenSaver::GetInstance()) {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
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
	if (theScreenSaver_.SetActive(true)) {
		pPlainTextEditOutput_->appendPlainText("screensaver activated");
		
		if (pCheckBoxAutoDeactivateScreenSaver_->isChecked()) {
			QTimer::singleShot(5000, this, SLOT(DeactivateScreenSaver()));
		}
	}
	else {
		pPlainTextEditOutput_->appendPlainText("failed to activate screensaver");
	}
}

void Widget::DeactivateScreenSaver() {
	if (theScreenSaver_.SetActive(false)) {
		pPlainTextEditOutput_->appendPlainText("screensaver deactivated");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("failed to deactivate screensaver");
	}
}

void Widget::LockScreenSaver() {
	if (theScreenSaver_.Lock()) {
		pPlainTextEditOutput_->appendPlainText("screensaver locked");
	}
	else {
		pPlainTextEditOutput_->appendPlainText("failed to lock screensaver");
	}
}

void Widget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(&theScreenSaver_, SIGNAL(ActiveChanged(bool)), SLOT(ScreenSaverActiveChanged(bool))));
	CPPDEVTK_VERIFY(connect(pPushButtonActivateScreenSaver_, SIGNAL(clicked(bool)), SLOT(ActivateScreenSaver())));
	CPPDEVTK_VERIFY(connect(pPushButtonLockScreenSaver_, SIGNAL(clicked(bool)), SLOT(LockScreenSaver())));
	
	CPPDEVTK_VERIFY(connect(pPushButtonClearOutput_, SIGNAL(clicked(bool)), pPlainTextEditOutput_, SLOT(clear())));
}


}	// namespace test_pc_man
}	// namespace cppdevtk
