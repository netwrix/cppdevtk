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


#ifndef CPPDEVTK_UTIL_LOGIND_MANAGER_LNX_HPP_INCLUDED_
#define CPPDEVTK_UTIL_LOGIND_MANAGER_LNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif

#include "logind_session_lnx.hpp"
#include <cppdevtk/base/singletons.hpp>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>
#include <QtCore/QString>

#include <memory>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \sa <a href=https://www.freedesktop.org/wiki/Software/systemd/logind">logind Manager</a>
/// \note All functions, except slots and destructor, may throw DBusException
class CPPDEVTK_UTIL_API LogindManager: public ::cppdevtk::base::MeyersSingleton<LogindManager> {
	friend class ::cppdevtk::base::MeyersSingleton<LogindManager>;
Q_SIGNALS:
	void PrepareForShutdown(bool start);
	void PrepareForSleep(bool start);
public Q_SLOTS:
	bool LockSession(const QString& sessionId);
	bool UnlockSession(const QString& sessionId);
	bool TerminateSession(const QString& sessionId);
	bool ActivateSession(const QString& sessionId);
	
	bool PowerOff(bool interactive);
	bool Reboot(bool interactive);
	bool Suspend(bool interactive);
	bool Hibernate(bool interactive);
	bool HybridSleep(bool interactive);
public:
	::std::auto_ptr<LogindSession> GetCurrentSession() const;	///< \note Returned pointer is not NULL
	::std::auto_ptr<LogindSession> GetSession(const QString& sessionId) const;	///< \note Returned pointer is not NULL
	::std::auto_ptr<LogindSession> GetSessionByPID(uint pid) const;	///< \note Returned pointer is not NULL
	
	QString CanPowerOff() const;
	QString CanReboot() const;
	QString CanSuspend() const;
	QString CanHibernate() const;
	QString CanHybridSleep() const;
	
	QDBusError GetLastError() const;
	
	
	static bool IsLogindServiceRegistered();
private:
	LogindManager();
	~LogindManager();
	
	
	mutable QDBusInterface logindManagerInterface_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline LogindManager::~LogindManager() {}

inline QDBusError LogindManager::GetLastError() const {
	return logindManagerInterface_.lastError();
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_LOGIND_MANAGER_LNX_HPP_INCLUDED_
