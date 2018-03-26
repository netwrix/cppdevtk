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


#include <cppdevtk/gui/password_widget.hpp>

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include "ui_password_widget.h"
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/string_utils.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/zeroize.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/util/damerau_levenshtein_distance.hpp>

#include <QtCore/QtGlobal>

#include <new>


namespace cppdevtk {
namespace gui {


PasswordWidget::PasswordWidget(QWidget* pParent): QWidget(pParent), WidgetBase(),
		pUiPasswordWidget_(new Ui::PasswordWidget()), mode_(pwmChangePassword), isValid_(false), passwordRequirements_(),
		acceptableColor_(Qt::darkGreen), intermediateColor_(Qt::darkYellow), invalidColor_(Qt::darkRed) {
	CPPDEVTK_ASSERT(mode_ == pwmChangePassword);
	CPPDEVTK_ASSERT(passwordRequirements_.HasRequirements());
	CPPDEVTK_ASSERT(passwordRequirements_.GetPwdMaxLen() <= 0);
	CPPDEVTK_ASSERT(passwordRequirements_.GetHintMaxLen() <= 0);
	
	// NOTE: do not global qualify because moc will generate bad code
	if (QMetaType::type("cppdevtk::gui::PasswordWidget::Mode")
#			if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
			== QMetaType::UnknownType) {
#			else
			== 0) {
#			endif
		if (qRegisterMetaType< ::cppdevtk::gui::PasswordWidget::Mode>("cppdevtk::gui::PasswordWidget::Mode")
#				if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
				== QMetaType::UnknownType) {
#				else
				== 0) {
#				endif
			throw CPPDEVTK_RUNTIME_EXCEPTION("failed to register metatype cppdevtk::gui::PasswordWidget::Mode");
		}
	}
	
	pUiPasswordWidget_->setupUi(this);
	SetupStrings();
	ValidateUi();
	
	pUiPasswordWidget_->pCapsLockWidget_->SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "caps_lock_widget");
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "password_widget");
	
	Clear();
	
	MakeConnections();
}

PasswordWidget::~PasswordWidget() {
	pUiPasswordWidget_->pLineEditOldPassword_->clear();
	pUiPasswordWidget_->pLineEditNewPassword_->clear();
	pUiPasswordWidget_->pLineEditConfirmNewPassword_->clear();
	
	delete pUiPasswordWidget_;
}

void PasswordWidget::SetMode(::cppdevtk::gui::PasswordWidget::Mode mode) {
	mode_ = mode;
	pUiPasswordWidget_->pLabelOldPassword_->setVisible(((mode_ == pwmChangePassword) ? true : false));
	pUiPasswordWidget_->pLineEditOldPassword_->setVisible(((mode_ == pwmChangePassword) ? true : false));
	SetupStrings();
	Validate();
}

void PasswordWidget::SetRequirements(const ::cppdevtk::util::PasswordRequirements& passwordRequirements) {
	passwordRequirements_ = passwordRequirements;
	
	pUiPasswordWidget_->pPushButtonShowRequirements_->setVisible(passwordRequirements_.HasRequirements());
	
	const int kPwdMaxLen = passwordRequirements_.GetPwdMaxLen();
	if (kPwdMaxLen > 0) {
		pUiPasswordWidget_->pLineEditOldPassword_->setMaxLength(kPwdMaxLen);
		pUiPasswordWidget_->pLineEditNewPassword_->setMaxLength(kPwdMaxLen);
		pUiPasswordWidget_->pLineEditConfirmNewPassword_->setMaxLength(kPwdMaxLen);
	}
	else {
		pUiPasswordWidget_->pLineEditOldPassword_->setMaxLength(util::PasswordRequirements::kQLineEditMaxLength_);
		pUiPasswordWidget_->pLineEditNewPassword_->setMaxLength(util::PasswordRequirements::kQLineEditMaxLength_);
		pUiPasswordWidget_->pLineEditConfirmNewPassword_->setMaxLength(util::PasswordRequirements::kQLineEditMaxLength_);
	}
	
	const int kHintMaxLen = passwordRequirements_.GetHintMaxLen();
	if (kHintMaxLen > 0) {
		pUiPasswordWidget_->pLineEditNewPasswordHint_->setMaxLength(kHintMaxLen);
	}
	else {
		pUiPasswordWidget_->pLineEditNewPasswordHint_->setMaxLength(util::PasswordRequirements::kQLineEditMaxLength_);
	}
	
	SetupStrings();
	Validate();
}

QString PasswordWidget::GetOldPassword() const {
	CPPDEVTK_DBC_CHECK_STATE((mode_ == pwmChangePassword), "change password mode", "set password mode");
	
	return pUiPasswordWidget_->pLineEditOldPassword_->text();
}

QString PasswordWidget::GetNewPassword() const {
	return pUiPasswordWidget_->pLineEditNewPassword_->text();
}

QString PasswordWidget::GetNewPasswordHint() const {
	return pUiPasswordWidget_->pLineEditNewPasswordHint_->text();
}

bool PasswordWidget::IsShowCharactersChecked() const {
	return pUiPasswordWidget_->pCheckBoxShowCharacters_->isChecked();
}

void PasswordWidget::SetCapsLockOnColor(const QColor& color) {
	pUiPasswordWidget_->pCapsLockWidget_->SetOnColor(color);
}

QColor PasswordWidget::GetCapsLockOnColor() const {
	return pUiPasswordWidget_->pCapsLockWidget_->GetOnColor();
}

void PasswordWidget::SetCapsLockOffColor(const QColor& color) {
	pUiPasswordWidget_->pCapsLockWidget_->SetOffColor(color);
}

QColor PasswordWidget::GetCapsLockOffColor() const {
	return pUiPasswordWidget_->pCapsLockWidget_->GetOffColor();
}

void PasswordWidget::Clear() {
	pUiPasswordWidget_->pLineEditOldPassword_->setText("");
	pUiPasswordWidget_->pLineEditNewPassword_->setText("");
	pUiPasswordWidget_->pLineEditConfirmNewPassword_->setText("");
	pUiPasswordWidget_->pLineEditNewPasswordHint_->setText("");
	pUiPasswordWidget_->pLabelPasswordInfo_->setText("");
	
	Validate();
}

void PasswordWidget::SetShowCharactersChecked(bool checked) {
	pUiPasswordWidget_->pCheckBoxShowCharacters_->setChecked(checked);
}

void PasswordWidget::SetPasswordHintVisible(bool visible) {
	pUiPasswordWidget_->pVerticalSpacerPasswordHint_->changeSize(
			pUiPasswordWidget_->pVerticalSpacerPasswordHint_->sizeHint().width(), (visible ? 13 : 0),
			QSizePolicy::Expanding, (visible ? QSizePolicy::Expanding : QSizePolicy::Fixed));
	pUiPasswordWidget_->pLabelNewPasswordHint_->setVisible(visible);
	pUiPasswordWidget_->pLineEditNewPasswordHint_->setVisible(visible);
}

void PasswordWidget::RefreshCapsLockWidget() {
	pUiPasswordWidget_->pCapsLockWidget_->Refresh();
}

void PasswordWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiPasswordWidget_->retranslateUi(this);
			SetupStrings();
			Validate();
			break;
		default:
			break;
	}
}

QString PasswordWidget::GetConfirmNewPassword() const {
	return pUiPasswordWidget_->pLineEditConfirmNewPassword_->text();
}

void PasswordWidget::MarkOldPasswordAcceptable() {
	MarkLineEditAcceptable(*(pUiPasswordWidget_->pLineEditOldPassword_));
}

void PasswordWidget::MarkOldPasswordIntermediate() {
	MarkLineEditIntermediate(*(pUiPasswordWidget_->pLineEditOldPassword_));
}

void PasswordWidget::MarkOldPasswordInvalid() {
	MarkLineEditInvalid(*(pUiPasswordWidget_->pLineEditOldPassword_));
}

void PasswordWidget::MarkNewPasswordAcceptable() {
	MarkLineEditAcceptable(*(pUiPasswordWidget_->pLineEditNewPassword_));
}

void PasswordWidget::MarkNewPasswordIntermediate() {
	MarkLineEditIntermediate(*(pUiPasswordWidget_->pLineEditNewPassword_));
}

void PasswordWidget::MarkNewPasswordInvalid() {
	MarkLineEditInvalid(*(pUiPasswordWidget_->pLineEditNewPassword_));
}

void PasswordWidget::MarkConfirmNewPasswordAcceptable() {
	MarkLineEditAcceptable(*(pUiPasswordWidget_->pLineEditConfirmNewPassword_));
}

void PasswordWidget::MarkConfirmNewPasswordIntermediate() {
	MarkLineEditIntermediate(*(pUiPasswordWidget_->pLineEditConfirmNewPassword_));
}

void PasswordWidget::MarkConfirmNewPasswordInvalid() {
	MarkLineEditInvalid(*(pUiPasswordWidget_->pLineEditConfirmNewPassword_));
}

void PasswordWidget::MarkNewPasswordHintAcceptable() {
	MarkLineEditAcceptable(*(pUiPasswordWidget_->pLineEditNewPasswordHint_));
}

void PasswordWidget::MarkNewPasswordHintIntermediate() {
	MarkLineEditIntermediate(*(pUiPasswordWidget_->pLineEditNewPasswordHint_));
}

void PasswordWidget::MarkNewPasswordHintInvalid() {
	MarkLineEditInvalid(*(pUiPasswordWidget_->pLineEditNewPasswordHint_));
}

bool PasswordWidget::Validate() {
	const bool kWasValid = isValid_;
	isValid_ = true;
	DoValidate();
	if (kWasValid != isValid_) {
		Q_EMIT ValidChanged(isValid_);
	}
	return isValid_;
}

void PasswordWidget::ShowCharacters(bool show) {
	const QLineEdit::EchoMode kEchoMode = show ? QLineEdit::Normal : QLineEdit::Password;
	pUiPasswordWidget_->pLineEditOldPassword_->setEchoMode(kEchoMode);
	pUiPasswordWidget_->pLineEditNewPassword_->setEchoMode(kEchoMode);
	pUiPasswordWidget_->pLineEditConfirmNewPassword_->setEchoMode(kEchoMode);
}

void PasswordWidget::ShowRequirements() {
	MessageBox::Information(this, tr("Password Requirements"), passwordRequirements_.ToString());
}

void PasswordWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(pUiPasswordWidget_->pLineEditOldPassword_->echoMode() == QLineEdit::Password);
	CPPDEVTK_ASSERT(pUiPasswordWidget_->pLineEditNewPassword_->echoMode() == QLineEdit::Password);
	CPPDEVTK_ASSERT(pUiPasswordWidget_->pLineEditConfirmNewPassword_->echoMode() == QLineEdit::Password);
	
	CPPDEVTK_ASSERT(pUiPasswordWidget_->pVerticalLayout_->contentsMargins() == QMargins(0, 0, 0, 0));
}

void PasswordWidget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pCheckBoxShowCharacters_, SIGNAL(toggled(bool)),
			SLOT(ShowCharacters(bool))));
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pCheckBoxShowCharacters_, SIGNAL(toggled(bool)),
			SIGNAL(ShowCharactersToggled(bool))));
	
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pPushButtonShowRequirements_, SIGNAL(clicked()),
			SLOT(ShowRequirements())));
	
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pLineEditOldPassword_, SIGNAL(textEdited(QString)),
			SLOT(Validate())));
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pLineEditNewPassword_, SIGNAL(textEdited(QString)),
			SLOT(Validate())));
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pLineEditConfirmNewPassword_, SIGNAL(textEdited(QString)),
			SLOT(Validate())));
	CPPDEVTK_VERIFY(connect(pUiPasswordWidget_->pLineEditNewPasswordHint_, SIGNAL(textEdited(QString)),
			SLOT(Validate())));
}

void PasswordWidget::SetupStrings() {
	if (mode_ == pwmChangePassword) {
		pUiPasswordWidget_->pLabelOldPassword_->setText(tr("&Old Password:"));
		pUiPasswordWidget_->pLabelNewPassword_->setText(tr("&New Password:"));
		pUiPasswordWidget_->pLabelConfirmNewPassword_->setText(tr("Confirm New &Password:"));
		pUiPasswordWidget_->pLabelNewPasswordHint_->setText(tr("New Password &Hint:"));
	}
	else {
		//pUiPasswordWidget_->pLabelOldPassword_->setText(tr(""));
		pUiPasswordWidget_->pLabelNewPassword_->setText(tr("&Password:"));
		pUiPasswordWidget_->pLabelConfirmNewPassword_->setText(tr("Confirm P&assword:"));
		pUiPasswordWidget_->pLabelNewPasswordHint_->setText(tr("Password &Hint:"));
	}
}

void PasswordWidget::SetValidationMessage(const QColor& color, const QString& msg) {
	pUiPasswordWidget_->pLabelPasswordInfo_->setText(QString("<FONT COLOR=%1>%2</FONT>").arg(color.name(), msg));
}

void PasswordWidget::MarkLineEdit(const QColor& color, QLineEdit& lineEdit) {
	// FIXME: old stylesheet is lost
	// Possible workarounds:
	// 1. Parse old stylesheet and replace background-color (don't have time now...)
	// 2. Use QPalette; may or may not work depending on platform, style etc...
	lineEdit.setStyleSheet(QString("\nbackground-color: %1;\n").arg(color.name()));
}

void PasswordWidget::DoValidate() {
	CPPDEVTK_ASSERT(isValid_);
	
	if (mode_ == pwmChangePassword) {
		DoValidateOldPassword();
	}
	DoValidateNewPassword();
	DoValidateConfirmNewPassword();
	DoValidateNewPasswordHint();
	
	if (isValid_) {
		SetAcceptableValidationMessage(tr("OK"));
	}
}

void PasswordWidget::DoValidateOldPassword() {
	CPPDEVTK_ASSERT(mode_ == pwmChangePassword);
	
	QString oldPassword = GetOldPassword();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&oldPassword)) {
		base::Zeroize(oldPassword);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	{
		if (!passwordRequirements_.GetUnicode()) {
			const int kCountNonAscii = base::CountNonAscii(oldPassword);
			if (kCountNonAscii > 0) {
				MarkOldPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(tr("Old password contains %1 non-ASCII characters.").arg(kCountNonAscii));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	const int kPwdLen = oldPassword.length();
	
	{
		int reqMaxLen = passwordRequirements_.GetPwdMaxLen();
		if (reqMaxLen <= 0) {
			reqMaxLen = util::PasswordRequirements::kQLineEditMaxLength_;
		}
		
		if (kPwdLen > reqMaxLen) {
			MarkOldPasswordInvalid();
			if (isValid_) {
				SetInvalidValidationMessage(tr("Old password length (%1) exceeds maximum (%2).").arg(QString::number(kPwdLen), QString::number(reqMaxLen)));
				isValid_ = false;
			}
			return;
		}
	}
	
	{
		const int kReqMinLen = passwordRequirements_.GetPwdMinLen();
		if (kReqMinLen > 0) {
			if (kPwdLen < kReqMinLen) {
				MarkOldPasswordIntermediate();
				if (isValid_) {
					SetIntermediateValidationMessage(tr("Old password length (%1) is less than minimum (%2).").arg(
							QString::number(kPwdLen), QString::number(kReqMinLen)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinLetters = passwordRequirements_.GetPwdMinLetters();
		if (kReqMinLetters > 0) {
			const int kPwdCountLetters = base::CountLetters(oldPassword);
			if (kPwdCountLetters < kReqMinLetters) {
				MarkOldPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Old password number of letters (%1) is less than minimum (%2).").arg(
							QString::number(kPwdCountLetters), QString::number(kReqMinLetters)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinUpper = passwordRequirements_.GetPwdMinUpper();
		if (kReqMinUpper > 0) {
			const int kPwdCountUpper = base::CountUpper(oldPassword);
			if (kPwdCountUpper < kReqMinUpper) {
				MarkOldPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Old password number of upper case letters (%1) is less than minimum (%2).").arg(
							QString::number(kPwdCountUpper), QString::number(kReqMinUpper)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinLower = passwordRequirements_.GetPwdMinLower();
		if (kReqMinLower > 0) {
			const int kPwdCountLower = base::CountLower(oldPassword);
			if (kPwdCountLower < kReqMinLower) {
				MarkOldPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Old password number of lower case letters (%1) is less than minimum (%2).").arg(
							QString::number(kPwdCountLower), QString::number(kReqMinLower)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinDigits = passwordRequirements_.GetPwdMinDigits();
		if (kReqMinDigits > 0) {
			const int kPwdCountDigits = base::CountDigits(oldPassword);
			if (kPwdCountDigits < kReqMinDigits) {
				MarkOldPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Old password number of digits (%1) is less than minimum (%2).").arg(
							QString::number(kPwdCountDigits), QString::number(kReqMinDigits)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinSymbols = passwordRequirements_.GetPwdMinSymbols();
		if (kReqMinSymbols > 0) {
			const int kPwdCountSymbols = base::CountSymbols(oldPassword);
			if (kPwdCountSymbols < kReqMinSymbols) {
				MarkOldPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Old password number of symbols (%1) is less than minimum (%2).").arg(
							QString::number(kPwdCountSymbols), QString::number(kReqMinSymbols)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	MarkOldPasswordAcceptable();
}

void PasswordWidget::DoValidateNewPassword() {
	QString newPassword = GetNewPassword();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&newPassword)) {
		base::Zeroize(newPassword);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	{
		if (!passwordRequirements_.GetUnicode()) {
			const int kCountNonAscii = base::CountNonAscii(newPassword);
			if (kCountNonAscii > 0) {
				MarkNewPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(tr("Password contains %1 non-ASCII characters.").arg(kCountNonAscii));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	const int kPwdLen = newPassword.length();
	
	{
		int reqMaxLen = passwordRequirements_.GetPwdMaxLen();
		if (reqMaxLen <= 0) {
			reqMaxLen = util::PasswordRequirements::kQLineEditMaxLength_;
		}
		
		if (kPwdLen > reqMaxLen) {
			MarkNewPasswordInvalid();
			if (isValid_) {
				SetInvalidValidationMessage(tr("Password length (%1) exceeds maximum permitted (%2).").arg(QString::number(kPwdLen), QString::number(reqMaxLen)));
				isValid_ = false;
			}
			return;
		}
	}
	
	{
		const int kReqMinLen = passwordRequirements_.GetPwdMinLen();
		if (kReqMinLen > 0) {
			if (kPwdLen < kReqMinLen) {
				MarkNewPasswordIntermediate();
				if (isValid_) {
					SetIntermediateValidationMessage(tr("Password length (%1) is less than minimum required (%2).").arg(
							QString::number(kPwdLen), QString::number(kReqMinLen)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinLetters = passwordRequirements_.GetPwdMinLetters();
		if (kReqMinLetters > 0) {
			const int kPwdCountLetters = base::CountLetters(newPassword);
			if (kPwdCountLetters < kReqMinLetters) {
				MarkNewPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Password number of letters (%1) is less than minimum required (%2).").arg(
							QString::number(kPwdCountLetters), QString::number(kReqMinLetters)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinUpper = passwordRequirements_.GetPwdMinUpper();
		if (kReqMinUpper > 0) {
			const int kPwdCountUpper = base::CountUpper(newPassword);
			if (kPwdCountUpper < kReqMinUpper) {
				MarkNewPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Password number of upper case letters (%1) is less than minimum required (%2).").arg(
							QString::number(kPwdCountUpper), QString::number(kReqMinUpper)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinLower = passwordRequirements_.GetPwdMinLower();
		if (kReqMinLower > 0) {
			const int kPwdCountLower = base::CountLower(newPassword);
			if (kPwdCountLower < kReqMinLower) {
				MarkNewPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Password number of lower case letters (%1) is less than minimum required (%2).").arg(
							QString::number(kPwdCountLower), QString::number(kReqMinLower)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinDigits = passwordRequirements_.GetPwdMinDigits();
		if (kReqMinDigits > 0) {
			const int kPwdCountDigits = base::CountDigits(newPassword);
			if (kPwdCountDigits < kReqMinDigits) {
				MarkNewPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Password number of digits (%1) is less than minimum required (%2).").arg(
							QString::number(kPwdCountDigits), QString::number(kReqMinDigits)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqMinSymbols = passwordRequirements_.GetPwdMinSymbols();
		if (kReqMinSymbols > 0) {
			const int kPwdCountSymbols = base::CountSymbols(newPassword);
			if (kPwdCountSymbols < kReqMinSymbols) {
				MarkNewPasswordInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Password number of symbols (%1) is less than minimum required (%2).").arg(
							QString::number(kPwdCountSymbols), QString::number(kReqMinSymbols)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	
	if (!isValid_) {	// do not validate dependencies if they are not valid
		MarkNewPasswordIntermediate();
		return;
	}
	
	
	// validate dependencies
	
	
	if (mode_ == pwmChangePassword) {
		QString oldPassword = GetOldPassword();
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&oldPassword)) {
			base::Zeroize(oldPassword);
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		{
			if (passwordRequirements_.GetNewOldNotEqual()) {
				if (newPassword == oldPassword) {
					MarkNewPasswordInvalid();
					if (isValid_) {
						SetInvalidValidationMessage(tr("New password can not be equal with old password."));
						isValid_ = false;
					}
					return;
				}
			}
		}
		
		{
			const int kReqNewOldMinSimilarity = passwordRequirements_.GetNewOldMinSimilarity();
			if (kReqNewOldMinSimilarity > 0) {
				const int kNewOldSimilarity = util::DamerauLevenshteinDistance(newPassword, oldPassword);
				if (kNewOldSimilarity < kReqNewOldMinSimilarity) {
					MarkNewPasswordInvalid();
					if (isValid_) {
						SetInvalidValidationMessage(
								tr("New password similarity with old password (%1) is less than minimum allowed (%2).").arg(
								QString::number(kNewOldSimilarity), QString::number(kReqNewOldMinSimilarity)));
						isValid_ = false;
					}
					return;
				}
			}
		}
	}
	
	MarkNewPasswordAcceptable();
}

void PasswordWidget::DoValidateConfirmNewPassword() {
	QString confirmNewPassword = GetConfirmNewPassword();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&confirmNewPassword)) {
		base::Zeroize(confirmNewPassword);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	{
		const int kReqMinLen = passwordRequirements_.GetPwdMinLen();
		if (kReqMinLen > 0) {
			if (confirmNewPassword.isEmpty()) {
				MarkConfirmNewPasswordIntermediate();
				if (isValid_) {
					SetIntermediateValidationMessage(tr("Password confirmation in progress."));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	
	if (!isValid_) {	// do not validate dependencies if they are not valid
		MarkConfirmNewPasswordIntermediate();
		return;
	}
	
	
	// validate dependencies
	
	
	QString newPassword = GetNewPassword();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&newPassword)) {
		base::Zeroize(newPassword);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (confirmNewPassword.length() < newPassword.length()) {
		if (newPassword.startsWith(confirmNewPassword)) {
			MarkConfirmNewPasswordIntermediate();
			if (isValid_) {
				SetIntermediateValidationMessage(tr("Password confirmation in progress."));
				isValid_ = false;
			}
			return;
		}
		else {
			MarkConfirmNewPasswordInvalid();
			if (isValid_) {
				SetInvalidValidationMessage(tr("Password does not start with confirmed password."));
				isValid_ = false;
			}
			return;
		}
	}
	else {
		if (confirmNewPassword != newPassword) {
			MarkConfirmNewPasswordInvalid();
			if (isValid_) {
				SetInvalidValidationMessage(tr("Confirmed password is not equal with password."));
				isValid_ = false;
			}
			return;
		}
	}
	
	MarkConfirmNewPasswordAcceptable();
}

void PasswordWidget::DoValidateNewPasswordHint() {
	const QString kNewPasswordHint = pUiPasswordWidget_->pLineEditNewPasswordHint_->text();
	
	{
		if (!passwordRequirements_.GetUnicode()) {
			const int kCountNonAscii = base::CountNonAscii(kNewPasswordHint);
			if (kCountNonAscii > 0) {
				MarkNewPasswordHintInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(tr("Password hint contains %1 non-ASCII characters.").arg(kCountNonAscii));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	const int kPwdHintLen = kNewPasswordHint.length();
	
	{
		int reqMaxLen = passwordRequirements_.GetHintMaxLen();
		if (reqMaxLen <= 0) {
			reqMaxLen = util::PasswordRequirements::kQLineEditMaxLength_;
		}
		
		if (kPwdHintLen > reqMaxLen) {
			MarkNewPasswordHintInvalid();
			if (isValid_) {
				SetInvalidValidationMessage(tr("Password hint length (%1) exceeds maximum allowed (%2).").arg(
						QString::number(kPwdHintLen), QString::number(reqMaxLen)));
				isValid_ = false;
			}
			return;
		}
	}
	
	{
		const int kReqMinLen = passwordRequirements_.GetHintMinLen();
		if (kReqMinLen > 0) {
			if (kPwdHintLen < kReqMinLen) {
				MarkNewPasswordHintIntermediate();
				if (isValid_) {
					SetIntermediateValidationMessage(tr("Password hint length (%1) is less than minimum (%2).").arg(
							QString::number(kPwdHintLen), QString::number(kReqMinLen)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	
	if (!isValid_) {	// do not validate dependencies if they are not valid
		MarkNewPasswordHintIntermediate();
		return;
	}
	
	
	// validate dependencies
	
	QString newPassword = GetNewPassword();
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&newPassword)) {
		base::Zeroize(newPassword);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	{
		if (passwordRequirements_.GetHintNotEqual()) {
			if (kNewPasswordHint == newPassword) {
				MarkNewPasswordHintInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(tr("Password hint can not be equal with password."));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	{
		const int kReqHintMinSimilarity = passwordRequirements_.GetHintMinSimilarity();
		if (kReqHintMinSimilarity > 0) {
			const int kHintSimilarity = util::DamerauLevenshteinDistance(kNewPasswordHint, newPassword);
			if (kHintSimilarity < kReqHintMinSimilarity) {
				MarkNewPasswordHintInvalid();
				if (isValid_) {
					SetInvalidValidationMessage(
							tr("Password hint similarity with password (%1) is less than minimum (%2).").arg(
							QString::number(kHintSimilarity), QString::number(kReqHintMinSimilarity)));
					isValid_ = false;
				}
				return;
			}
		}
	}
	
	MarkNewPasswordHintAcceptable();
}


}	// namespace gui
}	// namespace cppdevtk
