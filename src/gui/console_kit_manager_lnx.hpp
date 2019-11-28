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


#ifndef CPPDEVTK_GUI_CONSOLE_KIT_MANAGER_LNX_HPP_INCLUDED_
#define CPPDEVTK_GUI_CONSOLE_KIT_MANAGER_LNX_HPP_INCLUDED_


#include <cppdevtk/gui/config.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include "session_manager_impl_lnx.hpp"
#include <cppdevtk/base/get_current_process_id.hpp>

#include <QtCore/QString>


namespace cppdevtk {
namespace gui {
namespace detail {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/software/ConsoleKit/doc/ConsoleKit.html#Manager">org.freedesktop.ConsoleKit.Manager</a>
/// \note All functions, except slots and destructor, may throw DBusException
class CPPDEVTK_UTIL_API ConsoleKitManager: public SessionManager::Impl {
	friend class ::cppdevtk::gui::SessionManager;
public:
	virtual bool Shutdown();
	virtual ::std::auto_ptr< ::cppdevtk::gui::Session> GetCurrentProcessSession() const;
	
	
	static bool IsConsoleKitServiceRegistered();
private:
	ConsoleKitManager();
	
	bool Stop();
	
	::std::auto_ptr<Session> GetSessionForUnixProcess(uint pid) const;	///< \note Returned pointer is not NULL
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline bool ConsoleKitManager::Shutdown() {
	if (!SessionManager::Impl::Shutdown()) {
		return Stop();
	}
	return true;
}

inline ::std::auto_ptr< ::cppdevtk::gui::Session> ConsoleKitManager::GetCurrentProcessSession() const {
	return GetSessionForUnixProcess(::cppdevtk::base::GetCurrentProcessId());
}


}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_CONSOLE_KIT_MANAGER_LNX_HPP_INCLUDED_
