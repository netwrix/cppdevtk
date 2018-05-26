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


#ifndef CPPDEVTK_GUI_CONSOLE_KIT_SESSION_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_CONSOLE_KIT_SESSION_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include "session_impl_lnx.hpp"

#include <QtDBus/QDBusObjectPath>


namespace cppdevtk {
namespace gui {
namespace detail {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/software/ConsoleKit/doc/ConsoleKit.html#Session">org.freedesktop.ConsoleKit.Session</a>
/// \note All functions, except slots and destructor, may throw DBusException
/// \attention Tested and Locked()/Unlocked() signals:
/// - are not emitted when Lock/Unlock from DE (tested with KDE); screensaver signals are emitted.
/// - are emitted when Lock()/Unlock() method are called
/// - verify: dbus-monitor --monitor --system "type='signal',sender='org.freedesktop.ConsoleKit',interface='org.freedesktop.ConsoleKit.Session',path='/org/freedesktop/ConsoleKit/Session2'"
class ConsoleKitSession: public Session::Impl {
	friend class ::cppdevtk::gui::Session;
	friend class ConsoleKitManager;
	
	Q_OBJECT
public:
	virtual ~ConsoleKitSession();
	
	virtual bool Activate();
	virtual bool Lock();
	virtual bool Unlock();
	
	virtual QString GetId() const;
	virtual bool GetIdleHint() const;
	virtual Session::IdleTime GetIdleSinceHint() const;
	virtual QString GetType() const;
	virtual uint GetUser() const;
	virtual QString GetRemoteHost() const;
	
	virtual bool IsActive() const;
	virtual bool IsRemote() const;
	
	bool operator==(const ConsoleKitSession& other) const;
	bool operator!=(const ConsoleKitSession& other) const;
	
	static bool IsConsoleKitServiceRegistered();
private:
	Q_DISABLE_COPY(ConsoleKitSession)
	
	explicit ConsoleKitSession(const QDBusObjectPath& ckSessionPath);
	
	QString DoGetSystemIdleSinceHint() const;
	QString GetSessionType() const;
	uint GetUnixUser() const;
	QString GetRemoteHostName() const;
	bool IsLocal() const;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QString ConsoleKitSession::GetType() const {
	return GetSessionType();
}

inline uint ConsoleKitSession::GetUser() const {
	return GetUnixUser();
}

inline QString ConsoleKitSession::GetRemoteHost() const {
	return GetRemoteHostName();
}

inline bool ConsoleKitSession::IsRemote() const {
	return !IsLocal();
}

inline bool ConsoleKitSession::operator==(const ConsoleKitSession& other) const {
	return GetId() == other.GetId();
}

inline bool ConsoleKitSession::operator!=(const ConsoleKitSession& other) const {
	return GetId() != other.GetId();
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_CONSOLE_KIT_SESSION_LNX_HPP_INCLUDED_
