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


#include <cppdevtk/gui/computer_manager.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>

#if (!CPPDEVTK_PLATFORM_ANDROID)
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#endif


namespace cppdevtk {
namespace gui {
namespace detail {

#if (!CPPDEVTK_PLATFORM_ANDROID)
static bool KdeLogoutUser();
static bool GnomeLogoutUser();
#endif

}	// namespace detail


bool ComputerManager::ShutdownComputer() {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	const QDBusMessage kMsg = QDBusMessage::createMethodCall("org.freedesktop.ConsoleKit",
			"/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", "Stop");
	const QDBusMessage kReply = QDBusConnection::systemBus().call(kMsg, QDBus::Block);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ConsoleKit.Manager, Stop() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature().isEmpty());
	
	return true;
	
#	else
	// TODO: Android port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::ShutdownComputer(): Not ported on Android!");
	return false;
#	endif
}

bool ComputerManager::LockComputer() {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	const QDBusMessage kMsg = QDBusMessage::createMethodCall("org.freedesktop.ScreenSaver",
			"/ScreenSaver", "org.freedesktop.ScreenSaver", "Lock");
	const QDBusMessage kReply = QDBusConnection::sessionBus().call(kMsg, QDBus::Block);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ScreenSaver, Lock() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature().isEmpty());
	
	return true;
	
#	else
	// TODO: Android port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::LockComputer(): Not ported on Android!");
	return false;
#	endif
}

bool ComputerManager::LogoutUser() {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	if (!detail::KdeLogoutUser()) {
		if (!detail::GnomeLogoutUser()) {
			CPPDEVTK_LOG_ERROR("failed to LogoutUser");
			return false;
		}
	}
	
	return true;
	
#	else
	// TODO: Android port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::LockComputer(): Not ported on Android!");
	return false;
#	endif
}


namespace detail {


#if (!CPPDEVTK_PLATFORM_ANDROID)


bool KdeLogoutUser() {
	QDBusInterface ksmServerInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface");
	const QDBusMessage kReply = ksmServerInterface.call(QDBus::Block, "logout", 0, 0, 0);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to KSMServerInterface, logout() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature().isEmpty());
	
	return true;
}

bool GnomeLogoutUser() {
	QDBusInterface gnomeSessionManagerInterface("org.gnome.SessionManager", "/org/gnome/SessionManager",
			"org.gnome.SessionManager");
	const QDBusMessage kReply = gnomeSessionManagerInterface.call(QDBus::Block, "Logout", 1);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to SessionManager, Logout() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature().isEmpty());
	
	return true;
}


#endif	// !ANDROID


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
