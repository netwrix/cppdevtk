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


#include <cppdevtk/gui/session.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif

#include <cppdevtk/util/dynamic_library.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/logger.hpp>
#if (MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_7)
#include <cppdevtk/base/cerrno.hpp>
#endif

#include <cstddef>
#include <stdexcept>

#include <ApplicationServices/ApplicationServices.h>


namespace cppdevtk {
namespace gui {
namespace detail {


#if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7)


// Verified and login framework was added in 10.7 and is still present in 10.13
// Being private framework we preffer dynamic loading instead of linking.
class LibLogin: public util::DynamicLibrary {
public:
	LibLogin();
	
	int SACLockScreenImmediate();	
private:
	typedef int (*TPFNSACLockScreenImmediate)(void);
	
	
	void LoadSymbols();
	
	
	TPFNSACLockScreenImmediate pFnSACLockScreenImmediate_;
};


inline LibLogin::LibLogin(): util::DynamicLibrary("/System/Library/PrivateFrameworks/login.framework/Versions/Current/login"),
		pFnSACLockScreenImmediate_(NULL) {
	LoadSymbols();
}

inline int LibLogin::SACLockScreenImmediate() {
	CPPDEVTK_ASSERT(pFnSACLockScreenImmediate_ != NULL);
	return pFnSACLockScreenImmediate_();
}

inline void LibLogin::LoadSymbols() {
	CPPDEVTK_ASSERT(pFnSACLockScreenImmediate_ == NULL);
	pFnSACLockScreenImmediate_ = (TPFNSACLockScreenImmediate)LoadSymbol("SACLockScreenImmediate");
	CPPDEVTK_ASSERT(pFnSACLockScreenImmediate_ != NULL);
}


#endif	// (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7)


}	// namespace detail


bool Session::Lock() try {
#	if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7)
	detail::LibLogin().SACLockScreenImmediate();
	return true;
#	else
	errno = ENOTSUP;
	return false;
#	endif
}
catch (const ::std::runtime_error& exc) {
	CPPDEVTK_LOG_ERROR("Lock() failed; exc: " << base::Exception::GetDetailedInfo(exc));
	return false;
}

Session::IdleTime Session::GetIdleTime() const {
	// TODO: Test on Mac OS X 10.4 Tiger
	// According to some articles CGEventSourceSecondsSinceLastEventType() does not work on 10.4.
	// If it does not work try:
	// - kIOHIDIdleTimeKey: http://www.danandcheryl.com/2010/06/how-to-check-the-system-idle-time-using-cocoa
	// - kEventLoopIdleTimerStarted + kEventLoopIdleTimerIdling
	return CGEventSourceSecondsSinceLastEventType(kCGEventSourceStateCombinedSessionState, kCGAnyInputEventType) * 1000;
}

SecuritySessionId Session::GetCallerSecuritySessionId() {
	SecuritySessionId securitySessionId = noSecuritySession;
	SessionAttributeBits sessionAttributeBits = 0;
	const OSStatus kSessionsApiRetCode = SessionGetInfo(callerSecuritySession, &securitySessionId, &sessionAttributeBits);
	if (kSessionsApiRetCode != errSessionSuccess) {
		if (kSessionsApiRetCode == errSessionAuthorizationDenied) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("SessionGetInfo() failed: errSessionAuthorizationDenied");
		}
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("SessionGetInfo() failed; kSessionsApiRetCode: %1").arg(kSessionsApiRetCode));
	}
	return securitySessionId;
}


}	// namespace gui
}	// namespace cppdevtk
