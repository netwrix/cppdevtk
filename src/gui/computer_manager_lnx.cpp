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
#include <cppdevtk/base/exception.hpp>

#if (!CPPDEVTK_PLATFORM_ANDROID)
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>
#endif

#if (CPPDEVTK_HAVE_LOGIND)
#include <cppdevtk/util/logind_manager_lnx.hpp>
#include <cppdevtk/util/logind_session_lnx.hpp>
#else
#include <cppdevtk/util/console_kit_manager_lnx.hpp>
#include <cppdevtk/util/console_kit_session_lnx.hpp>
#endif


namespace cppdevtk {
namespace gui {
namespace detail {

#if (!CPPDEVTK_PLATFORM_ANDROID)
static bool KdeLogoutUser();
static bool GnomeLogoutUser();
#endif

}	// namespace detail


bool ComputerManager::ShutdownComputer() try {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
#	if (CPPDEVTK_HAVE_LOGIND)
	util::LogindManager& theLogindManager = util::LogindManager::GetInstance();
	if (theLogindManager.CanPowerOff() != "yes") {
		CPPDEVTK_LOG_ERROR("theLogindManager can not power off");
		return false;
	}
	return theLogindManager.PowerOff(false);
#	else
	util::ConsoleKitManager& theConsoleKitManager = util::ConsoleKitManager::GetInstance();
	if (!theConsoleKitManager.CanStop()) {
		CPPDEVTK_LOG_ERROR("theConsoleKitManager can not stop");
		return false;
	}
	return theConsoleKitManager.Stop();
#	endif
		
#	else
	// TODO: Android port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::ShutdownComputer(): Not ported on Android!");
	return false;
#	endif
}
catch (const ::std::exception& exc) {
	CPPDEVTK_LOG_ERROR("failed to ShutdownComputer; exc: " << base::Exception::GetDetailedInfo(exc));
	return false;
}

bool ComputerManager::LockComputer() try {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	try {
#		if (CPPDEVTK_HAVE_LOGIND)
		if (util::LogindManager::GetInstance().GetCurrentSession()->Lock()) {
#		else
		if (util::ConsoleKitManager::GetInstance().GetCurrentSession()->Lock()) {
#		endif
			return true;
		}
		CPPDEVTK_LOG_WARN("failed to LockComputer using ConsoleKit/logind");
	}
	catch (const ::std::exception& exc) {
		CPPDEVTK_LOG_WARN("failed to LockComputer using ConsoleKit/logind; exc: " << base::Exception::GetDetailedInfo(exc));
	}
	
	QDBusInterface screenSaverInterface("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver");
	if (!screenSaverInterface.isValid()) {
		const QDBusError kLastError = screenSaverInterface.lastError();
		CPPDEVTK_LOG_ERROR("ScreenSaver DBus interface is not valid; errorName: " << kLastError.name()
				<< "; errorMessage: " << kLastError.message());
		return false;
	}
	const QDBusMessage kReply = screenSaverInterface.call("logout", 0, 0, 0);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to ScreenSaver::logout() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
	
#	else
	// TODO: Android port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::LockComputer(): Not ported on Android!");
	return false;
#	endif
}
catch (const ::std::exception& exc) {
	CPPDEVTK_LOG_ERROR("failed to LockComputer; exc: " << base::Exception::GetDetailedInfo(exc));
	return false;
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
	if (!ksmServerInterface.isValid()) {
		const QDBusError kLastError = ksmServerInterface.lastError();
		CPPDEVTK_LOG_ERROR("kde.KSMServerInterface DBus interface is not valid; errorName: " << kLastError.name()
				<< "; errorMessage: " << kLastError.message());
		return false;
	}
	const QDBusMessage kReply = ksmServerInterface.call("logout", 0, 0, 0);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to kde.KSMServerInterface::logout() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}

bool GnomeLogoutUser() {
	QDBusInterface gnomeSessionManagerInterface("org.gnome.SessionManager", "/org/gnome/SessionManager",
			"org.gnome.SessionManager");
	if (!gnomeSessionManagerInterface.isValid()) {
		const QDBusError kLastError = gnomeSessionManagerInterface.lastError();
		CPPDEVTK_LOG_ERROR("gnome.SessionManager DBus interface is not valid; errorName: " << kLastError.name()
				<< "; errorMessage: " << kLastError.message());
		return false;
	}
	const QDBusMessage kReply = gnomeSessionManagerInterface.call("Logout", 1);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("DBus call to gnome.SessionManager::Logout() failed; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	return true;
}


#endif	// !ANDROID


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
