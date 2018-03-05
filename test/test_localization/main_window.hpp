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


#ifndef CPPDEVTK_TEST_LOCALIZATION_MAIN_WINDOW_HPP_INCLUDED_
#define CPPDEVTK_TEST_LOCALIZATION_MAIN_WINDOW_HPP_INCLUDED_


#include "config.hpp"
#include "ui_main_window.h"
#include "application.hpp"

#include <cppdevtk/gui/widget_base.hpp>
#include <cppdevtk/util/language_info.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#else
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#endif

#include <cstddef>


namespace cppdevtk {
namespace test_localization {


class MainWindow: public QMainWindow, public ::cppdevtk::gui::WidgetBase, private Ui::MainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* pParent = NULL);
protected:
	virtual void changeEvent(QEvent* pEvent);
	virtual void showEvent(QShowEvent* pShowEvent);
private slots:
	void Exit();
	
	void ChangeLanguage(const ::cppdevtk::util::LanguageInfo& languageInfo);
	
	void About();
	void AboutQt();
	void Homepage();
	void Support();
	void Email();
private:
	Q_DISABLE_COPY(MainWindow)
	
	void SetupUi();
	void SetupStrings();
	void PopulateLanguages();
	void MakeConnections();
	void SetCurrentLanguage();
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline void MainWindow::Exit() {
	close();
}

inline void MainWindow::AboutQt() {
	Application::aboutQt();
}


}	// namespace test_localization
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_LOCALIZATION_MAIN_WINDOW_HPP_INCLUDED_
