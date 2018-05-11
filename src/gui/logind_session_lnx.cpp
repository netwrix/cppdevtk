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


#include "logind_session_lnx.hpp"
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/on_block_exit.hpp>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>

#include <cstddef>


// https://www.freedesktop.org/wiki/Software/systemd/logind
#define CPPDEVTK_LOGIND_SERVICE_NAME "org.freedesktop.login1"
#define CPPDEVTK_LOGIND_SESSION_INTERFACE "org.freedesktop.login1.Session"

// https://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-properties
#define CPPDEVTK_DBUS_PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"


namespace cppdevtk {
namespace gui {
namespace detail {


LogindSession::~LogindSession() {
	QDBusInterface& dbusInterface = DBusInterfaceRef();
	QDBusConnection connection = dbusInterface.connection();
	
	if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Lock", this, SIGNAL(Locked()))) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("failed to disconnect from Logind.Session::Lock signal"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
	}
	
	if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Unlock", this, SIGNAL(Unlocked()))) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("failed to disconnect from Logind.Session::Unlock signal"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
	}
	
	connection = logindSessionPropertiesInterface_.connection();
	if (!connection.disconnect(logindSessionPropertiesInterface_.service(), logindSessionPropertiesInterface_.path(),
			logindSessionPropertiesInterface_.interface(), "PropertiesChanged",
			this, SLOT(OnDBusPropertiesChanged(QString,QVariantMap,QStringList)))) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("failed to disconnect from Logind.Session.Properties::PropertiesChanged signal"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
	}
}

bool LogindSession::Activate() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Activate");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Activate() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

bool LogindSession::Lock() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Lock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Lock() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

bool LogindSession::Unlock() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Unlock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Unlock() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

bool LogindSession::IsLogindServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("systemBus is not connected", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_LOGIND_SERVICE_NAME, kSystemBus);
}

void LogindSession::OnDBusPropertiesChanged(const QString& interfaceName, const QVariantMap& changedProperties,
		const QStringList& invalidatedProperties) {
	base::SuppressUnusedWarning(interfaceName);
	base::SuppressUnusedWarning(invalidatedProperties);
	CPPDEVTK_ASSERT(interfaceName == DBusInterfaceRef().interface());
	
	QVariantMap::ConstIterator kIter = changedProperties.constFind("Active");
	if (kIter != changedProperties.constEnd()) {
		const QVariant kValue = kIter.value();
		CPPDEVTK_ASSERT(!kValue.isNull());
		CPPDEVTK_ASSERT(kValue.isValid());
		CPPDEVTK_ASSERT(kValue.type() == QVariant::Bool);
		Q_EMIT ActiveChanged(kValue.value<bool>());
	}
}

LogindSession::LogindSession(const QDBusObjectPath& logindSessionPath):
		Session::Impl(CPPDEVTK_LOGIND_SERVICE_NAME, logindSessionPath.path(), CPPDEVTK_LOGIND_SESSION_INTERFACE),
		logindSessionPropertiesInterface_(CPPDEVTK_LOGIND_SERVICE_NAME, logindSessionPath.path(),
		CPPDEVTK_DBUS_PROPERTIES_INTERFACE, QDBusConnection::systemBus()) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(logindSessionPath.path().isEmpty(), "logindSessionPath");
	//CPPDEVTK_LOG_DEBUG("logindSessionPath: " << logindSessionPath.path());
	
	QDBusInterface& dbusInterface = DBusInterfaceRef();
	QDBusConnection connection = dbusInterface.connection();
	if (!connection.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("Logind.Session DBus interface is not connected", connection.lastError());
	}
	
	if (!connection.connect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Lock", this, SIGNAL(Locked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to Logind.Session::Lock signal", connection.lastError());
	}
	bool commit = false;
	LogindSession* pThis = this;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&connection)(&dbusInterface)(&pThis)) {
		if (!commit) {
			if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
					dbusInterface.interface(), "Lock", pThis, SIGNAL(Locked()))) {
				const QDBusError kLastError = connection.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from Logind.Session::Lock signal"
						<< "; errorType: " << QDBusError::errorString(kLastError.type())
						<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!connection.connect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Unlock", this, SIGNAL(Unlocked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to Logind.Session::Unlock()", connection.lastError());
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&connection)(&dbusInterface)(&pThis)) {
		if (!commit) {
			if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
					dbusInterface.interface(), "Unlock", pThis, SIGNAL(Unlocked()))) {
				const QDBusError kLastError = connection.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from Logind.Session::Unlock signal"
						<< "; errorType: " << QDBusError::errorString(kLastError.type())
						<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!logindSessionPropertiesInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("Logind.Session.Properties DBus interface is not valid",
				logindSessionPropertiesInterface_.lastError());
	}
	
	connection = logindSessionPropertiesInterface_.connection();
	if (!connection.connect(logindSessionPropertiesInterface_.service(), logindSessionPropertiesInterface_.path(),
			logindSessionPropertiesInterface_.interface(), "PropertiesChanged",
			this, SLOT(OnDBusPropertiesChanged(QString,QVariantMap,QStringList)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to Logind.Session.Properties::PropertiesChanged signal",
				connection.lastError());
	}
	
	commit = true;
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
