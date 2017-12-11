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


#include <cppdevtk/gui/caps_lock_widget.hpp>
#include "ui_caps_lock_widget.h"
#include <cppdevtk/gui/is_caps_lock_on.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QtGlobal>
#include <QtGui/QColor>
#include <QtGui/QShowEvent>
#include <QtGui/QKeyEvent>

#include <new>


namespace cppdevtk {
namespace gui {


CapsLockWidget::CapsLockWidget(QWidget* pParent): QWidget(pParent), WidgetBase(), pUiCapsLockWidget_(new Ui::CapsLockWidget()),
		onColor_(Qt::darkRed), offColor_(Qt::darkGreen) {
	pUiCapsLockWidget_->setupUi(this);
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "caps_lock_widget");
	CPPDEVTK_ASSERT(pUiCapsLockWidget_->pGridLayout_->contentsMargins() == QMargins(0, 0, 0, 0));
	
	//Refresh();
}

CapsLockWidget::~CapsLockWidget() {
	delete pUiCapsLockWidget_;
}

void CapsLockWidget::Refresh() {
	// avoid issues caused by non-spontaneous show events
	/*
	if (!isVisible()) {
		return;
	}
	*/
	
	bool isCapsLockOn = false;
	if (!IsCapsLockOn(isCapsLockOn)) {
		CPPDEVTK_LOG_ERROR("failed to detect caps lock status");
		return;
	}
	
	const QString kMsg = QString("%1 <FONT COLOR=%2>%3</FONT>").arg(tr("Caps Lock:"),
			(isCapsLockOn ? onColor_.name() : offColor_.name()), (isCapsLockOn ? tr("ON") : tr("OFF")));
	pUiCapsLockWidget_->pLabelCapsLock_->setText(kMsg);
}

void CapsLockWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiCapsLockWidget_->retranslateUi(this);
			Refresh();
			break;
#		if (CPPDEVTK_DETAIL_GUI_CAPS_LOCK_WIDGET_ENABLE_TEST)
		case QEvent::ApplicationActivate:
			CPPDEVTK_LOG_INFO("got QEvent::ApplicationActivate");
			Refresh();
			break;
		case QEvent::WindowActivate:
			CPPDEVTK_LOG_INFO("got QEvent::WindowActivate");
			Refresh();
			break;
		case QEvent::ActivationChange:	// top-level
			CPPDEVTK_LOG_INFO("got QEvent::ActivationChange");
			Refresh();
			break;
		case QEvent::WindowStateChange:
			CPPDEVTK_LOG_INFO("got QEvent::WindowStateChange");
			Refresh();
			break;
		/*
		case QEvent::UpdateRequest:
			CPPDEVTK_LOG_INFO("got QEvent::UpdateRequest");
			Refresh();
			break;
		*/
#		endif	// (CPPDEVTK_DETAIL_GUI_CAPS_LOCK_WIDGET_ENABLE_TEST)
		default:
			break;
	}
}

#if (CPPDEVTK_DETAIL_GUI_CAPS_LOCK_WIDGET_ENABLE_TEST)

void CapsLockWidget::keyPressEvent(QKeyEvent* pKeyEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pKeyEvent);
	
	CPPDEVTK_LOG_INFO("got QKeyEvent");
	
	QWidget::keyPressEvent(pKeyEvent);
	
	switch (pKeyEvent->key()) {
		case Qt::Key_CapsLock:
			CPPDEVTK_LOG_INFO("Qt::Key_CapsLock");
			Refresh();
			break;
		default:
			break;
	}
}

void CapsLockWidget::keyReleaseEvent(QKeyEvent* pKeyEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pKeyEvent);
	
	CPPDEVTK_LOG_INFO("got QKeyEvent");
	
	QWidget::keyReleaseEvent(pKeyEvent);
	
	switch (pKeyEvent->key()) {
		case Qt::Key_CapsLock:
			CPPDEVTK_LOG_INFO("Qt::Key_CapsLock");
			Refresh();
			break;
		default:
			break;
	}
}

#endif	// (CPPDEVTK_DETAIL_GUI_CAPS_LOCK_WIDGET_ENABLE_TEST)


}	// namespace gui
}	// namespace cppdevtk
