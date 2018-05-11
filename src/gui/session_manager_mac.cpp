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


#include <cppdevtk/gui/session_manager.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif

#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QString>

#include <cstddef>
#include <new>
#include <stdexcept>

#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>


namespace cppdevtk {
namespace gui {


using ::cppdevtk::base::Exception;
using ::std::runtime_error;


bool SessionManager::Logout() try {
	SendAppleEventToSystemProcess(kAEReallyLogOut);
	return true;
}
catch (const runtime_error& exc) {
	CPPDEVTK_LOG_ERROR("Logout() failed; exc: " << Exception::GetDetailedInfo(exc));
	return false;
}

bool SessionManager::Shutdown() try {
	SendAppleEventToSystemProcess(kAEShutDown);
	return true;
}
catch (const runtime_error& exc) {
	CPPDEVTK_LOG_ERROR("Shutdown() failed; exc: " << Exception::GetDetailedInfo(exc));
	return false;
}

SessionManager::IdleTime SessionManager::GetIdleTime() const {
	// TODO: Test on Mac OS X 10.4 Tiger
	// According to some articles CGEventSourceSecondsSinceLastEventType() does not work on 10.4.
	// If it does not work try:
	// - kIOHIDIdleTimeKey: http://www.danandcheryl.com/2010/06/how-to-check-the-system-idle-time-using-cocoa
	// - kEventLoopIdleTimerStarted + kEventLoopIdleTimerIdling
	return CGEventSourceSecondsSinceLastEventType(kCGEventSourceStateCombinedSessionState, kCGAnyInputEventType) * 1000;
}

::std::auto_ptr< ::cppdevtk::gui::Session> SessionManager::GetThisProcessSession() const {
	return ::std::auto_ptr<Session>(new Session());
}

SessionManager::SessionManager(): QObject(), ::cppdevtk::base::MeyersSingleton<SessionManager>() {}

SessionManager::~SessionManager() {}

void SessionManager::SendAppleEventToSystemProcess(AEEventID eventToSend) {
	const ProcessSerialNumber kPSNOfSystemProcess = {0, kSystemProcess};
	AEAddressDesc targetDesc;
	OSStatus osStatus = AECreateDesc(typeProcessSerialNumber, &kPSNOfSystemProcess, sizeof(kPSNOfSystemProcess), &targetDesc);
	if (osStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("AECreateDesc() failed; osStatus: %1").arg(osStatus));
	}
	
	AppleEvent appleEventToSend = {typeNull, NULL};
	osStatus = AECreateAppleEvent(kCoreEventClass, eventToSend, &targetDesc, kAutoGenerateReturnID, kAnyTransactionID,
			&appleEventToSend);
	AEDisposeDesc(&targetDesc);
	if (osStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("AECreateAppleEvent() failed; osStatus: %1").arg(osStatus));
	}
	
	AppleEvent replyEvent = {typeNull, NULL};
	osStatus = AESend(&appleEventToSend, &replyEvent, kAENoReply, kAENormalPriority, kAEDefaultTimeout, NULL, NULL);
	AEDisposeDesc(&appleEventToSend);
	if (osStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("AESend() failed; osStatus: %1").arg(osStatus));
	}
	AEDisposeDesc(&replyEvent);
}


}	// namespace gui
}	// namespace cppdevtk
