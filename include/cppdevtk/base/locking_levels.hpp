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


#ifndef CPPDEVTK_BASE_LOCKING_LEVELS_HPP_INCLUDED_
#define CPPDEVTK_BASE_LOCKING_LEVELS_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"

#include <ctime>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup locking_levels Locking level classes
/// @{


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Per object lock.
template <class TMutex>
class ObjectLevelLocking: private NonCopyable {
protected:
	typedef TMutex MutexType;
	
	
	ObjectLevelLocking();
	/* virtual */ ~ObjectLevelLocking();	///< \remark Not virtual.
	
	TMutex& GetMutexRef() const;
private:
	mutable MutexType mutex_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Per class lock.
/// \tparam TMutex Must be a recursive mutex or deadlock will occur when locking two instances in the same thread.
template <class TMutex>
class ClassLevelLocking: private NonCopyable {
protected:
	typedef TMutex MutexType;
	
	
	ClassLevelLocking();
	/* virtual */ ~ClassLevelLocking();	///< \remark Not virtual.
	
	static TMutex& GetMutexRef();
private:
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	static TMutex mutex_;
#	endif
};


/// @}	// locking_levels
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

template <class TMutex>
inline ObjectLevelLocking<TMutex>::ObjectLevelLocking(): NonCopyable(), mutex_() {}

template <class TMutex>
inline ObjectLevelLocking<TMutex>::~ObjectLevelLocking() {}

template <class TMutex>
inline TMutex& ObjectLevelLocking<TMutex>::GetMutexRef() const {
	return mutex_;
}


#if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
template <class TMutex>
TMutex ClassLevelLocking<TMutex>::mutex_;
#endif

template <class TMutex>
inline ClassLevelLocking<TMutex>::ClassLevelLocking(): NonCopyable() {}

template <class TMutex>
inline ClassLevelLocking<TMutex>::~ClassLevelLocking() {}

template <class TMutex>
inline TMutex& ClassLevelLocking<TMutex>::GetMutexRef() {
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	return mutex_;
#	else
	static TMutex mutex;
	return mutex;
#	endif
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_LOCKING_LEVELS_HPP_INCLUDED_
