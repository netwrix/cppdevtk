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


#include "udisks1_device_notifier_lnx.hpp"
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/stdexcept.hpp>

#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>

#include <memory>

#include <paths.h>


// https://hal.freedesktop.org/releases/udisks-1.0.5.tar.gz
#define CPPDEVTK_UDISKS1_SERVICE_NAME "org.freedesktop.UDisks"
#define CPPDEVTK_UDISKS1_OBJ_PATH "/org/freedesktop/UDisks"
#define CPPDEVTK_UDISKS1_INTERFACE "org.freedesktop.UDisks"

#define CPPDEVTK_UDISKS1_DEVICE_INTERFACE "org.freedesktop.UDisks.Device"
#define CPPDEVTK_UDISKS1_DEVICES_PATH "/org/freedesktop/UDisks/devices/"


namespace cppdevtk {
namespace gui {
namespace detail {


using ::std::auto_ptr;


UDisks1DeviceNotifier::UDisks1DeviceNotifier(): StorageDeviceNotifier::Impl() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	
	if (!systemBus.connect(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
			"DeviceAdded", this, SLOT(OnDeviceAdded(QDBusObjectPath)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to UDisks1 DeviceAdded signal", systemBus.lastError());
	}
	bool commit = false;
	UDisks1DeviceNotifier* pThis = this;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&pThis)(&systemBus)) {
		if (!commit) {
			if (!systemBus.disconnect(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
					"DeviceAdded", pThis, SLOT(OnDeviceAdded(QDBusObjectPath)))) {
				const QDBusError kLastSystemBusError = systemBus.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from UDisks1 DeviceAdded signal"
						<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
						<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!systemBus.connect(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
			"DeviceRemoved", this, SLOT(OnDeviceRemoved(QDBusObjectPath)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to UDisks1 DeviceRemoved signal", systemBus.lastError());
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&pThis)(&systemBus)) {
		if (!commit) {
			if (!systemBus.disconnect(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
					"DeviceRemoved", pThis, SLOT(OnDeviceRemoved(QDBusObjectPath)))) {
				const QDBusError kLastSystemBusError = systemBus.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from UDisks1 DeviceRemoved signal"
						<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
						<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	const QList<QDBusObjectPath> kUDisks1FilesystemBlockDevicePaths = GetUDisks1FilesystemBlockDevicePaths();
	try {
		for (QList<QDBusObjectPath>::ConstIterator kIter = kUDisks1FilesystemBlockDevicePaths.constBegin();
				kIter != kUDisks1FilesystemBlockDevicePaths.constEnd(); ++kIter) {
			auto_ptr<UDisks1FilesystemBlockDevice> pUDisks1FilesystemBlockDevice(
					new UDisks1FilesystemBlockDevice(*kIter));
			
			CPPDEVTK_VERIFY(connect(pUDisks1FilesystemBlockDevice.get(),
					SIGNAL(Mounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
					SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
			CPPDEVTK_VERIFY(connect(pUDisks1FilesystemBlockDevice.get(),
					SIGNAL(Unmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
					SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
			
			udisks1FilesystemBlockDevices_.append(pUDisks1FilesystemBlockDevice.get());
			
			pUDisks1FilesystemBlockDevice.release();
		}
	}
	catch (...) {
		for (QList<UDisks1FilesystemBlockDevice*>::Iterator iter = udisks1FilesystemBlockDevices_.begin();
				iter != udisks1FilesystemBlockDevices_.end(); ++iter) {
			delete *iter;
		}
	}
	
	commit = true;
}

UDisks1DeviceNotifier::~UDisks1DeviceNotifier() {
	for (QList<UDisks1FilesystemBlockDevice*>::Iterator iter = udisks1FilesystemBlockDevices_.begin();
			iter != udisks1FilesystemBlockDevices_.end(); ++iter) {
		delete *iter;
	}
	
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (systemBus.isConnected()) {
		if (!systemBus.disconnect(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
				"DeviceRemoved", this, SLOT(OnDeviceRemoved(QDBusObjectPath)))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UDisks1 DeviceRemoved signal"
					<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
					<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
		}
		if (!systemBus.disconnect(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
				"DeviceAdded", this, SLOT(OnDeviceAdded(QDBusObjectPath)))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UDisks1 DeviceAdded signal"
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

bool UDisks1DeviceNotifier::IsUDisks1ServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_UDISKS1_SERVICE_NAME, kSystemBus);
}

QString UDisks1DeviceNotifier::GetStorageDeviceName(const QDBusObjectPath& dbusStorageDevicePath) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(dbusStorageDevicePath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH),
			QString("dbusStorageDevicePath: '%1' is not a UDisks1 device path").arg(dbusStorageDevicePath.path()));
	
	return UDisks1FilesystemBlockDevice::GetDeviceFile(dbusStorageDevicePath);
}

QDBusObjectPath UDisks1DeviceNotifier::GetStorageDeviceId(const QString& storageDeviceName) {
	CPPDEVTK_ASSERT(storageDeviceName.startsWith(_PATH_DEV));
	
	const QList<QDBusObjectPath> kFilesystemBlockDevicePaths = GetUDisks1FilesystemBlockDevicePaths();
	for (QList<QDBusObjectPath>::ConstIterator kIter = kFilesystemBlockDevicePaths.constBegin();
			kIter != kFilesystemBlockDevicePaths.constEnd(); ++kIter) {
		if (UDisks1FilesystemBlockDevice::GetDeviceFile(*kIter) == storageDeviceName) {
			return *kIter;
		}
	}
	
	throw CPPDEVTK_RUNTIME_EXCEPTION(QString("no FilesystemBlockDevice for storageDeviceName: %1").arg(storageDeviceName));
}

void UDisks1DeviceNotifier::OnDeviceAdded(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	if (!UDisks1FilesystemBlockDevice::IsBlockDevice(dbusObjectPath)) {
		return;
	}
	if (!UDisks1FilesystemBlockDevice::IsFilesystem(dbusObjectPath)) {
		return;
	}
	
	CPPDEVTK_LOG_DEBUG("udisks1 device added: " << dbusObjectPath.path());
	Q_EMIT StorageDeviceAdded(dbusObjectPath);
	
#	ifndef NDEBUG
	for (QList<UDisks1FilesystemBlockDevice*>::ConstIterator kIter = udisks1FilesystemBlockDevices_.constBegin();
			kIter != udisks1FilesystemBlockDevices_.constEnd(); ++kIter) {
		CPPDEVTK_ASSERT(dbusObjectPath != (*kIter)->GetDBusObjectPath());
	}
#	endif
	
	auto_ptr<UDisks1FilesystemBlockDevice> pUDisks1FilesystemBlockDevice(
			new UDisks1FilesystemBlockDevice(dbusObjectPath));
	
	CPPDEVTK_VERIFY(connect(pUDisks1FilesystemBlockDevice.get(),
			SIGNAL(Mounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
			SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
	CPPDEVTK_VERIFY(connect(pUDisks1FilesystemBlockDevice.get(),
			SIGNAL(Unmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
			SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
	
	udisks1FilesystemBlockDevices_.append(pUDisks1FilesystemBlockDevice.get());
	
	pUDisks1FilesystemBlockDevice.release();
}

void UDisks1DeviceNotifier::OnDeviceRemoved(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS1_DEVICES_PATH));
	
	for (QList<UDisks1FilesystemBlockDevice*>::Iterator iter = udisks1FilesystemBlockDevices_.begin();
			iter != udisks1FilesystemBlockDevices_.end(); ++iter) {
		if (dbusObjectPath == (*iter)->GetDBusObjectPath()) {
			delete *iter;
			udisks1FilesystemBlockDevices_.erase(iter);
			
			CPPDEVTK_LOG_DEBUG("udisks1 device removed: " << dbusObjectPath.path());
			Q_EMIT StorageDeviceRemoved(dbusObjectPath);
			
			break;
		}
	}
}

QList<QDBusObjectPath> UDisks1DeviceNotifier::GetUDisks1FilesystemBlockDevicePaths() {
	QList<QDBusObjectPath> udisks1FilesystemBlockDevicePaths;
	
	const QList<QDBusObjectPath> kDevices = EnumerateDevices();
	for (QList<QDBusObjectPath>::ConstIterator kIter = kDevices.constBegin(); kIter != kDevices.constEnd(); ++kIter) {
		if (UDisks1FilesystemBlockDevice::IsBlockDevice(*kIter)
				&& UDisks1FilesystemBlockDevice::IsFilesystem(*kIter)) {
			CPPDEVTK_LOG_DEBUG("found udisks1FilesystemBlockDevicePath: " << kIter->path());
			udisks1FilesystemBlockDevicePaths.append(*kIter);
		}
	}
	
	return udisks1FilesystemBlockDevicePaths;
}

QList<QDBusObjectPath> UDisks1DeviceNotifier::EnumerateDevices() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", kSystemBus.lastError());
	}
	
	QDBusInterface dbusInterface(CPPDEVTK_UDISKS1_SERVICE_NAME, CPPDEVTK_UDISKS1_OBJ_PATH, CPPDEVTK_UDISKS1_INTERFACE,
			kSystemBus);
	if (!dbusInterface.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("dbusUDisks1Interface interface is not valid", dbusInterface.lastError());
	}
	const QDBusMessage kReply = dbusInterface.call("EnumerateDevices");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("dbusUDisks1Interface call to EnumerateDevices() failed",
				dbusInterface.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "ao");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UserType);
#	ifndef NDEBUG
	qMetaTypeId<QDBusArgument>();	// compile time
#	endif
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("QDBusArgument") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("QDBusArgument") != 0);
#	endif
	CPPDEVTK_ASSERT(kReplyArg.userType() == QMetaType::type("QDBusArgument"));
	const QDBusArgument kDBusReplyArg = kReplyArg.value<QDBusArgument>();
	CPPDEVTK_ASSERT(kDBusReplyArg.currentType() == QDBusArgument::ArrayType);
	
	QList<QDBusObjectPath> devices;
	kDBusReplyArg >> devices;
	return devices;
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
