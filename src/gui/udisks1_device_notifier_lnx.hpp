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


#ifndef CPPDEVTK_GUI_UDISKS1_DEVICE_NOTIFIER_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_UDISKS1_DEVICE_NOTIFIER_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#include "storage_device_notifier_impl_lnx.hpp"
#include "udisks1_filesystem_block_device_lnx.hpp"

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtDBus/QDBusObjectPath>


namespace cppdevtk {
namespace gui {
namespace detail {


class UDisks1DeviceNotifier: public StorageDeviceNotifier::Impl {
	Q_OBJECT
public:
	UDisks1DeviceNotifier();
	virtual ~UDisks1DeviceNotifier();
	
	
	static bool IsUDisks1ServiceRegistered();
	static QString GetStorageDeviceName(const QDBusObjectPath& dbusStorageDevicePath);
	static QDBusObjectPath GetStorageDeviceId(const QString& storageDeviceName);
private Q_SLOTS:
	void OnDeviceAdded(const QDBusObjectPath& dbusObjectPath);
	void OnDeviceRemoved(const QDBusObjectPath& dbusObjectPath);
private:
	Q_DISABLE_COPY(UDisks1DeviceNotifier);
	
	
	static QList<QDBusObjectPath> GetUDisks1FilesystemBlockDevicePaths();
	static QList<QDBusObjectPath> EnumerateDevices();
	
	
	QList<UDisks1FilesystemBlockDevice*> udisks1FilesystemBlockDevices_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_UDISKS1_DEVICE_NOTIFIER_LNX_HPP_INCLUDED_
