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


#ifndef CPPDEVTK_GUI_LANGUAGE_MENU_HPP_INCLUDED_
#define CPPDEVTK_GUI_LANGUAGE_MENU_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/util/language_info.hpp>

#include <QtCore/QList>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QMenu>
#include <QtWidgets/QActionGroup>
#else
#include <QtGui/QMenu>
#include <QtGui/QActionGroup>
#endif

#include <cstddef>


namespace cppdevtk {
namespace gui {


class CPPDEVTK_GUI_API LanguageMenu: public QMenu {
	Q_OBJECT
signals:
	void Triggered(const ::cppdevtk::util::LanguageInfo& languageInfo);
public:
	explicit LanguageMenu(QWidget* pParent = NULL);
	virtual ~LanguageMenu();
	
	int GetCount() const;
	
	/// \remark For convenience returned list is sorted by native name.
	QList< ::cppdevtk::util::LanguageInfo> GetLanguageInfos() const;
	
	::cppdevtk::util::LanguageInfo GetCheckedLanguageInfo() const;	///< \return LanguageInfo() if no (checked) item
	
	void AddLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo);
	void AddLanguageInfos(const QList< ::cppdevtk::util::LanguageInfo>& languageInfos);
	
	/// \attention May not emit Triggered()
	void RemoveLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo);
public slots:
	void Clear();
	void SetCheckedLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo);
protected:
	virtual void changeEvent(QEvent* pEvent);
private slots:
	void ConvertTriggered(QAction* pAction);
private:
	Q_DISABLE_COPY(LanguageMenu)
	
	void SetTitle();
	
	
	QActionGroup* pActionGroup_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline LanguageMenu::~LanguageMenu() {
	Clear();
}

inline void LanguageMenu::AddLanguageInfos(const QList< ::cppdevtk::util::LanguageInfo>& languageInfos) {
	for (QList< ::cppdevtk::util::LanguageInfo>::ConstIterator kIter = languageInfos.constBegin();
			kIter != languageInfos.constEnd(); ++kIter) {
		AddLanguageInfo(*kIter);
	}
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_LANGUAGE_MENU_HPP_INCLUDED_
