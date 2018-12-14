/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#include <cppdevtk/gui/timed_computer_management_widget.hpp>
#include "ui_timed_computer_management_widget.h"
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/stdexcept.hpp>

#include <new>


namespace cppdevtk {
namespace gui {


TimedComputerManagementWidget::TimedComputerManagementWidget(QWidget* pParent): QWidget(pParent), WidgetBase(),
		pUiTimedComputerManagementWidget_(new Ui::TimedComputerManagementWidget()) {
	// NOTE: do not global qualify because moc will generate bad code
	if (QMetaType::type("cppdevtk::gui::TimedComputerManagementWidget::Method")
#			if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
			== QMetaType::UnknownType) {
#			else
			== 0) {
#			endif
		if (qRegisterMetaType< ::cppdevtk::gui::TimedComputerManagementWidget::Method>(
				"cppdevtk::gui::TimedComputerManagementWidget::Method")
#				if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
				== QMetaType::UnknownType) {
#				else
				== 0) {
#				endif
			throw CPPDEVTK_RUNTIME_EXCEPTION("failed to register metatype cppdevtk::gui::TimedComputerManagementWidget::Method");
		}
	}
	
	pUiTimedComputerManagementWidget_->setupUi(this);
	ValidateUi();
	
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "timed_computer_management_widget");
	
	MakeConnections();
}

TimedComputerManagementWidget::~TimedComputerManagementWidget() {
	delete pUiTimedComputerManagementWidget_;
}

bool TimedComputerManagementWidget::IsChecked() const {
	return pUiTimedComputerManagementWidget_->pGroupBoxTimedComputerManagement_->isChecked();
}

TimedComputerManagementWidget::Method TimedComputerManagementWidget::GetCurrentMethod() const {
	if (pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementNone_->isChecked()) {
		return cmmNone;
	}
	
	if (pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLock_->isChecked()) {
		return cmmLockComputer;
	}
	
	if (pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLogout_->isChecked()) {
		return cmmLogoutUser;
	}
	
	CPPDEVTK_ASSERT(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementShutdown_->isChecked());
	return cmmShutdownComputer;
}

void TimedComputerManagementWidget::AddTimeout(int timeout) {
	pUiTimedComputerManagementWidget_->pComboBoxTimeout_->addItem(QString::number(timeout));
}

int TimedComputerManagementWidget::GetTimeoutCount() const {
	return pUiTimedComputerManagementWidget_->pComboBoxTimeout_->count();
}

int TimedComputerManagementWidget::GetTimeout(int index) const {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((0 <= index) && (index < GetTimeoutCount())), "index out of range");
	
	bool convOk = false;
	const int kTimeout = pUiTimedComputerManagementWidget_->pComboBoxTimeout_->itemText(index).toInt(&convOk);
	CPPDEVTK_ASSERT(convOk);
	::cppdevtk::base::SuppressUnusedWarning(convOk);
	
	return kTimeout;
}

void TimedComputerManagementWidget::RemoveTimeoutItem(int index) {
	pUiTimedComputerManagementWidget_->pComboBoxTimeout_->removeItem(index);
}

int TimedComputerManagementWidget::GetCurrentTimeoutIndex() const {
	return pUiTimedComputerManagementWidget_->pComboBoxTimeout_->currentIndex();
}

void TimedComputerManagementWidget::SetCurrentTimeout(int timeout) {
	SetCurrentTimeoutIndex(pUiTimedComputerManagementWidget_->pComboBoxTimeout_->findText(QString::number(timeout)));
}

void TimedComputerManagementWidget::SetChecked(bool value) {
	pUiTimedComputerManagementWidget_->pGroupBoxTimedComputerManagement_->setChecked(value);
}

void TimedComputerManagementWidget::SetCurrentMethod(::cppdevtk::gui::TimedComputerManagementWidget::Method method) {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((cmmFirst <= method) && (method <= cmmLast)),
			"method is not valid Method enum value");
	
	switch (method) {
		case cmmNone:
			pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementNone_->setChecked(true);
			break;
		case cmmLockComputer:
			pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLock_->setChecked(true);
			break;
		case cmmLogoutUser:
			pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLogout_->setChecked(true);
			break;
		case cmmShutdownComputer:
			pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementShutdown_->setChecked(true);
			break;
		default:
			break;
	}
}

void TimedComputerManagementWidget::ClearTimeout() {
	pUiTimedComputerManagementWidget_->pComboBoxTimeout_->clear();
}

void TimedComputerManagementWidget::SetCurrentTimeoutIndex(int index) {
	pUiTimedComputerManagementWidget_->pComboBoxTimeout_->setCurrentIndex(index);
}

void TimedComputerManagementWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiTimedComputerManagementWidget_->retranslateUi(this);
			break;
		default:
			break;
	}
}

void TimedComputerManagementWidget::OnMethodRadioButtonToggled(bool checked) {
	if (!checked) {
		return;
	}
	
	if (pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementNone_->isChecked()) {
		Q_EMIT CurrentMethodChanged(cmmNone);
		return;
	}
	
	if (pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLock_->isChecked()) {
		Q_EMIT CurrentMethodChanged(cmmLockComputer);
		return;
	}
	
	if (pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLogout_->isChecked()) {
		Q_EMIT CurrentMethodChanged(cmmLogoutUser);
		return;
	}
	
	CPPDEVTK_ASSERT(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementShutdown_->isChecked());
	Q_EMIT CurrentMethodChanged(cmmShutdownComputer);
}

void TimedComputerManagementWidget::OnCurrentTimeoutIndexChanged(const QString& text) {
	bool convOk = false;
	const int kTimeout = text.toInt(&convOk);
	CPPDEVTK_ASSERT(convOk);
	::cppdevtk::base::SuppressUnusedWarning(convOk);
	
	Q_EMIT CurrentTimeoutChanged(kTimeout);
}

void TimedComputerManagementWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(pUiTimedComputerManagementWidget_->pGroupBoxTimedComputerManagement_->isCheckable());
	CPPDEVTK_ASSERT(pUiTimedComputerManagementWidget_->pComboBoxTimeout_->insertPolicy() == QComboBox::InsertAlphabetically);
	CPPDEVTK_ASSERT(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementNone_->isChecked());
	
	CPPDEVTK_ASSERT(pUiTimedComputerManagementWidget_->pGridLayout_->contentsMargins() == QMargins(0, 0, 0, 0));
}

void TimedComputerManagementWidget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pUiTimedComputerManagementWidget_->pGroupBoxTimedComputerManagement_, SIGNAL(toggled(bool)),
			SIGNAL(Toggled(bool))));
	
	CPPDEVTK_VERIFY(connect(pUiTimedComputerManagementWidget_->pComboBoxTimeout_, SIGNAL(currentIndexChanged(QString)),
			SLOT(OnCurrentTimeoutIndexChanged(QString))));
	
	CPPDEVTK_VERIFY(connect(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementNone_, SIGNAL(toggled(bool)),
			SLOT(OnMethodRadioButtonToggled(bool))));
	CPPDEVTK_VERIFY(connect(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLock_, SIGNAL(toggled(bool)),
			SLOT(OnMethodRadioButtonToggled(bool))));
	CPPDEVTK_VERIFY(connect(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementLogout_, SIGNAL(toggled(bool)),
			SLOT(OnMethodRadioButtonToggled(bool))));
	CPPDEVTK_VERIFY(connect(pUiTimedComputerManagementWidget_->pRadioButtonComputerManagementShutdown_, SIGNAL(toggled(bool)),
			SLOT(OnMethodRadioButtonToggled(bool))));
}


}	// namespace gui
}	// namespace cppdevtk
