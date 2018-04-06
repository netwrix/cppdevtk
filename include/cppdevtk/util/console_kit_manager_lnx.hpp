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


#ifndef CPPDEVTK_UTIL_CONSOLE_KIT_MANAGER_LNX_HPP_INCLUDED_
#define CPPDEVTK_UTIL_CONSOLE_KIT_MANAGER_LNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include "console_kit_session_lnx.hpp"
#include <cppdevtk/base/singletons.hpp>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>

#include <memory>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/software/ConsoleKit/doc/ConsoleKit.html#Manager">org.freedesktop.ConsoleKit.Manager</a>
/// \note All functions, except slots and destructor, may throw DBusException
class CPPDEVTK_UTIL_API ConsoleKitManager: public ::cppdevtk::base::MeyersSingleton<ConsoleKitManager> {
	friend class ::cppdevtk::base::MeyersSingleton<ConsoleKitManager>;
public Q_SLOTS:
	bool Restart();
	bool Stop();
public:
	bool CanRestart() const;
	bool CanStop() const;
	
	::std::auto_ptr<ConsoleKitSession> GetCurrentSession() const;	///< \note Returned pointer is not NULL
	::std::auto_ptr<ConsoleKitSession> GetSessionForUnixProcess(uint pid) const;	///< \note Returned pointer is not NULL
	
	QDBusError GetLastError() const;
	
	
	static bool IsConsoleKitServiceRegistered();
private:
	ConsoleKitManager();
	~ConsoleKitManager();
	
	
	mutable QDBusInterface ckManagerInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline ConsoleKitManager::~ConsoleKitManager() {}

inline QDBusError ConsoleKitManager::GetLastError() const {
	return ckManagerInterface_.lastError();
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_CONSOLE_KIT_MANAGER_LNX_HPP_INCLUDED_
