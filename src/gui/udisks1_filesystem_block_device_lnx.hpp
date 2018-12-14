/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_GUI_UDISKS1_FILESYSTEM_BLOCK_DEVICE_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_UDISKS1_FILESYSTEM_BLOCK_DEVICE_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/gui/storage_device_notifier.hpp>

#include <QtCore/QObject>
#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtDBus/QDBusObjectPath>


namespace cppdevtk {
namespace gui {
namespace detail {


class UDisks1FilesystemBlockDevice: public QObject {
	friend class UDisks1DeviceNotifier;
	
	Q_OBJECT
Q_SIGNALS:
	void Mounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void Unmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
public:
	virtual ~UDisks1FilesystemBlockDevice();
	
	QDBusObjectPath GetDBusObjectPath() const;
	
	
	QString GetDeviceFile() const;
	bool GetDeviceIsDrive() const;
	bool GetDeviceIsMounted() const;
	qulonglong GetDeviceBlockSize() const;
	QString GetIdUsage() const;
	
	static QString GetDeviceFile(const QDBusObjectPath& dbusObjectPath);
	static bool GetDeviceIsDrive(const QDBusObjectPath& dbusObjectPath);
	static bool GetDeviceIsMounted(const QDBusObjectPath& dbusObjectPath);
	static qulonglong GetDeviceBlockSize(const QDBusObjectPath& dbusObjectPath);
	static QString GetIdUsage(const QDBusObjectPath& dbusObjectPath);
	
	static bool IsBlockDevice(const QDBusObjectPath& dbusObjectPath);
	static bool IsFilesystem(const QDBusObjectPath& dbusObjectPath);
private Q_SLOTS:
	void OnChanged();
private:
	explicit UDisks1FilesystemBlockDevice(const QDBusObjectPath& dbusObjectPath);
	
	Q_DISABLE_COPY(UDisks1FilesystemBlockDevice);
	
	
	const QDBusObjectPath kDBusObjectPath_;
	bool deviceIsMounted_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QDBusObjectPath UDisks1FilesystemBlockDevice::GetDBusObjectPath() const {
	return kDBusObjectPath_;
}

inline QString UDisks1FilesystemBlockDevice::GetDeviceFile() const {
	return GetDeviceFile(kDBusObjectPath_);
}

inline bool UDisks1FilesystemBlockDevice::GetDeviceIsDrive() const {
	return GetDeviceIsDrive(kDBusObjectPath_);
}

inline bool UDisks1FilesystemBlockDevice::GetDeviceIsMounted() const {
	return GetDeviceIsMounted(kDBusObjectPath_);
}

inline qulonglong UDisks1FilesystemBlockDevice::GetDeviceBlockSize() const {
	return GetDeviceBlockSize(kDBusObjectPath_);
}

inline QString UDisks1FilesystemBlockDevice::GetIdUsage() const {
	return GetIdUsage(kDBusObjectPath_);
}

inline bool UDisks1FilesystemBlockDevice::IsBlockDevice(const QDBusObjectPath& dbusObjectPath) {
	return GetDeviceBlockSize(dbusObjectPath) > 0;
}

inline bool UDisks1FilesystemBlockDevice::IsFilesystem(const QDBusObjectPath& dbusObjectPath) {
	return GetIdUsage(dbusObjectPath) == "filesystem";
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_UDISKS1_FILESYSTEM_BLOCK_DEVICE_LNX_HPP_INCLUDED_
