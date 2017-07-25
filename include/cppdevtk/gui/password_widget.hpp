/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_GUI_PASSWORD_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_PASSWORD_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"
#include <cppdevtk/util/password_requirements.hpp>

#include <QtCore/QMetaType>
#include <QtGui/QColor>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#else
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#endif

#include <cstddef>


namespace Ui {
	class PasswordWidget;
}


namespace cppdevtk {
namespace gui {


/// \brief Set or Change password widget.
class CPPDEVTK_GUI_API PasswordWidget: public QWidget, public WidgetBase {
	Q_OBJECT
	Q_ENUMS(Mode)
signals:
	void ShowCharactersToggled(bool checked);
	void ValidChanged(bool value);
public:
	enum Mode {
		pwmSetPassword,
		pwmChangePassword
	};
	
	
	explicit PasswordWidget(QWidget* pParent = NULL);
	virtual ~PasswordWidget();
	
	void SetMode(::cppdevtk::gui::PasswordWidget::Mode mode);
	Mode GetMode() const;
	
	void SetRequirements(const ::cppdevtk::util::PasswordRequirements& passwordRequirements);
	::cppdevtk::util::PasswordRequirements GetRequirements() const;
	
	QString GetOldPassword() const;	///< \pre GetMode() == pwmChangePassword
	QString GetNewPassword() const;
	QString GetNewPasswordHint() const;
	
	bool IsShowCharactersChecked() const;
	
	bool IsValid() const;
	
	void SetAcceptableColor(const QColor& color);
	QColor GetAcceptableColor() const;
	
	void SetIntermediateColor(const QColor& color);
	QColor GetIntermediateColor() const;
	
	void SetInvalidColor(const QColor& color);
	QColor GetInvalidColor() const;
	
	void SetCapsLockOnColor(const QColor& color);
	QColor GetCapsLockOnColor() const;
	
	void SetCapsLockOffColor(const QColor& color);
	QColor GetCapsLockOffColor() const;
public slots:
	void Clear();
	
	void SetShowCharactersChecked(bool checked);
	void SetPasswordHintVisible(bool visible);
	
	void RefreshCapsLockWidget();

	void SetAcceptableValidationMessage(const QString& msg);
	void SetIntermediateValidationMessage(const QString& msg);
	void SetInvalidValidationMessage(const QString& msg);	
protected:
	virtual void changeEvent(QEvent* pEvent);
	
	// make these protected in case someone wish to override Validate()
	
	void SetValid(bool value);
	
	QString GetConfirmNewPassword() const;
	
	void MarkOldPasswordAcceptable();
	void MarkOldPasswordIntermediate();
	void MarkOldPasswordInvalid();
	
	void MarkNewPasswordAcceptable();
	void MarkNewPasswordIntermediate();
	void MarkNewPasswordInvalid();
	
	void MarkConfirmNewPasswordAcceptable();
	void MarkConfirmNewPasswordIntermediate();
	void MarkConfirmNewPasswordInvalid();
	
	void MarkNewPasswordHintAcceptable();
	void MarkNewPasswordHintIntermediate();
	void MarkNewPasswordHintInvalid();
protected slots:
	virtual bool Validate();	// virtual just in case...
private slots:
	void ShowCharacters(bool show);
	void ShowRequirements();
private:
	Q_DISABLE_COPY(PasswordWidget)
	
	void ValidateUi() const;
	void MakeConnections();
	void SetupStrings();
	
	void SetValidationMessage(const QColor& color, const QString& msg);
	
	void MarkLineEdit(const QColor& color, QLineEdit& lineEdit);
	void MarkLineEditAcceptable(QLineEdit& lineEdit);
	void MarkLineEditIntermediate(QLineEdit& lineEdit);
	void MarkLineEditInvalid(QLineEdit& lineEdit);
	
	void DoValidate();
	void DoValidateOldPassword();
	void DoValidateNewPassword();
	void DoValidateConfirmNewPassword();
	void DoValidateNewPasswordHint();
	
	
	Ui::PasswordWidget* pUiPasswordWidget_;
	Mode mode_;
	bool isValid_;
	::cppdevtk::util::PasswordRequirements passwordRequirements_;
	QColor acceptableColor_;
	QColor intermediateColor_;
	QColor invalidColor_;
};


}	// namespace gui
}	// namespace cppdevtk


// NOTE: do not global qualify because moc will generate bad code
Q_DECLARE_METATYPE(cppdevtk::gui::PasswordWidget::Mode)




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

namespace cppdevtk {
namespace gui {


inline PasswordWidget::Mode PasswordWidget::GetMode() const {
	return mode_;
}

inline ::cppdevtk::util::PasswordRequirements PasswordWidget::GetRequirements() const {
	return passwordRequirements_;
}

inline bool PasswordWidget::IsValid() const {
	return isValid_;
}

inline void PasswordWidget::SetAcceptableColor(const QColor& color) {
	acceptableColor_ = color;
	
	Validate();
}

inline QColor PasswordWidget::GetAcceptableColor() const {
	return acceptableColor_;
}

inline void PasswordWidget::SetIntermediateColor(const QColor& color) {
	intermediateColor_ = color;
	
	Validate();
}

inline QColor PasswordWidget::GetIntermediateColor() const {
	return intermediateColor_;
}

inline void PasswordWidget::SetInvalidColor(const QColor& color) {
	invalidColor_ = color;
	
	Validate();
}

inline QColor PasswordWidget::GetInvalidColor() const {
	return invalidColor_;
}

inline void PasswordWidget::SetAcceptableValidationMessage(const QString& msg) {
	SetValidationMessage(acceptableColor_, msg);
}

inline void PasswordWidget::SetIntermediateValidationMessage(const QString& msg) {
	SetValidationMessage(intermediateColor_, msg);
}

inline void PasswordWidget::SetInvalidValidationMessage(const QString& msg) {
	SetValidationMessage(invalidColor_, msg);
}

inline void PasswordWidget::SetValid(bool value) {
	isValid_ = value;
}

inline void PasswordWidget::MarkLineEditAcceptable(QLineEdit& lineEdit) {
	MarkLineEdit(acceptableColor_, lineEdit);
}

inline void PasswordWidget::MarkLineEditIntermediate(QLineEdit& lineEdit) {
	MarkLineEdit(intermediateColor_, lineEdit);
}

inline void PasswordWidget::MarkLineEditInvalid(QLineEdit& lineEdit) {
	MarkLineEdit(invalidColor_, lineEdit);
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_PASSWORD_WIDGET_HPP_INCLUDED_
