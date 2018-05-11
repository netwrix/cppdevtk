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


#ifndef CPPDEVTK_UTIL_DBUS_UTILS_HPP_INCLUDED_
#define CPPDEVTK_UTIL_DBUS_UTILS_HPP_INCLUDED_


#include "config.hpp"
#ifdef QT_NO_DBUS
#error "This file require QtDBus"
#endif

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusObjectPath>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include <QtCore/QVariant>


namespace cppdevtk {
namespace util {


/// \pre !dbusServiceName.isEmpty()
/// \pre dbusConnection.isConnected()
/// \throw DBusException
CPPDEVTK_UTIL_API bool IsDBusServiceRegistered(const QString& dbusServiceName,
		const QDBusConnection& dbusConnection = QDBusConnection::sessionBus());


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DBus properties support
/// \sa <a href="https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-properties">org.freedesktop.DBus.Properties</a>
/// \throw DBusException

CPPDEVTK_UTIL_API QVariant GetDBusGenericProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API QDBusObjectPath GetDBusObjectPathProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API QString GetDBusStringProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API bool GetDBusBooleanProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API uchar GetDBusByteProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API short GetDBusInt16Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API ushort GetDBusUInt16Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API int GetDBusInt32Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API uint GetDBusUInt32Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API qlonglong GetDBusInt64Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API qulonglong GetDBusUInt64Property(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API double GetDBusDoubleProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API QByteArray GetDBusByteArrayProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);

CPPDEVTK_UTIL_API QStringList GetDBusStringArrayProperty(const QString& service, const QDBusObjectPath& path,
		const QString& interface, const QDBusConnection& connection, const QString& property);


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_DBUS_UTILS_HPP_INCLUDED_
