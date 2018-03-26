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


#ifndef CPPDEVTK_GUI_TIMED_COMPUTER_MANAGEMENT_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_GUI_TIMED_COMPUTER_MANAGEMENT_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "widget_base.hpp"

#include <QtCore/QMetaType>
#include <QtCore/QList>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>


namespace Ui {
	class TimedComputerManagementWidget;
}


namespace cppdevtk {
namespace gui {


// NOTE: Q_ENUMS(), fully qualified enum, Q_DECLARE_METATYPE() and qRegisterMetaType() are need
// to make an enum usable in QT's in signal/slot mechanism.
// Please see: Enums and Typedefs Must Be Fully Qualified for Signal and Slot Parameters
// http://doc.qt.io/qt-4.8/moc.html#enums-and-typedefs-must-be-fully-qualified-for-signal-and-slot-parameters)

class CPPDEVTK_GUI_API TimedComputerManagementWidget: public QWidget, public WidgetBase {
	Q_OBJECT
	Q_ENUMS(Method)
public:
	enum Method {
		cmmFirst = 0,
		cmmNone = cmmFirst,
		cmmShutdownComputer,
		cmmLockComputer,
		cmmLogoutUser,
		cmmLast = cmmLogoutUser	// TODO: keep updated
	};
	
	
	explicit TimedComputerManagementWidget(QWidget* pParent = NULL);
	virtual ~TimedComputerManagementWidget();
	
	bool IsChecked() const;
	Method GetCurrentMethod() const;
	
	// timeout combobox support
	void AddTimeout(int timeout);
	void AddTimeouts(const QList<int>& timeouts);
	int GetTimeoutCount() const;
	int GetTimeout(int index) const;
	void RemoveTimeoutItem(int index);
	int GetCurrentTimeoutIndex() const;
	int GetCurrentTimeout() const;
	void SetCurrentTimeout(int timeout);
	void RemoveCurrentTimeoutItem();
Q_SIGNALS:
	void Toggled(bool checked);
	void CurrentMethodChanged(::cppdevtk::gui::TimedComputerManagementWidget::Method method);
	void CurrentTimeoutChanged(int timeout);
public Q_SLOTS:
	void SetChecked(bool value);
	void SetCurrentMethod(::cppdevtk::gui::TimedComputerManagementWidget::Method method);
	
	void ClearTimeout();
	void SetCurrentTimeoutIndex(int index);
protected:
	virtual void changeEvent(QEvent* pEvent);
private Q_SLOTS:
	void OnMethodRadioButtonToggled(bool checked);
	void OnCurrentTimeoutIndexChanged(const QString& text);
private:
	Q_DISABLE_COPY(TimedComputerManagementWidget)
	
	void ValidateUi() const;
	void MakeConnections();
	
	
	Ui::TimedComputerManagementWidget* pUiTimedComputerManagementWidget_;
};


}	// namespace gui
}	// namespace cppdevtk


// NOTE: do not global qualify because moc will generate bad code
Q_DECLARE_METATYPE(cppdevtk::gui::TimedComputerManagementWidget::Method)




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

namespace cppdevtk {
namespace gui {


inline void TimedComputerManagementWidget::AddTimeouts(const QList<int>& timeouts) {
	for (QList<int>::ConstIterator kIter = timeouts.constBegin(); kIter != timeouts.constEnd(); ++kIter) {
		AddTimeout(*kIter);
	}
}

inline int TimedComputerManagementWidget::GetCurrentTimeout() const {
	return GetTimeout(GetCurrentTimeoutIndex());
}

inline void TimedComputerManagementWidget::RemoveCurrentTimeoutItem() {
	RemoveTimeoutItem(GetCurrentTimeoutIndex());
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_TIMED_COMPUTER_MANAGEMENT_WIDGET_HPP_INCLUDED_
