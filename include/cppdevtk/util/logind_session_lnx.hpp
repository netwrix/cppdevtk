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


#ifndef CPPDEVTK_UTIL_LOGIND_SESSION_LNX_HPP_INCLUDED_
#define CPPDEVTK_UTIL_LOGIND_SESSION_LNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusInterface>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/wiki/Software/systemd/logind">logind Session</a>
/// \note All functions, except slots and destructor, may throw DBusException
class CPPDEVTK_UTIL_API LogindSession: public QObject {
	friend class LogindManager;
	
	Q_OBJECT
Q_SIGNALS:
	void ActiveChanged(bool isActive);	///< \note This signal that is in ConsoleKit is not in logind, so we implemented it.
	
	/// \attention Tested and Locked()/Unlocked() signals:
	/// - are not emitted when Lock/Unlock from DE (tested with KDE).
	/// - are emitted when Lock()/Unlock() method are called
	/// - verify: dbus-monitor --monitor --system "type='signal',sender='org.freedesktop.login1',interface='org.freedesktop.login1.Session',path='/org/freedesktop/login1/session/_32'"
	void Locked();
	void Unlocked();
public Q_SLOTS:
	bool Terminate();
	bool Activate();
	bool Lock();
	bool Unlock();
public:
	QString GetId() const;
	QString GetType() const;
	uint GetUser() const;
	QString GetRemoteHost() const;
	
	bool IsActive() const;
	bool IsRemote() const;
	
	QDBusError GetLastError() const;
	
	bool operator==(const LogindSession& other) const;
	bool operator!=(const LogindSession& other) const;
private Q_SLOTS:
	void DBusPropertiesChangedHandler(const QString& interfaceName, const QVariantMap& changedProperties,
			const QStringList& invalidatedProperties);
private:
	Q_DISABLE_COPY(LogindSession)
	
	explicit LogindSession(const QDBusObjectPath& logindSessionPath);
	
	QVariant GetProperty(const QString& propertyName) const;
	
	
	mutable QDBusInterface logindSessionInterface_;
	mutable QDBusInterface logindSessionPropertiesInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QString LogindSession::GetId() const {
	return GetProperty("Id").toString();
}

inline QString LogindSession::GetType() const {
	return GetProperty("Type").toString();
}

inline uint LogindSession::GetUser() const {
	return GetProperty("User").toUInt();
}

inline QString LogindSession::GetRemoteHost() const {
	return GetProperty("RemoteHost").toString();
}

inline bool LogindSession::IsActive() const {
	return GetProperty("Active").toBool();
}

inline bool LogindSession::IsRemote() const {
	return GetProperty("Remote").toBool();
}

inline QDBusError LogindSession::GetLastError() const {
	return logindSessionInterface_.lastError();
}

inline bool LogindSession::operator==(const LogindSession& other) const {
	return GetId() == other.GetId();
}

inline bool LogindSession::operator!=(const LogindSession& other) const {
	return GetId() != other.GetId();
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_LOGIND_SESSION_LNX_HPP_INCLUDED_
