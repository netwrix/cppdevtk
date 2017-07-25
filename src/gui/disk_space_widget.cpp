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


#include <cppdevtk/gui/disk_space_widget.hpp>
#include "ui_disk_space_widget.h"
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/QDir>

#include <cmath>
#include <new>


#define CPPDEVTK_DETAIL_GUI_DISK_SPACE_WIDGET_ENABLE_TEST 0
#define CPPDEVTK_DETAIL_GUI_DISK_SPACE_WIDGET_SET_RANGE_0_0 1


namespace cppdevtk {
namespace gui {


const QString DiskSpaceWidget::kDiskNameTextFmt_("<FONT COLOR=%1>%2: </FONT>");
const QString DiskSpaceWidget::kSpaceInfoTextFmt_("<FONT COLOR=%1>%2 %3 %4 %5</FONT>");


DiskSpaceWidget::DiskSpaceWidget(QWidget* pParent): QWidget(pParent), WidgetBase(), pUiDiskSpaceWidget_(new Ui::DiskSpaceWidget()),
		diskNameColor_(Qt::black), spaceInfoColor_(Qt::darkBlue), path_(), mountPoint_(),
		autoRefreshTimer_() {
	pUiDiskSpaceWidget_->setupUi(this);
	SetStyleSheetFromFileCross(":/cppdevtk/gui/res/qss", "disk_space_widget");
	ValidateUi();
	
	autoRefreshTimer_.setInterval(2500);
	CPPDEVTK_ASSERT(!autoRefreshTimer_.isActive());
	CPPDEVTK_ASSERT(!autoRefreshTimer_.isSingleShot());
	
	//Refresh();
	
	MakeConnections();
}

DiskSpaceWidget::~DiskSpaceWidget() {
	delete pUiDiskSpaceWidget_;
}

void DiskSpaceWidget::Refresh() {
	if (!isVisible()) {	// GetFileSystemSpaceInfo() is time consuming, so avoid if not needed...
#		if (CPPDEVTK_DETAIL_GUI_DISK_SPACE_WIDGET_ENABLE_TEST)
		CPPDEVTK_LOG_INFO("skipping refresh (not visible)...");
#		endif
		return;
	}
	
#	if (CPPDEVTK_DETAIL_GUI_DISK_SPACE_WIDGET_ENABLE_TEST)
	CPPDEVTK_LOG_INFO("refreshing...");
#	endif
	
	if (path_.isEmpty()) {
		Reset();
		return;
	}
	
	util::FileSystemSpaceInfo fileSystemSpaceInfo;
	base::ErrorCode errorCode;
	if (!util::GetFileSystemSpaceInfo(path_, fileSystemSpaceInfo, errorCode)) {
		CPPDEVTK_LOG_ERROR("GetFileSystemSpaceInfo() failed; error code: " << errorCode.ToString());
		FileSystemSpaceInfoNotAvailable();
		return;
	}
	
	const qreal kSizeInGb = ((qreal)fileSystemSpaceInfo.size_) / (1024 * 1024 * 1024);
	//const qreal kTotalFreeInGb = ((qreal)fileSystemSpaceInfo.totalFree_) / (1024 * 1024 * 1024);
	const qreal kUserFreeInGb = ((qreal)fileSystemSpaceInfo.userFree_) / (1024 * 1024 * 1024);
	DoRefresh(kSizeInGb, kUserFreeInGb);
}

void DiskSpaceWidget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_ASSERT(pEvent != NULL);
	
	QWidget::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			pUiDiskSpaceWidget_->retranslateUi(this);
			Refresh();
			break;
		default:
			break;
	}
}

void DiskSpaceWidget::ValidateUi() const {
	CPPDEVTK_ASSERT(pUiDiskSpaceWidget_->pProgressBar_->isTextVisible() == false);
	
	CPPDEVTK_ASSERT(sizePolicy().horizontalPolicy() == QSizePolicy::Expanding);
	CPPDEVTK_ASSERT(sizePolicy().verticalPolicy() == QSizePolicy::Maximum);
	CPPDEVTK_ASSERT(pUiDiskSpaceWidget_->pVerticalLayout_->contentsMargins() == QMargins(0, 0, 0, 0));
}

void DiskSpaceWidget::MakeConnections() {
	CPPDEVTK_VERIFY(connect(&autoRefreshTimer_, SIGNAL(timeout()), this, SLOT(Refresh())));
}

void DiskSpaceWidget::DoRefresh(qreal totalSize, qreal freeSpace) {
	CPPDEVTK_ASSERT(totalSize >= 0);
	CPPDEVTK_ASSERT(freeSpace >= 0);
	CPPDEVTK_ASSERT(totalSize >= freeSpace);
	CPPDEVTK_ASSERT(!path_.isEmpty());
	
	const int kRoundedTotalSize = ::std::ceil(totalSize * 100);
	pUiDiskSpaceWidget_->pProgressBar_->setRange(0, kRoundedTotalSize);
	const int kRoundedFreeSpace = ::std::ceil(freeSpace * 100);
	CPPDEVTK_ASSERT(kRoundedTotalSize >= kRoundedFreeSpace);
	pUiDiskSpaceWidget_->pProgressBar_->setValue((kRoundedTotalSize - kRoundedFreeSpace));
	
	if (mountPoint_.isEmpty()) {
		try {
			const QStringList kMountPoints = util::GetMountPointsFromPath(path_);
			if (!kMountPoints.isEmpty()) {
				mountPoint_ = kMountPoints[0];
				CPPDEVTK_ASSERT(!mountPoint_.isEmpty());
			}
			else {
				CPPDEVTK_LOG_ERROR("failed to GetMountPointsFromPath() for path: " << path_);
			}
		}
		catch (const ::std::exception& exc) {
			CPPDEVTK_LOG_ERROR("failed to GetMountPointsFromPath() for path: " << path_
					<< "\nreason: " << base::Exception::GetDetailedInfo(exc));
			base::SuppressUnusedWarning(exc);
		}
	}
	const QString kDiskNameText = kDiskNameTextFmt_.arg(diskNameColor_.name(),
			(mountPoint_.isEmpty() ? QDir::toNativeSeparators(path_) : QDir::toNativeSeparators(mountPoint_)));
	
	const QString kSpaceInfoText = kSpaceInfoTextFmt_.arg(spaceInfoColor_.name(),
			QString::number(freeSpace, 'f', 2), tr("free of"), QString::number(totalSize, 'f', 2), tr("GB"));
	
	pUiDiskSpaceWidget_->pLabel_->setText(kDiskNameText + kSpaceInfoText);
}

void DiskSpaceWidget::Reset() {
	CPPDEVTK_ASSERT(path_.isEmpty());
	
#	if (CPPDEVTK_DETAIL_GUI_DISK_SPACE_WIDGET_SET_RANGE_0_0)
	pUiDiskSpaceWidget_->pProgressBar_->setRange(0, 0);
	pUiDiskSpaceWidget_->pProgressBar_->setValue(0);
#	else
	pUiDiskSpaceWidget_->pProgressBar_->reset();
#	endif
	
	const QString kDiskNameText = kDiskNameTextFmt_.arg(diskNameColor_.name(), tr("No Disk"));
	const QString kSpaceInfoText = kSpaceInfoTextFmt_.arg(spaceInfoColor_.name(), "?", tr("free of"), "?", tr("GB"));
	pUiDiskSpaceWidget_->pLabel_->setText(kDiskNameText + kSpaceInfoText);
}

void DiskSpaceWidget::FileSystemSpaceInfoNotAvailable() {
	CPPDEVTK_ASSERT(!path_.isEmpty());
	
#	if (CPPDEVTK_DETAIL_GUI_DISK_SPACE_WIDGET_SET_RANGE_0_0)
	pUiDiskSpaceWidget_->pProgressBar_->setRange(0, 0);
	pUiDiskSpaceWidget_->pProgressBar_->setValue(0);
#	else
	pUiDiskSpaceWidget_->pProgressBar_->reset();
#	endif
	
	if (mountPoint_.isEmpty()) {
		try {
			const QStringList kMountPoints = util::GetMountPointsFromPath(path_);
			if (!kMountPoints.isEmpty()) {
				mountPoint_ = kMountPoints[0];
				CPPDEVTK_ASSERT(!mountPoint_.isEmpty());
			}
			else {
				CPPDEVTK_LOG_ERROR("failed to GetMountPointsFromPath() for path: " << path_);
			}
		}
		catch (const ::std::exception& exc) {
			CPPDEVTK_LOG_ERROR("failed to GetMountPointsFromPath() for path: " << path_
					<< "\nreason: " << base::Exception::GetDetailedInfo(exc));
			base::SuppressUnusedWarning(exc);
		}
	}
	const QString kDiskNameText = kDiskNameTextFmt_.arg(diskNameColor_.name(),
			(mountPoint_.isEmpty() ? QDir::toNativeSeparators(path_) : QDir::toNativeSeparators(mountPoint_)));
	
	const QString kSpaceInfoText = kSpaceInfoTextFmt_.arg(spaceInfoColor_.name(), "?", tr("free of"), "?", tr("GB"));
	
	pUiDiskSpaceWidget_->pLabel_->setText(kDiskNameText + kSpaceInfoText);
}


}	// namespace gui
}	// namespace cppdevtk
