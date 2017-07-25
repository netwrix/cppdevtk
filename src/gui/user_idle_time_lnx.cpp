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
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QtGlobal>

#if (!CPPDEVTK_PLATFORM_ANDROID)

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtX11Extras/QX11Info>
#else
#include <QtGui/QX11Info>
#endif

#include <X11/extensions/scrnsaver.h>

#endif	// !ANDROID

#include <cstddef>
#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace gui {


CPPDEVTK_GUI_API bool GetUserIdleTime(idle_time_t& userIdleTime) {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	Display* pDisplay = QX11Info::display();
	CPPDEVTK_ASSERT(pDisplay != NULL);
	
	//XSynchronize(pDisplay, True);
	
	int eventBaseReturn = 0;
	int errorBaseReturn = 0;
	if (!XScreenSaverQueryExtension(pDisplay, &eventBaseReturn, &errorBaseReturn)) {
		CPPDEVTK_LOG_ERROR("XScreenSaver extension is not available");
		return false;
	}
	
	CPPDEVTK_TR1_NS::shared_ptr<XScreenSaverInfo> pScreenSaverInfo(XScreenSaverAllocInfo(), &XFree);
	if (pScreenSaverInfo.get() == NULL) {
		CPPDEVTK_LOG_ERROR("XScreenSaverAllocInfo() failed (not enough memory)");
		return false;
	}
	
	if (XScreenSaverQueryInfo(pDisplay, QX11Info::appRootWindow(), pScreenSaverInfo.get()) == 0) {
		CPPDEVTK_LOG_ERROR("XScreenSaver extension is not supported");
		return false;
	}
	
	userIdleTime = pScreenSaverInfo->idle;
	
	return true;
	
#	else
	// TODO: Android port
	::cppdevtk::base::SuppressUnusedWarning(userIdleTime);
	CPPDEVTK_COMPILER_WARNING("GetUserIdleTime(): Not ported on Android!");
	return false;
#	endif
}


}	// namespace gui
}	// namespace cppdevtk
