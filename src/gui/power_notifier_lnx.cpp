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


#include <cppdevtk/gui/power_notifier.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include "logind_power_notifier_lnx.hpp"
#include "upower_notifier_lnx.hpp"
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/cassert.hpp>

#include <QtDBus/QDBusError>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>
#include <new>


namespace cppdevtk {
namespace gui {


using detail::LogindPowerNotifier;
using detail::UPowerNotifier;


void PowerNotifier::Uninit() {
	if (pImpl_.get() != NULL) {
		pImpl_.reset();
	}
}

bool PowerNotifier::IsPowerNotifierServiceRegistered() {
	return LogindPowerNotifier::IsLogindServiceRegistered()
			|| UPowerNotifier::IsUPowerServiceRegistered();
}

PowerNotifier::PowerNotifier(): QObject(), ::cppdevtk::base::MeyersSingleton<PowerNotifier>(),
		pImpl_() {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	if (LogindPowerNotifier::IsLogindServiceRegistered()) {
		pImpl_.reset(new LogindPowerNotifier());
	}
	else {
		if (UPowerNotifier::IsUPowerServiceRegistered()) {
			pImpl_.reset(new UPowerNotifier());
		}
		else {
			throw CPPDEVTK_DBUS_EXCEPTION("None of logind/UPower services is registered",
					QDBusError(QDBusError::ServiceUnknown, "PowerNotifier ServiceUnknown"));
		}
	}
	
	CPPDEVTK_ASSERT(pImpl_.get() != NULL);
	
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(Sleeping()), SIGNAL(Sleeping())));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(Resuming()), SIGNAL(Resuming())));
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

PowerNotifier::~PowerNotifier() {
	CPPDEVTK_ASSERT((pImpl_.get() == NULL) && "Call Uninit() before leaving main()");
}


}	// namespace gui
}	// namespace cppdevtk
