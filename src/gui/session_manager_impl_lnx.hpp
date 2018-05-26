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


#ifndef CPPDEVTK_GUI_SESSION_MANAGER_IMPL_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_SESSION_MANAGER_IMPL_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/gui/session_manager.hpp>
#include <cppdevtk/base/non_copyable.hpp>

#include <QtCore/QString>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>


namespace cppdevtk {
namespace gui {


class SessionManager::Impl: private ::cppdevtk::base::NonCopyable {
public:
	virtual ~Impl();
	
	virtual bool Logout();
	virtual bool Shutdown();
	
	virtual ::std::auto_ptr< ::cppdevtk::gui::Session> GetCurrentProcessSession() const = 0;
	
	QDBusError GetLastError() const;
protected:
	Impl(const QString& service, const QString& path, const QString& interface);
	
	QDBusInterface& DBusInterfaceRef() const;
private:
	static bool KdeSmLogout();
	static bool GnomeSmLogout();
	static bool XfceSmLogout();
	static bool LxdeSmLogout();
	static bool LxqtSmLogout();
	
	static bool KdeSmShutdown();
	static bool GnomeSmRequestShutdown();
	static bool GnomeSmShutdown();
	static bool XfceSmShutdown();
	static bool LxdeSmRequestShutdown();
	static bool LxdeSmShutdown();
	static bool LxqtSmShutdown();
	
	
	mutable QDBusInterface dbusInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline SessionManager::Impl::~Impl() {}

inline QDBusError SessionManager::Impl::GetLastError() const {
	return dbusInterface_.lastError();
}

inline QDBusInterface& SessionManager::Impl::DBusInterfaceRef() const {
	return dbusInterface_;
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SESSION_MANAGER_IMPL_LNX_HPP_INCLUDED_
