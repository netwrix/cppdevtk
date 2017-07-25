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


#ifndef CPPDEVTK_UTIL_FILESYSTEM_UTILS_WIN_HPP_INCLUDED_
#define CPPDEVTK_UTIL_FILESYSTEM_UTILS_WIN_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <windows.h>


// 1. windef.h:
//#define MAX_PATH 260
// there is no MAX_NAME
// 2. limits.h (guarded by undocumented #ifdef _POSIX_):
//#define _POSIX_NAME_MAX 14
//#define _POSIX_PATH_MAX 255
//#define NAME_MAX 255
//#define PATH_MAX 512
// no comments...
// We choose to use MAX_PATH
// NOTE: MAX_PATH includes the terminating null character:
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247%28v=vs.85%29.aspx
#define CPPDEVTK_NAME_MAX (MAX_PATH - 1)
#define CPPDEVTK_PATH_MAX MAX_PATH


namespace cppdevtk {
namespace util {


typedef HFILE fhandle_t;


static const fhandle_t kInvalidFileHandle = HFILE_ERROR;


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_FILESYSTEM_UTILS_WIN_HPP_INCLUDED_
