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


#include <cppdevtk/gui/session_manager.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include "logind_manager_lnx.hpp"
#include "console_kit_manager_lnx.hpp"
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/util/dbus_exception.hpp>

#include <new>


namespace cppdevtk {
namespace gui {


using detail::LogindManager;
using detail::ConsoleKitManager;


bool SessionManager::Logout() {
	return pImpl_->Logout();
}

bool SessionManager::Shutdown() {
	return pImpl_->Shutdown();
}

::std::auto_ptr< ::cppdevtk::gui::Session> SessionManager::GetCurrentProcessSession() const {
	return pImpl_->GetCurrentProcessSession();
}

bool SessionManager::IsSessionManagerServiceRegistered() {
	return LogindManager::IsLogindServiceRegistered() || ConsoleKitManager::IsConsoleKitServiceRegistered();
}

SessionManager::SessionManager(): QObject(), ::cppdevtk::base::MeyersSingleton<SessionManager>(), pImpl_() {
	if (LogindManager::IsLogindServiceRegistered()) {
		pImpl_.reset(new LogindManager());
	}
	else {
		if (ConsoleKitManager::IsConsoleKitServiceRegistered()) {
			pImpl_.reset(new ConsoleKitManager());
		}
		else {
			throw CPPDEVTK_DBUS_EXCEPTION("None of logind/ConsoleKit services is registered",
					QDBusError(QDBusError::ServiceUnknown, "SessionManager ServiceUnknown"));
		}
	}
	
	CPPDEVTK_ASSERT(pImpl_.get() != NULL);
}

SessionManager::~SessionManager() {}


}	// namespace gui
}	// namespace cppdevtk
