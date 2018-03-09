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


#include <cppdevtk/gui/eula_widget.hpp>
#include "ui_eula_widget.h"
#include <cppdevtk/gui/application_base.hpp>
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>

#include <QtCore/QLocale>
#include <QtCore/QUrl>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <new>


namespace cppdevtk {
namespace gui {


EulaWidget::EulaWidget(QWidget* pParent): QWidget(pParent), WidgetBase(), pUiEulaWidget_(new Ui::EulaWidget()),
		fileNamePrefix_(), fileExt_() {
	pUiEulaWidget_->setupUi(this);
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "eula_widget");
	ValidateUi();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	adjustSize();
#	endif
	
	//Refresh();
	
	MakeConnections();
}

EulaWidget::~EulaWidget() {
	delete pUiEulaWidget_;
}

void EulaWidget::SetSourceInfo(const QString& fileNamePrefix, const QString& fileExt) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(util::IsValidPath(fileNamePrefix), "fileNamePrefix must be valid");
	
	fileNamePrefix_ = fileNamePrefix;
	fileExt_ = fileExt;
	
	Refresh();
}

void EulaWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_ASSERT(pEvent != NULL);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiEulaWidget_->retranslateUi(this);
			Refresh();
			break;
		default:
			break;
	}
}

void EulaWidget::OpenEula() {
	CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(qApp);
	CPPDEVTK_DBC_CHECK_NON_NULL_POINTER(dynamic_cast<ApplicationBase*>(qApp));
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG(util::IsValidPath(fileNamePrefix_), "fileNamePrefix_ must be valid");
	
	const bool kRetCode = dynamic_cast<ApplicationBase*>(qApp)->OpenLocalizedFileInDefaultApp(fileNamePrefix_, fileExt_);
	if (!kRetCode) {
		CPPDEVTK_LOG_ERROR("failed to open EULA in default viewer; fileNamePrefix_: " << fileNamePrefix_
				<< "; fileExt: " << fileExt_);
		MessageBox::Critical(this, tr("Error"), tr("Failed to open EULA in default viewer!"));
	}
}

void EulaWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(pUiEulaWidget_->pLabelEula_->wordWrap() == true);
	
	CPPDEVTK_ASSERT(pUiEulaWidget_->pTextBrowserEula_->tabChangesFocus() == true);
	CPPDEVTK_ASSERT(pUiEulaWidget_->pTextBrowserEula_->openExternalLinks() == true);
	CPPDEVTK_ASSERT(pUiEulaWidget_->pTextBrowserEula_->minimumHeight() == 192);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	pUiEulaWidget_->pTextBrowserEula_->setMinimumHeight(0);
	pUiEulaWidget_->pTextBrowserEula_->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
#	endif
	
	CPPDEVTK_ASSERT(pUiEulaWidget_->pVerticalLayout_->contentsMargins() == QMargins(0, 0, 0, 0));
}

void EulaWidget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(pUiEulaWidget_->pPushButtonOpenEula_, SIGNAL(clicked()), this, SLOT(OpenEula())));
}

void EulaWidget::Refresh() {
	CPPDEVTK_ASSERT(qApp != NULL);
	CPPDEVTK_ASSERT(dynamic_cast<ApplicationBase*>(qApp) != NULL);
	CPPDEVTK_ASSERT(util::IsValidPath(fileNamePrefix_));
	
	const QLocale kLocale(dynamic_cast<ApplicationBase*>(qApp)->GetCurrentLanguageInfo().GetName());
	const QString kLocalizedFileName = util::GetLocalizedFileName(fileNamePrefix_, fileExt_, kLocale);
	if (kLocalizedFileName.isEmpty()) {
		CPPDEVTK_LOG_ERROR("failed to get localizedFileName for fileNamePrefix: " << fileNamePrefix_
				<< "; fileExt: " << fileExt_ << "; locale: " << kLocale.name());
		return;
	}
	
	const QUrl kUrl = QUrl::fromLocalFile(kLocalizedFileName);
	pUiEulaWidget_->pTextBrowserEula_->setSource(kUrl);
}


}	// namespace gui
}	// namespace cppdevtk
