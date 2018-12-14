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


#ifndef CPPDEVTK_BASE_THREAD_SHARED_OBJ_LOCKING_PTR_HPP_INCLUDED_
#define CPPDEVTK_BASE_THREAD_SHARED_OBJ_LOCKING_PTR_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "logger.hpp"
#include "dbc.hpp"
#include "cassert.hpp"

#include <exception>
#include <cstddef>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Thread shared object locking pointer.
/// \details Provides pointer like thread-safe access to an object shared between threads.
/// \attention Objects that are shared between threads must be volatile qualified and must have a mutex associated.
/// \sa <a href="http://loki-lib.sourceforge.net/html/a00441.html">Loki::LockingPtr</a>
template <typename TThreadSharedObj, class TMutex>
class ThreadSharedObjLockingPtr: private NonCopyable {
public:
	typedef TThreadSharedObj ThreadSharedObjType;
	typedef TMutex MutexType;
	
	
	ThreadSharedObjLockingPtr(volatile ThreadSharedObjType& threadSharedObj,
			MutexType& threadSharedObjMtx);	///< Locks mutex associated with object.
	~ThreadSharedObjLockingPtr();	///< Unlocks mutex.
	
	ThreadSharedObjType& operator*() const;	///< \pre Is not released.
	ThreadSharedObjType* operator->() const;	///< \pre Is not released.
	
	ThreadSharedObjType* Get() const;
	
	void Reset();
	void Reset(volatile ThreadSharedObjType& threadSharedObj, MutexType& threadSharedObjMtx);
private:
	ThreadSharedObjType* pThreadSharedObj_;
	MutexType* pThreadSharedObjMtx_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

template <typename TThreadSharedObj, class TMutex>
inline ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::ThreadSharedObjLockingPtr(
		volatile ThreadSharedObjType& threadSharedObj, MutexType& threadSharedObjMtx): NonCopyable(),
		pThreadSharedObj_(const_cast<ThreadSharedObjType*>(&threadSharedObj)),
		pThreadSharedObjMtx_(&threadSharedObjMtx) {
	pThreadSharedObjMtx_->Lock();
}

template <typename TThreadSharedObj, class TMutex>
inline ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::~ThreadSharedObjLockingPtr() {
	Reset();
}

template <typename TThreadSharedObj, class TMutex>
TThreadSharedObj& ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::operator*() const {
	CPPDEVTK_DBC_CHECK_STATE((pThreadSharedObj_ != NULL), "not released", "released");
	CPPDEVTK_ASSERT(pThreadSharedObjMtx_ != NULL);
	
	return *pThreadSharedObj_;
}

template <typename TThreadSharedObj, class TMutex>
TThreadSharedObj* ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::operator->() const {
	CPPDEVTK_DBC_CHECK_STATE((pThreadSharedObj_ != NULL), "not released", "released");
	CPPDEVTK_ASSERT(pThreadSharedObjMtx_ != NULL);
	
	return pThreadSharedObj_;
}

template <typename TThreadSharedObj, class TMutex>
inline TThreadSharedObj* ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::Get() const {
	return pThreadSharedObj_;
}

template <typename TThreadSharedObj, class TMutex>
void ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::Reset() {
	if (pThreadSharedObjMtx_ == NULL) {
		CPPDEVTK_ASSERT(pThreadSharedObj_ == NULL);
		
		return;
	}
	
	CPPDEVTK_ASSERT(pThreadSharedObj_ != NULL);
	
	pThreadSharedObjMtx_->Unlock();
	
	pThreadSharedObj_ = NULL;
	pThreadSharedObjMtx_ = NULL;
}

template <typename TThreadSharedObj, class TMutex>
void ThreadSharedObjLockingPtr<TThreadSharedObj, TMutex>::Reset(
		volatile ThreadSharedObjType& threadSharedObj, MutexType& threadSharedObjMtx) {
	if (pThreadSharedObjMtx_ == &threadSharedObjMtx) {
		if (pThreadSharedObj_ == &threadSharedObj) {
			return;
		}
		
		pThreadSharedObj_ = const_cast<ThreadSharedObjType*>(&threadSharedObj);
		return;
	}
	
	threadSharedObjMtx.Lock();
	
	Reset();
	
	pThreadSharedObj_ = const_cast<ThreadSharedObjType*>(&threadSharedObj);
	pThreadSharedObjMtx_ = &threadSharedObjMtx;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// #ifndef CPPDEVTK_BASE_THREAD_SHARED_OBJ_LOCKING_PTR_HPP_INCLUDED_
