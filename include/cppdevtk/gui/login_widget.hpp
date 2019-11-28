/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_GUI_LOGIN_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_LOGIN_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"
#include <cppdevtk/util/password_requirements.hpp>

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


namespace Ui {
	class LoginWidget;
}


namespace cppdevtk {
namespace gui {


class CPPDEVTK_GUI_API LoginWidget: public QWidget, public WidgetBase {
	Q_OBJECT
Q_SIGNALS:
	void RequestPasswordHint();
	
	void ShowCharactersToggled(bool checked);
	
	void ReadOnlyToggled(bool checked);
	
	void ValidChanged(bool value);
public:
	explicit LoginWidget(QWidget* pParent = NULL);
	virtual ~LoginWidget();
	
	QString GetPassword() const;
	int GetPasswordMaxLength() const;
	
	QString GetPasswordHint() const;
	
	bool IsShowCharactersChecked() const;
	
	bool IsReadOnlyAllowed() const;
	bool IsReadOnlyChecked() const;	///< \pre IsReadOnlyAllowed()
	
	void SetRequirements(const ::cppdevtk::util::PasswordRequirements& passwordRequirements);
	::cppdevtk::util::PasswordRequirements GetRequirements() const;
	
	bool IsValid() const;
	
	void SetMarkPasswordColorEnabled(bool enabled);
	bool IsMarkPasswordColorEnabled() const;
	
	void SetAcceptableColor(const QColor& color);
	QColor GetAcceptableColor() const;
	
	void SetIntermediateColor(const QColor& color);
	QColor GetIntermediateColor() const;
	
	void SetInvalidColor(const QColor& color);
	QColor GetInvalidColor() const;
	
	void SetPasswordHintTextColor(const QColor& color);	///< \note default is Qt::darkBlue
	QColor GetPasswordHintTextColor() const;
	
	void SetCapsLockOnColor(const QColor& color);
	QColor GetCapsLockOnColor() const;
	
	void SetCapsLockOffColor(const QColor& color);
	QColor GetCapsLockOffColor() const;
public Q_SLOTS:
	void Clear();
	
	void SetTitle(const QString& title);
	
	void SetShowCharactersChecked(bool checked);
	
	void SetPasswordHint(const QString& passwordHint);
	void SetPasswordHintVisible(bool visible);
	
	void SetReadOnlyAllowed(bool allowed);
	void SetReadOnlyChecked(bool checked);	///< \pre IsReadOnlyAllowed()
	
	void RefreshCapsLockWidget();
protected:
	virtual void changeEvent(QEvent* pEvent);
private Q_SLOTS:
	void ShowCharacters(bool show);
	bool Validate();
private:
	Q_DISABLE_COPY(LoginWidget)
	
	void ValidateUi() const;
	void MakeConnections();
	void SetupStrings();
	
	void MarkPasswordAcceptable();
	void MarkPasswordIntermediate();
	void MarkPasswordInvalid();
	
	void DoValidate();
	
	
	Ui::LoginWidget* pUiLoginWidget_;
	bool readOnlyAllowed_;
	::cppdevtk::util::PasswordRequirements passwordRequirements_;
	bool isValid_;
	bool isMarkPasswordColorEnabled_;
	QColor acceptableColor_;
	QColor intermediateColor_;
	QColor invalidColor_;
	QColor passwordHintTextColor_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline bool LoginWidget::IsReadOnlyAllowed() const {
	return readOnlyAllowed_;
}

inline ::cppdevtk::util::PasswordRequirements LoginWidget::GetRequirements() const {
	return passwordRequirements_;
}

inline bool LoginWidget::IsValid() const {
	return isValid_;
}

inline void LoginWidget::SetMarkPasswordColorEnabled(bool enabled) {
	isMarkPasswordColorEnabled_ = enabled;
	
	Validate();
}

inline bool LoginWidget::IsMarkPasswordColorEnabled() const {
	return isMarkPasswordColorEnabled_;
}

inline void LoginWidget::SetAcceptableColor(const QColor& color) {
	acceptableColor_ = color;
	
	Validate();
}

inline QColor LoginWidget::GetAcceptableColor() const {
	return acceptableColor_;
}

inline void LoginWidget::SetIntermediateColor(const QColor& color) {
	intermediateColor_ = color;
	
	Validate();
}

inline QColor LoginWidget::GetIntermediateColor() const {
	return intermediateColor_;
}

inline void LoginWidget::SetInvalidColor(const QColor& color) {
	invalidColor_ = color;
	
	Validate();
}

inline QColor LoginWidget::GetInvalidColor() const {
	return invalidColor_;
}

inline void LoginWidget::SetPasswordHintTextColor(const QColor& color) {
	passwordHintTextColor_ = color;
	
	SetupStrings();
}

inline QColor LoginWidget::GetPasswordHintTextColor() const {
	return passwordHintTextColor_;
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_LOGIN_WIDGET_HPP_INCLUDED_
