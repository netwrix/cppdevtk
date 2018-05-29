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


#include "udisks2_device_notifier_lnx.hpp"
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/stdexcept.hpp>

#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>

#include <new>
#include <memory>

#include <paths.h>


// http://storaged.org/doc/udisks2-api/latest/ref-dbus.html

// http://storaged.org/doc/udisks2-api/latest/ref-dbus-overview.html#ref-dbus-well-known-name
#define CPPDEVTK_UDISKS2_SERVICE_NAME "org.freedesktop.UDisks2"

// http://storaged.org/doc/udisks2-api/latest/ref-dbus-udisks2-well-known-object.html
#define CPPDEVTK_UDISKS2_OBJ_PATH "/org/freedesktop/UDisks2"

// http://storaged.org/doc/udisks2-api/latest/ref-dbus-manager-well-known-object.html
#define CPPDEVTK_UDISKS2_MANAGER_OBJ_PATH "/org/freedesktop/UDisks2/Manager"

// http://storaged.org/doc/udisks2-api/latest/gdbus-org.freedesktop.UDisks2.Manager.html
#define CPPDEVTK_UDISKS2_MANAGER_INTERFACE "org.freedesktop.UDisks2.Manager"

// http://storaged.org/doc/udisks2-api/latest/ref-dbus-block-devices.html
#define CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH "/org/freedesktop/UDisks2/block_devices/"

// http://storaged.org/doc/udisks2-api/latest/gdbus-org.freedesktop.UDisks2.Block.html
#define CPPDEVTK_UDISKS2_BLOCK_INTERFACE "org.freedesktop.UDisks2.Block"

// http://storaged.org/doc/udisks2-api/latest/gdbus-org.freedesktop.UDisks2.Filesystem.html
#define CPPDEVTK_UDISKS2_FILESYSTEM_INTERFACE "org.freedesktop.UDisks2.Filesystem"


// https://dbus.freedesktop.org/doc/dbus-specification.html

// https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-objectmanager
#define CPPDEVTK_DBUS_OBJ_MAN_INTERFACE "org.freedesktop.DBus.ObjectManager"

// https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-properties
#define CPPDEVTK_DBUS_PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"


namespace cppdevtk {
namespace gui {
namespace detail {


using ::std::auto_ptr;


UDisks2DeviceNotifier::UDisks2DeviceNotifier(): StorageDeviceNotifier::Impl(), udisks2FilesystemBlockDevices_() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	
	if (!systemBus.connect(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH, CPPDEVTK_DBUS_OBJ_MAN_INTERFACE,
			"InterfacesAdded", this, SLOT(OnInterfacesAdded(QDBusObjectPath,QVariantMap)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to UDisks2 InterfacesAdded signal", systemBus.lastError());
	}
	bool commit = false;
	UDisks2DeviceNotifier* pThis = this;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&pThis)(&systemBus)) {
		if (!commit) {
			if (!systemBus.disconnect(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH,
					CPPDEVTK_DBUS_OBJ_MAN_INTERFACE, "InterfacesAdded",
					pThis, SLOT(OnInterfacesAdded(QDBusObjectPath,QVariantMap)))) {
				const QDBusError kLastSystemBusError = systemBus.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from UDisks2 InterfacesAdded signal"
						<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
						<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!systemBus.connect(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH, CPPDEVTK_DBUS_OBJ_MAN_INTERFACE,
			"InterfacesRemoved", this, SLOT(OnInterfacesRemoved(QDBusObjectPath,QStringList)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to UDisks2 InterfacesRemoved signal", systemBus.lastError());
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&pThis)(&systemBus)) {
		if (!commit) {
			if (!systemBus.disconnect(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH,
					CPPDEVTK_DBUS_OBJ_MAN_INTERFACE, "InterfacesRemoved",
					pThis, SLOT(OnInterfacesRemoved(QDBusObjectPath,QStringList)))) {
				const QDBusError kLastSystemBusError = systemBus.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from UDisks2 InterfacesRemoved signal"
						<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
						<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	const QList<QDBusObjectPath> kUDisks2FilesystemBlockDevicePaths = GetUDisks2FilesystemBlockDevicePaths();
	try {
		for (QList<QDBusObjectPath>::ConstIterator kIter = kUDisks2FilesystemBlockDevicePaths.constBegin();
				kIter != kUDisks2FilesystemBlockDevicePaths.constEnd(); ++kIter) {
			auto_ptr<UDisks2FilesystemBlockDevice> pUDisks2FilesystemBlockDevice(
					new UDisks2FilesystemBlockDevice(*kIter));
			
			CPPDEVTK_VERIFY(connect(pUDisks2FilesystemBlockDevice.get(),
					SIGNAL(Mounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
					SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
			CPPDEVTK_VERIFY(connect(pUDisks2FilesystemBlockDevice.get(),
					SIGNAL(Unmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
					SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
			
			udisks2FilesystemBlockDevices_.append(pUDisks2FilesystemBlockDevice.get());
			
			pUDisks2FilesystemBlockDevice.release();
		}
	}
	catch (...) {
		for (QList<UDisks2FilesystemBlockDevice*>::Iterator iter = udisks2FilesystemBlockDevices_.begin();
				iter != udisks2FilesystemBlockDevices_.end(); ++iter) {
				delete *iter;
		}
		
		throw;
	}
	
	commit = true;
}

UDisks2DeviceNotifier::~UDisks2DeviceNotifier() {
	for (QList<UDisks2FilesystemBlockDevice*>::Iterator iter = udisks2FilesystemBlockDevices_.begin();
			iter != udisks2FilesystemBlockDevices_.end(); ++iter) {
			delete *iter;
	}
	
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (systemBus.isConnected()) {
		if (!systemBus.disconnect(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH, CPPDEVTK_DBUS_OBJ_MAN_INTERFACE,
				"InterfacesRemoved", this, SLOT(OnInterfacesRemoved(QDBusObjectPath,QStringList)))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UDisks2 InterfacesRemoved signal"
					<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
					<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
		}
		if (!systemBus.disconnect(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH, CPPDEVTK_DBUS_OBJ_MAN_INTERFACE,
				"InterfacesAdded", this, SLOT(OnInterfacesAdded(QDBusObjectPath,QVariantMap)))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UDisks2 InterfacesAdded signal"
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

bool UDisks2DeviceNotifier::IsUDisks2ServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_UDISKS2_SERVICE_NAME, kSystemBus);
}

QString UDisks2DeviceNotifier::GetStorageDeviceName(const QDBusObjectPath& dbusStorageDevicePath) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(dbusStorageDevicePath.path().startsWith(CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH),
			QString("dbusStorageDevicePath: '%1' is not a UDisks2 block device path").arg(dbusStorageDevicePath.path()));
	
	return UDisks2FilesystemBlockDevice::GetDevice(dbusStorageDevicePath);
}

QDBusObjectPath UDisks2DeviceNotifier::GetStorageDeviceId(const QString& storageDeviceName) {
	CPPDEVTK_ASSERT(storageDeviceName.startsWith(_PATH_DEV));
	
	const QList<QDBusObjectPath> kFilesystemBlockDevicePaths = GetUDisks2FilesystemBlockDevicePaths();
	for (QList<QDBusObjectPath>::ConstIterator kIter = kFilesystemBlockDevicePaths.constBegin();
			kIter != kFilesystemBlockDevicePaths.constEnd(); ++kIter) {
		if (UDisks2FilesystemBlockDevice::GetDevice(*kIter) == storageDeviceName) {
			return *kIter;
		}
	}
	
	throw CPPDEVTK_RUNTIME_EXCEPTION(QString("no FilesystemBlockDevice for storageDeviceName: %1").arg(storageDeviceName));
}

void UDisks2DeviceNotifier::OnInterfacesAdded(const QDBusObjectPath& dbusObjectPath,
		const QVariantMap& addedInterfacesAndProperties) {
	if (!dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH)) {
		return;
	}
	
	for (QVariantMap::ConstIterator kIter = addedInterfacesAndProperties.constBegin();
			kIter != addedInterfacesAndProperties.constEnd(); ++kIter) {
		if (kIter.key() != CPPDEVTK_UDISKS2_FILESYSTEM_INTERFACE) {
			continue;
		}
		
		CPPDEVTK_LOG_DEBUG("udisks2 device added: " << dbusObjectPath.path());
		Q_EMIT StorageDeviceAdded(dbusObjectPath);
		
		try {
#			ifndef NDEBUG
			for (QList<UDisks2FilesystemBlockDevice*>::ConstIterator kIter = udisks2FilesystemBlockDevices_.constBegin();
					kIter != udisks2FilesystemBlockDevices_.constEnd(); ++kIter) {
				CPPDEVTK_ASSERT(dbusObjectPath != (*kIter)->GetDBusObjectPath());
			}
#			endif
			
			auto_ptr<UDisks2FilesystemBlockDevice> pUDisks2FilesystemBlockDevice(
					new UDisks2FilesystemBlockDevice(dbusObjectPath));
			
			CPPDEVTK_VERIFY(connect(pUDisks2FilesystemBlockDevice.get(),
					SIGNAL(Mounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
					SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
			CPPDEVTK_VERIFY(connect(pUDisks2FilesystemBlockDevice.get(),
					SIGNAL(Unmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
					SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
			
			udisks2FilesystemBlockDevices_.append(pUDisks2FilesystemBlockDevice.get());
			
			pUDisks2FilesystemBlockDevice.release();
		}
		catch (const ::std::runtime_error& exc) {
			CPPDEVTK_LOG_WARN("Failed to append UDisks2FilesystemBlockDevice: " << dbusObjectPath.path()
					<< "; exc: " << base::Exception::GetDetailedInfo(exc));
		}
		
		break;
	}
}

void UDisks2DeviceNotifier::OnInterfacesRemoved(const QDBusObjectPath& dbusObjectPath,
		const QStringList& removedInterfaces) {
	if (!dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH)) {
		return;
	}
	if (!removedInterfaces.contains(CPPDEVTK_UDISKS2_FILESYSTEM_INTERFACE)) {
		return;
	}
	
#	ifndef NDEBUG
	bool found = false;
#	endif
	for (QList<UDisks2FilesystemBlockDevice*>::Iterator iter = udisks2FilesystemBlockDevices_.begin();
			iter != udisks2FilesystemBlockDevices_.end(); ++iter) {
		if (dbusObjectPath != (*iter)->GetDBusObjectPath()) {
			continue;
		}
		
#		ifndef NDEBUG
		found = true;
#		endif
		
		delete *iter;
		udisks2FilesystemBlockDevices_.erase(iter);
		
		break;
	}
#	ifndef NDEBUG
	CPPDEVTK_ASSERT(found);
#	endif
	
	CPPDEVTK_LOG_DEBUG("udisks2 device removed: " << dbusObjectPath.path());
	Q_EMIT StorageDeviceRemoved(dbusObjectPath);
}

QList<QDBusObjectPath> UDisks2DeviceNotifier::GetUDisks2FilesystemBlockDevicePaths() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	
	QDBusInterface dbusUDisks2ObjManInterface(CPPDEVTK_UDISKS2_SERVICE_NAME, CPPDEVTK_UDISKS2_OBJ_PATH,
			CPPDEVTK_DBUS_OBJ_MAN_INTERFACE, systemBus);
	if (!dbusUDisks2ObjManInterface.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("dbusUDisks2ObjManInterface is not valid", dbusUDisks2ObjManInterface.lastError());
	}
	
	const QDBusMessage kReply = dbusUDisks2ObjManInterface.call("GetManagedObjects");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("dbusUDisks2ObjManInterface call to GetManagedObjects() failed",
				dbusUDisks2ObjManInterface.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "a{oa{sa{sv}}}");
	
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
	const QDBusArgument kDBusArgument = kReplyArg.value<QDBusArgument>();
	CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::MapType);
	
	QList<QDBusObjectPath> udisks2FilesystemBlockDevicePaths;
	
	kDBusArgument.beginMap();
	while (!kDBusArgument.atEnd()) {
		CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::MapEntryType);
		
		kDBusArgument.beginMapEntry();
		CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::BasicType);
		
		QDBusObjectPath objectPath;
		kDBusArgument >> objectPath;
		CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::MapType);
		
		kDBusArgument.beginMap();
		while (!kDBusArgument.atEnd()) {
			CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::MapEntryType);
			
			kDBusArgument.beginMapEntry();
			CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::BasicType);
			QString interface;
			kDBusArgument >> interface;
			if (interface == CPPDEVTK_UDISKS2_FILESYSTEM_INTERFACE) {
				CPPDEVTK_LOG_DEBUG("found udisks2FilesystemBlockDevicePath: " << objectPath.path());
				udisks2FilesystemBlockDevicePaths.append(objectPath);
				kDBusArgument.endMapEntry();
				break;
			}
			kDBusArgument.endMapEntry();
		}
		kDBusArgument.endMap();
		
		kDBusArgument.endMapEntry();
	}
	kDBusArgument.endMap();
	
	return udisks2FilesystemBlockDevicePaths;
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
