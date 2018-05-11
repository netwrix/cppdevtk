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


#include "logind_power_notifier_lnx.hpp"
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/verify.h>

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>


// https://www.freedesktop.org/wiki/Software/systemd/logind
#define CPPDEVTK_LOGIND_SERVICE_NAME "org.freedesktop.login1"
#define CPPDEVTK_LOGIND_OBJ_PATH "/org/freedesktop/login1"
#define CPPDEVTK_LOGIND_MANAGER_INTERFACE "org.freedesktop.login1.Manager"


namespace cppdevtk {
namespace gui {
namespace detail {


using ::std::auto_ptr;


LogindPowerNotifier::LogindPowerNotifier(): PowerNotifier::Impl() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (!systemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", systemBus.lastError());
	}
	
	if (!systemBus.connect(CPPDEVTK_LOGIND_SERVICE_NAME, CPPDEVTK_LOGIND_OBJ_PATH, CPPDEVTK_LOGIND_MANAGER_INTERFACE,
			"PrepareForSleep", this, SLOT(OnPrepareForSleep(bool)))) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to Logind PrepareForSleep signal", systemBus.lastError());
	}
}

LogindPowerNotifier::~LogindPowerNotifier() {
	QDBusConnection systemBus = QDBusConnection::systemBus();
	if (systemBus.isConnected()) {
		if (!systemBus.disconnect(CPPDEVTK_LOGIND_SERVICE_NAME, CPPDEVTK_LOGIND_OBJ_PATH, CPPDEVTK_LOGIND_MANAGER_INTERFACE,
				"PrepareForSleep", this, SLOT(OnPrepareForSleep(bool)))) {
			const QDBusError kLastSystemBusError = systemBus.lastError();
			CPPDEVTK_LOG_WARN("failed to disconnect from Logind PrepareForSleep signal"
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

bool LogindPowerNotifier::IsLogindServiceRegistered() {
	const QDBusConnection kSystemBus = QDBusConnection::systemBus();
	if (!kSystemBus.isConnected()) {
		throw CPPDEVTK_DBUS_EXCEPTION("failed to connect to system bus", kSystemBus.lastError());
	}
	
	return util::IsDBusServiceRegistered(CPPDEVTK_LOGIND_SERVICE_NAME, kSystemBus);
}

void LogindPowerNotifier::OnPrepareForSleep(bool start) {
	if (start) {
		Q_EMIT Sleeping();
	}
	else {
		Q_EMIT Resuming();
	}
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
