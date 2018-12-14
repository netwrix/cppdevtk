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


#include "widget.hpp"
#include <cppdevtk/gui/application.hpp>
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/info_tr.hpp>

#include <QtGui/QIcon>
#include <QtGui/QCloseEvent>


#define CPPDEVTK_TEST_SLOT_THROW_ENABLE_STD_EXC 0


namespace cppdevtk {
namespace test_slot_throw {


Widget::Widget(QWidget* pParent): QWidget(pParent), WidgetBase(), Ui::Widget() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	setupUi(this);
	
	SetStyleSheetFromFileCross(":/cppdevtk/test_caps_lock_widget/res/qss", "widget");
	setWindowIcon(QIcon(":/cppdevtk/test_caps_lock_widget/res/ico/application.ico"));
	
	CPPDEVTK_ASSERT(pRethrowRadioButton_->isChecked());
	
	CPPDEVTK_VERIFY(connect(pRethrowRadioButton_, SIGNAL(toggled(bool)),
			SLOT(OnRethrowRadioButtonToggled(bool))));
	CPPDEVTK_VERIFY(connect(pQuitRadioButton_, SIGNAL(toggled(bool)),
			SLOT(OnQuitRadioButtonToggled(bool))));
	CPPDEVTK_VERIFY(connect(pReturnFalseRadioButton_, SIGNAL(toggled(bool)),
			SLOT(OnReturnFalseRadioButtonToggled(bool))));
	CPPDEVTK_VERIFY(connect(pThrowPushButton_, SIGNAL(clicked()),
			SLOT(OnThrowPushButtonClicked())));
	
	adjustSize();
}

Widget::~Widget() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

void Widget::changeEvent(QEvent* pEvent) {
	QWidget::changeEvent(pEvent);
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}

void Widget::closeEvent(QCloseEvent* pCloseEvent) {
	using ::cppdevtk::gui::MessageBox;
	
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	MessageBox::StandardButton retValue = MessageBox::Question(this, "test", "Do you want to quit?",
			(MessageBox::Yes | MessageBox::No));
	if (retValue == MessageBox::Yes) {
		pCloseEvent->accept();
	}
	else {
		pCloseEvent->ignore();
	}
}

void Widget::OnRethrowRadioButtonToggled(bool checked) {
	if (checked) {
		Q_EMIT NotifyThrowChanged(gui::Application::ntaRethrow);
	}
}

void Widget::OnQuitRadioButtonToggled(bool checked) {
	if (checked) {
		Q_EMIT NotifyThrowChanged(gui::Application::ntaQuit);
	}
}

void Widget::OnReturnFalseRadioButtonToggled(bool checked) {
	if (checked) {
		Q_EMIT NotifyThrowChanged(gui::Application::ntaAbsorb);
	}
}

void Widget::OnThrowPushButtonClicked() {
#	if (CPPDEVTK_TEST_SLOT_THROW_ENABLE_STD_EXC)
	throw ::std::runtime_error("thrown in slot");
#	else
	throw CPPDEVTK_RUNTIME_EXCEPTION_W_CAUSE("something went wrong on runtime", CPPDEVTK_OVERFLOW_EXCEPTION("overflowing..."));
#	endif
}


}	// namespace test_slot_throw
}	// namespace cppdevtk
