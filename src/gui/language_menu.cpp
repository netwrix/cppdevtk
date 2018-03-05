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


#include <cppdevtk/gui/language_menu.hpp>
#include <cppdevtk/util/core_application_base.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QVariant>
#include <QtCore/QtAlgorithms>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#else
#include <QtGui/QAction>
#include <QtGui/QApplication>
#endif

#include <new>


namespace cppdevtk {
namespace gui {


using ::cppdevtk::util::LanguageInfo;


LanguageMenu::LanguageMenu(QWidget* pParent): QMenu(pParent), pActionGroup_(new QActionGroup(this)) {
#	ifndef NDEBUG
	LanguageInfo languageInfo;
	CPPDEVTK_ASSERT(QMetaType::type("cppdevtk::util::LanguageInfo") != QMetaType::UnknownType);
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	CPPDEVTK_ASSERT(QMetaType::hasRegisteredComparators< ::cppdevtk::util::LanguageInfo>());
#	endif
#	endif
	
	SetTitle();
	
	CPPDEVTK_VERIFY(connect(this, SIGNAL(triggered(QAction*)), this, SLOT(ConvertTriggered(QAction*))));
}

int LanguageMenu::GetCount() const {
	CPPDEVTK_ASSERT(actions().count() == pActionGroup_->actions().count());
	
	return pActionGroup_->actions().count();
}

QList< ::cppdevtk::util::LanguageInfo> LanguageMenu::GetLanguageInfos() const {
	QList<LanguageInfo> languageInfos;
	
	const QList<QAction*> kActions = pActionGroup_->actions();
	for (QList<QAction*>::ConstIterator kIter = kActions.constBegin(); kIter != kActions.constEnd(); ++kIter) {
		languageInfos.append((*kIter)->data().value<LanguageInfo>());
	}
	
	qSort(languageInfos.begin(), languageInfos.end());
	
	return languageInfos;
}

::cppdevtk::util::LanguageInfo LanguageMenu::GetCheckedLanguageInfo() const {
	QAction* pAction = pActionGroup_->checkedAction();
	return (pAction != NULL) ? pAction->data().value<LanguageInfo>() : LanguageInfo::GetCLanguageInfo();
}

void LanguageMenu::AddLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((languageInfo != LanguageInfo::GetCLanguageInfo()), "languageInfo: C");
	
	// do not allow duplicates
	const QList<QAction*> kActions = pActionGroup_->actions();
	for (QList<QAction*>::ConstIterator kIter = kActions.constBegin(); kIter != kActions.constEnd(); ++kIter) {
		if (languageInfo == (*kIter)->data().value<LanguageInfo>()) {
			return;
		}
	}
	
	QAction* pAction = new QAction(languageInfo.GetNativeName(), NULL);
	pAction->setData(QVariant::fromValue(languageInfo));
	pAction->setCheckable(true);
	pActionGroup_->addAction(pAction);
	addAction(pAction);
}

void LanguageMenu::RemoveLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((languageInfo != LanguageInfo::GetCLanguageInfo()), "languageInfo: C");
	
	const QList<QAction*> kActions = pActionGroup_->actions();
	for (QList<QAction*>::ConstIterator kIter = kActions.constBegin(); kIter != kActions.constEnd(); ++kIter) {
		if (languageInfo == (*kIter)->data().value<LanguageInfo>()) {
			pActionGroup_->removeAction(*kIter);
			removeAction(*kIter);
			delete *kIter;
			break;
		}
	}
}

void LanguageMenu::Clear() {
	const QList<QAction*> kActions = pActionGroup_->actions();
	for (QList<QAction*>::ConstIterator kIter = kActions.constBegin(); kIter != kActions.constEnd(); ++kIter) {
		pActionGroup_->removeAction(*kIter);
		removeAction(*kIter);
		delete *kIter;
	}
}

void LanguageMenu::SetCheckedLanguageInfo(const ::cppdevtk::util::LanguageInfo& languageInfo) {
	if (languageInfo == LanguageInfo::GetCLanguageInfo()) {
		QAction* pAction = pActionGroup_->checkedAction();
		if (pAction != NULL) {
			pAction->setChecked(false);
			return;
		}
	}
	
	const QList<QAction*> kActions = pActionGroup_->actions();
	for (QList<QAction*>::ConstIterator kIter = kActions.constBegin(); kIter != kActions.constEnd(); ++kIter) {
		if (languageInfo == (*kIter)->data().value<LanguageInfo>()) {
			(*kIter)->setChecked(true);
			break;
		}
	}
}

void LanguageMenu::changeEvent(QEvent* pEvent) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(pEvent);
	
	QMenu::changeEvent(pEvent);
	
	switch (pEvent->type()) {
		case QEvent::LanguageChange:
			SetTitle();
			break;
		default:
			break;
	}
}

void LanguageMenu::ConvertTriggered(QAction* pAction) {
	if (pAction != NULL) {
		emit Triggered(pAction->data().value<LanguageInfo>());
	}
	else {
		emit Triggered(LanguageInfo::GetCLanguageInfo());
	}
}

void LanguageMenu::SetTitle() {
	setTitle(tr("&Language"));
}


}	// namespace gui
}	// namespace cppdevtk
