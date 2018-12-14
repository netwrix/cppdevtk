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


#ifndef CPPDEVTK_UTIL_DYNAMIC_LIBRARY_HPP_INCLUDED_
#define CPPDEVTK_UTIL_DYNAMIC_LIBRARY_HPP_INCLUDED_


#include "config.hpp"
#include "dynamic_loader_exception.hpp"
#include <cppdevtk/base/non_copyable.hpp>
#if (CPPDEVTK_PLATFORM_UNIX)
#	include <dlfcn.h>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#	include <windows.h>
#else
#	error "Unsupported platform!!!"
#endif

#include <QtCore/QString>


namespace cppdevtk {
namespace util {


class CPPDEVTK_UTIL_API DynamicLibrary: private ::cppdevtk::base::NonCopyable {
public:
#	if (CPPDEVTK_PLATFORM_UNIX)
	typedef void* Handle;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	typedef HMODULE Handle;
#	else
#	error "Unsupported platform!!!"
#	endif
	
	
	bool GetAutoUnload() const;
	void SetAutoUnload(bool value);	///< Default value is \c true if not set.
protected:
#	if (CPPDEVTK_PLATFORM_UNIX)
	typedef void* SymbolAddress;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	typedef FARPROC SymbolAddress;
#	else
#	error "Unsupported platform!!!"
#	endif
	
	
	DynamicLibrary(const QString& dynLibName);
	virtual ~DynamicLibrary() = 0;	///< \remark Unload library if \c autoUnload is \c true (otherwise OS will unload when exit()).
	
	SymbolAddress LoadSymbol(const QString& symbolName);
private:
	Handle handle_;
	bool autoUnload_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline bool DynamicLibrary::GetAutoUnload() const {
	return autoUnload_;
}

inline void DynamicLibrary::SetAutoUnload(bool value) {
	autoUnload_ = value;
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_DYNAMIC_LIBRARY_HPP_INCLUDED_
