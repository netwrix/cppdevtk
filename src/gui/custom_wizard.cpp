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


#include <cppdevtk/gui/custom_wizard.hpp>
#include "ui_custom_wizard.h"
#include <cppdevtk/gui/message_box.hpp>
#include <cppdevtk/gui/application.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/Qt>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWizardPage>
#else
#include <QtGui/QWizardPage>
#endif

#include <new>


namespace cppdevtk {
namespace gui {


CustomWizard::CustomWizard(QWidget* pParent, Qt::WindowFlags flags): QDialog(pParent, flags), WidgetBase(),
		pUiCustomWizard_(new Ui::CustomWizard()), options_(NoOption), startId_(-1), initializedPageIds_(), homepage_() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	pUiCustomWizard_->setupUi(this);
	setTitle("");
	setSubtitle("");
	pUiCustomWizard_->pToolButtonLogo_->setText("");
	validateUi();
	
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "custom_wizard");
	
	updateButtonStates();
	
	makeConnections();
}

CustomWizard::~CustomWizard() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	delete pUiCustomWizard_;
}

int CustomWizard::addPage(QWizardPage* pPage) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pPage);
	CPPDEVTK_DBC_CHECK_ARGUMENT((pUiCustomWizard_->pStackedWidgetWizardPages_->indexOf(pPage) == -1),
			"page already added");
	
	const int kPageId = pUiCustomWizard_->pStackedWidgetWizardPages_->count();
	setPage(kPageId, pPage);
	return kPageId;
}

void CustomWizard::setPage(int pageId, QWizardPage* pPage) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pPage);
	CPPDEVTK_DBC_CHECK_ARGUMENT((pUiCustomWizard_->pStackedWidgetWizardPages_->indexOf(pPage) == -1),
			"page already added");
	CPPDEVTK_DBC_CHECK_ARGUMENT((pageId == pUiCustomWizard_->pStackedWidgetWizardPages_->count()),
			"pageId has invalid value");
	
	const int kInsertedPageId = pUiCustomWizard_->pStackedWidgetWizardPages_->insertWidget(pageId, pPage);
	CPPDEVTK_ASSERT(kInsertedPageId == pageId);
	if (startId_ == -1) {
		startId_ = kInsertedPageId;
	}
	
	CPPDEVTK_VERIFY(connect(pPage, SIGNAL(completeChanged()), this, SLOT(updateButtonStates())));
	
	emit pageAdded(pageId);
}

QWizardPage* CustomWizard::page(int pageId) const {
	CPPDEVTK_DBC_CHECK_ARGUMENT(((pageId == -1) || (pageId >= 0)), "pageId has invalid value");
	QWidget* pWidget = pUiCustomWizard_->pStackedWidgetWizardPages_->widget(pageId);
	CPPDEVTK_ASSERT((pWidget == NULL) || dynamic_cast<QWizardPage*>(pWidget) != NULL);
	return static_cast<QWizardPage*>(pWidget);
}

void CustomWizard::setStartId(int pageId) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(((pageId == -1) || (pageId >= 0)), "pageId has invalid value");
	CPPDEVTK_ASSERT(pUiCustomWizard_->pStackedWidgetWizardPages_->count() > 0);
	
	if (pageId == -1) {
		pageId = 0;
	}
	CPPDEVTK_ASSERT(page(pageId) != NULL);
	startId_ = pageId;
}

int CustomWizard::startId() const {
	CPPDEVTK_ASSERT((startId_ == -1) || ((0 <= startId_) && (startId_ < pUiCustomWizard_->pStackedWidgetWizardPages_->count())));
	return startId_;
}

QWizardPage* CustomWizard::currentPage() const {
	QWidget* pCurrentWidget = pUiCustomWizard_->pStackedWidgetWizardPages_->currentWidget();
	CPPDEVTK_ASSERT((pCurrentWidget == NULL) ||
			dynamic_cast<QWizardPage*>(pUiCustomWizard_->pStackedWidgetWizardPages_->currentWidget()) != NULL);
	return static_cast<QWizardPage*>(pCurrentWidget);
}

int CustomWizard::currentId() const {
	return pUiCustomWizard_->pStackedWidgetWizardPages_->currentIndex();
}

bool CustomWizard::validateCurrentPage() {
	QWizardPage* pWizardPage = currentPage();
	if (pWizardPage == NULL) {
		return true;
	}
	
	return pWizardPage->validatePage();
}

int CustomWizard::nextId() const {
	int retValue = -1;
	const QWizardPage* pWizardPage = currentPage();
	if (pWizardPage != NULL) {
		retValue = pWizardPage->nextId();
	}
	
	CPPDEVTK_ASSERT((retValue == -1) || (((0 < retValue) && (retValue < pUiCustomWizard_->pStackedWidgetWizardPages_->count()))
			&& (retValue > currentId())));
	return retValue;
}

void CustomWizard::setWizardStyle(WizardStyle wizardStyle) {
	CPPDEVTK_ASSERT(wizardStyle == ModernStyle);
	base::SuppressUnusedWarning(wizardStyle);
}

void CustomWizard::setOptions(WizardOptions options) {
	if (!(options_ ^ options)) {
		return;
	}
	
	options_ = options;
	
	pUiCustomWizard_->pLabelSubtitle_->setVisible(!(options & IgnoreSubTitles));
	
	updateButtonStates();
}

void CustomWizard::setTitleFormat(Qt::TextFormat format) {
	pUiCustomWizard_->pLabelTitle_->setTextFormat(format);
}

Qt::TextFormat CustomWizard::titleFormat() const {
	return pUiCustomWizard_->pLabelTitle_->textFormat();
}

void CustomWizard::setSubTitleFormat(Qt::TextFormat format) {
	pUiCustomWizard_->pLabelSubtitle_->setTextFormat(format);
}

Qt::TextFormat CustomWizard::subTitleFormat() const {
	return pUiCustomWizard_->pLabelSubtitle_->textFormat();
}

void CustomWizard::setPixmap(WizardPixmap which, const QPixmap& pixmap) {
	CPPDEVTK_ASSERT(which == LogoPixmap);
	base::SuppressUnusedWarning(which);
	
	pUiCustomWizard_->pToolButtonLogo_->setIcon(QIcon(pixmap));
	pUiCustomWizard_->pToolButtonLogo_->setIconSize(pixmap.size());
}

QPixmap CustomWizard::pixmap(WizardPixmap which) const {
	CPPDEVTK_ASSERT(which == LogoPixmap);
	base::SuppressUnusedWarning(which);
	
	return pUiCustomWizard_->pToolButtonLogo_->icon().pixmap(pUiCustomWizard_->pToolButtonLogo_->icon().availableSizes()[0]);
}

void CustomWizard::setVisible(bool visible) {
	if (visible) {
		const int kCurrentId = currentId();
		if (kCurrentId == -1) {
			restart();
		}
		else {
			switchToPage(kCurrentId);
		}
	}
	
	QDialog::setVisible(visible);
}

void CustomWizard::back() {
	const int kCurrentId = currentId();
	CPPDEVTK_ASSERT(kCurrentId > 0);
	switchToPage(kCurrentId - 1);
}

void CustomWizard::next() {
	const int kCurrentId = currentId();
	CPPDEVTK_ASSERT((0 <= kCurrentId) && (kCurrentId < pUiCustomWizard_->pStackedWidgetWizardPages_->count()));
	CPPDEVTK_ASSERT(validateCurrentPage());
	
	const int kNextId = nextId();
	if (kNextId == -1) {
		accept();
	}
	else {
		CPPDEVTK_ASSERT((kNextId > kCurrentId) && ((0 < kNextId) &&
				(kNextId < pUiCustomWizard_->pStackedWidgetWizardPages_->count())));
		switchToPage(kNextId);
	}
	
	base::SuppressUnusedWarning(kCurrentId);
}

void CustomWizard::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QDialog::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiCustomWizard_->retranslateUi(this);
			break;
		default:
			break;
	}
}

void CustomWizard::closeEvent(QCloseEvent* pCloseEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pCloseEvent);
	
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	QDialog::closeEvent(pCloseEvent);
}

void CustomWizard::done(int result) {
	if (result == Rejected) {
		reset();
	}
	else {
		if (!validateCurrentPage()) {
			return;
		}
	}
	
	QDialog::done(result);
}

void CustomWizard::initializePage(int pageId) {
	QWizardPage* pWizardPage = page(pageId);
	CPPDEVTK_ASSERT(pWizardPage != NULL);
	pWizardPage->initializePage();
}

void CustomWizard::cleanupPage(int pageId) {
	QWizardPage* pWizardPage = page(pageId);
	CPPDEVTK_ASSERT(pWizardPage != NULL);
	pWizardPage->cleanupPage();
}

void CustomWizard::validateUi() const {
	CPPDEVTK_ASSERT(pUiCustomWizard_->pLabelTitle_->font().bold() == true);
	CPPDEVTK_ASSERT(pUiCustomWizard_->pLabelSubtitle_->wordWrap() == true);
	CPPDEVTK_ASSERT(pUiCustomWizard_->pLabelSubtitle_->alignment() == (Qt::AlignTop | Qt::AlignLeft));
	CPPDEVTK_ASSERT(pUiCustomWizard_->pLabelSubtitle_->minimumHeight() == 26);
	CPPDEVTK_ASSERT(pUiCustomWizard_->pToolButtonLogo_->focusPolicy() == Qt::NoFocus);
	CPPDEVTK_ASSERT(pUiCustomWizard_->pToolButtonLogo_->autoRaise() == true);
}

void CustomWizard::makeConnections() {
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pStackedWidgetWizardPages_, SIGNAL(currentChanged(int)),
			this, SLOT(updateButtonStates())));
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pStackedWidgetWizardPages_, SIGNAL(currentChanged(int)),
			this, SIGNAL(currentIdChanged(int))));
	
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pToolButtonLogo_, SIGNAL(clicked()), this, SLOT(openHomepage())));
	
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pPushButtonBack_, SIGNAL(clicked()), this, SLOT(back())));
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pPushButtonNext_, SIGNAL(clicked()), this, SLOT(next())));
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pPushButtonCancel_, SIGNAL(clicked()), this, SLOT(reject())));
	CPPDEVTK_VERIFY(connect(pUiCustomWizard_->pPushButtonHelp_, SIGNAL(clicked()), this, SIGNAL(helpRequested())));
}

void CustomWizard::switchToPage(int pageId) {
	CPPDEVTK_DBC_CHECK_STATE((pUiCustomWizard_->pStackedWidgetWizardPages_->count() > 0),
			"has pages", "does not have pages");
	CPPDEVTK_DBC_CHECK_ARGUMENT(((pageId == -1)
			|| ((0 <= pageId) && (pageId < pUiCustomWizard_->pStackedWidgetWizardPages_->count()))),
			"pageId has invalid value");
	if (pageId == -1) {
		pageId = 0;
	}
	
	const int kOldPageId = currentId();
	if (kOldPageId != pageId) {
		if (kOldPageId < pageId) {	// forward
			if (!initializedPageIds_.contains(pageId)) {
				initializedPageIds_.insert(pageId);
				initializePage(pageId);
			}
		}
		else {	// backward
			if (!(options_ & IndependentPages)) {
				cleanupPage(kOldPageId);
				initializedPageIds_.remove(kOldPageId);
			}
		}
		
		pUiCustomWizard_->pStackedWidgetWizardPages_->setCurrentIndex(pageId);
	}
	
	setTitle(currentPage()->title());
	if (!(options_ & IgnoreSubTitles)) {
		setSubtitle(currentPage()->subTitle());
	}
}

void CustomWizard::reset() {
	if (currentId() == -1) {
		return;
	}
	
	for (QSet<int>::ConstIterator kIter = initializedPageIds_.constBegin();
			kIter != initializedPageIds_.constEnd(); ++kIter) {
		cleanupPage(*kIter);
	}
	initializedPageIds_.clear();
	
	pUiCustomWizard_->pStackedWidgetWizardPages_->setCurrentIndex(-1);
}

void CustomWizard::setTitle(const QString& msg) {
	pUiCustomWizard_->pLabelTitle_->setText(msg);
}

void CustomWizard::setSubtitle(const QString& msg) {
	CPPDEVTK_DBC_CHECK_STATE(!(options_ & IgnoreSubTitles), "subtitle enabled", "subtitle disabled");
	pUiCustomWizard_->pLabelSubtitle_->setText(msg);
}

void CustomWizard::updateButtonStates() {
	pUiCustomWizard_->pPushButtonBack_->setEnabled(currentId() > 0);
	
	const QWizardPage* pCurrentPage = currentPage();
	const bool kCurrentPageIsComplete = (pCurrentPage != NULL) && pCurrentPage->isComplete();
	pUiCustomWizard_->pPushButtonNext_->setEnabled(kCurrentPageIsComplete);
	
	pUiCustomWizard_->pPushButtonCancel_->setVisible(!(options_ & NoCancelButton));
	pUiCustomWizard_->pPushButtonHelp_->setVisible(options_ & HaveHelpButton);
}

void CustomWizard::openHomepage() {
	QString homepage;
	if (homepage_.isEmpty()) {
		homepage = Application::organizationDomain();
	}
	else {
		homepage = homepage_;
	}
	if (!QDesktopServices::openUrl(QUrl(homepage, QUrl::TolerantMode))) {
		MessageBox::Critical(this, tr("Error"),
				tr("Failed to open URL in default web browser"), QString("URL: %1").arg(homepage));
	}
}


}	// namespace gui
}	// namespace cppdevtk
