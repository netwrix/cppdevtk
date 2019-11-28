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


#ifndef CPPDEVTK_TEST_PC_MAN_WIDGET_HPP_INCLUDED_
#define CPPDEVTK_TEST_PC_MAN_WIDGET_HPP_INCLUDED_


#include "config.hpp"
#include "ui_widget.h"
#include <cppdevtk/gui/widget_base.hpp>
#include <cppdevtk/gui/storage_device_notifier.hpp>
#include <cppdevtk/gui/session.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

#include <cstddef>
#include <memory>


namespace cppdevtk {
namespace test_pc_man {


class Widget: public QWidget, public ::cppdevtk::gui::WidgetBase, private Ui::Widget {
	Q_OBJECT
public:
	explicit Widget(QWidget* pParent = NULL);
	virtual ~Widget();
protected:
	virtual void changeEvent(QEvent* pEvent);
	virtual void closeEvent(QCloseEvent* pCloseEvent);
private Q_SLOTS:
	void ScreenSaverActiveChanged(bool isActive);
	void ActivateScreenSaver();
	void DeactivateScreenSaver();
	void LockScreenSaver();
	
	void OnStorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void OnStorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void OnStorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void OnStorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	
	void OnSleeping();
	void OnResuming();
	
	void OnSessionActivated();
	void OnSessionDeactivated();
	void OnSessionLocked();
	void OnSessionUnlocked();
	void LockSession();
	void GetSessionId();
	
	void Logout();
	void Shutdown();
	void GetIdleTime();
	
	void OnIdleTimerTimeout();
private:
	Q_DISABLE_COPY(Widget)
	
	void MakeConnections();
	
	::std::auto_ptr< ::cppdevtk::gui::Session> pSession_;
};


}	// namespace test_pc_man
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_PC_MAN_WIDGET_HPP_INCLUDED_
