/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_GUI_WIDGET_BASE_HPP_INCLUDED_
#define CPPDEVTK_GUI_WIDGET_BASE_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/base/non_copyable.hpp>

#include <QtCore/QCoreApplication>

#include <cstddef>


namespace cppdevtk {
namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief This class provides common functionality for our widgets.
/// \details This is a workaround for Qt multiple (virtual) inheritance limitations.
/// \sa <a href="http://doc.qt.io/qt-4.8/moc.html#multiple-inheritance-requires-qobject-to-be-first">Qt multiple inheritance</a>
class CPPDEVTK_GUI_API WidgetBase: private ::cppdevtk::base::NonCopyable {
	Q_DECLARE_TR_FUNCTIONS(WidgetBase)
public:
	WidgetBase();
	virtual ~WidgetBase() = 0;
	
	bool SetStyleSheetFromFile(const QString& path, const QString& completeBaseName,
			bool append = true, const QString& suffix = "qss");
	bool SetStyleSheetFromFileCross(const QString& path, const QString& completeBaseName,
			bool append = true, const QString& suffix = "qss");
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline WidgetBase::WidgetBase(): NonCopyable() {}

inline WidgetBase::~WidgetBase() {}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_WIDGET_BASE_HPP_INCLUDED_
