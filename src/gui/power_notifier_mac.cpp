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


#include <cppdevtk/gui/power_notifier.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif

#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/verify.h>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>

#include <IOKit/pwr_mgt/IOPmLib.h>
#include <IOKit/IOMessage.h>
#include <CoreFoundation/CoreFoundation.h>


namespace cppdevtk {
namespace gui {


void PowerNotifier::Uninit() {
	if (spSession_ == IO_OBJECT_NULL) {
		return;
	}
	
	CFRunLoopRemoveSource(CFRunLoopGetCurrent(), IONotificationPortGetRunLoopSource(spIONotificationPortRef_),
			kCFRunLoopCommonModes);
	
	const IOReturn kIOReturnCode = IODeregisterForSystemPower(&spNotifierObject_);
	if (kIOReturnCode != kIOReturnSuccess) {
		CPPDEVTK_LOG_WARN("IODeregisterForSystemPower() failed (absorbing); kIOReturnCode: " << kIOReturnCode);
	}
	spNotifierObject_ = IO_OBJECT_NULL;
	
	IONotificationPortDestroy(spIONotificationPortRef_);
	spIONotificationPortRef_ = NULL;
	
	const kern_return_t kKernReturnCode = IOServiceClose(spSession_);
	if (kKernReturnCode != KERN_SUCCESS) {
		CPPDEVTK_LOG_WARN("IOServiceClose() failed (absorbing); kKernReturnCode: " << kKernReturnCode);
	}
	spSession_ = IO_OBJECT_NULL;
}

PowerNotifier::PowerNotifier(): QObject(), ::cppdevtk::base::MeyersSingleton<PowerNotifier>(), spSession_(IO_OBJECT_NULL),
		spIONotificationPortRef_(NULL), spNotifierObject_(IO_OBJECT_NULL) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	spSession_ = IORegisterForSystemPower(this, &spIONotificationPortRef_, &IOSystemPowerCallback, &spNotifierObject_);
	if (spSession_ == MACH_PORT_NULL) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(::cppdevtk::base::GetLastSystemErrorCode(),
				"Failed to register for system power");
	}
	
	CFRunLoopAddSource(CFRunLoopGetCurrent(), IONotificationPortGetRunLoopSource(spIONotificationPortRef_),
			kCFRunLoopCommonModes);
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

PowerNotifier::~PowerNotifier() {
	CPPDEVTK_ASSERT((spSession_ == IO_OBJECT_NULL) && "Call Uninit() before leaving main()");
}

void PowerNotifier::IOSystemPowerCallback(void* pRefcon, io_service_t service, uint32_t messageType,
		void* pMessageArgument) {
	base::SuppressUnusedWarning(service);
	
	switch (messageType) {
		case kIOMessageSystemWillSleep: {
				CPPDEVTK_ASSERT(pRefcon != NULL);
				PowerNotifier* pPowerNotifier = static_cast<PowerNotifier*>(pRefcon);
				
				Q_EMIT pPowerNotifier->Sleeping();
				
				const IOReturn kIOReturnCode = IOAllowPowerChange(pPowerNotifier->spSession_, (long)pMessageArgument);
				if (kIOReturnCode != kIOReturnSuccess) {
					CPPDEVTK_LOG_ERROR("IOAllowPowerChange() failed; kIOReturnCode: " << kIOReturnCode);
				}
			}
			break;
		case kIOMessageSystemHasPoweredOn:
			CPPDEVTK_ASSERT(pRefcon != NULL);
			Q_EMIT static_cast<PowerNotifier*>(pRefcon)->Resuming();
			break;
		default:
			break;
	}
}


}	// namespace gui
}	// namespace cppdevtk
