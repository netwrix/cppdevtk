/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
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


#include <cppdevtk/gui/user_idle_time.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif
#include <cppdevtk/base/unused.hpp>

#if (!CPPDEVTK_PLATFORM_IOS)
#	include <ApplicationServices/ApplicationServices.h>
#endif


namespace cppdevtk {
namespace gui {


// TODO: Test on Mac OS X 10.4 Tiger
// According to some articles CGEventSourceSecondsSinceLastEventType() does not work on 10.4.
// If it does not work try:
// - kIOHIDIdleTimeKey: http://www.danandcheryl.com/2010/06/how-to-check-the-system-idle-time-using-cocoa
// - kEventLoopIdleTimerStarted + kEventLoopIdleTimerIdling
CPPDEVTK_GUI_API bool GetUserIdleTime(idle_time_t& userIdleTime) {
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	userIdleTime = CGEventSourceSecondsSinceLastEventType(
			kCGEventSourceStateCombinedSessionState, kCGAnyInputEventType) * 1000;
	
	return true;
	
#	else
	// TODO: iOS port
	::cppdevtk::base::SuppressUnusedWarning(userIdleTime);
	CPPDEVTK_COMPILER_WARNING("GetUserIdleTime() not ported on iOS");
	return false;
#	endif
}


}	// namespace gui
}	// namespace cppdevtk
