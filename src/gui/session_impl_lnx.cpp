/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#include "session_impl_lnx.hpp"
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusError>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtX11Extras/QX11Info>
#else
#include <QtGui/QX11Info>
#endif

#include <X11/extensions/scrnsaver.h>
#include <X11/Xlib.h>

#include CPPDEVTK_TR1_HEADER(memory)
#include <new>


// https://github.com/linuxmint/cinnamon-screensaver/blob/master/libcscreensaver/org.cinnamon.ScreenSaver.xml
#define CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME "org.cinnamon.ScreenSaver"
#define CPPDEVTK_CINNAMON_SCREENSAVER_OBJ_PATH "/org/cinnamon/ScreenSaver"
#define CPPDEVTK_CINNAMON_SCREENSAVER_INTERFACE "org.cinnamon.ScreenSaver"

// https://github.com/mate-desktop/mate-screensaver/blob/master/doc/dbus-interface.xml
#define CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME "org.mate.ScreenSaver"
#define CPPDEVTK_MATE_SCREENSAVER_OBJ_PATH "/org/mate/ScreenSaver"
#define CPPDEVTK_MATE_SCREENSAVER_INTERFACE "org.mate.ScreenSaver"

// https://github.com/GNOME/mutter/blob/master/src/org.gnome.Mutter.IdleMonitor.xml
#define CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_SERVICE_NAME "org.gnome.Mutter.IdleMonitor"
#define CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_CORE_OBJ_PATH "/org/gnome/Mutter/IdleMonitor/Core"
#define CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_INTERFACE "org.gnome.Mutter.IdleMonitor"

// https://github.com/GNOME/gnome-settings-daemon/blob/master/gnome-settings-daemon/org.gnome.ScreenSaver.xml
// https://people.gnome.org/~mccann/gnome-screensaver/docs/gnome-screensaver.html
#define CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME "org.gnome.ScreenSaver"
#define CPPDEVTK_GNOME_SCREENSAVER_OBJ_PATH "/org/gnome/ScreenSaver"
#define CPPDEVTK_GNOME_SCREENSAVER_INTERFACE "org.gnome.ScreenSaver"

// https://github.com/KDE/kscreenlocker/blob/master/dbus/org.freedesktop.ScreenSaver.xml
#define CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME "org.freedesktop.ScreenSaver"
#define CPPDEVTK_FREEDESKTOP_SCREENSAVER_OBJ_PATH "/ScreenSaver"
#define CPPDEVTK_FREEDESKTOP_SCREENSAVER_INTERFACE "org.freedesktop.ScreenSaver"


namespace cppdevtk {
namespace gui {


using util::IsDBusServiceRegistered;


Session::IdleTime Session::Impl::GetIdleTime() const {
	uint uIdleTime = 0;
	if (CinnamonScreenSaverGetSessionIdleTime(uIdleTime)) {
		return uIdleTime;
	}
	if (MateScreenSaverGetSessionIdleTime(uIdleTime)) {
		return uIdleTime;
	}
	qulonglong ullIdleTime = 0;
	if (GnomeMutterIdleMonitorGetIdletime(ullIdleTime)) {
		return ullIdleTime;
	}
	if (GnomeScreenSaverGetSessionIdleTime(uIdleTime)) {
		return uIdleTime;
	}
	if (FreedesktopScreenSaverGetSessionIdleTime(uIdleTime)) {
		return uIdleTime;
	}
	return XScreenSaverInfoGetIdle();
}

Session::Impl::Impl(const QString& service, const QString& path, const QString& interface): QObject(),
		dbusInterface_(service, path, interface, QDBusConnection::systemBus()) {
	CPPDEVTK_ASSERT(!service.isEmpty());
	CPPDEVTK_ASSERT(!path.isEmpty());
	CPPDEVTK_ASSERT(!interface.isEmpty());
	if (!dbusInterface_.isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("Session DBus interface is not valid", dbusInterface_.lastError());
	}
	
	CPPDEVTK_VERIFY(connect(this, SIGNAL(ActiveChanged(bool)), SLOT(OnActiveChanged(bool))));
}

bool Session::Impl::CinnamonScreenSaverGetSessionIdleTime(uint& msIdleTime) {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
			const QDBusError kLastSessionBusError = kSessionBus.lastError();
			CPPDEVTK_LOG_ERROR("systemBus is not connected"
					<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
					<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
			return false;
	}
	
	if (!IsDBusServiceRegistered(CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME, kSessionBus)) {
		CPPDEVTK_LOG_TRACE("CinnamonScreenSaver service is not registered");
		return false;
	}
	
	QDBusInterface dbusCinnamonScreenSaverInterface(CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME,
			CPPDEVTK_CINNAMON_SCREENSAVER_OBJ_PATH, CPPDEVTK_CINNAMON_SCREENSAVER_INTERFACE, kSessionBus);
	if (!dbusCinnamonScreenSaverInterface.isValid()) {
		const QDBusError kLastError = dbusCinnamonScreenSaverInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusCinnamonScreenSaverInterface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusCinnamonScreenSaverInterface.call("GetSessionIdleTime");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusCinnamonScreenSaverInterface.GetSessionIdleTime() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "u");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UInt);
	msIdleTime = kReplyArg.value<uint>();
	
	return true;
}

bool Session::Impl::MateScreenSaverGetSessionIdleTime(uint& msIdleTime) {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
			const QDBusError kLastSessionBusError = kSessionBus.lastError();
			CPPDEVTK_LOG_ERROR("systemBus is not connected"
					<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
					<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
			return false;
	}
	
	if (!IsDBusServiceRegistered(CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME, kSessionBus)) {
		CPPDEVTK_LOG_TRACE("MateScreenSaver service is not registered");
		return false;
	}
	
	QDBusInterface dbusMateScreenSaverInterface(CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME,
			CPPDEVTK_MATE_SCREENSAVER_OBJ_PATH, CPPDEVTK_MATE_SCREENSAVER_INTERFACE, kSessionBus);
	if (!dbusMateScreenSaverInterface.isValid()) {
		const QDBusError kLastError = dbusMateScreenSaverInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusMateScreenSaverInterface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusMateScreenSaverInterface.call("GetSessionIdleTime");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusMateScreenSaverInterface.GetSessionIdleTime() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "u");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UInt);
	msIdleTime = kReplyArg.value<uint>();
	
	return true;
}

bool Session::Impl::GnomeMutterIdleMonitorGetIdletime(qulonglong& msIdleTime) {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
			const QDBusError kLastSessionBusError = kSessionBus.lastError();
			CPPDEVTK_LOG_ERROR("systemBus is not connected"
					<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
					<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
			return false;
	}
	
	if (!IsDBusServiceRegistered(CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_SERVICE_NAME, kSessionBus)) {
		CPPDEVTK_LOG_TRACE("GnomeMutterIdleMonitor service is not registered");
		return false;
	}
	
	QDBusInterface dbusGnomeMutterIdleMonitorInterface(CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_SERVICE_NAME,
			CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_CORE_OBJ_PATH, CPPDEVTK_GNOME_MUTTER_IDLE_MONITOR_INTERFACE, kSessionBus);
	if (!dbusGnomeMutterIdleMonitorInterface.isValid()) {
		const QDBusError kLastError = dbusGnomeMutterIdleMonitorInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusGnomeMutterIdleMonitorInterface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusGnomeMutterIdleMonitorInterface.call("GetIdletime");	// lower t is correct (not a typo...)
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusGnomeMutterIdleMonitorInterface.GetIdletime() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "t");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::ULongLong);
	msIdleTime = kReplyArg.value<qulonglong>();
	
	return true;
}

bool Session::Impl::GnomeScreenSaverGetSessionIdleTime(uint& msIdleTime) {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
			const QDBusError kLastSessionBusError = kSessionBus.lastError();
			CPPDEVTK_LOG_ERROR("systemBus is not connected"
					<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
					<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
			return false;
	}
	
	if (!IsDBusServiceRegistered(CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME, kSessionBus)) {
		CPPDEVTK_LOG_TRACE("GnomeScreenSaver service is not registered");
		return false;
	}
	
	QDBusInterface dbusGnomeScreenSaverInterface(CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME,
			CPPDEVTK_GNOME_SCREENSAVER_OBJ_PATH, CPPDEVTK_GNOME_SCREENSAVER_INTERFACE, kSessionBus);
	if (!dbusGnomeScreenSaverInterface.isValid()) {
		const QDBusError kLastError = dbusGnomeScreenSaverInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusGnomeScreenSaverInterface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusGnomeScreenSaverInterface.call("GetSessionIdleTime");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusGnomeScreenSaverInterface.GetSessionIdleTime() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "u");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UInt);
	msIdleTime = kReplyArg.value<uint>();
	
	return true;
}

bool Session::Impl::FreedesktopScreenSaverGetSessionIdleTime(uint& msIdleTime) {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
			const QDBusError kLastSessionBusError = kSessionBus.lastError();
			CPPDEVTK_LOG_ERROR("systemBus is not connected"
					<< "; errorType: " << QDBusError::errorString(kLastSessionBusError.type())
					<< "; errorName: " << kLastSessionBusError.name() << "; errorMsg: " << kLastSessionBusError.message());
			return false;
	}
	
	if (!IsDBusServiceRegistered(CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME, kSessionBus)) {
		CPPDEVTK_LOG_TRACE("FreedesktopScreenSaver service is not registered");
		return false;
	}
	
	QDBusInterface dbusFreedesktopScreenSaverInterface(CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME,
			CPPDEVTK_FREEDESKTOP_SCREENSAVER_OBJ_PATH, CPPDEVTK_FREEDESKTOP_SCREENSAVER_INTERFACE, kSessionBus);
	if (!dbusFreedesktopScreenSaverInterface.isValid()) {
		const QDBusError kLastError = dbusFreedesktopScreenSaverInterface.lastError();
		CPPDEVTK_LOG_ERROR("dbusFreedesktopScreenSaverInterface is not valid"
				<< "; errorType: " << QDBusError::errorString(kLastError.type())
				<< "; errorName: " << kLastError.name() << "; errorMsg: " << kLastError.message());
		return false;
	}
	
	const QDBusMessage kReply = dbusFreedesktopScreenSaverInterface.call("GetSessionIdleTime");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		CPPDEVTK_LOG_ERROR("dbusFreedesktopScreenSaverInterface.GetSessionIdleTime() failed"
				<< "; errorName: " << kReply.errorName()
				<< "; errorMessage: " << kReply.errorMessage());
		return false;
	}
	CPPDEVTK_ASSERT(kReply.type() == QDBusMessage::ReplyMessage);
	CPPDEVTK_ASSERT(kReply.signature() == "u");
	
	const QList<QVariant> kReplyArgs = kReply.arguments();
	CPPDEVTK_ASSERT(kReplyArgs.size() == 1);
	
	const QVariant kReplyArg = kReplyArgs[0];
	CPPDEVTK_ASSERT(!kReplyArg.isNull());
	CPPDEVTK_ASSERT(kReplyArg.isValid());
	CPPDEVTK_ASSERT(kReplyArg.type() == QVariant::UInt);
	msIdleTime = kReplyArg.value<uint>();
	
	return true;
}

unsigned long Session::Impl::XScreenSaverInfoGetIdle() {
	Display* pDisplay = QX11Info::display();
	if (pDisplay == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("failed to get X11 display");
	}
	
	int eventBaseReturn = 0;
	int errorBaseReturn = 0;
	if (!XScreenSaverQueryExtension(pDisplay, &eventBaseReturn, &errorBaseReturn)) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("XScreenSaver extension is not available");
	}
	
	CPPDEVTK_TR1_NS::shared_ptr<XScreenSaverInfo> pScreenSaverInfo(XScreenSaverAllocInfo(), &XFree);
	if (pScreenSaverInfo.get() == NULL) {
		throw ::std::bad_alloc();
	}
	
	if (XScreenSaverQueryInfo(pDisplay, QX11Info::appRootWindow(), pScreenSaverInfo.get()) == 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("XScreenSaver extension is not supported");
	}
	
	return pScreenSaverInfo->idle;
}


}	// namespace gui
}	// namespace cppdevtk
