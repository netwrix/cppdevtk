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


#include "upower_notifier_lnx.hpp"
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/verify.h>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>


// https://upower.freedesktop.org/releases/upower-0.9.23.tar.xz
#define CPPDEVTK_UPOWER_SERVICE_NAME "org.freedesktop.UPower"
#define CPPDEVTK_UPOWER_OBJ_PATH "/org/freedesktop/UPower"
#define CPPDEVTK_UPOWER_INTERFACE "org.freedesktop.UPower"


namespace cppdevtk {
namespace gui {
namespace detail {


UPowerNotifier::UPowerNotifier(): PowerNotifier::Impl() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	
	if (!systemBus.connect(CPPDEVTK_UPOWER_SERVICE_NAME, CPPDEVTK_UPOWER_OBJ_PATH, CPPDEVTK_UPOWER_INTERFACE,
			"Sleeping", this, SIGNAL(Sleeping()))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to UPower Sleeping signal", systemBus.lastError());
	}
	
	if (!systemBus.connect(CPPDEVTK_UPOWER_SERVICE_NAME, CPPDEVTK_UPOWER_OBJ_PATH, CPPDEVTK_UPOWER_INTERFACE,
			"Resuming", this, SIGNAL(Resuming()))) {
		const QDBusError kLastSystemBusError = systemBus.lastError();
		
		if (!systemBus.disconnect(CPPDEVTK_UPOWER_SERVICE_NAME, CPPDEVTK_UPOWER_OBJ_PATH, CPPDEVTK_UPOWER_INTERFACE,
			"Sleeping", this, SIGNAL(Sleeping()))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UPower Sleeping signal"
					<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
					<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
		}
		
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to UPower Resuming signal", kLastSystemBusError);
	}
}

UPowerNotifier::~UPowerNotifier() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (systemBus.isConnected()) {
		if (!systemBus.disconnect(CPPDEVTK_UPOWER_SERVICE_NAME, CPPDEVTK_UPOWER_OBJ_PATH, CPPDEVTK_UPOWER_INTERFACE,
			"Resuming", this, SIGNAL(Resuming()))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UPower Resuming signal"
					<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
					<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
		}
		
		if (!systemBus.disconnect(CPPDEVTK_UPOWER_SERVICE_NAME, CPPDEVTK_UPOWER_OBJ_PATH, CPPDEVTK_UPOWER_INTERFACE,
			"Sleeping", this, SIGNAL(Sleeping()))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from UPower Sleeping signal"
					<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
					<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
		}
	}
	else {
		const QDBusError kLastSystemBusError = systemBus.lastError();
		CPPDEVTK_LOG_WARN("failed to connect to system bus"
				<< "; errorType: " << QDBusError::errorString(kLastSystemBusError.type())
				<< "; errorName: " << kLastSystemBusError.name() << "; errorMsg: " << kLastSystemBusError.message());
	}
}

bool UPowerNotifier::IsUPowerServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_UPOWER_SERVICE_NAME, kSystemBus);
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
