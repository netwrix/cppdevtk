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


#include <cppdevtk/util/logind_manager_lnx.hpp>

#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/get_current_process_id.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/get_current_process_id.hpp>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusReply>
#include <QtCore/QList>
#include <QtCore/QVariant>

#include <cstddef>
#include <cstdlib>
#include <new>


namespace cppdevtk {
namespace util {


bool LogindManager::LockSession(const QString& sessionId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(sessionId.isEmpty(), "sessionId");
	
	const QDBusMessage kReply = logindManagerInterface_.call("LockSession", sessionId);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::LockSession() failed for sessionId: " << sessionId
				<< "; dbusErrorName: " << kLastError.name() << "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::UnlockSession(const QString& sessionId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(sessionId.isEmpty(), "sessionId");
	
	const QDBusMessage kReply = logindManagerInterface_.call("UnlockSession", sessionId);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::UnlockSession() failed for sessionId: " << sessionId
				<< "; dbusErrorName: " << kLastError.name() << "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::TerminateSession(const QString& sessionId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(sessionId.isEmpty(), "sessionId");
	
	const QDBusMessage kReply = logindManagerInterface_.call("TerminateSession", sessionId);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::TerminateSession() failed for sessionId: " << sessionId
				<< "; dbusErrorName: " << kLastError.name() << "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::ActivateSession(const QString& sessionId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(sessionId.isEmpty(), "sessionId");
	
	const QDBusMessage kReply = logindManagerInterface_.call("ActivateSession", sessionId);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::ActivateSession() failed for sessionId: " << sessionId
				<< "; dbusErrorName: " << kLastError.name() << "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::PowerOff(bool interactive) {
	const QDBusMessage kReply = logindManagerInterface_.call("PowerOff", interactive);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::PowerOff() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::Reboot(bool interactive) {
	const QDBusMessage kReply = logindManagerInterface_.call("Reboot", interactive);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::Reboot() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::Suspend(bool interactive) {
	const QDBusMessage kReply = logindManagerInterface_.call("Suspend", interactive);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::Suspend() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::Hibernate(bool interactive) {
	const QDBusMessage kReply = logindManagerInterface_.call("Hibernate", interactive);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::Hibernate() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool LogindManager::HybridSleep(bool interactive) {
	const QDBusMessage kReply = logindManagerInterface_.call("HybridSleep", interactive);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = logindManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to Logind.Manager::HybridSleep() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

::std::auto_ptr<LogindSession> LogindManager::GetCurrentSession() const {
	return GetSessionByPID(base::GetCurrentProcessId());
}

::std::auto_ptr<LogindSession> LogindManager::GetSession(const QString& sessionId) const {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(sessionId.isEmpty(), "sessionId");
	
	const QDBusMessage kReply = logindManagerInterface_.call("GetSession", sessionId);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("DBus call to Logind.Manager::GetSession() failed for sessionId '%1'").arg(sessionId),
				logindManagerInterface_.lastError());
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
	return ::std::auto_ptr<LogindSession>(new LogindSession(kDBusObjectPath));
}

::std::auto_ptr<LogindSession> LogindManager::GetSessionByPID(uint pid) const {
	const QDBusMessage kReply = logindManagerInterface_.call("GetSessionByPID", pid);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("DBus call to Logind.Manager::GetSessionByPID() failed for pid %1").arg(pid),
				logindManagerInterface_.lastError());
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
	return ::std::auto_ptr<LogindSession>(new LogindSession(kDBusObjectPath));
}

QString LogindManager::CanPowerOff() const {
	const QDBusMessage kReply = logindManagerInterface_.call("CanPowerOff");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to Logind.Manager::CanPowerOff() failed", logindManagerInterface_.lastError());
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

QString LogindManager::CanReboot() const {
	const QDBusMessage kReply = logindManagerInterface_.call("CanReboot");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to Logind.Manager::CanReboot() failed", logindManagerInterface_.lastError());
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

QString LogindManager::CanSuspend() const {
	const QDBusMessage kReply = logindManagerInterface_.call("CanSuspend");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to Logind.Manager::CanSuspend() failed", logindManagerInterface_.lastError());
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

QString LogindManager::CanHibernate() const {
	const QDBusMessage kReply = logindManagerInterface_.call("CanHibernate");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to Logind.Manager::CanHibernate() failed", logindManagerInterface_.lastError());
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

QString LogindManager::CanHybridSleep() const {
	const QDBusMessage kReply = logindManagerInterface_.call("CanHybridSleep");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to Logind.Manager::CanHybridSleep() failed", logindManagerInterface_.lastError());
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

bool LogindManager::IsLogindServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("systemBus is not connected", kSystemBus.lastError());
	}
	
	return IsDBusServiceRegistered("org.freedesktop.login1", kSystemBus);
}

LogindManager::LogindManager(): ::cppdevtk::base::MeyersSingleton<LogindManager>(),
		logindManagerInterface_("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager",
		QDBusConnection::systemBus()) {
	if (!logindManagerInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("Logind.Manager DBus interface is not valid", logindManagerInterface_.lastError());
	}
}


}	// namespace util
}	// namespace cppdevtk
