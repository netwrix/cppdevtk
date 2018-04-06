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


#ifndef CPPDEVTK_UTIL_CONSOLE_KIT_SESSION_LNX_HPP_INCLUDED_
#define CPPDEVTK_UTIL_CONSOLE_KIT_SESSION_LNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusInterface>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/software/ConsoleKit/doc/ConsoleKit.html#Session">org.freedesktop.ConsoleKit.Session</a>
/// \note All functions, except slots and destructor, may throw DBusException
class CPPDEVTK_UTIL_API ConsoleKitSession: public QObject {
	friend class ConsoleKitManager;
	
	Q_OBJECT
Q_SIGNALS:
	void ActiveChanged(bool isActive);
	
	/// \attention Tested and Locked()/Unlocked() signals:
	/// - are not emitted when Lock/Unlock from DE (tested with KDE).
	/// - are emitted when Lock()/Unlock() method are called
	/// - verify: dbus-monitor --monitor --system "type='signal',sender='org.freedesktop.ConsoleKit',interface='org.freedesktop.ConsoleKit.Session',path='/org/freedesktop/ConsoleKit/Session2'"
	void Locked();
	void Unlocked();
public Q_SLOTS:
	bool Activate();
	bool Lock();
	bool Unlock();
public:
	QDBusObjectPath GetId() const;
	QString GetSessionType() const;
	uint GetUnixUser() const;
	QString GetRemoteHostName() const;
	QString GetLoginSessionId() const;
	
	bool IsActive() const;
	bool IsLocal() const;
	
	QDBusError GetLastError() const;
	
	bool operator==(const ConsoleKitSession& other) const;
	bool operator!=(const ConsoleKitSession& other) const;
private:
	Q_DISABLE_COPY(ConsoleKitSession)
	
	explicit ConsoleKitSession(const QDBusObjectPath& ckSessionPath);
	
	
	mutable QDBusInterface ckSessionInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QDBusError ConsoleKitSession::GetLastError() const {
	return ckSessionInterface_.lastError();
}

inline bool ConsoleKitSession::operator==(const ConsoleKitSession& other) const {
	return GetId().path() == other.GetId().path();
}

inline bool ConsoleKitSession::operator!=(const ConsoleKitSession& other) const {
	return GetId().path() != other.GetId().path();
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_CONSOLE_KIT_SESSION_LNX_HPP_INCLUDED_
