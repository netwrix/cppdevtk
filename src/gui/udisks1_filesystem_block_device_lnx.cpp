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


#include "udisks1_filesystem_block_device_lnx.hpp"

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>

#include <stdexcept>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusConnection>


// https://hal.freedesktop.org/releases/udisks-1.0.5.tar.gz
#define CPPDEVTK_UDISKS1_SERVICE_NAME "org.freedesktop.UDisks"
#define CPPDEVTK_UDISKS1_INTERFACE "org.freedesktop.UDisks"

#define CPPDEVTK_UDISKS1_DEVICE_INTERFACE "org.freedesktop.UDisks.Device"
#define CPPDEVTK_UDISKS1_DEVICES_PATH "/org/freedesktop/UDisks/devices/"


namespace cppdevtk {
namespace gui {
namespace detail {


UDisks1FilesystemBlockDevice::~UDisks1FilesystemBlockDevice() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (systemBus.isConnected()) {
		if (!systemBus.disconnect(CPPDEVTK_UDISKS1_SERVICE_NAME, kDBusObjectPath_.path(),
				CPPDEVTK_UDISKS1_DEVICE_INTERFACE, "Changed", this, SLOT(OnChanged()))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UDisks1 Device interface Changed signal"
					<< " for dbusObjectPath: " << kDBusObjectPath_.path()
					<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
					<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
		}
	}
	else {
		const QDBusError kLastSystemBusError = systemBus.lastError();
		CPPDEVTK_LOG_WARN("failed to connect to system bus"
				<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
				<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
	}
}

QString UDisks1FilesystemBlockDevice::GetDeviceFile(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	return util::GetDBusStringProperty(CPPDEVTK_UDISKS1_SERVICE_NAME, dbusObjectPath,
		CPPDEVTK_UDISKS1_INTERFACE, QDBusConnection::systemBus(), "DeviceFile");
}

bool UDisks1FilesystemBlockDevice::GetDeviceIsDrive(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	return util::GetDBusBooleanProperty(CPPDEVTK_UDISKS1_SERVICE_NAME, dbusObjectPath,
		CPPDEVTK_UDISKS1_INTERFACE, QDBusConnection::systemBus(), "DeviceIsDrive");
}

bool UDisks1FilesystemBlockDevice::GetDeviceIsMounted(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	return util::GetDBusBooleanProperty(CPPDEVTK_UDISKS1_SERVICE_NAME, dbusObjectPath,
		CPPDEVTK_UDISKS1_INTERFACE, QDBusConnection::systemBus(), "DeviceIsMounted");
}

qulonglong UDisks1FilesystemBlockDevice::GetDeviceBlockSize(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	return util::GetDBusUInt64Property(CPPDEVTK_UDISKS1_SERVICE_NAME, dbusObjectPath,
		CPPDEVTK_UDISKS1_INTERFACE, QDBusConnection::systemBus(), "DeviceBlockSize");
}

QString UDisks1FilesystemBlockDevice::GetIdUsage(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	return util::GetDBusStringProperty(CPPDEVTK_UDISKS1_SERVICE_NAME, dbusObjectPath,
		CPPDEVTK_UDISKS1_INTERFACE, QDBusConnection::systemBus(), "IdUsage");
}

void UDisks1FilesystemBlockDevice::OnChanged() {
	const bool kDeviceWasMounted_ = deviceIsMounted_;
	try {
		deviceIsMounted_ = GetDeviceIsMounted();
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_WARN("GetDeviceIsMounted() failed; exc: " << base::Exception::GetDetailedInfo(exc));
		return;
	}
	
	if (kDeviceWasMounted_ != deviceIsMounted_) {
		if (deviceIsMounted_) {
			Q_EMIT Mounted(kDBusObjectPath_);
		}
		else {
			Q_EMIT Unmounted(kDBusObjectPath_);
		}
	}
}

UDisks1FilesystemBlockDevice::UDisks1FilesystemBlockDevice(const QDBusObjectPath& dbusObjectPath): QObject(),
		kDBusObjectPath_(dbusObjectPath), deviceIsMounted_(GetDeviceIsMounted(dbusObjectPath)) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	CPPDEVTK_ASSERT(IsBlockDevice(dbusObjectPath));
	CPPDEVTK_ASSERT(IsFilesystem(dbusObjectPath));
	
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	if (!systemBus.connect(CPPDEVTK_UDISKS1_SERVICE_NAME, kDBusObjectPath_.path(),
			CPPDEVTK_UDISKS1_DEVICE_INTERFACE, "Changed", this, SLOT(OnChanged()))) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("failed to connect to UDisks1 Device interface Changed signal for dbusObjectPath %1").arg(
				kDBusObjectPath_.path()), systemBus.lastError());
	}
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
