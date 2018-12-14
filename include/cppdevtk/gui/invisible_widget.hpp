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


#ifndef CPPDEVTK_GUI_INVISIBLE_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_INVISIBLE_WIDGET_HPP_INCLUDED_


#include "config.hpp"

#include <QtCore/QObject>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


#define CPPDEVTK_DETAIL_GUI_INVISIBLE_WIDGET_ENABLE_TEST 0


namespace cppdevtk {
namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief A widget that is not visible on screen.
/// \remark The widget sets Qt::WA_QuitOnClose to false; we consider this to be the expected behaviour
/// and also Qt was inconsistent about this. If this is not desired, use QWidget::setAttribute(Qt::WA_QuitOnClose, true).
class CPPDEVTK_GUI_API InvisibleWidget: public QWidget {
	Q_OBJECT
public:
	explicit InvisibleWidget(QWidget* pParent = NULL, Qt::WindowFlags windowFlags = 0);
#	if (CPPDEVTK_DETAIL_GUI_INVISIBLE_WIDGET_ENABLE_TEST)
	virtual ~InvisibleWidget();
protected:
	virtual void closeEvent(QCloseEvent* pCloseEvent);
#	endif
private:
	Q_DISABLE_COPY(InvisibleWidget)
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline InvisibleWidget::InvisibleWidget(QWidget* pParent, Qt::WindowFlags windowFlags): QWidget(pParent, windowFlags) {
	setAttribute(Qt::WA_DontShowOnScreen, true);
	setAttribute(Qt::WA_QuitOnClose, false);
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_INVISIBLE_WIDGET_HPP_INCLUDED_
