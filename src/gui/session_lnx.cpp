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


#include <cppdevtk/gui/session.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include "logind_session_lnx.hpp"
#include "console_kit_session_lnx.hpp"
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/util/dbus_utils.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/logger.hpp>

#include <memory>


#define CPPDEVTK_LOGIND_SERVICE_NAME "org.freedesktop.login1"
#define CPPDEVTK_CONSOLE_KIT_SERVICE_NAME "org.freedesktop.ConsoleKit"


namespace cppdevtk {
namespace gui {


bool Session::Lock() {
	return pImpl_->Lock();
}

Session::~Session() {}

Session::IdleTime Session::GetIdleTime() const {
	return pImpl_->GetIdleTime();
}

Session::Session(const QString& service, const QString& path): QObject(), id_(), pImpl_() {
	CPPDEVTK_ASSERT((service == CPPDEVTK_LOGIND_SERVICE_NAME) || (service == CPPDEVTK_CONSOLE_KIT_SERVICE_NAME));
	CPPDEVTK_ASSERT(!path.isEmpty());
	
	if (service == CPPDEVTK_LOGIND_SERVICE_NAME) {
		pImpl_.reset(new detail::LogindSession(QDBusObjectPath(path)));
	}
	else {
		pImpl_.reset(new detail::ConsoleKitSession(QDBusObjectPath(path)));
	}
	CPPDEVTK_ASSERT(pImpl_.get() != NULL);
	
	id_ = pImpl_->GetId();
	CPPDEVTK_ASSERT(!id_.isEmpty());
	
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(Activated()), SIGNAL(Activated())));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(Deactivated()), SIGNAL(Deactivated())));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(Locked()), SIGNAL(Locked())));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(Unlocked()), SIGNAL(Unlocked())));
}


}	// namespace gui
}	// namespace cppdevtk
