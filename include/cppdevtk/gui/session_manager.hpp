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


#ifndef CPPDEVTK_GUI_SESSION_MANAGER_HPP_INCLUDED_
#define CPPDEVTK_GUI_SESSION_MANAGER_HPP_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif
#if (CPPDEVTK_PLATFORM_IOS)
#	error "This file is not for iOS!!!"
#endif

#include "session.hpp"
#include <cppdevtk/base/singletons.hpp>

#include <QtCore/QObject>

#if (CPPDEVTK_PLATFORM_LINUX)
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#include <windows.h>
#elif (CPPDEVTK_PLATFORM_MACOSX)
#include <CoreServices/CoreServices.h>
#else
#error "Unsupported platform!!!"
#endif

#include <memory>


namespace cppdevtk {
namespace gui {
#if (CPPDEVTK_PLATFORM_LINUX)
namespace detail {


class LogindManager;
class ConsoleKitManager;


}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note On Linux KDE(KSM)/Gnome/XFCE/LXDE/LXQT/Logind/ConsoleKit session managers are supported
class CPPDEVTK_GUI_API SessionManager: public QObject, public ::cppdevtk::base::MeyersSingleton<SessionManager> {
	friend class ::cppdevtk::base::MeyersSingleton<SessionManager>;
#	if (CPPDEVTK_PLATFORM_LINUX)
	friend class ::cppdevtk::gui::detail::LogindManager;
	friend class ::cppdevtk::gui::detail::ConsoleKitManager;
#	endif
	
	Q_OBJECT
public Q_SLOTS:
	bool Logout();
	bool Shutdown();
public:
	::std::auto_ptr< ::cppdevtk::gui::Session> GetCurrentProcessSession() const;
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	static bool IsSessionManagerServiceRegistered();
#	endif
private:
#	if (CPPDEVTK_PLATFORM_LINUX)
#	if (__GNUC__ < 5)
	// workaround: although we declared LogindManager and ConsoleKitManager friends we get:
	// error: class cppdevtk::gui::SessionManager::Impl is private
	public:
#	endif
	class Impl;
#	if (__GNUC__ < 5)
	private:
#	endif
#	endif
	
	
	Q_DISABLE_COPY(SessionManager);
	
	SessionManager();
	~SessionManager();
	
	
#	if (CPPDEVTK_PLATFORM_LINUX)	
	::std::auto_ptr<Impl> pImpl_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	static void EnableShutdownPrivilege();
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	static void SendAppleEventToSystemProcess(AEEventID eventToSend);
#	else
#	error "Unsupported platform!!!"
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SESSION_MANAGER_HPP_INCLUDED_
