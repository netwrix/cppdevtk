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


#include <cppdevtk/gui/computer_manager.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QUrl>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#	if (CPPDEVTK_COMPILER_CLANG)
#		include <QtGui/QDesktopServices>
#	else
#		include <QtWidgets/QDesktopServices>
#	endif
#else
#	include <QtGui/QDesktopServices>
#endif

#if (!CPPDEVTK_PLATFORM_IOS)
#include <IOKit/IOKitLib.h>
#include <CoreServices/CoreServices.h>
#include <Carbon/Carbon.h>
#endif

#include <mach/port.h>
#include <mach/kern_return.h>

#include <cstddef>


namespace cppdevtk {
namespace gui {
namespace detail {

#if (!CPPDEVTK_PLATFORM_IOS)

static bool SendAppleEventToSystemProcess(AEEventID eventToSend);

#endif

}	// namespace detail


bool ComputerManager::ShutdownComputer() {
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	if (!detail::SendAppleEventToSystemProcess(kAEShutDown)) {
		CPPDEVTK_LOG_ERROR("failed to ShutdownComputer");		
		return false;
	}
	
	return true;
	
#	else
	// TODO: iOS port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::ShutdownComputer() not ported on iOS");
	return false;
#	endif
}

bool ComputerManager::LockComputer() {
	const QUrl kScreenSaverEngineUrl = QUrl::fromLocalFile(
			"/System/Library/Frameworks/ScreenSaver.framework/Resources/ScreenSaverEngine.app");
	CPPDEVTK_ASSERT(kScreenSaverEngineUrl.isValid());
	if (!QDesktopServices::openUrl(kScreenSaverEngineUrl)) {
		CPPDEVTK_LOG_ERROR("failed to start ScreenSaverEngine: " << kScreenSaverEngineUrl.toString());
		return false;
	}
	
	return true;
}

bool ComputerManager::LogoutUser() {
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	if (!detail::SendAppleEventToSystemProcess(kAEReallyLogOut)) {
		CPPDEVTK_LOG_ERROR("failed to LogoutUser");		
		return false;
	}
	
	return true;
	
#	else
	// TODO: iOS port
	CPPDEVTK_COMPILER_WARNING("ComputerManager::LogoutUser() not ported on iOS");
	return false;
#	endif
}


namespace detail {


#if (!CPPDEVTK_PLATFORM_IOS)

bool SendAppleEventToSystemProcess(AEEventID eventToSend) {
	const ProcessSerialNumber kPSNOfSystemProcess = {0, kSystemProcess};
	AEAddressDesc targetDesc;
	OSStatus osStatus = AECreateDesc(typeProcessSerialNumber, &kPSNOfSystemProcess, sizeof(kPSNOfSystemProcess), &targetDesc);
	if (osStatus != noErr) {
		CPPDEVTK_LOG_ERROR("AECreateDesc() failed");
		return false;
	}
	
	AppleEvent appleEventToSend = {typeNull, NULL};
	osStatus = AECreateAppleEvent(kCoreEventClass, eventToSend, &targetDesc, kAutoGenerateReturnID, kAnyTransactionID,
			&appleEventToSend);
	AEDisposeDesc(&targetDesc);
	if (osStatus != noErr) {
		CPPDEVTK_LOG_ERROR("AECreateAppleEvent() failed");
		return false;
	}
	
	AppleEvent eventReply = {typeNull, NULL};
	osStatus = AESend(&appleEventToSend, &eventReply, kAENoReply, kAENormalPriority, kAEDefaultTimeout, NULL, NULL);
	AEDisposeDesc(&eventReply);
	AEDisposeDesc(&appleEventToSend);
	if (osStatus != noErr) {
		CPPDEVTK_LOG_ERROR("AESend() failed");
		return false;
	}
	
	return true;
}

#endif

}	// namespace detail
}	// namespace gui
}	// namespace cppdevtk
