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


#include <cppdevtk/gui/language_widget.hpp>
#include "ui_language_widget.h"
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QVariant>

#include <new>


namespace cppdevtk {
namespace gui {


LanguageWidget::LanguageWidget(QWidget* pParent): QWidget(pParent), WidgetBase(),
		pUiLanguageWidget_(new Ui::LanguageWidget()) {
	pUiLanguageWidget_->setupUi(this);
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "language_widget");
	ValidateUi();
	
	MakeConnections();
}

LanguageWidget::~LanguageWidget() {
	delete pUiLanguageWidget_;
}

void LanguageWidget::AddItem(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	pUiLanguageWidget_->pComboBoxLanguage_->addItem(languageInfo.GetNativeName(), languageInfo.GetLocale());
}

int LanguageWidget::GetCount() const {
	return pUiLanguageWidget_->pComboBoxLanguage_->count();
}

int LanguageWidget::GetCurrentIndex() const {
	return pUiLanguageWidget_->pComboBoxLanguage_->currentIndex();
}

::cppdevtk::util::LanguageInfo LanguageWidget::GetLanguageInfo(int index) const {
	CPPDEVTK_DBC_CHECK_IN_RANGE(((0 <= index) && (index < GetCount())), "index");
	
	const QString kNativeName = pUiLanguageWidget_->pComboBoxLanguage_->itemText(index);
	CPPDEVTK_ASSERT(!kNativeName.isEmpty());
	QVariant kLocaleVariant = pUiLanguageWidget_->pComboBoxLanguage_->itemData(index);
	CPPDEVTK_ASSERT(kLocaleVariant.type() == QVariant::Locale);
	
	return ::cppdevtk::util::LanguageInfo(kLocaleVariant.toLocale(), kNativeName);
}

void LanguageWidget::SetCurrentLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	SetCurrentIndex(pUiLanguageWidget_->pComboBoxLanguage_->findText(languageInfo.GetNativeName()));
}

void LanguageWidget::Clear() {
	pUiLanguageWidget_->pComboBoxLanguage_->clear();
}

void LanguageWidget::SetCurrentIndex(int index) {
	pUiLanguageWidget_->pComboBoxLanguage_->setCurrentIndex(index);
}

void LanguageWidget::RemoveItem(int index) {
	pUiLanguageWidget_->pComboBoxLanguage_->removeItem(index);
}

void LanguageWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiLanguageWidget_->retranslateUi(this);
			break;
		default:
			break;
	}
}

void LanguageWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(pUiLanguageWidget_->pComboBoxLanguage_->insertPolicy() == QComboBox::InsertAlphabetically);
	
	CPPDEVTK_ASSERT(pUiLanguageWidget_->pGridLayout_->contentsMargins() == QMargins(0, 0, 0, 0));	
}

void LanguageWidget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pUiLanguageWidget_->pComboBoxLanguage_, SIGNAL(currentIndexChanged(int)),
			this, SIGNAL(CurrentIndexChanged(int))));
}


}	// namespace gui
}	// namespace cppdevtk
