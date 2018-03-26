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


#ifndef CPPDEVTK_GUI_MESSAGE_BOX_HPP_INCLUDED_
#define CPPDEVTK_GUI_MESSAGE_BOX_HPP_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <windows.h>
#undef MessageBox
#endif
#include "widget_base.hpp"
#include <cppdevtk/base/exception.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

#include <cstddef>


namespace cppdevtk {
namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Extends the QMessageBox static functions API to support icon pixmap, detailedText and exception.
class CPPDEVTK_GUI_API MessageBox: public QMessageBox, public WidgetBase {
	Q_OBJECT
public:
	explicit MessageBox(QWidget* pParent = NULL);
	MessageBox(Icon icon, const QString& title, const QString& text, StandardButtons buttons = NoButton,
			QWidget* pParent = NULL, Qt::WindowFlags windowFlags = Qt::WindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint));
	virtual ~MessageBox();
	
	
	static StandardButton Critical(QWidget* pParent, const QString& title, const QString& text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Information(QWidget* pParent, const QString& title, const QString& text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Question(QWidget* pParent, const QString& title, const QString& text,
			StandardButtons buttons = (Yes | No), StandardButton defaultButton = NoButton);
	static StandardButton Warning(QWidget* pParent, const QString& title, const QString& text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	
	static StandardButton Critical(QWidget* pParent, const QString& title, const QString& text, const QString& detailedText,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Information(QWidget* pParent, const QString& title, const QString& text, const QString& detailedText,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Question(QWidget* pParent, const QString& title, const QString& text, const QString& detailedText,
			StandardButtons buttons = (Yes | No), StandardButton defaultButton = NoButton);
	static StandardButton Warning(QWidget* pParent, const QString& title, const QString& text, const QString& detailedText,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	
	static StandardButton Critical(QWidget* pParent, const QString& title, const QString& text, const ::std::exception& exc,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Information(QWidget* pParent, const QString& title, const QString& text, const ::std::exception& exc,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Question(QWidget* pParent, const QString& title, const QString& text, const ::std::exception& exc,
			StandardButtons buttons = (Yes | No), StandardButton defaultButton = NoButton);
	static StandardButton Warning(QWidget* pParent, const QString& title, const QString& text, const ::std::exception& exc,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	
	
	static StandardButton Critical(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Information(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Question(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			StandardButtons buttons = (Yes | No), StandardButton defaultButton = NoButton);
	static StandardButton Warning(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	
	static StandardButton Critical(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const QString& detailedText, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Information(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const QString& detailedText, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Question(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const QString& detailedText, StandardButtons buttons = (Yes | No), StandardButton defaultButton = NoButton);
	static StandardButton Warning(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const QString& detailedText, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	
	static StandardButton Critical(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const ::std::exception& exc, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Information(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const ::std::exception& exc, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
	static StandardButton Question(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const ::std::exception& exc, StandardButtons buttons = (Yes | No), StandardButton defaultButton = NoButton);
	static StandardButton Warning(QWidget* pParent, const QPixmap& iconPixmap, const QString& title, const QString& text,
			const ::std::exception& exc, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton);
private:
	Q_DISABLE_COPY(MessageBox)
	
	// NOTE: this function (including implementation) was taken from Qt 4.8.7 showNewMessageBox()
	// and added detailedText + pIconPixmap
	static StandardButton ExecNewMessageBox(QWidget* pParent, Icon icon, const QPixmap* pIconPixmap,
			const QString& title, const QString& text, const QString& detailedText,
			StandardButtons buttons, StandardButton defaultButton);
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline MessageBox::MessageBox(QWidget* pParent): QMessageBox(pParent), WidgetBase() {
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "message_box");
}

inline MessageBox::MessageBox(Icon icon, const QString& title, const QString& text, StandardButtons buttons,
		QWidget* pParent, Qt::WindowFlags windowFlags): QMessageBox(icon, title, text, buttons, pParent, windowFlags),
		WidgetBase() {
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "message_box");
}

inline MessageBox::~MessageBox() {}

inline MessageBox::StandardButton MessageBox::Critical(QWidget* pParent, const QString& title, const QString& text,
		StandardButtons buttons, StandardButton defaultButton) {
	return Critical(pParent, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Information(QWidget* pParent, const QString& title, const QString& text,
		StandardButtons buttons, StandardButton defaultButton) {
	return Information(pParent, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Question(QWidget* pParent, const QString& title, const QString& text,
		StandardButtons buttons, StandardButton defaultButton) {
	return Question(pParent, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Warning(QWidget* pParent, const QString& title, const QString& text,
		StandardButtons buttons, StandardButton defaultButton) {
	return Warning(pParent, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Critical(QWidget* pParent, const QString& title, const QString& text,
		const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Critical, NULL, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Information(QWidget* pParent, const QString& title, const QString& text,
		const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Information, NULL, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Question(QWidget* pParent, const QString& title, const QString& text,
		const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Question, NULL, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Warning(QWidget* pParent, const QString& title, const QString& text,
		const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Warning, NULL, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Critical(QWidget* pParent, const QString& title, const QString& text,
		const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Critical(pParent, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Information(QWidget* pParent, const QString& title, const QString& text,
		const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Information(pParent, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Question(QWidget* pParent, const QString& title, const QString& text,
		const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Question(pParent, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Warning(QWidget* pParent, const QString& title, const QString& text,
		const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Warning(pParent, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}


inline MessageBox::StandardButton MessageBox::Critical(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, StandardButtons buttons, StandardButton defaultButton) {
	return Critical(pParent, iconPixmap, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Information(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, StandardButtons buttons, StandardButton defaultButton) {
	return Information(pParent, iconPixmap, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Question(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, StandardButtons buttons, StandardButton defaultButton) {
	return Question(pParent, iconPixmap, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Warning(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, StandardButtons buttons, StandardButton defaultButton) {
	return Warning(pParent, iconPixmap, title, text, QString(), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Critical(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Critical, &iconPixmap, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Information(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Information, &iconPixmap, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Question(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Question, &iconPixmap, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Warning(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const QString& detailedText, StandardButtons buttons, StandardButton defaultButton) {
	return ExecNewMessageBox(pParent, QMessageBox::Warning, &iconPixmap, title, text, detailedText, buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Critical(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Critical(pParent, iconPixmap, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Information(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Information(pParent, iconPixmap, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Question(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Question(pParent, iconPixmap, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}

inline MessageBox::StandardButton MessageBox::Warning(QWidget* pParent, const QPixmap& iconPixmap, const QString& title,
		const QString& text, const ::std::exception& exc, StandardButtons buttons, StandardButton defaultButton) {
	return Warning(pParent, iconPixmap, title, text, ::cppdevtk::base::Exception::GetDetailedInfo(exc), buttons, defaultButton);
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_MESSAGE_BOX_HPP_INCLUDED_
