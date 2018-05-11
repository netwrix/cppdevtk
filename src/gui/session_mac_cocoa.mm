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


#include <cppdevtk/gui/session.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#ifdef QT_MAC_USE_COCOA


#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QObject>

#include <cstddef>

#include <AppKit/AppKit.h>


@interface CocoaSession: NSObject {
@public
	::cppdevtk::gui::Session* pSession_;
}

@end


@implementation CocoaSession

-(id)init {
	self = [super init];
	if (self != nil) {
		pSession_ = NULL;
		
		[[[NSWorkspace sharedWorkspace] notificationCenter]
				addObserver:self
				selector:@selector(OnSessionDidBecomeActive:)
				name:NSWorkspaceSessionDidBecomeActiveNotification
				object:nil];
		
		[[[NSWorkspace sharedWorkspace] notificationCenter]
				addObserver:self
				selector:@selector(OnSessionDidResignActive:)
				name:NSWorkspaceSessionDidResignActiveNotification
				object:nil];
	}
	return self;
}

-(void)dealloc {
	[[[NSWorkspace sharedWorkspace] notificationCenter] removeObserver:self];
    [super dealloc];
}

-(void)OnSessionDidBecomeActive:(NSNotification*)pNSNotification {
	CPPDEVTK_ASSERT(pSession_ != NULL);
	Q_EMIT pSession_->Activated();
}

-(void)OnSessionDidResignActive:(NSNotification*)pNSNotification {
	CPPDEVTK_ASSERT(pSession_ != NULL);
	Q_EMIT pSession_->Deactivated();
}

@end


#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/stdexcept.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace cppdevtk {
namespace gui {


Session::~Session() {
	[pCocoaSession_ release];
}

Session::Session(): QObject(), id_(), pCocoaSession_(NULL) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	const SecuritySessionId kSecuritySessionId = GetCallerSecuritySessionId();
	id_ = QString::number(kSecuritySessionId);
	
	pCocoaSession_ = [[CocoaSession alloc] init];
	if (pCocoaSession_ == nil) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("Failed to create CocoaSession");
	}
	pCocoaSession_->pSession_ = this;
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// QT_MAC_USE_COCOA
