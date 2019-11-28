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


#ifndef CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_IMPL_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_IMPL_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/gui/storage_device_notifier.hpp>

#include <QtCore/QObject>


namespace cppdevtk {
namespace gui {


class StorageDeviceNotifier::Impl: public QObject {
	Q_OBJECT
Q_SIGNALS:
	void StorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void StorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	
	void StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
protected:
	Impl();
private:
	Q_DISABLE_COPY(Impl);
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline StorageDeviceNotifier::Impl::Impl(): QObject() {}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_IMPL_LNX_HPP_INCLUDED_
