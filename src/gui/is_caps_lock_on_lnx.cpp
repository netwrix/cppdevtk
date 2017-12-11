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


#include <cppdevtk/gui/is_caps_lock_on.hpp>
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

#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>

#endif	// !ANDROID

#include <cstddef>
#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace gui {


CPPDEVTK_GUI_API bool IsCapsLockOn(bool& value) {
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	Display* pDisplay = QX11Info::display();
	CPPDEVTK_ASSERT(pDisplay != NULL);
	
	//XSynchronize(pDisplay, True);
	
	KeyCode keyCode = XKeysymToKeycode(pDisplay, XK_Caps_Lock);
	if(keyCode == NoSymbol) {
		CPPDEVTK_LOG_ERROR("XKeysymToKeycode() failed");
		return false;
	}
	
	CPPDEVTK_TR1_NS::shared_ptr<XModifierKeymap> pModifierKeymap(XGetModifierMapping(pDisplay), &XFreeModifiermap);
	if(pModifierKeymap.get() == NULL) {
		CPPDEVTK_LOG_ERROR("XGetModifierMapping() failed");
		return false;
	}
	
	int keyMask = 0;
	for (int i = 0; i < 8; ++i) {
		if (pModifierKeymap->modifiermap[pModifierKeymap->max_keypermod * i] == keyCode) {
			keyMask = 1 << i;
		}
	}
	
	Window root_return = None, child_return = None;
	int root_x_return = 0, root_y_return = 0, win_x_return = 0, win_y_return = 0;
	unsigned int mask = 0;
	if (XQueryPointer(pDisplay, DefaultRootWindow(pDisplay), &root_return, &child_return, &root_x_return, &root_y_return,
			&win_x_return, &win_y_return, &mask) != True) {
		CPPDEVTK_LOG_ERROR("XQueryPointer() failed");
		return false;
	}
	
	value = (mask & keyMask) != 0;
	
	return true;
	
#	else
	// TODO: Android port
	::cppdevtk::base::SuppressUnusedWarning(value);
	CPPDEVTK_COMPILER_WARNING("IsCapsLockOn(): Not ported on Android!");
	return false;
#	endif
}


}	// namespace gui
}	// namespace cppdevtk
