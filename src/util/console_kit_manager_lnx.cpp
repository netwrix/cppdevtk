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


#include <cppdevtk/util/console_kit_manager_lnx.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusReply>
#include <QtCore/QVariant>
#include <QtCore/QList>

#include <cstddef>
#include <new>


namespace cppdevtk {
namespace util {


bool ConsoleKitManager::Restart() {
	const QDBusMessage kReply = ckManagerInterface_.call("Restart");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = ckManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Manager::Restart() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool ConsoleKitManager::Stop() {
	const QDBusMessage kReply = ckManagerInterface_.call("Stop");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = ckManagerInterface_.lastError();
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Manager::Stop() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool ConsoleKitManager::CanRestart() const {
	const QDBusMessage kReply = ckManagerInterface_.call("CanRestart");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Manager::CanRestart() failed", ckManagerInterface_.lastError());
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

bool ConsoleKitManager::CanStop() const {
	const QDBusMessage kReply = ckManagerInterface_.call("CanStop");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Manager::CanStop() failed", ckManagerInterface_.lastError());
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

::std::auto_ptr<ConsoleKitSession> ConsoleKitManager::GetCurrentSession() const {
	const QDBusMessage kReply = ckManagerInterface_.call("GetCurrentSession");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to ConsoleKit.Manager::GetCurrentSession() failed", ckManagerInterface_.lastError());
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
	return ::std::auto_ptr<ConsoleKitSession>(new ConsoleKitSession(kDBusObjectPath));
}

::std::auto_ptr<ConsoleKitSession> ConsoleKitManager::GetSessionForUnixProcess(uint pid) const {
	const QDBusMessage kReply = ckManagerInterface_.call("GetSessionForUnixProcess", pid);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("DBus call to ConsoleKit.Manager::GetSessionForUnixProcess() failed for pid %1").arg(pid),
				ckManagerInterface_.lastError());
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
	return ::std::auto_ptr<ConsoleKitSession>(new ConsoleKitSession(kDBusObjectPath));
}

bool ConsoleKitManager::IsConsoleKitServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("systemBus is not connected", kSystemBus.lastError());
	}
	
	return IsDBusServiceRegistered("org.freedesktop.ConsoleKit", kSystemBus);
}

ConsoleKitManager::ConsoleKitManager(): ::cppdevtk::base::MeyersSingleton<ConsoleKitManager>(),
		ckManagerInterface_("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager",
		"org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus()) {
	if (!ckManagerInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("ConsoleKit.Manager DBus interface is not valid", ckManagerInterface_.lastError());
	}
}


}	// namespace util
}	// namespace cppdevtk
