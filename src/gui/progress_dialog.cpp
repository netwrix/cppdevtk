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


#include <cppdevtk/gui/progress_dialog.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace cppdevtk {
namespace gui {


ProgressDialog::ProgressDialog(QWidget* pParent, Qt::WindowFlags windowFlags):
		QProgressDialog(pParent, windowFlags), WidgetBase(), isCancelable_(false) {
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "progress_dialog");
	
	Initialize();
	if (windowFlags & Qt::WindowCloseButtonHint) {
		SetCancelable(tr("&Cancel"), false);
	}
}

ProgressDialog::ProgressDialog(const QString& labelText, const QString& cancelButtonText, int minimum, int maximum,
		QWidget* pParent, Qt::WindowFlags windowFlags): QProgressDialog(labelText, cancelButtonText, minimum, maximum,
		pParent, windowFlags), WidgetBase(), isCancelable_(false) {
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "progress_dialog");
	
	Initialize();
	SetCancelable(cancelButtonText, false);
}

void ProgressDialog::SetCancelable(const QString& cancelButtonText, bool processEvents) {
	isCancelable_ = !cancelButtonText.isEmpty();
	if (isCancelable_) {
		setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) | Qt::WindowCloseButtonHint);
		setCancelButtonText(cancelButtonText);
	}
	else {
		setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);
		setCancelButton(NULL);
	}
	
	if (processEvents) {
		QApplication::processEvents();
	}
}

void ProgressDialog::SetNumStages(int numStages) {
	CPPDEVTK_DBC_CHECK_IN_RANGE((numStages >= 1), "numStages");
	
	setAutoClose(false);
	setAutoReset(false);
	
	const int kValue = numStages - 1;
	setRange(0, kValue);
	//setValue(kValue);
	
	//QApplication::processEvents();
}

void ProgressDialog::SetStage(int numStage, const QString& labelText) {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((1 <= numStage) && (numStage <= (maximum() + 1))), "numStage");
	
	setLabelText(tr("%1 of %2: %3").arg(QString::number(numStage), QString::number((maximum() + 1)), labelText));
	setValue(numStage - 1);
	
	QApplication::processEvents();
}

void ProgressDialog::IncrementStage(const QString& labelText) {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((value() + 1) >= 1), " value not in range; call SetStage() with numStage 1 before IncrementStage()");
	
	CPPDEVTK_DBC_CHECK_IN_RANGE(((1 <= (value() + 1)) && (((value() + 1) + 1) <= (maximum() + 1))), "value");
	
	SetStage(((value() + 1) + 1), labelText);
}

void ProgressDialog::ShowWait(const QString& labelText, int msMinimumDuration) {
	//setWindowModality(Qt::WindowModal);
	setMinimumDuration(msMinimumDuration);
	setAutoClose(false);
	setAutoReset(false);
	SetCancelable("");
	setLabelText(labelText);
	setRange(0, 1);
	setValue(0);
	setRange(0, 0);
	
	QApplication::processEvents();
}

void ProgressDialog::ShowBussy(const QString& labelText, const QString& cancelButtonText, int msMinimumDuration) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(cancelButtonText.isEmpty(), "cancelButtonText");
	
	//setWindowModality(Qt::WindowModal);
	setMinimumDuration(msMinimumDuration);
	setAutoClose(false);
	setAutoReset(false);
	SetCancelable(cancelButtonText);
	setLabelText(labelText);
	setRange(0, 1);
	setValue(0);
	setRange(0, 0);
	
	QApplication::processEvents();
}

void ProgressDialog::Initialize() {
	setWindowModality(Qt::WindowModal);
	setMinimumDuration(0);
	setAutoClose(true);
	setAutoReset(true);
	SetCancelable("", false);
}


}	// namespace gui
}	// namespace cppdevtk
