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
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/info_tr.hpp>

#include <QtGui/QIcon>
#include <QtGui/QShowEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QKeyEvent>


namespace cppdevtk {
namespace test_caps_lock_widget {


Widget::Widget(QWidget* pParent): QWidget(pParent), WidgetBase(), Ui::Widget() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	setupUi(this);
	
	//pCapsLockWidget_->SetBold(true);
	SetStyleSheetFromFileCross(":/cppdevtk/test_caps_lock_widget/res/qss", "widget");
	setWindowIcon(QIcon(":/cppdevtk/test_caps_lock_widget/res/ico/application.ico"));
	
	adjustSize();
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
			CPPDEVTK_LOG_INFO("got QEvent::ApplicationActivate");
			pCapsLockWidget_->Refresh();
			break;
		case QEvent::WindowActivate:
			CPPDEVTK_LOG_INFO("got QEvent::WindowActivate");
			pCapsLockWidget_->Refresh();
			break;
		case QEvent::ActivationChange:
			CPPDEVTK_LOG_INFO("got QEvent::ActivationChange");
			pCapsLockWidget_->Refresh();
			break;
		case QEvent::WindowStateChange:
			CPPDEVTK_LOG_INFO("got QEvent::WindowStateChange");
			pCapsLockWidget_->Refresh();
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

void Widget::keyPressEvent(QKeyEvent* pKeyEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pKeyEvent);
	
	CPPDEVTK_LOG_INFO("got QKeyEvent");
	
	QWidget::keyPressEvent(pKeyEvent);
	
	switch (pKeyEvent->key()) {
		case Qt::Key_CapsLock:
			CPPDEVTK_LOG_INFO("Qt::Key_CapsLock");
			pCapsLockWidget_->Refresh();
			break;
		default:
			break;
	}
}

void Widget::keyReleaseEvent(QKeyEvent* pKeyEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pKeyEvent);
	
	CPPDEVTK_LOG_INFO("got QKeyEvent");
	
	QWidget::keyReleaseEvent(pKeyEvent);
	
	switch (pKeyEvent->key()) {
		case Qt::Key_CapsLock:
			CPPDEVTK_LOG_INFO("Qt::Key_CapsLock");
			pCapsLockWidget_->Refresh();
			break;
		default:
			break;
	}
}


}	// namespace test_caps_lock_widget
}	// namespace cppdevtk
