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


#include <cppdevtk/util/console_kit_session_lnx.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>

#include <cstddef>


namespace cppdevtk {
namespace util {


bool ConsoleKitSession::Activate() {
	const QDBusMessage kReply = ckSessionInterface_.call("Activate");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = ckSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Session::Activate() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool ConsoleKitSession::Lock() {
	const QDBusMessage kReply = ckSessionInterface_.call("Lock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = ckSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Session::Lock() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool ConsoleKitSession::Unlock() {
	const QDBusMessage kReply = ckSessionInterface_.call("Unlock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = ckSessionInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Session::Unlock() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

QDBusObjectPath ConsoleKitSession::GetId() const {
	const QDBusMessage kReply = ckSessionInterface_.call("GetId");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetId() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "o");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.userType() == qMetaTypeId<QDBusObjectPath>());
	CPPDEVTK_ASSERT(qVariantCanConvert<QDBusObjectPath>(kReplyArg));
	
	const QDBusObjectPath kDBusObjectPath = qvariant_cast<QDBusObjectPath>(kReplyArg);
	CPPDEVTK_ASSERT(!kDBusObjectPath.path().isEmpty());
	return kDBusObjectPath;
}

QString ConsoleKitSession::GetSessionType() const {
	const QDBusMessage kReply = ckSessionInterface_.call("GetSessionType");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetSessionType() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "s");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::String);
	return kReplyArg.toString();
}

uint ConsoleKitSession::GetUnixUser() const {
	const QDBusMessage kReply = ckSessionInterface_.call("GetUnixUser");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetUnixUser() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "u");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UInt);
	return kReplyArg.toUInt();
}

QString ConsoleKitSession::GetRemoteHostName() const {
	const QDBusMessage kReply = ckSessionInterface_.call("GetRemoteHostName");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetRemoteHostName() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "s");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::String);
	return kReplyArg.toString();
}

QString ConsoleKitSession::GetLoginSessionId() const {
	const QDBusMessage kReply = ckSessionInterface_.call("GetLoginSessionId");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetLoginSessionId() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "s");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::String);
	return kReplyArg.toString();
}

bool ConsoleKitSession::IsActive() const {
	const QDBusMessage kReply = ckSessionInterface_.call("IsActive");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::IsActive() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "b");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::Bool);
	return kReplyArg.toBool();
}

bool ConsoleKitSession::IsLocal() const {
	const QDBusMessage kReply = ckSessionInterface_.call("IsLocal");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::IsLocal() failed", ckSessionInterface_.lastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "b");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::Bool);
	return kReplyArg.toBool();
}

ConsoleKitSession::ConsoleKitSession(const QDBusObjectPath& ckSessionPath): QObject(),
		ckSessionInterface_("org.freedesktop.ConsoleKit", ckSessionPath.path(), "org.freedesktop.ConsoleKit.Session",
		QDBusConnection::systemBus()) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(ckSessionPath.path().isEmpty(), "ckSessionPath");
	//CPPDEVTK_LOG_DEBUG("ckSessionPath: " << ckSessionPath.path());
	
	if (!ckSessionInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("ConsoleKit.Session DBus interface is not valid", ckSessionInterface_.lastError());
	}
	
	QDBusConnection connection = ckSessionInterface_.connection();
	if (!connection.connect(ckSessionInterface_.service(), ckSessionInterface_.path(),
			ckSessionInterface_.interface(), "ActiveChanged", this, SIGNAL(ActiveChanged(bool)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to ConsoleKit.Session::ActiveChanged()", connection.lastError());
	}
	if (!connection.connect(ckSessionInterface_.service(), ckSessionInterface_.path(),
			ckSessionInterface_.interface(), "Lock", this, SIGNAL(Locked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to ConsoleKit.Session::Lock()", connection.lastError());
	}
	if (!connection.connect(ckSessionInterface_.service(), ckSessionInterface_.path(),
			ckSessionInterface_.interface(), "Unlock", this, SIGNAL(Unlocked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to ConsoleKit.Session::Unlock()", connection.lastError());
	}
}


}	// namespace util
}	// namespace cppdevtk
