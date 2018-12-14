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


#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMessage>
#include <QtCore/QMetaType>

#include <cstddef>


// https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-properties
#define CPPDEVTK_DBUS_PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"


namespace cppdevtk {
namespace util {


CPPDEVTK_UTIL_API bool IsDBusServiceRegistered(const QString& dbusServiceName, const QDBusConnection& dbusConnection) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(dbusServiceName.isEmpty(), "dbusServiceName");
	CPPDEVTK_DBC_CHECK_ARGUMENT(dbusConnection.isConnected(),
			QString("dbusConnection '%1' is not connected").arg(dbusConnection.name()));
	
	const QDBusConnectionInterface* pConnectionInterface = dbusConnection.interface();
	if (pConnectionInterface == NULL) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("failed to get connection interface for dbusConnection '%1'").arg(dbusConnection.name()),
				dbusConnection.lastError());
	}
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	const
#	endif
	QDBusReply<bool> reply = pConnectionInterface->isServiceRegistered(dbusServiceName);
	if (!reply.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("failed to detect if dbusServiceName '%1' is registered on dbusConnection '2'").arg(
				dbusServiceName, dbusConnection.name()), reply.error());
	}
	
	return reply.value();
}

CPPDEVTK_UTIL_API QVariant GetDBusGenericProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(service.isEmpty(), "service");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(path.path().isEmpty(), "path");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(interface.isEmpty(), "interface");
	CPPDEVTK_DBC_CHECK_ARGUMENT(connection.isConnected(), "connection is not connected");
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(property.isEmpty(), "property");
	
	QDBusInterface propertiesInterface(service, path.path(), CPPDEVTK_DBUS_PROPERTIES_INTERFACE, connection);
	if (!propertiesInterface.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("propertiesInterface is not valid for service '%1', path '%2', connection '%3'").arg(
				service, path.path(), connection.name()), propertiesInterface.lastError());
	}
	
	const QDBusMessage kReply = propertiesInterface.call("Get", interface, property);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("failed to Get DBus property '%1'").arg(property),
				propertiesInterface.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "v");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UserType);
#	ifndef NDEBUG
	qMetaTypeId<QDBusVariant>();	// compile time
#	endif
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("QDBusVariant") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("QDBusVariant") != 0);
#	endif
	CPPDEVTK_ASSERT(kReplyArg.userType() == QMetaType::type("QDBusVariant"));
	const QVariant kVariantReplyArg = kReplyArg.value<QDBusVariant>().variant();
	CPPDEVTK_ASSERT(!kVariantReplyArg.isNull());
	CPPDEVTK_ASSERT(kVariantReplyArg.isValid());
	return kVariantReplyArg;
}

CPPDEVTK_UTIL_API QDBusObjectPath GetDBusObjectPathProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
#	ifndef NDEBUG
	qMetaTypeId<QDBusObjectPath>();	// compile time
#	endif
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("QDBusObjectPath") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("QDBusObjectPath") != 0);
#	endif
	
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (!((kGenericProperty.type() == QVariant::UserType)
			&& (kGenericProperty.userType() == QMetaType::type("QDBusObjectPath")))) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, QString::number(kGenericProperty.userType()),
				QString::number(QMetaType::type("QDBusObjectPath"))),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<QDBusObjectPath>();
}

CPPDEVTK_UTIL_API QString GetDBusStringProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::String) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::String)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<QString>();
}

CPPDEVTK_UTIL_API bool GetDBusBooleanProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::Bool) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::Bool)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<bool>();
}

CPPDEVTK_UTIL_API uchar GetDBusByteProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (!((kGenericProperty.type() == QVariant::UserType)
			&& (kGenericProperty.userType() == QMetaType::UChar))) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, QString::number(kGenericProperty.userType()),
				QString::number(QMetaType::UChar)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<uchar>();
}

CPPDEVTK_UTIL_API short GetDBusInt16Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (!((kGenericProperty.type() == QVariant::UserType)
			&& (kGenericProperty.userType() == QMetaType::Short))) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, QString::number(kGenericProperty.userType()),
				QString::number(QMetaType::Short)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<short>();
}

CPPDEVTK_UTIL_API ushort GetDBusUInt16Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (!((kGenericProperty.type() == QVariant::UserType)
			&& (kGenericProperty.userType() == QMetaType::UShort))) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, QString::number(kGenericProperty.userType()),
				QString::number(QMetaType::UShort)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<ushort>();
}

CPPDEVTK_UTIL_API int GetDBusInt32Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::Int) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::Int)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<int>();
}

CPPDEVTK_UTIL_API uint GetDBusUInt32Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::UInt) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::UInt)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<uint>();
}

CPPDEVTK_UTIL_API qlonglong GetDBusInt64Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::LongLong) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::LongLong)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<qlonglong>();
}

CPPDEVTK_UTIL_API qulonglong GetDBusUInt64Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::ULongLong) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::ULongLong)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<qulonglong>();
}

CPPDEVTK_UTIL_API double GetDBusDoubleProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::Double) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::Double)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<double>();
}

CPPDEVTK_UTIL_API QByteArray GetDBusByteArrayProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::ByteArray) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::ByteArray)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<QByteArray>();
}

CPPDEVTK_UTIL_API QStringList GetDBusStringArrayProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property) {
	const QVariant kGenericProperty = GetDBusGenericProperty(service, path, interface, connection, property);
	if (kGenericProperty.type() != QVariant::StringList) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("wrong property '%1' type (interface '%2'): got '%3'; expected '%4'").arg(
				property, interface, kGenericProperty.typeName(), QVariant::typeToName(QVariant::StringList)),
				QDBusError(QDBusError::InvalidSignature, "InvalidSignature"));
	}
	return kGenericProperty.value<QStringList>();
}


}	// namespace util
}	// namespace cppdevtk
