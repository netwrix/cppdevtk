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


#ifndef CPPDEVTK_GUI_CAPS_LOCK_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_CAPS_LOCK_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"

#include <QtCore/Qt>
#include <QtGui/QColor>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


#define CPPDEVTK_DETAIL_GUI_CAPS_LOCK_WIDGET_ENABLE_TEST 0


namespace Ui {
	class CapsLockWidget;
}


namespace cppdevtk {
namespace gui {


/// \details Example:
/// \code
/// // widget.hpp
/// class Widget: public QWidget {
/// // ...
/// protected:
/// 	virtual void changeEvent(QEvent* pEvent);
/// 	virtual void keyPressEvent(QKeyEvent* pKeyEvent);
/// 	virtual void keyReleaseEvent(QKeyEvent* pKeyEvent);
/// // ...
/// };
///
/// // widget.cpp
/// void Widget::changeEvent(QEvent* pEvent) {
/// 	QWidget::changeEvent(pEvent);
/// 	switch (pEvent->type()) {
///			case QEvent::ApplicationActivate:
///				pUi_->pCapsLockWidget_->Refresh();
///				break;
/// 		case QEvent::WindowActivate:
/// 			pUi_->pCapsLockWidget_->Refresh();
/// 			break;
/// 		case QEvent::ActivationChange:
/// 			pUi_->pCapsLockWidget_->Refresh();
/// 			break;
/// 		case QEvent::WindowStateChange:
/// 			pUi_->pCapsLockWidget_->Refresh();
/// 			break;
/// 		default:
/// 			break;
/// 	}
/// }
/// 
/// void Widget::keyPressEvent(QKeyEvent* pKeyEvent) {
/// 	QWidget::keyPressEvent(pKeyEvent);
/// 	switch (pKeyEvent->key()) {
/// 		case Qt::Key_CapsLock:
/// 			pUi_->pCapsLockWidget_->Refresh();
/// 			break;
/// 		default:
/// 			break;
/// 	}
/// }
/// 
/// void Widget::keyReleaseEvent(QKeyEvent* pKeyEvent) {
/// 	QWidget::keyReleaseEvent(pKeyEvent);
/// 	switch (pKeyEvent->key()) {
/// 		case Qt::Key_CapsLock:
/// 			pUi_->pCapsLockWidget_->Refresh();
/// 			break;
/// 		default:
/// 			break;
/// 	}
/// }
/// \endcode
/// \note
/// - default OnColor is Qt::darkRed
/// - default OffColor is Qt::darkGreen
class CPPDEVTK_GUI_API CapsLockWidget: public QWidget, public WidgetBase {
	Q_OBJECT
public:
	explicit CapsLockWidget(QWidget* pParent = NULL);
	virtual ~CapsLockWidget();
	
	void SetOnColor(const QColor& onColor);
	QColor GetOnColor() const;
	
	void SetOffColor(const QColor& offColor);
	QColor GetOffColor() const;
	
	void SetBold(bool value);
	bool GetBold() const;
public slots:
	void Refresh();
protected:
	virtual void changeEvent(QEvent* pEvent);
	virtual void showEvent(QShowEvent* pShowEvent);
#	if (CPPDEVTK_DETAIL_GUI_CAPS_LOCK_WIDGET_ENABLE_TEST)
	virtual void keyPressEvent(QKeyEvent* pKeyEvent);
	virtual void keyReleaseEvent(QKeyEvent* pKeyEvent);
#	endif
private:
	Q_DISABLE_COPY(CapsLockWidget)
	
	
	Ui::CapsLockWidget* pUiCapsLockWidget_;
	QColor onColor_;
	QColor offColor_;
	bool bold_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline void CapsLockWidget::SetOnColor(const QColor& onColor) {
	onColor_ = onColor;
	
	Refresh();
}

inline QColor CapsLockWidget::GetOnColor() const {
	return onColor_;
}

inline void CapsLockWidget::SetOffColor(const QColor& offColor) {
	offColor_ = offColor;
	
	Refresh();
}

inline QColor CapsLockWidget::GetOffColor() const {
	return offColor_;
}

inline void CapsLockWidget::SetBold(bool value) {
	bold_ = value;
}

inline bool CapsLockWidget::GetBold() const {
	return bold_;
}

inline void CapsLockWidget::showEvent(QShowEvent* pShowEvent) {	
	QWidget::showEvent(pShowEvent);
	
	Refresh();
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_CAPS_LOCK_WIDGET_HPP_INCLUDED_
