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


#include <cppdevtk/gui/login_widget.hpp>

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include "ui_login_widget.h"
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/string_utils.hpp>
#include <cppdevtk/base/zeroize.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/Qt>

#include <new>


namespace cppdevtk {
namespace gui {


LoginWidget::LoginWidget(QWidget* pParent): QWidget(pParent), WidgetBase(), pUiLoginWidget_(new Ui::LoginWidget()),
		readOnlyAllowed_(true), passwordRequirements_(), isValid_(false), isMarkPasswordColorEnabled_(true),
		acceptableColor_(Qt::darkGreen), intermediateColor_(Qt::darkYellow), invalidColor_(Qt::darkRed),
		passwordHintTextColor_(Qt::darkBlue) {
	CPPDEVTK_ASSERT(passwordRequirements_.HasRequirements());
	CPPDEVTK_ASSERT(passwordRequirements_.GetPwdMaxLen() <= 0);
	passwordRequirements_.ClearNew();
	passwordRequirements_.ClearHint();
	
	pUiLoginWidget_->setupUi(this);
	SetupStrings();
	pUiLoginWidget_->pGroupBox_->setTitle("");
	ValidateUi();
	
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "login_widget");
	
	Clear();
	
	MakeConnections();
}

LoginWidget::~LoginWidget() {
	pUiLoginWidget_->pLineEditPassword_->clear();
	
	delete pUiLoginWidget_;
}

QString LoginWidget::GetPassword() const {
	return pUiLoginWidget_->pLineEditPassword_->text();
}

int LoginWidget::GetPasswordMaxLength() const {
	return pUiLoginWidget_->pLineEditPassword_->maxLength();
}

QString LoginWidget::GetPasswordHint() const {
	return pUiLoginWidget_->pLabelPasswordHintValue_->text();
}

bool LoginWidget::IsShowCharactersChecked() const {
	return pUiLoginWidget_->pCheckBoxShowCharacters_->isChecked();
}

bool LoginWidget::IsReadOnlyChecked() const {
	CPPDEVTK_DBC_CHECK_STATE(IsReadOnlyAllowed(), "read only allowed", "read only not allowed");
	
	return pUiLoginWidget_->pCheckBoxReadOnly_->isChecked();
}

void LoginWidget::SetRequirements(const ::cppdevtk::util::PasswordRequirements& passwordRequirements) {
	passwordRequirements_ = passwordRequirements;
	passwordRequirements_.ClearNew();
	passwordRequirements_.ClearHint();
	
	const int kPwdMaxLen = passwordRequirements_.GetPwdMaxLen();
	if (kPwdMaxLen > 0) {
		pUiLoginWidget_->pLineEditPassword_->setMaxLength(kPwdMaxLen);
	}
	else {
		pUiLoginWidget_->pLineEditPassword_->setMaxLength(util::PasswordRequirements::kQLineEditMaxLength_);
	}
	
	Validate();
}

void LoginWidget::SetCapsLockOnColor(const QColor& color) {
	pUiLoginWidget_->pCapsLockWidget_->SetOnColor(color);
}

QColor LoginWidget::GetCapsLockOnColor() const {
	return pUiLoginWidget_->pCapsLockWidget_->GetOnColor();
}

void LoginWidget::SetCapsLockOffColor(const QColor& color) {
	pUiLoginWidget_->pCapsLockWidget_->SetOffColor(color);
}

QColor LoginWidget::GetCapsLockOffColor() const {
	return pUiLoginWidget_->pCapsLockWidget_->GetOffColor();
}

void LoginWidget::Clear() {
	pUiLoginWidget_->pLineEditPassword_->setText("");
	pUiLoginWidget_->pLabelPasswordHintValue_->setText("");
	
	Validate();
}

void LoginWidget::SetTitle(const QString& title) {
	pUiLoginWidget_->pGroupBox_->setTitle(title);
}

void LoginWidget::SetShowCharactersChecked(bool checked) {
	pUiLoginWidget_->pCheckBoxShowCharacters_->setChecked(checked);
}

void LoginWidget::SetPasswordHint(const QString& passwordHint) {
	pUiLoginWidget_->pLabelPasswordHintValue_->setText(passwordHint);
}

void LoginWidget::SetPasswordHintVisible(bool visible) {
	pUiLoginWidget_->pVerticalSpacerReadOnly_->changeSize(
			pUiLoginWidget_->pVerticalSpacerReadOnly_->sizeHint().width(), (visible ? 13 : 0),
			QSizePolicy::Expanding, (visible ? QSizePolicy::Expanding : QSizePolicy::Fixed));
	pUiLoginWidget_->pLabelPasswordHint_->setVisible(visible);
	pUiLoginWidget_->pLabelPasswordHintValue_->setVisible(visible);
}

void LoginWidget::SetReadOnlyAllowed(bool allowed) {
	readOnlyAllowed_ = allowed;
	pUiLoginWidget_->pVerticalSpacerReadOnly_->changeSize(
			pUiLoginWidget_->pVerticalSpacerReadOnly_->sizeHint().width(), (allowed ? 13 : 0),
			QSizePolicy::Expanding, (allowed ? QSizePolicy::Expanding : QSizePolicy::Fixed));
	pUiLoginWidget_->pCheckBoxReadOnly_->setVisible(allowed);
}

void LoginWidget::SetReadOnlyChecked(bool checked) {
	CPPDEVTK_DBC_CHECK_STATE(IsReadOnlyAllowed(), "read only allowed", "read only not allowed");
	
	pUiLoginWidget_->pCheckBoxReadOnly_->setChecked(checked);
}

void LoginWidget::RefreshCapsLockWidget() {
	pUiLoginWidget_->pCapsLockWidget_->Refresh();
}

void LoginWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiLoginWidget_->retranslateUi(this);
			SetupStrings();
			Validate();
			break;
		default:
			break;
	}
}

void LoginWidget::ShowCharacters(bool show) {
	pUiLoginWidget_->pLineEditPassword_->setEchoMode(show ? QLineEdit::Normal : QLineEdit::Password);
}

bool LoginWidget::Validate() {
	const bool kWasValid = isValid_;
	isValid_ = true;
	DoValidate();
	if (kWasValid != isValid_) {
		emit ValidChanged(isValid_);
	}
	return isValid_;
}

void LoginWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(pUiLoginWidget_->pLineEditPassword_->echoMode() == QLineEdit::Password);
	CPPDEVTK_ASSERT(pUiLoginWidget_->pLabelPasswordHintValue_->wordWrap());
	CPPDEVTK_ASSERT(pUiLoginWidget_->pVerticalLayout_->contentsMargins() == QMargins(0, 0, 0, 0));
}

void LoginWidget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pUiLoginWidget_->pLineEditPassword_, SIGNAL(textEdited(QString)), this, SLOT(Validate())));
	
	CPPDEVTK_VERIFY(connect(pUiLoginWidget_->pLabelPasswordHint_, SIGNAL(linkActivated(QString)),
			this, SIGNAL(RequestPasswordHint())));
	
	CPPDEVTK_VERIFY(connect(pUiLoginWidget_->pCheckBoxShowCharacters_, SIGNAL(toggled(bool)),
			this, SLOT(ShowCharacters(bool))));
	CPPDEVTK_VERIFY(connect(pUiLoginWidget_->pCheckBoxShowCharacters_, SIGNAL(toggled(bool)),
			this, SIGNAL(ShowCharactersToggled(bool))));
	
	CPPDEVTK_VERIFY(connect(pUiLoginWidget_->pCheckBoxReadOnly_, SIGNAL(toggled(bool)),
			this, SIGNAL(ReadOnlyToggled(bool))));
}

void LoginWidget::SetupStrings() {
	pUiLoginWidget_->pLabelPasswordHint_->setText(QString("<a href=fake_url style=color:%1;>%2</a>").arg(
			passwordHintTextColor_.name(), tr("Password Hint:")));
}

void LoginWidget::MarkPasswordAcceptable() {
	QString styleSheet;
	if (isMarkPasswordColorEnabled_) {
		styleSheet = QString("\nbackground-color: %1;\n").arg(acceptableColor_.name());
	}
	pUiLoginWidget_->pLineEditPassword_->setStyleSheet(styleSheet);
}

void LoginWidget::MarkPasswordIntermediate() {
	QString styleSheet;
	if (isMarkPasswordColorEnabled_) {
		styleSheet = QString("\nbackground-color: %1;\n").arg(intermediateColor_.name());
	}
	pUiLoginWidget_->pLineEditPassword_->setStyleSheet(styleSheet);
}

void LoginWidget::MarkPasswordInvalid() {
	QString styleSheet;
	if (isMarkPasswordColorEnabled_) {
		styleSheet = QString("\nbackground-color: %1;\n").arg(invalidColor_.name());
	}
	pUiLoginWidget_->pLineEditPassword_->setStyleSheet(styleSheet);
}

void LoginWidget::DoValidate() {
	CPPDEVTK_ASSERT(isValid_);
	
	QLineEdit& lineEditPassword = *(pUiLoginWidget_->pLineEditPassword_);
	
	QString password = lineEditPassword.text();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&password)) {
		base::Zeroize(password);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	{
		if (!passwordRequirements_.GetUnicode()) {
			const int kCountNonAscii = base::CountNonAscii(password);
			if (kCountNonAscii > 0) {
				MarkPasswordInvalid();
				CPPDEVTK_LOG_WARN("Password contains " << kCountNonAscii << " non-ASCII characters.");
				isValid_ = false;
				return;
			}
		}
	}
	
	const int kPwdLen = password.length();
	
	{
		int reqMaxLen = passwordRequirements_.GetPwdMaxLen();
		if (reqMaxLen <= 0) {
			reqMaxLen = util::PasswordRequirements::kQLineEditMaxLength_;
		}
		
		if (kPwdLen > reqMaxLen) {
			MarkPasswordInvalid();
			CPPDEVTK_LOG_WARN("Password length " << kPwdLen << " exceeds maximum " << reqMaxLen);
			isValid_ = false;
			return;
		}
	}
	
	{
		const int kReqMinLen = passwordRequirements_.GetPwdMinLen();
		if (kReqMinLen > 0) {
			if (kPwdLen < kReqMinLen) {
				MarkPasswordIntermediate();
				CPPDEVTK_LOG_WARN("Password length " << kPwdLen << "is less than minimum " << kReqMinLen);
				isValid_ = false;
				return;
			}
		}
	}
	
	MarkPasswordAcceptable();
	CPPDEVTK_LOG_INFO("Password OK");
}


}	// namespace gui
}	// namespace cppdevtk
