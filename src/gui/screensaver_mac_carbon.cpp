/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#include <cppdevtk/gui/screensaver.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#ifndef QT_MAC_USE_COCOA


#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/verify.h>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>


namespace cppdevtk {
namespace gui {


void ScreenSaver::Uninit() {
	if (eventHandlerUPP_ != NULL) {
		DisposeEventHandlerUPP(eventHandlerUPP_);
		eventHandlerUPP_ = NULL;
	}
}

ScreenSaver::ScreenSaver(): QObject(), ::cppdevtk::base::MeyersSingleton<ScreenSaver>(), isActive_(IsActive()),
		restore_(false), askForPassword_(GetAskForPassword()), askForPasswordDelay_(GetAskForPasswordDelay()),
		eventHandlerUPP_(NULL) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	//CPPDEVTK_LOG_DEBUG("askForPassword_: " << askForPassword_);
	//CPPDEVTK_LOG_DEBUG("askForPasswordDelay_: " << askForPasswordDelay_);
	
	eventHandlerUPP_ = NewEventHandlerUPP(&OnAppFrontSwitched);
	if (eventHandlerUPP_ == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("Failed to create event handler UPP");
	}
	
	const EventTypeSpec kEventTypeSpec = {kEventClassApplication, kEventAppFrontSwitched};
	const OSStatus kOSStatus = InstallApplicationEventHandler(eventHandlerUPP_, 1, &kEventTypeSpec, this, NULL);
	if (kOSStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(
				QString("Failed to install screensaver application event handler; kOSStatus: %1").arg(kOSStatus));
	}
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

ScreenSaver::~ScreenSaver() {
	CPPDEVTK_ASSERT((eventHandlerUPP_ == NULL) && "Call Uninit() before leaving main()");
}

OSStatus ScreenSaver::OnAppFrontSwitched(EventHandlerCallRef nextHandler, EventRef theEvent, void* pUserData) {
	base::SuppressUnusedWarning(nextHandler);
	CPPDEVTK_ASSERT(GetEventClass(theEvent) == kEventClassApplication);
	CPPDEVTK_ASSERT(GetEventKind(theEvent) == kEventAppFrontSwitched);
	CPPDEVTK_ASSERT(pUserData != NULL);
	
	//CPPDEVTK_LOG_DEBUG("AppFrontSwitched");
	
	static_cast<ScreenSaver*>(pUserData)->Refresh();
	
	return noErr;
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// QT_MAC_USE_COCOA
