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


#include "widget.hpp"
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/info_tr.hpp>

#include <QtCore/QDir>


#define CPPDEVTK_DETAIL_TEST_DISK_SPACE_WIDGET_PATH_EMPTY 0


namespace cppdevtk {
namespace test_disk_space_widget {


Widget::Widget(QWidget* pParent): QWidget(pParent), WidgetBase(), Ui::Widget() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
	
	setupUi(this);
	
	SetStyleSheetFromFileCross(":/cppdevtk/test_disk_space_widget/res/qss", "widget");
	setWindowIcon(QIcon(":/cppdevtk/test_disk_space_widget/res/ico/application.ico"));
	
	pDiskSpaceWidget_->SetDiskNameColor(Qt::darkRed);
	pDiskSpaceWidget_->SetSpaceInfoColor(Qt::darkBlue);
	pDiskSpaceWidget_->SetBold(true);
#	if (!CPPDEVTK_DETAIL_TEST_DISK_SPACE_WIDGET_PATH_EMPTY)
	const QString kPath = QDir::currentPath();
	CPPDEVTK_LOG_DEBUG("setting path to: " << kPath);
	pDiskSpaceWidget_->SetPath(kPath);
#	endif
	
	pSpinBoxAutoRefreshInterval_->setValue(pDiskSpaceWidget_->GetAutoRefreshInterval());
	
	CPPDEVTK_VERIFY(connect(pPushButtonRefresh_, SIGNAL(clicked()),
			pDiskSpaceWidget_, SLOT(Refresh())));
	CPPDEVTK_VERIFY(connect(pGroupBoxAutoRefresh_, SIGNAL(toggled(bool)),
			pDiskSpaceWidget_, SLOT(SetAutoRefreshEnabled(bool))));
	CPPDEVTK_VERIFY(connect(pSpinBoxAutoRefreshInterval_, SIGNAL(valueChanged(int)),
			this, SLOT(SetAutoRefreshInterval(int))));
	
	adjustSize();
}

Widget::~Widget() {
	CPPDEVTK_LOG_TRACE_FUNCTION();
}

void Widget::SetAutoRefreshInterval(int sec) {
	pDiskSpaceWidget_->SetAutoRefreshInterval(sec);
}

void Widget::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QWidget::changeEvent(pEvent);
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}


}	// namespace test_disk_space_widget
}	// namespace cppdevtk
