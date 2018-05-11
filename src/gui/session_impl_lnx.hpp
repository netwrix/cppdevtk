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


#ifndef CPPDEVTK_GUI_SESSION_IMPL_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_SESSION_IMPL_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/gui/session.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>


namespace cppdevtk {
namespace gui {


class Session::Impl: public QObject {
	Q_OBJECT
Q_SIGNALS:
	void ActiveChanged(bool isActive);
	void Activated();
	void Deactivated();
	
	void Locked();
	void Unlocked();
public Q_SLOTS:
	virtual bool Activate() = 0;
	virtual bool Lock() = 0;
	virtual bool Unlock() = 0;
public:
	virtual QString GetId() const = 0;
	virtual QString GetType() const = 0;
	virtual uint GetUser() const = 0;
	virtual QString GetRemoteHost() const = 0;
	
	virtual bool IsActive() const = 0;
	virtual bool IsRemote() const = 0;
	
	QDBusError GetLastError() const;
protected:
	Impl(const QString& service, const QString& path, const QString& interface);
	
	QDBusInterface& DBusInterfaceRef() const;
public Q_SLOTS:
	void OnActiveChanged(bool isActive);
private:
	Q_DISABLE_COPY(Impl);
	
	
	mutable QDBusInterface dbusInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QDBusError Session::Impl::GetLastError() const {
	return dbusInterface_.lastError();
}

inline QDBusInterface& Session::Impl::DBusInterfaceRef() const {
	return dbusInterface_;
}

inline void Session::Impl::OnActiveChanged(bool isActive) {
	if (isActive) {
		Q_EMIT Activated();
	}
	else {
		Q_EMIT Deactivated();
	}
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SESSION_IMPL_LNX_HPP_INCLUDED_
