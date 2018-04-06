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


#include "application.hpp"

#include <cppdevtk/base/verify.h>
#include <cppdevtk/util/dbus_exception.hpp>

#include <QtDBus/QDBusConnection>


namespace cppdevtk {
namespace test_term_sig {


Application::Application(int& argc, char** argv): CoreApplication(argc, argv)
#		if (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)
		, pLogindSession_(), pConsoleKitSession_()
#		endif
		{
	CPPDEVTK_COUT << "Application ctor" << endl;
	
#	if (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)
	if (!util::LogindManager::IsLogindServiceRegistered()) {
		if (!util::ConsoleKitManager::IsConsoleKitServiceRegistered()) {
			throw CPPDEVTK_DBUS_EXCEPTION("None of Logind or ConsoleKit services is registered",
					QDBusConnection::systemBus().lastError());
		}
		
		const util::ConsoleKitManager& kTheConsoleKitManager = util::ConsoleKitManager::GetInstance();
		CPPDEVTK_COUT << "kTheConsoleKitManager.CanRestart(): " << kTheConsoleKitManager.CanRestart() << endl;
		CPPDEVTK_COUT << "kTheConsoleKitManager.CanStop(): " << kTheConsoleKitManager.CanStop() << endl;
		
		pConsoleKitSession_ = kTheConsoleKitManager.GetCurrentSession();
		CPPDEVTK_COUT << "pConsoleKitSession_->GetId(): " << pConsoleKitSession_->GetId().path() << endl;
		CPPDEVTK_COUT << "pConsoleKitSession_->GetUnixUser(): " << pConsoleKitSession_->GetUnixUser() << endl;
		CPPDEVTK_COUT << "pConsoleKitSession_->GetRemoteHostName(): " << pConsoleKitSession_->GetRemoteHostName() << endl;
		CPPDEVTK_COUT << "pConsoleKitSession_->GetLoginSessionId(): " << pConsoleKitSession_->GetLoginSessionId() << endl;
		CPPDEVTK_COUT << "pConsoleKitSession_->IsActive(): " << pConsoleKitSession_->IsActive() << endl;
		CPPDEVTK_COUT << "pConsoleKitSession_->IsLocal(): " << pConsoleKitSession_->IsLocal() << endl;
		
		CPPDEVTK_VERIFY(connect(pConsoleKitSession_.get(), SIGNAL(ActiveChanged(bool)), SLOT(OnActiveSessionChanged(bool))));
		CPPDEVTK_VERIFY(connect(pConsoleKitSession_.get(), SIGNAL(Locked()), SLOT(OnSessionLocked())));
		CPPDEVTK_VERIFY(connect(pConsoleKitSession_.get(), SIGNAL(Unlocked()), SLOT(OnSessionUnlocked())));
	}
	else {
		const util::LogindManager& kTheLogindManager = util::LogindManager::GetInstance();
		CPPDEVTK_COUT << "kTheLogindManager.CanPowerOff(): " << kTheLogindManager.CanPowerOff() << endl;
		CPPDEVTK_COUT << "kTheLogindManager.CanReboot(): " << kTheLogindManager.CanReboot() << endl;
		CPPDEVTK_COUT << "kTheLogindManager.CanSuspend(): " << kTheLogindManager.CanSuspend() << endl;
		CPPDEVTK_COUT << "kTheLogindManager.CanHibernate(): " << kTheLogindManager.CanHibernate() << endl;
		CPPDEVTK_COUT << "kTheLogindManager.CanHybridSleep(): " << kTheLogindManager.CanHybridSleep() << endl;
		
		pLogindSession_ = kTheLogindManager.GetCurrentSession();
		CPPDEVTK_COUT << "pLogindSession_->GetId(): " << pLogindSession_->GetId() << endl;
		CPPDEVTK_COUT << "pLogindSession_->GetUser(): " << pLogindSession_->GetUser() << endl;
		CPPDEVTK_COUT << "pLogindSession_->GetRemoteHost(): " << pLogindSession_->GetRemoteHost() << endl;
		CPPDEVTK_COUT << "pLogindSession_->IsActive(): " << pLogindSession_->IsActive() << endl;
		CPPDEVTK_COUT << "pLogindSession_->IsRemote(): " << pLogindSession_->IsRemote() << endl;
		
		CPPDEVTK_VERIFY(connect(pLogindSession_.get(), SIGNAL(ActiveChanged(bool)), SLOT(OnActiveSessionChanged(bool))));
		CPPDEVTK_VERIFY(connect(pLogindSession_.get(), SIGNAL(Locked()), SLOT(OnSessionLocked())));
		CPPDEVTK_VERIFY(connect(pLogindSession_.get(), SIGNAL(Unlocked()), SLOT(OnSessionUnlocked())));
	}
	
	CPPDEVTK_COUT << "Lock session or switch user to test session monitoring" << endl;
#	endif	// (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)
}

#if (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)

void Application::OnActiveSessionChanged(bool isActive) {
	CPPDEVTK_COUT << (isActive ? "session activated" : "session deactivated") << endl;
}

void Application::OnSessionLocked() {
	CPPDEVTK_COUT << "session locked" << endl;
}

void Application::OnSessionUnlocked() {
	CPPDEVTK_COUT << "session unlocked" << endl;
}

#endif


}	// namespace test_term_sig
}	// namespace cppdevtk
