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


#ifndef CPPDEVTK_GUI_LOGIND_SESSION_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_LOGIND_SESSION_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include "session_impl_lnx.hpp"
#include <cppdevtk/util/dbus_utils.hpp>

#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusInterface>


namespace cppdevtk {
namespace gui {
namespace detail {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/wiki/Software/systemd/logind">logind Session</a>
/// \note All functions, except slots and destructor, may throw DBusException
/// \attention Tested and Locked()/Unlocked() signals:
/// - are not emitted when Lock/Unlock from DE (tested with KDE); screensaver signals are emitted.
/// - are emitted when Lock()/Unlock() method are called
/// - verify: dbus-monitor --monitor --system "type='signal',sender='org.freedesktop.login1',interface='org.freedesktop.login1.Session',path='/org/freedesktop/login1/session/_32'"
class LogindSession: public Session::Impl {
	friend class ::cppdevtk::gui::Session;
	friend class LogindManager;
	
	Q_OBJECT
public:
	virtual ~LogindSession();
	
	virtual bool Activate();
	// On Ubuntu 14.04 fails with org.freedesktop.DBus.Error.AccessDenied
	// TODO: report bug
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
	
	bool operator==(const LogindSession& other) const;
	bool operator!=(const LogindSession& other) const;
	
	
	static bool IsLogindServiceRegistered();
private Q_SLOTS:
	void OnDBusPropertiesChanged(const QString& interfaceName, const QVariantMap& changedProperties,
			const QStringList& invalidatedProperties);
private:
	Q_DISABLE_COPY(LogindSession)
	
	explicit LogindSession(const QDBusObjectPath& logindSessionPath);
	
	qulonglong DoGetIdleSinceHint() const;
	
	mutable QDBusInterface logindSessionPropertiesInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QString LogindSession::GetId() const {
	const QDBusInterface& kDBusInterface = DBusInterfaceRef();
	return util::GetDBusStringProperty(kDBusInterface.service(), QDBusObjectPath(kDBusInterface.path()),
			kDBusInterface.interface(), kDBusInterface.connection(), "Id");
}

inline QString LogindSession::GetType() const {
	const QDBusInterface& kDBusInterface = DBusInterfaceRef();
	return util::GetDBusStringProperty(kDBusInterface.service(), QDBusObjectPath(kDBusInterface.path()),
			kDBusInterface.interface(), kDBusInterface.connection(), "Type");
}

inline uint LogindSession::GetUser() const {
	const QDBusInterface& kDBusInterface = DBusInterfaceRef();
	return util::GetDBusUInt32Property(kDBusInterface.service(), QDBusObjectPath(kDBusInterface.path()),
			kDBusInterface.interface(), kDBusInterface.connection(), "User");
}

inline QString LogindSession::GetRemoteHost() const {
	const QDBusInterface& kDBusInterface = DBusInterfaceRef();
	return util::GetDBusStringProperty(kDBusInterface.service(), QDBusObjectPath(kDBusInterface.path()),
			kDBusInterface.interface(), kDBusInterface.connection(), "RemoteHost");
}

inline bool LogindSession::IsActive() const {
	const QDBusInterface& kDBusInterface = DBusInterfaceRef();
	return util::GetDBusBooleanProperty(kDBusInterface.service(), QDBusObjectPath(kDBusInterface.path()),
			kDBusInterface.interface(), kDBusInterface.connection(), "Active");
}

inline bool LogindSession::IsRemote() const {
	const QDBusInterface& kDBusInterface = DBusInterfaceRef();
	return util::GetDBusBooleanProperty(kDBusInterface.service(), QDBusObjectPath(kDBusInterface.path()),
			kDBusInterface.interface(), kDBusInterface.connection(), "Remote");
}

inline bool LogindSession::operator==(const LogindSession& other) const {
	return GetId() == other.GetId();
}

inline bool LogindSession::operator!=(const LogindSession& other) const {
	return GetId() != other.GetId();
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_LOGIND_SESSION_LNX_HPP_INCLUDED_
