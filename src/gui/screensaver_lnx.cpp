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


#include <cppdevtk/gui/screensaver.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtCore/QList>
#include <QtCore/QVariant>

#include <new>
#include <cstddef>


// https://github.com/KDE/kscreenlocker/blob/master/dbus/org.kde.screensaver.xml
#define CPPDEVTK_KDE_SCREENSAVER_SERVICE_NAME "org.kde.screensaver"
#define CPPDEVTK_KDE_SCREENSAVER_OBJ_PATH "/ScreenSaver"
#define CPPDEVTK_KDE_SCREENSAVER_INTERFACE "org.freedesktop.ScreenSaver"

// https://github.com/linuxmint/cinnamon-screensaver/blob/master/libcscreensaver/org.cinnamon.ScreenSaver.xml
#define CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME "org.cinnamon.ScreenSaver"
#define CPPDEVTK_CINNAMON_SCREENSAVER_OBJ_PATH "/org/cinnamon/ScreenSaver"
#define CPPDEVTK_CINNAMON_SCREENSAVER_INTERFACE "org.cinnamon.ScreenSaver"

// https://github.com/mate-desktop/mate-screensaver/blob/master/doc/dbus-interface.xml
#define CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME "org.mate.ScreenSaver"
#define CPPDEVTK_MATE_SCREENSAVER_OBJ_PATH "/org/mate/ScreenSaver"
#define CPPDEVTK_MATE_SCREENSAVER_INTERFACE "org.mate.ScreenSaver"

// https://github.com/GNOME/gnome-settings-daemon/blob/master/gnome-settings-daemon/org.gnome.ScreenSaver.xml
#define CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME "org.gnome.ScreenSaver"
#define CPPDEVTK_GNOME_SCREENSAVER_OBJ_PATH "/org/gnome/ScreenSaver"
#define CPPDEVTK_GNOME_SCREENSAVER_INTERFACE "org.gnome.ScreenSaver"

// https://github.com/KDE/kscreenlocker/blob/master/dbus/org.freedesktop.ScreenSaver.xml
#define CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME "org.freedesktop.ScreenSaver"
#define CPPDEVTK_FREEDESKTOP_SCREENSAVER_OBJ_PATH "/ScreenSaver"
#define CPPDEVTK_FREEDESKTOP_SCREENSAVER_INTERFACE "org.freedesktop.ScreenSaver"


namespace cppdevtk {
namespace gui {


using ::cppdevtk::util::IsDBusServiceRegistered;


bool ScreenSaver::Lock() {
	const QDBusMessage kReply = (pScreenSaverInterface_->service() != CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME)
			? pScreenSaverInterface_->call("Lock") : pScreenSaverInterface_->call("Lock", "");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = pScreenSaverInterface_->lastError();
		CPPDEVTK_LOG_ERROR("DBus call to screensaver::Lock() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool ScreenSaver::SetActive(bool active) {
	const QDBusMessage kReply = pScreenSaverInterface_->call("SetActive", active);
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		const QDBusError kLastError = pScreenSaverInterface_->lastError();
		CPPDEVTK_LOG_ERROR("DBus call to screensaver::SetActive() failed; dbusErrorName: " << kLastError.name()
				<< "; dbusErrorMessage: " << kLastError.message());
		return false;
	}
	return true;
}

bool ScreenSaver::IsActive() const {
	const QDBusMessage kReply = pScreenSaverInterface_->call("IsActive");
	if (kReply.type() == QDBusMessage::ErrorMessage) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus call to screensaver::IsActive() failed",
				pScreenSaverInterface_->lastError());
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

QDBusError ScreenSaver::GetLastError() const {
	return pScreenSaverInterface_->lastError();
}

bool ScreenSaver::IsScreenSaverServiceRegistered() {
	const QDBusConnection kSessionBus = QDBusConnection::sessionBus();
	if (!kSessionBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to session bus", kSessionBus.lastError());
	}
	
	return IsDBusServiceRegistered(CPPDEVTK_KDE_SCREENSAVER_SERVICE_NAME, kSessionBus)
			|| IsDBusServiceRegistered(CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME, kSessionBus)
			|| IsDBusServiceRegistered(CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME, kSessionBus)
			|| IsDBusServiceRegistered(CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME, kSessionBus)
			|| IsDBusServiceRegistered(CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME, kSessionBus);
}

void ScreenSaver::Refresh() {}

ScreenSaver::ScreenSaver(): QObject(), ::cppdevtk::base::MeyersSingleton<ScreenSaver>(),
		pScreenSaverInterface_() {
	QDBusConnection sessionBus = QDBusConnection::sessionBus();
	if (!sessionBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to session bus", sessionBus.lastError());
	}
	
	if (IsDBusServiceRegistered(CPPDEVTK_KDE_SCREENSAVER_SERVICE_NAME, sessionBus)) {
		pScreenSaverInterface_.reset(new QDBusInterface(CPPDEVTK_KDE_SCREENSAVER_SERVICE_NAME,
				CPPDEVTK_KDE_SCREENSAVER_OBJ_PATH, CPPDEVTK_KDE_SCREENSAVER_INTERFACE, sessionBus));
	}
	else {
		if (IsDBusServiceRegistered(CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME, sessionBus)) {
			pScreenSaverInterface_.reset(new QDBusInterface(CPPDEVTK_CINNAMON_SCREENSAVER_SERVICE_NAME,
					CPPDEVTK_CINNAMON_SCREENSAVER_OBJ_PATH, CPPDEVTK_CINNAMON_SCREENSAVER_INTERFACE, sessionBus));
		}
		else {
			if (IsDBusServiceRegistered(CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME, sessionBus)) {
				pScreenSaverInterface_.reset(new QDBusInterface(CPPDEVTK_MATE_SCREENSAVER_SERVICE_NAME,
						CPPDEVTK_MATE_SCREENSAVER_OBJ_PATH, CPPDEVTK_MATE_SCREENSAVER_INTERFACE, sessionBus));
			}
			else {
				if (IsDBusServiceRegistered(CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME, sessionBus)) {
					pScreenSaverInterface_.reset(new QDBusInterface(CPPDEVTK_GNOME_SCREENSAVER_SERVICE_NAME,
							CPPDEVTK_GNOME_SCREENSAVER_OBJ_PATH, CPPDEVTK_GNOME_SCREENSAVER_INTERFACE, sessionBus));
				}
				else {
					if (IsDBusServiceRegistered(CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME, sessionBus)) {
						pScreenSaverInterface_.reset(new QDBusInterface(CPPDEVTK_FREEDESKTOP_SCREENSAVER_SERVICE_NAME,
								CPPDEVTK_FREEDESKTOP_SCREENSAVER_OBJ_PATH, CPPDEVTK_FREEDESKTOP_SCREENSAVER_INTERFACE, sessionBus));
					}
					else {
						throw CPPDEVTK_DBUS_EXCEPTION("no supported DBus screensaver service registered",
								sessionBus.lastError());
					}
				}
			}
		}
	}
	
	CPPDEVTK_ASSERT(pScreenSaverInterface_.get() != NULL);
	
	CPPDEVTK_LOG_DEBUG("ScreenSaverInterface: service: " << pScreenSaverInterface_->service()
			<< "; path: " << pScreenSaverInterface_->path() << "; interface: " << pScreenSaverInterface_->interface());
	
	if (!pScreenSaverInterface_->isValid()) {
		throw CPPDEVTK_DBUS_EXCEPTION("DBus screensaver interface is not valid",
				pScreenSaverInterface_->lastError());
	}
	
	if (!sessionBus.connect(pScreenSaverInterface_->service(), pScreenSaverInterface_->path(),
			pScreenSaverInterface_->interface(), "ActiveChanged", this, SIGNAL(ActiveChanged(bool)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to DBus screensaver interface ActiveChanged() signal",
				sessionBus.lastError());
	}
}

ScreenSaver::~ScreenSaver() {}


}	// namespace gui
}	// namespace cppdevtk
