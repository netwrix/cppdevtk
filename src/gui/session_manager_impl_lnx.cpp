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


#include "session_manager_impl_lnx.hpp"
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/base/verify.h>

#include <QtCore/QVariant>
#include <QtDBus/QDBusConnection>


// https://github.com/KDE/plasma-workspace/blob/master/ksmserver/org.kde.KSMServerInterface.xml
#define CPPDEVTK_KDE_SM_SERVICE_NAME "org.kde.ksmserver"
#define CPPDEVTK_KDE_SM_KSMSERVER_OBJ_PATH "/KSMServer"
#define CPPDEVTK_KDE_SM_KSMSERVER_INTERFACE "org.kde.KSMServerInterface"

// Cinnamon, Gnome 3, Mate (Gnome 2)
// https://github.com/GNOME/gnome-session/blob/master/gnome-session/org.gnome.SessionManager.xml
// https://github.com/mate-desktop/mate-session-manager/blob/master/mate-session/org.gnome.SessionManager.xml
#define CPPDEVTK_GNOME_SM_SERVICE_NAME "org.gnome.SessionManager"
#define CPPDEVTK_GNOME_SM_OBJ_PATH "/org/gnome/SessionManager"
#define CPPDEVTK_GNOME_SM_INTERFACE "org.gnome.SessionManager"

// https://git.devuan.org/devuan-packages/xfce4-session/blob/d72c7194acc74e614e8baf3b75ea9d61feb16125/xfce4-session/xfsm-manager-dbus.xml
#define CPPDEVTK_XFCE_SM_SERVICE_NAME "org.xfce.SessionManager"
#define CPPDEVTK_XFCE_SM_OBJ_PATH "/org/xfce/SessionManager"
#define CPPDEVTK_XFCE_SM_INTERFACE "org.xfce.SessionManager"

// https://wiki.lxde.org/en/LXSession#Dbus_interface
#define CPPDEVTK_LXDE_SM_SERVICE_NAME "org.lxde.SessionManager"
#define CPPDEVTK_LXDE_SM_OBJ_PATH "/org/lxde/SessionManager"
#define CPPDEVTK_LXDE_SM_INTERFACE "org.lxde.SessionManager"

#define CPPDEVTK_LXQT_SM_SERVICE_NAME "org.lxqt.session"
#define CPPDEVTK_LXQT_SM_OBJ_PATH "/LXQtSession"
#define CPPDEVTK_LXQT_SM_INTERFACE "org.lxqt.session"


namespace cppdevtk {
namespace gui {


using util::IsDBusServiceRegistered;


bool SessionManager::Impl::Logout() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	if (IsDBusServiceRegistered(CPPDEVTK_KDE_SM_SERVICE_NAME, kSessionBus)) {
		return KdeSmLogout();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_GNOME_SM_SERVICE_NAME, kSessionBus)) {
		return GnomeSmLogout();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_XFCE_SM_SERVICE_NAME, kSessionBus)) {
		return XfceSmLogout();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_LXDE_SM_SERVICE_NAME, kSessionBus)) {
		return LxdeSmLogout();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_LXQT_SM_SERVICE_NAME, kSessionBus)) {
		return LxqtSmLogout();
	}
	
	CPPDEVTK_LOG_WARN("SessionManager not supported (none of KDE(KSM)/Gnome/XFCE/LXDE/LXQT)");
	return false;
}

bool SessionManager::Impl::Shutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	if (IsDBusServiceRegistered(CPPDEVTK_KDE_SM_SERVICE_NAME, kSessionBus)) {
		return KdeSmShutdown();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_GNOME_SM_SERVICE_NAME, kSessionBus)) {
		if (GnomeSmRequestShutdown()) {
			return true;
		}
		return GnomeSmShutdown();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_XFCE_SM_SERVICE_NAME, kSessionBus)) {
		return XfceSmShutdown();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_LXDE_SM_SERVICE_NAME, kSessionBus)) {
		if (LxdeSmRequestShutdown()) {
			return true;
		}
		return LxdeSmShutdown();
	}
	if (IsDBusServiceRegistered(CPPDEVTK_LXQT_SM_SERVICE_NAME, kSessionBus)) {
		return LxqtSmShutdown();
	}
	
	CPPDEVTK_LOG_WARN("SessionManager not supported (none of KDE(KSM)/Gnome/XFCE/LXDE/LXQT)");
	return false;
}

SessionManager::Impl::Impl(const QString& service, const QString& path, const QString& interface):
		::cppdevtk::base::NonCopyable(),
		dbusInterface_(service, path, interface, QDBusConnection::systemBus()) {
	CPPDEVTK_ASSERT(!service.isEmpty());
	CPPDEVTK_ASSERT(!path.isEmpty());
	CPPDEVTK_ASSERT(!interface.isEmpty());
	if (!dbusInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("SessionManager DBus interface is not valid", dbusInterface_.lastError());
	}
}

bool SessionManager::Impl::KdeSmLogout() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_KDE_SM_SERVICE_NAME, kSessionBus));
	
	// https://github.com/KDE/plasma-workspace/blob/master/ksmserver/org.kde.KSMServerInterface.xml
	QDBusInterface dbusKsmInterface(CPPDEVTK_KDE_SM_SERVICE_NAME, CPPDEVTK_KDE_SM_KSMSERVER_OBJ_PATH,
			CPPDEVTK_KDE_SM_KSMSERVER_INTERFACE, kSessionBus);
	if (!dbusKsmInterface.isValid()) {
		const QDBusError kLastError = dbusKsmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusKsmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	/*
	<method name="logout">
	  <arg direction="in" type="i"/>	// KWorkspace::ShutdownConfirm,
	  <arg direction="in" type="i"/>	// KWorkspace::ShutdownType
	  <arg direction="in" type="i"/>	// KWorkspace::ShutdownMode
	</method>
	
	https://api.kde.org/4.x-api/kde-workspace-apidocs/plasma-workspace/libkworkspace/html/namespaceKWorkSpace.html
	enum ShutdownConfirm { ShutdownConfirmDefault = -1, ShutdownConfirmNo = 0, ShutdownConfirmYes = 1 }
	enum ShutdownType {ShutdownTypeDefault = -1, ShutdownTypeNone = 0, ShutdownTypeReboot = 1, ShutdownTypeHalt = 2,ShutdownTypeLogout = 3}
	enum ShutdownMode {ShutdownModeDefault = -1, ShutdownModeSchedule = 0, ShutdownModeTryNow = 1, ShutdownModeForceNow = 2, ShutdownModeInteractive = 3}
	*/
	const QDBusMessage kReply = dbusKsmInterface.call("logout", QVariant(0), QVariant(3), QVariant(1));
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusKsmInterface call to logout(0, 3, 1) failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::GnomeSmLogout() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_GNOME_SM_SERVICE_NAME, kSessionBus));
	
	// https://github.com/GNOME/gnome-session/blob/master/gnome-session/org.gnome.SessionManager.xml
	QDBusInterface dbusGnomeSmInterface(CPPDEVTK_GNOME_SM_SERVICE_NAME, CPPDEVTK_GNOME_SM_OBJ_PATH,
			CPPDEVTK_GNOME_SM_INTERFACE, kSessionBus);
	if (!dbusGnomeSmInterface.isValid()) {
		const QDBusError kLastError = dbusGnomeSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusGnomeSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	// Logout(): 0 = Normal; 1 = No confirmation inferface should be shown
	const QDBusMessage kReply = dbusGnomeSmInterface.call("Logout", QVariant((uint)1));
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusGnomeSmInterface call to Logout(1) failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::XfceSmLogout() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_XFCE_SM_SERVICE_NAME, kSessionBus));
	
	// https://git.devuan.org/devuan-packages/xfce4-session/blob/d72c7194acc74e614e8baf3b75ea9d61feb16125/xfce4-session/xfsm-manager-dbus.xml
	QDBusInterface dbusXfceSmInterface(CPPDEVTK_XFCE_SM_SERVICE_NAME, CPPDEVTK_XFCE_SM_OBJ_PATH,
			CPPDEVTK_XFCE_SM_INTERFACE, kSessionBus);
	if (!dbusXfceSmInterface.isValid()) {
		const QDBusError kLastError = dbusXfceSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusXfceSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	/*
	<method name="Logout">
	  <arg name="show_dialog" type="b" direction="in"/>
	  <arg name="allow_save" type="b" direction="in"/>
	</method>
	*/
	const QDBusMessage kReply = dbusXfceSmInterface.call("Logout", QVariant(false), QVariant(true));
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusXfceSmInterface call to Logout(false, true) failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::LxdeSmLogout() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_LXDE_SM_SERVICE_NAME, kSessionBus));
	
	QDBusInterface dbusLxdeSmInterface(CPPDEVTK_LXDE_SM_SERVICE_NAME, CPPDEVTK_LXDE_SM_OBJ_PATH,
			CPPDEVTK_LXDE_SM_INTERFACE, kSessionBus);
	if (!dbusLxdeSmInterface.isValid()) {
		const QDBusError kLastError = dbusLxdeSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusLxdeSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusLxdeSmInterface.call("Logout");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusLxdeSmInterface call to Logout() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::LxqtSmLogout() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_LXQT_SM_SERVICE_NAME, kSessionBus));
	
	QDBusInterface dbusLxqtSmInterface(CPPDEVTK_LXQT_SM_SERVICE_NAME, CPPDEVTK_LXQT_SM_OBJ_PATH,
			CPPDEVTK_LXQT_SM_INTERFACE, kSessionBus);
	if (!dbusLxqtSmInterface.isValid()) {
		const QDBusError kLastError = dbusLxqtSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusLxqtSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusLxqtSmInterface.call("logout");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusLxqtSmInterface call to logout() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::KdeSmShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_KDE_SM_SERVICE_NAME, kSessionBus));
	
	// https://github.com/KDE/plasma-workspace/blob/master/ksmserver/org.kde.KSMServerInterface.xml
	QDBusInterface dbusKsmInterface(CPPDEVTK_KDE_SM_SERVICE_NAME, CPPDEVTK_KDE_SM_KSMSERVER_OBJ_PATH,
			CPPDEVTK_KDE_SM_KSMSERVER_INTERFACE, kSessionBus);
	if (!dbusKsmInterface.isValid()) {
		const QDBusError kLastError = dbusKsmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusKsmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	/*
	<method name="logout">
	  <arg direction="in" type="i"/>	// KWorkspace::ShutdownConfirm,
	  <arg direction="in" type="i"/>	// KWorkspace::ShutdownType
	  <arg direction="in" type="i"/>	// KWorkspace::ShutdownMode
	</method>
	
	https://api.kde.org/4.x-api/kde-workspace-apidocs/plasma-workspace/libkworkspace/html/namespaceKWorkSpace.html
	enum ShutdownConfirm { ShutdownConfirmDefault = -1, ShutdownConfirmNo = 0, ShutdownConfirmYes = 1 }
	enum ShutdownType {ShutdownTypeDefault = -1, ShutdownTypeNone = 0, ShutdownTypeReboot = 1, ShutdownTypeHalt = 2,ShutdownTypeLogout = 3}
	enum ShutdownMode {ShutdownModeDefault = -1, ShutdownModeSchedule = 0, ShutdownModeTryNow = 1, ShutdownModeForceNow = 2, ShutdownModeInteractive = 3}
	*/
	const QDBusMessage kReply = dbusKsmInterface.call("logout", QVariant(0), QVariant(2), QVariant(1));
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusKsmInterface call to logout(0, 2, 1) failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::GnomeSmRequestShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_GNOME_SM_SERVICE_NAME, kSessionBus));
	
	// https://github.com/mate-desktop/mate-session-manager/blob/master/mate-session/org.gnome.SessionManager.xml
	QDBusInterface dbusGnomeSmInterface(CPPDEVTK_GNOME_SM_SERVICE_NAME, CPPDEVTK_GNOME_SM_OBJ_PATH,
			CPPDEVTK_GNOME_SM_INTERFACE, kSessionBus);
	if (!dbusGnomeSmInterface.isValid()) {
		const QDBusError kLastError = dbusGnomeSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusGnomeSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusGnomeSmInterface.call("RequestShutdown");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusGnomeSmInterface call to RequestShutdown() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::GnomeSmShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_GNOME_SM_SERVICE_NAME, kSessionBus));
	
	// https://github.com/GNOME/gnome-session/blob/master/gnome-session/org.gnome.SessionManager.xml
	QDBusInterface dbusGnomeSmInterface(CPPDEVTK_GNOME_SM_SERVICE_NAME, CPPDEVTK_GNOME_SM_OBJ_PATH,
			CPPDEVTK_GNOME_SM_INTERFACE, kSessionBus);
	if (!dbusGnomeSmInterface.isValid()) {
		const QDBusError kLastError = dbusGnomeSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusGnomeSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusGnomeSmInterface.call("Shutdown");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusGnomeSmInterface call to Shutdown() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::XfceSmShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_XFCE_SM_SERVICE_NAME, kSessionBus));
	
	// https://git.devuan.org/devuan-packages/xfce4-session/blob/d72c7194acc74e614e8baf3b75ea9d61feb16125/xfce4-session/xfsm-manager-dbus.xml
	QDBusInterface dbusXfceSmInterface(CPPDEVTK_XFCE_SM_SERVICE_NAME, CPPDEVTK_XFCE_SM_OBJ_PATH,
			CPPDEVTK_XFCE_SM_INTERFACE, kSessionBus);
	if (!dbusXfceSmInterface.isValid()) {
		const QDBusError kLastError = dbusXfceSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusXfceSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	/*
    <method name="Shutdown">
      <arg name="allow_save" type="b" direction="in"/>
    </method>
	*/
	const QDBusMessage kReply = dbusXfceSmInterface.call("Shutdown", QVariant(true));
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusXfceSmInterface call to Shutdown(true) failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::LxdeSmRequestShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_LXDE_SM_SERVICE_NAME, kSessionBus));
	
	QDBusInterface dbusLxdeSmInterface(CPPDEVTK_LXDE_SM_SERVICE_NAME, CPPDEVTK_LXDE_SM_OBJ_PATH,
			CPPDEVTK_LXDE_SM_INTERFACE, kSessionBus);
	if (!dbusLxdeSmInterface.isValid()) {
		const QDBusError kLastError = dbusLxdeSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusLxdeSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusLxdeSmInterface.call("RequestShutdown");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusLxdeSmInterface call to RequestShutdown() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::LxdeSmShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_LXDE_SM_SERVICE_NAME, kSessionBus));
	
	QDBusInterface dbusLxdeSmInterface(CPPDEVTK_LXDE_SM_SERVICE_NAME, CPPDEVTK_LXDE_SM_OBJ_PATH,
			CPPDEVTK_LXDE_SM_INTERFACE, kSessionBus);
	if (!dbusLxdeSmInterface.isValid()) {
		const QDBusError kLastError = dbusLxdeSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusLxdeSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusLxdeSmInterface.call("Shutdown");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusLxdeSmInterface call to Shutdown() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}

bool SessionManager::Impl::LxqtSmShutdown() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		const QDBusError kLastSessionBusError = kSessionBus.lastError();
		CPPDEVTK_LOG_ERROR("failed to connect to session bus"
				<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
				<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
		return false;
	}
	
	CPPDEVTK_ASSERT(IsDBusServiceRegistered(CPPDEVTK_LXQT_SM_SERVICE_NAME, kSessionBus));
	
	QDBusInterface dbusLxqtSmInterface(CPPDEVTK_LXQT_SM_SERVICE_NAME, CPPDEVTK_LXQT_SM_OBJ_PATH,
			CPPDEVTK_LXQT_SM_INTERFACE, kSessionBus);
	if (!dbusLxqtSmInterface.isValid()) {
		const QDBusError kLastError = dbusLxqtSmInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusLxqtSmInterface interface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusLxqtSmInterface.call("powerOff");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusLxqtSmInterface call to powerOff() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	
	return true;
}


}	// namespace gui
}	// namespace cppdevtk
