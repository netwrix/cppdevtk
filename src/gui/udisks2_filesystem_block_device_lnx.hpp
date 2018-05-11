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


#ifndef CPPDEVTK_GUI_UDISKS2_FILESYSTEM_BLOCK_DEVICE_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_UDISKS2_FILESYSTEM_BLOCK_DEVICE_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/gui/storage_device_notifier.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QDBusObjectPath>


namespace cppdevtk {
namespace gui {
namespace detail {


class UDisks2FilesystemBlockDevice: public QObject {
	friend class UDisks2DeviceNotifier;
	
	Q_OBJECT
Q_SIGNALS:
	void Mounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void Unmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
public:
	virtual ~UDisks2FilesystemBlockDevice();
	
	QDBusObjectPath GetDBusObjectPath() const;
	
	QString GetDevice() const;
	
	static QString GetDevice(const QDBusObjectPath& dbusObjectPath);
private Q_SLOTS:
	void OnPropertiesChanged(const QString& interfaceName, const QVariantMap& changedProperties,
			const QStringList& invalidatedProperties);
private:
	explicit UDisks2FilesystemBlockDevice(const QDBusObjectPath& dbusObjectPath);
	
	Q_DISABLE_COPY(UDisks2FilesystemBlockDevice);
	
	
	const QDBusObjectPath kDBusObjectPath_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QDBusObjectPath UDisks2FilesystemBlockDevice::GetDBusObjectPath() const {
	return kDBusObjectPath_;
}

inline QString UDisks2FilesystemBlockDevice::GetDevice() const {
	return GetDevice(kDBusObjectPath_);
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_UDISKS2_FILESYSTEM_BLOCK_DEVICE_LNX_HPP_INCLUDED_
