/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_TEST_DISK_SPACE_WIDGET_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_TEST_DISK_SPACE_WIDGET_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "ui_widget.h"
#include <cppdevtk/gui/widget_base.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


namespace cppdevtk {
namespace test_disk_space_widget {


class Widget: public QWidget, public ::cppdevtk::gui::WidgetBase, private Ui::Widget {
	Q_OBJECT
public:
	explicit Widget(QWidget* pParent = NULL);
	virtual ~Widget();
public Q_SLOTS:
	void SetAutoRefreshInterval(int sec);
protected:
	virtual void changeEvent(QEvent* pEvent);
private:
	Q_DISABLE_COPY(Widget)
};


}	// namespace test_disk_space_widget
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_DISK_SPACE_WIDGET_WIDGET_HPP_INCLUDED_
