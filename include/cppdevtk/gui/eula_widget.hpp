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


#ifndef CPPDEVTK_GUI_EULA_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_EULA_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"

#include <QtCore/QStringList>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


namespace Ui {
	class EulaWidget;
}


namespace cppdevtk {
namespace gui {


/// \attention EULA file must have the following format: path/completeBaseName[_localeLanguage[_localeCountry]].suffix
/// Example: C:/eula_en_US.htm
/// \remarks
/// - Eula file search order: current locale, default locale, system locale, no locale.
/// - For a given locale try first localeLanguage_localeCountry and if fails localeLanguage
/// \note If Eula file is in Qt resources then when opening default viewer application it will be copied to temp folder
/// and opened from there.
class CPPDEVTK_GUI_API EulaWidget: public QWidget, public WidgetBase {
	Q_OBJECT
public:
	explicit EulaWidget(QWidget* pParent = NULL);
	virtual ~EulaWidget();
public slots:
	void SetSourceInfo(const QString& fileNamePrefix, const QString& fileExt);
protected:
	virtual void changeEvent(QEvent* pEvent);
private slots:
	void OpenEula();
private:
	Q_DISABLE_COPY(EulaWidget)
	
	void ValidateUi() const;
	void MakeConnections();
	
	void Refresh();
	
	
	Ui::EulaWidget* pUiEulaWidget_;
	QString fileNamePrefix_;
	QString fileExt_;
};


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_EULA_WIDGET_HPP_INCLUDED_
