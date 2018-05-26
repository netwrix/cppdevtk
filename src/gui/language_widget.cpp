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


#include <cppdevtk/gui/config.hpp>

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
#if (CPPDEVTK_COMPILER_GCC)
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 2, 0))
#if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 7, 0))
// deleting object of polymorphic class type cppdevtk::util::LanguageInfo which has non-virtual destructor
// might cause undefined behaviour [-Wdelete-non-virtual-dtor]
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
#endif
#endif
#endif

#include <cppdevtk/gui/language_widget.hpp>
#include "ui_language_widget.h"
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QVariant>
#include <QtCore/QtAlgorithms>
#include <QtGui/QFont>

#include <new>


namespace cppdevtk {
namespace gui {


using ::cppdevtk::util::LanguageInfo;


LanguageWidget::LanguageWidget(QWidget* pParent): QWidget(pParent), WidgetBase(),
		pUiLanguageWidget_(new Ui::LanguageWidget()) {
#	ifndef NDEBUG
	LanguageInfo languageInfo;
	qMetaTypeId< ::cppdevtk::util::LanguageInfo>();	// compile time
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("cppdevtk::util::LanguageInfo") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("cppdevtk::util::LanguageInfo") != 0);
#	endif
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	CPPDEVTK_ASSERT(QMetaType::hasRegisteredComparators< ::cppdevtk::util::LanguageInfo>());
#	endif
#	endif
	
	pUiLanguageWidget_->setupUi(this);
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "language_widget");
	ValidateUi();
	
	CPPDEVTK_VERIFY(connect(pUiLanguageWidget_->pComboBoxLanguage_, SIGNAL(currentIndexChanged(int)),
			SLOT(ConvertCurrentChanged(int))));
}

LanguageWidget::~LanguageWidget() {
	delete pUiLanguageWidget_;
}

int LanguageWidget::GetCount() const {
	return pUiLanguageWidget_->pComboBoxLanguage_->count();
}

QList< ::cppdevtk::util::LanguageInfo> LanguageWidget::GetLanguageInfos() const {
	QList<LanguageInfo> languageInfos;
	
	const int kCount = GetCount();
	for (int idx = 0; idx < kCount; ++idx) {
		languageInfos.append(pUiLanguageWidget_->pComboBoxLanguage_->itemData(idx).value<LanguageInfo>());
	}
	
	qSort(languageInfos.begin(), languageInfos.end());
	
	return languageInfos;
}

::cppdevtk::util::LanguageInfo LanguageWidget::GetCurrentLanguageInfo() const {
	const int kCurrentIndex = pUiLanguageWidget_->pComboBoxLanguage_->currentIndex();
	return (kCurrentIndex != -1) ? pUiLanguageWidget_->pComboBoxLanguage_->itemData(kCurrentIndex).value<LanguageInfo>()
			: LanguageInfo::GetCLanguageInfo();
}

void LanguageWidget::AddLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((languageInfo != LanguageInfo::GetCLanguageInfo()), "languageInfo: C");
	
	pUiLanguageWidget_->pComboBoxLanguage_->addItem(languageInfo.GetNativeName(), QVariant::fromValue(languageInfo));
}

void LanguageWidget::RemoveLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((languageInfo != LanguageInfo::GetCLanguageInfo()), "languageInfo: C");
	
	// findData() use QVariant::operator==() that for custom types registered with qRegisterMetaType() require QMetaType::registerComparators()
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	const int kIdx = pUiLanguageWidget_->pComboBoxLanguage_->findData(QVariant::fromValue(languageInfo));
#	else
	const int kIdx = pUiLanguageWidget_->pComboBoxLanguage_->findText(languageInfo.GetNativeName());
#	endif
	if (kIdx != -1) {
		pUiLanguageWidget_->pComboBoxLanguage_->removeItem(kIdx);
	}
}

void LanguageWidget::SetBold(bool value) {
	QFont font = pUiLanguageWidget_->pLabelLanguage_->font();
	if (font.bold() != value) {
		font.setBold(value);
		pUiLanguageWidget_->pLabelLanguage_->setFont(font);
	}
}

bool LanguageWidget::GetBold() const {
	return pUiLanguageWidget_->pLabelLanguage_->font().bold();
}

void LanguageWidget::SetMaxVisibleItems(int value) {
	pUiLanguageWidget_->pComboBoxLanguage_->setMaxVisibleItems(value);
}

int LanguageWidget::GetMaxVisibleItems() const {
	return pUiLanguageWidget_->pComboBoxLanguage_->maxVisibleItems();
}

void LanguageWidget::Clear() {
	pUiLanguageWidget_->pComboBoxLanguage_->clear();
}

void LanguageWidget::SetCurrentLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	if (languageInfo == LanguageInfo::GetCLanguageInfo()) {
		pUiLanguageWidget_->pComboBoxLanguage_->setCurrentIndex(-1);
		return;
	}
	
	// findData() use QVariant::operator==() that for custom types registered with qRegisterMetaType() require QMetaType::registerComparators()
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	const int kIdx = pUiLanguageWidget_->pComboBoxLanguage_->findData(QVariant::fromValue(languageInfo));
#	else
	const int kIdx = pUiLanguageWidget_->pComboBoxLanguage_->findText(languageInfo.GetNativeName());
#	endif
	if (kIdx != -1) {
		pUiLanguageWidget_->pComboBoxLanguage_->setCurrentIndex(kIdx);
	}
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

void LanguageWidget::ConvertCurrentChanged(int index) {
	if (index != -1) {
		Q_EMIT CurrentLanguageInfoChanged(pUiLanguageWidget_->pComboBoxLanguage_->itemData(index).value<LanguageInfo>());
	}
	else {
		Q_EMIT CurrentLanguageInfoChanged(LanguageInfo::GetCLanguageInfo());
	}
}

void LanguageWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(!pUiLanguageWidget_->pComboBoxLanguage_->isEditable());
	CPPDEVTK_ASSERT(!pUiLanguageWidget_->pComboBoxLanguage_->duplicatesEnabled());
	CPPDEVTK_ASSERT(pUiLanguageWidget_->pComboBoxLanguage_->insertPolicy() == QComboBox::InsertAlphabetically);
	
	CPPDEVTK_ASSERT(pUiLanguageWidget_->pGridLayout_->contentsMargins() == QMargins(0, 0, 0, 0));	
}


}	// namespace gui
}	// namespace cppdevtk
