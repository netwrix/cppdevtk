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


#include "console_kit_session_lnx.hpp"
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/on_block_exit.hpp>

#include <QtCore/QDateTime>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>

#include <cstddef>


// https://www.freedesktop.org/software/ConsoleKit/doc/ConsoleKit.html#Session
#define CPPDEVTK_CONSOLE_KIT_SERVICE_NAME "org.freedesktop.ConsoleKit"
#define CPPDEVTK_CONSOLE_KIT_SESSION_INTERFACE "org.freedesktop.ConsoleKit.Session"


namespace cppdevtk {
namespace gui {
namespace detail {


ConsoleKitSession::~ConsoleKitSession() {
	QDBusInterface& dbusInterface = DBusInterfaceRef();
	QDBusConnection connection = dbusInterface.connection();
	if (!connection.isConnected()) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("ConsoleKit.Session DBus interface is not connected"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return;
	}
	
	if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Unlock", this, SIGNAL(Unlocked()))) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("failed to disconnect from ConsoleKit.Session::Unlock signal"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
	}
	if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Lock", this, SIGNAL(Locked()))) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("failed to disconnect from ConsoleKit.Session::Lock signal"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
	}
	if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "ActiveChanged", this, SIGNAL(ActiveChanged(bool)))) {
		const QDBusError kLastError = connection.lastError();
		CPPDEVTK_LOG_WARN("failed to disconnect from ConsoleKit.Session::ActiveChanged signal"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
	}
}

bool ConsoleKitSession::Activate() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Activate");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Session::Activate() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

bool ConsoleKitSession::Lock() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Lock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Session::Lock() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

bool ConsoleKitSession::Unlock() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Unlock");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Session::Unlock() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

QString ConsoleKitSession::GetId() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetId");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetId() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "o");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
#	ifndef NDEBUG
	qMetaTypeId<QDBusObjectPath>();	// compile time
#	endif
	// runtime; do not global qualify because moc will generate bad code
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	CPPDEVTK_ASSERT(QMetaType::type("QDBusObjectPath") != QMetaType::UnknownType);
#	else
	CPPDEVTK_ASSERT(QMetaType::type("QDBusObjectPath") != 0);
#	endif
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UserType);
	CPPDEVTK_ASSERT(kReplyArg.userType() == QMetaType::type("QDBusObjectPath"));
	
	const QDBusObjectPath kDBusObjectPath = kReplyArg.value<QDBusObjectPath>();
	CPPDEVTK_ASSERT(!kDBusObjectPath.path().isEmpty());
	return kDBusObjectPath.path();
}

bool ConsoleKitSession::GetIdleHint() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetSystemIdleHint");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetSystemIdleHint() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "b");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::Bool);
	return kReplyArg.value<bool>();
}

Session::IdleTime ConsoleKitSession::GetIdleSinceHint() const {
	const QString kStrSystemIdleSinceHint = DoGetSystemIdleSinceHint();
	CPPDEVTK_ASSERT(!kStrSystemIdleSinceHint.isEmpty());
	
	const QDateTime kSystemIdleSinceHint = QDateTime::fromString(kStrSystemIdleSinceHint,
#			if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
			Qt::ISODateWithMs
#			else
			Qt::ISODate
#			endif
			);
	CPPDEVTK_ASSERT(kSystemIdleSinceHint.isValid());
	
	const QDateTime kcurrentDateTime = QDateTime::currentDateTime();
	CPPDEVTK_ASSERT(kcurrentDateTime.isValid());
	
#	if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
	const qint64 kIdleTime = kSystemIdleSinceHint.msecsTo(kcurrentDateTime);
#	else
	const qint64 kIdleTime = kSystemIdleSinceHint.secsTo(kcurrentDateTime) * 1000;
#	endif
	return (kIdleTime >= 0) ? kIdleTime : 0;
}

QString ConsoleKitSession::DoGetSystemIdleSinceHint() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetSystemIdleSinceHint");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetSystemIdleSinceHint() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "s");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::String);
	return kReplyArg.value<QString>();
}

QString ConsoleKitSession::GetSessionType() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetSessionType");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetSessionType() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "s");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::String);
	return kReplyArg.value<QString>();
}

uint ConsoleKitSession::GetUnixUser() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetUnixUser");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetUnixUser() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "u");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UInt);
	return kReplyArg.value<uint>();
}

QString ConsoleKitSession::GetRemoteHostName() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetRemoteHostName");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::GetRemoteHostName() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "s");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::String);
	return kReplyArg.value<QString>();
}

bool ConsoleKitSession::IsActive() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("IsActive");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::IsActive() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "b");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::Bool);
	return kReplyArg.value<bool>();
}

bool ConsoleKitSession::IsLocal() const {
	const QDBusMessage kReply = DBusInterfaceRef().call("IsLocal");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Session::IsLocal() failed", GetLastError());
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "b");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::Bool);
	return kReplyArg.value<bool>();
}

bool ConsoleKitSession::IsConsoleKitServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("systemBus is not connected", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_CONSOLE_KIT_SERVICE_NAME, kSystemBus);
}

ConsoleKitSession::ConsoleKitSession(const QDBusObjectPath& ckSessionPath):
		Session::Impl(CPPDEVTK_CONSOLE_KIT_SERVICE_NAME, ckSessionPath.path(), CPPDEVTK_CONSOLE_KIT_SESSION_INTERFACE) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(ckSessionPath.path().isEmpty(), "ckSessionPath");
	//CPPDEVTK_LOG_DEBUG("ckSessionPath: " << ckSessionPath.path());
	
	QDBusInterface& dbusInterface = DBusInterfaceRef();
	QDBusConnection connection = dbusInterface.connection();
	if (!connection.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("ConsoleKit.Session DBus interface is not connected", connection.lastError());
	}
	
	if (!connection.connect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "ActiveChanged", this, SIGNAL(ActiveChanged(bool)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to ConsoleKit.Session::ActiveChanged signal", connection.lastError());
	}
	bool commit = false;
	ConsoleKitSession* pThis = this;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&connection)(&dbusInterface)(&pThis)) {
		if (!commit) {
			if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
					dbusInterface.interface(), "ActiveChanged", pThis, SIGNAL(ActiveChanged(bool)))) {
				const QDBusError kLastError = connection.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from ConsoleKit.Session::ActiveChanged signal"
						<< "; errorType: " << QDBusError::errorString(kLastError.type())
						<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!connection.connect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Lock", this, SIGNAL(Locked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to ConsoleKit.Session::Lock signal", connection.lastError());
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&connection)(&dbusInterface)(&pThis)) {
		if (!commit) {
			if (!connection.disconnect(dbusInterface.service(), dbusInterface.path(),
					dbusInterface.interface(), "Lock", pThis, SIGNAL(Locked()))) {
				const QDBusError kLastError = connection.lastError();
				CPPDEVTK_LOG_WARN("failed to disconnect from ConsoleKit.Session::Lock() signal"
						<< "; errorType: " << QDBusError::errorString(kLastError.type())
						<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
			}
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!connection.connect(dbusInterface.service(), dbusInterface.path(),
			dbusInterface.interface(), "Unlock", this, SIGNAL(Unlocked()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to ConsoleKit.Session::Unlock signal", connection.lastError());
	}
	
	commit = true;
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
