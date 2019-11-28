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


#include "console_kit_manager_lnx.hpp"
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusObjectPath>

#include <cstddef>
#include <new>


// https://www.freedesktop.org/software/ConsoleKit/doc/ConsoleKit.html#Manager
#define CPPDEVTK_CONSOLE_KIT_SERVICE_NAME "org.freedesktop.ConsoleKit"
#define CPPDEVTK_CONSOLE_KIT_MANAGER_OBJ_PATH "/org/freedesktop/ConsoleKit/Manager"
#define CPPDEVTK_CONSOLE_KIT_MANAGER_INTERFACE "org.freedesktop.ConsoleKit.Manager"


namespace cppdevtk {
namespace gui {
namespace detail {


bool ConsoleKitManager::IsConsoleKitServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("systemBus is not connected", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_CONSOLE_KIT_SERVICE_NAME, kSystemBus);
}

ConsoleKitManager::ConsoleKitManager(): SessionManager::Impl(CPPDEVTK_CONSOLE_KIT_SERVICE_NAME,
		CPPDEVTK_CONSOLE_KIT_MANAGER_OBJ_PATH, CPPDEVTK_CONSOLE_KIT_MANAGER_INTERFACE) {}

bool ConsoleKitManager::Stop() {
	const QDBusMessage kReply = DBusInterfaceRef().call("Stop");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Manager::Stop() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

::std::auto_ptr<Session> ConsoleKitManager::GetSessionForUnixProcess(uint pid) const {
	const QDBusMessage kReply = DBusInterfaceRef().call("GetSessionForUnixProcess", pid);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION(
				QString("DBus call to ConsoleKit.Manager::GetSessionForUnixProcess() failed for pid %1").arg(pid),
				DBusInterfaceRef().lastError());
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
	return ::std::auto_ptr<Session>(new Session(CPPDEVTK_CONSOLE_KIT_SERVICE_NAME, kDBusObjectPath.path()));
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
