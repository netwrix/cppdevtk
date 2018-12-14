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


#include <cppdevtk/gui/session.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#ifndef QT_MAC_USE_COCOA


#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>


namespace cppdevtk {
namespace gui {


Session::~Session() {
	DisposeEventHandlerUPP(eventHandlerUPP_);
}

Session::Session(): QObject(), id_(), eventHandlerUPP_(NULL) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	const SecuritySessionId kSecuritySessionId = GetCallerSecuritySessionId();
	id_ = QString::number(kSecuritySessionId);
	
	eventHandlerUPP_ = NewEventHandlerUPP(&OnUserSessionChanged);
	if (eventHandlerUPP_ == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("Failed to create event handler UPP");
	}
	
	const EventTypeSpec kEventTypeSpecs[] = {{kEventClassSystem, kEventSystemUserSessionActivated},
			{kEventClassSystem, kEventSystemUserSessionDeactivated}};
	const OSStatus kOSStatus = InstallApplicationEventHandler(eventHandlerUPP_, GetEventTypeCount(kEventTypeSpecs),
			kEventTypeSpecs, this, NULL);
	if (kOSStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(
				QString("Failed to install Session application event handler; kOSStatus: %1").arg(kOSStatus));
	}
}

OSStatus Session::OnUserSessionChanged(EventHandlerCallRef nextHandler, EventRef theEvent, void* pUserData) {
	base::SuppressUnusedWarning(nextHandler);
	CPPDEVTK_ASSERT(GetEventClass(theEvent) == kEventClassSystem);
	CPPDEVTK_ASSERT(pUserData != NULL);
	
	if (GetEventKind(theEvent) == kEventSystemUserSessionActivated) {
		//CPPDEVTK_LOG_DEBUG("got kEventSystemUserSessionActivated");
		Q_EMIT static_cast<Session*>(pUserData)->Activated();
	}
	else {
		CPPDEVTK_ASSERT(GetEventKind(theEvent) == kEventSystemUserSessionDeactivated);
		//CPPDEVTK_LOG_DEBUG("got kEventSystemUserSessionDeactivated");
		Q_EMIT static_cast<Session*>(pUserData)->Deactivated();
	}
	
	return noErr;
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// QT_MAC_USE_COCOA
