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


#include <cppdevtk/gui/screensaver.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#ifdef QT_MAC_USE_COCOA


#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.h>

#include <cstddef>


@interface CocoaScreenSaver: NSObject {
@public
	::cppdevtk::gui::ScreenSaver* pScreenSaver_;
}

@end


@implementation CocoaScreenSaver

-(id)init {
	self = [super init];
	if (self != nil) {
		pScreenSaver_ = NULL;
		
		NSDistributedNotificationCenter* pDefaultCenter = [NSDistributedNotificationCenter defaultCenter];
		[pDefaultCenter addObserver:self
				selector:@selector(OnScreenSaverDidStart:)
				name:@"com.apple.screensaver.didstart"
				object:nil];
		[pDefaultCenter addObserver:self
				selector:@selector(OnScreenSaverDidStop:)
				name:@"com.apple.screensaver.didstop"
				object:nil];
	}
	return self;
}

-(void)dealloc {
	[[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
	[super dealloc];
}

-(void)OnScreenSaverDidStart:(NSNotification*)pNSNotification {
	CPPDEVTK_UNUSED(pNSNotification);
	CPPDEVTK_ASSERT(pScreenSaver_ != NULL);
	pScreenSaver_->OnScreenSaverDidStart();
}

-(void)OnScreenSaverDidStop:(NSNotification*)pNSNotification {
	CPPDEVTK_UNUSED(pNSNotification);
	CPPDEVTK_ASSERT(pScreenSaver_ != NULL);
	pScreenSaver_->OnScreenSaverDidStop();
}

@end


#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace cppdevtk {
namespace gui {


void ScreenSaver::Uninit() {
	[pCocoaScreenSaver_ release];
	pCocoaScreenSaver_ = NULL;
}

void ScreenSaver::OnScreenSaverDidStart() {
	bool wasActive = isActive_;
	isActive_ = true;
	CheckActiveChanged(wasActive);
}

void ScreenSaver::OnScreenSaverDidStop() {
	bool wasActive = isActive_;
	isActive_ = false;
	CheckActiveChanged(wasActive);
}

ScreenSaver::ScreenSaver(): QObject(), ::cppdevtk::base::MeyersSingleton<ScreenSaver>(), isActive_(IsActive()),
		restore_(false), askForPassword_(GetAskForPassword()), askForPasswordDelay_(GetAskForPasswordDelay()),
		pCocoaScreenSaver_(NULL) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	//CPPDEVTK_LOG_DEBUG("askForPassword_: " << askForPassword_);
	//CPPDEVTK_LOG_DEBUG("askForPasswordDelay_: " << askForPasswordDelay_);
	
	pCocoaScreenSaver_ = [[CocoaScreenSaver alloc] init];
	if (pCocoaScreenSaver_ == nil) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("Failed to create CocoaScreenSaver");
	}
	pCocoaScreenSaver_->pScreenSaver_ = this;
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

ScreenSaver::~ScreenSaver() {
	CPPDEVTK_ASSERT((pCocoaScreenSaver_ == NULL) && "Call Uninit() before leaving main()");
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// QT_MAC_USE_COCOA
