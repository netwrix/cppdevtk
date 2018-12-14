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


#include "udisks2_filesystem_block_device_lnx.hpp"

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusError>


// http://storaged.org/doc/udisks2-api/latest/ref-dbus-overview.html#ref-dbus-well-known-name
#define CPPDEVTK_UDISKS2_SERVICE_NAME "org.freedesktop.UDisks2"

// http://storaged.org/doc/udisks2-api/latest/ref-dbus-block-devices.html
#define CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH "/org/freedesktop/UDisks2/block_devices/"

// http://storaged.org/doc/udisks2-api/latest/gdbus-org.freedesktop.UDisks2.Block.html
#define CPPDEVTK_UDISKS2_BLOCK_INTERFACE "org.freedesktop.UDisks2.Block"

// http://storaged.org/doc/udisks2-api/latest/gdbus-org.freedesktop.UDisks2.Filesystem.html
#define CPPDEVTK_UDISKS2_FILESYSTEM_INTERFACE "org.freedesktop.UDisks2.Filesystem"


// https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-properties
#define CPPDEVTK_DBUS_PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"


namespace cppdevtk {
namespace gui {
namespace detail {


UDisks2FilesystemBlockDevice::~UDisks2FilesystemBlockDevice() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (systemBus.isConnected()) {
		if (!systemBus.disconnect(CPPDEVTK_UDISKS2_SERVICE_NAME, kDBusObjectPath_.path(),
				CPPDEVTK_DBUS_PROPERTIES_INTERFACE, "PropertiesChanged",
				this, SLOT(OnPropertiesChanged(QString,QVariantMap,QStringList)))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UDisks2 Filesystem PropertiesChanged signal"
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

QString UDisks2FilesystemBlockDevice::GetDevice(const QDBusObjectPath& dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH));
	
	return QString::fromUtf8((const char *)util::GetDBusByteArrayProperty(CPPDEVTK_UDISKS2_SERVICE_NAME,
			dbusObjectPath, CPPDEVTK_UDISKS2_BLOCK_INTERFACE, QDBusConnection::systemBus(), "Device"));
}

void UDisks2FilesystemBlockDevice::OnPropertiesChanged(const QString& interfaceName, const QVariantMap& changedProperties,
		const QStringList& invalidatedProperties) {
	base::SuppressUnusedWarning(invalidatedProperties);
	
	if (!interfaceName.startsWith(CPPDEVTK_UDISKS2_FILESYSTEM_INTERFACE)) {
		return;
	}
	
	QVariantMap::ConstIterator kIter = changedProperties.constFind("MountPoints");
	if (kIter == changedProperties.constEnd()) {
		return;
	}
	
	const QVariant kValue = kIter.value();
	CPPDEVTK_ASSERT(!kValue.isNull());
	CPPDEVTK_ASSERT(kValue.isValid());
	
	CPPDEVTK_ASSERT(kValue.type() == QVariant::UserType);
#	ifndef NDEBUG
	qMetaTypeId<QDBusArgument>();	// compile time
#	endif
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("QDBusArgument") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("QDBusArgument") != 0);
#	endif
	CPPDEVTK_ASSERT(kValue.userType() == QMetaType::type("QDBusArgument"));
	const QDBusArgument kDBusArgument = kValue.value<QDBusArgument>();
	CPPDEVTK_ASSERT(kDBusArgument.currentType() == QDBusArgument::ArrayType);
	kDBusArgument.beginArray();
	if (kDBusArgument.atEnd()) {
		Q_EMIT Unmounted(kDBusObjectPath_);
	}
	else {
		Q_EMIT Mounted(kDBusObjectPath_);
	}
	kDBusArgument.endArray();
}

UDisks2FilesystemBlockDevice::UDisks2FilesystemBlockDevice(const QDBusObjectPath& dbusObjectPath): QObject(),
		kDBusObjectPath_(dbusObjectPath) {
	CPPDEVTK_ASSERT(dbusObjectPath.path().startsWith(CPPDEVTK_UDISKS2_BLOCK_DEVICES_PATH));
	
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	if (!systemBus.connect(CPPDEVTK_UDISKS2_SERVICE_NAME, kDBusObjectPath_.path(),
			CPPDEVTK_DBUS_PROPERTIES_INTERFACE, "PropertiesChanged",
			this, SLOT(OnPropertiesChanged(QString,QVariantMap,QStringList)))) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("failed to connect to UDisks2 Filesystem PropertiesChanged signal for dbusObjectPath %1").arg(
				kDBusObjectPath_.path()), systemBus.lastError());
	}
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
