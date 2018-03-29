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


#include <cppdevtk/util/logind_session_lnx.hpp>


#if (CPPDEVTK_HAVE_LOGIND)


#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>

#include <cstddef>


namespace cppdevtk {
namespace util {


bool LogindSession::Terminate() {
	const QDBusMessage kReply = logindSessionInterface_.call("Terminate");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Terminate() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindSession::Activate() {
	const QDBusMessage kReply = logindSessionInterface_.call("Activate");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Activate() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindSession::Lock() {
	const QDBusMessage kReply = logindSessionInterface_.call("Lock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Lock() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindSession::Unlock() {
	const QDBusMessage kReply = logindSessionInterface_.call("Unlock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Session::Unlock() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

LogindSession::LogindSession(const QDBusObjectPath& logindSessionPath): QObject(),
		logindSessionInterface_("org.freedesktop.login1", logindSessionPath.path(), "org.freedesktop.login1.Session") {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(logindSessionPath.path().isEmpty(), "logindSessionPath");
	
	if (!logindSessionInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("Logind.Session DBus interface is not valid", logindSessionInterface_.lastError());
	}
	
	QDBusConnection connection = logindSessionInterface_.connection();
	if (!connection.connect(logindSessionInterface_.service(), logindSessionInterface_.path(),
			logindSessionInterface_.interface(), "Lock", this, SIGNAL(Locked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to Logind.Session::Lock()", connection.lastError());
	}
	if (!connection.connect(logindSessionInterface_.service(), logindSessionInterface_.path(),
			logindSessionInterface_.interface(), "Unlock", this, SIGNAL(Unlocked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to Logind.Session::Unlock()", connection.lastError());
	}
}

QVariant LogindSession::GetProperty(const QString& propertyName) const {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(propertyName.isEmpty(), "propertyName");
	
	const QDBusMessage kReply = logindSessionInterface_.call("Get", propertyName);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION(QString("DBus call to Logind.Session::Get() failed for property '%1'").arg(propertyName),
				logindSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "v");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
		
	const QVariant kReplyArg = qvariant_cast<QDBusVariant>(kReplyArgs[0]).variant();
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	return kReplyArg;
}


}	// namespace util
}	// namespace cppdevtk


#endif	// (CPPDEVTK_HAVE_LOGIND)
