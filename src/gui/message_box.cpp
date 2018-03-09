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


#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>
#else
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPushButton>
#endif


namespace cppdevtk {
namespace gui {


// NOTE: this function (including implementation) was taken from Qt 4.8.7 showNewMessageBox() and added detailedText
MessageBox::StandardButton MessageBox::ExecNewMessageBox(QWidget* pParent, Icon icon, const QPixmap* pIconPixmap,
		const QString& title, const QString& text, const QString& detailedText,
		StandardButtons buttons, StandardButton defaultButton) {
	// MessageBoxPrivate not accesible here
	// TODO: investigate
	/*
	// necessary for source compatibility with Qt 4.0 and 4.1
	// handles (Yes, No) and (Yes|Default, No)
	if (defaultButton && !(buttons & defaultButton)) {
		return (MessageBox::StandardButton)QMessageBoxPrivate::showOldMessageBox(pParent, icon, title,
				text, int(buttons), int(defaultButton), 0);
	}
	*/
	
	MessageBox msgBox(icon, title, text, NoButton, pParent);
	if (pIconPixmap != NULL) {
		msgBox.setIconPixmap(*pIconPixmap);
	}
	msgBox.setDetailedText(detailedText);
	
	QDialogButtonBox *buttonBox = msgBox.findChild<QDialogButtonBox*>();
	CPPDEVTK_ASSERT(buttonBox != NULL);
	
	uint mask = FirstButton;
	while (mask <= LastButton) {
		uint sb = buttons & mask;
		mask <<= 1;
		if (!sb) {
			continue;
		}
		
		QPushButton *button = msgBox.addButton((StandardButton)sb);
		// Choose the first accept role as the default
		if (msgBox.defaultButton()) {
			continue;
		}
		
		if (((defaultButton == NoButton) && (buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole))
				|| ((defaultButton != NoButton) && (sb == uint(defaultButton)))) {
			msgBox.setDefaultButton(button);
		}
	}
	
	if (msgBox.exec() == -1) {
		return Cancel;
	}
	
	return msgBox.standardButton(msgBox.clickedButton());
}


}	// namespace gui
}	// namespace cppdevtk
