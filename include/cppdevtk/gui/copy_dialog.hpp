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


#ifndef CPPDEVTK_GUI_COPY_DIALOG_HPP_INCLUDED_
#define CPPDEVTK_GUI_COPY_DIALOG_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_ENABLE_QT_SOLUTIONS)
#error "This file require QtSolutions enabled!!!"
#endif

#include <cppdevtk/QtSolutions/QtCopyDialog/QtCopyDialog>
#include "widget_base.hpp"

#include <cstddef>


namespace cppdevtk {
namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
class CPPDEVTK_GUI_API CopyDialog: public ::cppdevtk::qtsol::QtCopyDialog, public WidgetBase {
	Q_OBJECT
public:
    explicit CopyDialog(QWidget* pParent = NULL,
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		Qt::WindowFlags
#		else
		Qt::WFlags
#		endif
		wFlags = 0);
	
    explicit CopyDialog(::cppdevtk::qtsol::QtFileCopier* pFileCopier, QWidget* pParent = NULL,
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		Qt::WindowFlags
#		else
		Qt::WFlags
#		endif
		wFlags = 0);
	
	virtual ~CopyDialog();
private:
	Q_DISABLE_COPY(CopyDialog)
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline CopyDialog::~CopyDialog() {}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_COPY_DIALOG_HPP_INCLUDED_
