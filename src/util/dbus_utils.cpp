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


#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusReply>

#include <cstddef>


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
	
	const QDBusReply<bool> kReply = pConnectionInterface->isServiceRegistered(dbusServiceName);
	if (!kReply.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("failed to detect if dbusServiceName '%1' is registered on dbusConnection '2'").arg(
				dbusServiceName, dbusConnection.name()), kReply.error());
	}
	
	return kReply.value();
}


}	// namespace util
}	// namespace cppdevtk
