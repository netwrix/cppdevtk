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


#ifndef CPPDEVTK_GUI_GET_USER_IDLE_TIME_HPP_INCLUDED_
#define CPPDEVTK_GUI_GET_USER_IDLE_TIME_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_PLATFORM_UNIX)
	// Unix specific code
#	if (CPPDEVTK_PLATFORM_LINUX)
		// Linux specific code
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#		include <CoreFoundation/CFDate.h>
#	else
#		error "Unsupported Unix platform!!!"
#	endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include <windows.h>
#else
#	error "Unsupported platform!!!"
#endif


namespace cppdevtk {
namespace gui {


#if (CPPDEVTK_PLATFORM_UNIX)
// Unix specific code
#if (CPPDEVTK_PLATFORM_LINUX)
typedef unsigned long idle_time_t;
#elif (CPPDEVTK_PLATFORM_MACOSX)
typedef CFTimeInterval idle_time_t;
#else
#error "Unsupported Unix platform!!!"
#endif
#elif (CPPDEVTK_PLATFORM_WINDOWS)
typedef DWORD idle_time_t;
#else
#error "Unsupported platform!!!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return The time, in milliseconds, elapsed since the last input event.
/// \attention
/// - On Linux need to link with Xext and Xss
/// - On Windows need to link with user32.lib
/// - On Mac OS X need to link with framework ApplicationServices and framework Carbon
CPPDEVTK_GUI_API bool GetUserIdleTime(idle_time_t& userIdleTime);


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_GET_USER_IDLE_TIME_HPP_INCLUDED_
