/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_GUI_LOGIND_MANAGER_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_LOGIND_MANAGER_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include "session_manager_impl_lnx.hpp"
#include <cppdevtk/base/get_current_process_id.hpp>


namespace cppdevtk {
namespace gui {
namespace detail {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/wiki/Software/systemd/logind">logind Manager</a>
/// \note All functions, except slots and destructor, may throw DBusException
class LogindManager: public SessionManager::Impl {
	friend class ::cppdevtk::gui::SessionManager;
public:
	virtual bool Shutdown();
	virtual ::std::auto_ptr< ::cppdevtk::gui::Session> GetCurrentProcessSession() const;
	
	
	static bool IsLogindServiceRegistered();
private:
	LogindManager();
	
	bool PowerOff(bool interactive);
	// Fails on latest Gnome (Ubuntu 18.04, openSUSE Tumbleweed) with org.freedesktop.login1.NoSessionForPID
	// TODO: report bug
	::std::auto_ptr<Session> GetSessionByPID(uint pid) const;	///< \note Returned pointer is not NULL
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline bool LogindManager::Shutdown() {
	if (!SessionManager::Impl::Shutdown()) {
		return PowerOff(false);
	}
	return true;
}

inline ::std::auto_ptr< ::cppdevtk::gui::Session> LogindManager::GetCurrentProcessSession() const {
	return GetSessionByPID(::cppdevtk::base::GetCurrentProcessId());
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_LOGIND_MANAGER_LNX_HPP_INCLUDED_
