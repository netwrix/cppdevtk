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


#ifndef CPPDEVTK_BASE_SINGLETONS_HPP_INCLUDED_
#define CPPDEVTK_BASE_SINGLETONS_HPP_INCLUDED_


#include "config.hpp"
#include "non_copyable.hpp"
#include "mutex.hpp"

#include <cstddef>
#include <stdexcept>
#include <new>


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Templatized Meyers singleton.
/// \remark The instance is created on demand, when GetInstance() is called first time.
/// If GetInstance() is never called, the instance is never created.
/// If instance is created, it will be destroyed upon program completion.
/// \attention Meyers singleton is not thread safe in C++03 (it is in C++11)!
/// (local static variable initialization is not thread safe in C++ 03)
template <typename T>
class MeyersSingleton: private NonCopyable {
public:
	static T& GetInstance();
protected:
	MeyersSingleton();
	/* virtual */ ~MeyersSingleton();	///< \remark Not virtual.
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Templatized thread safe singleton.
/// \remark The instance is created on demand, when GetInstance() is called first time.
/// If GetInstance() is never called, the instance is never created.
/// If instance is created, it will be destroyed upon program completion.
template <typename T>
class Singleton: private NonCopyable {
public:
	static volatile T& GetInstance();
protected:
	Singleton();
	/* virtual */ ~Singleton();	///< \remark Not virtual.
private:
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	static void DeleteInstance();
	
	
	static volatile T* volatile pInstance_;
	static DefaultMutex mtxInstance_;
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Templatized non thread-safe Phoenix singleton.
/// \remark The instance is created on demand, when GetInstance() is called first time.
/// If GetInstance() is never called, the instance is never created.
/// If instance is created, it will be destroyed upon program completion, then if need it can be created and destroyed again.
/// \attention This class is not thread safe in C++03 (it is in C++11)!
/// (local static variable initialization is not thread safe in C++ 03)
template <typename T>
class PhoenixSingleton: private NonCopyable {
public:
	static T& GetInstance();
protected:
	PhoenixSingleton();
	/* virtual */ ~PhoenixSingleton();	///< \remark Not virtual.
private:
	static void Create();
	static void ReCreate();
	static void Destroy();
	
	
	static T* pInstance_;
	static bool destroyed_;
};




template <typename T>
class ThreadSafePhoenixSingleton: private NonCopyable {
public:
	static volatile T& GetInstance();
protected:
	ThreadSafePhoenixSingleton();
	/* virtual */ ~ThreadSafePhoenixSingleton();	///< \remark Not virtual.
private:
	static void Create();
	static void ReCreate();
	static void Destroy();
	
	static DefaultMutex& GetMutexRef();
	
	
	static volatile T* volatile pInstance_;
	static volatile bool destroyed_;
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	static DefaultMutex mtx_;
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

// NOTE: Do not inline!
template <typename T>
T& MeyersSingleton<T>::GetInstance() {
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
#	if (CPPDEVTK_COMPILER_MSVC)
#	pragma warning(suppress: 4640)	// C4640: 'instance' : construction of local static object is not thread-safe
#	endif
#	endif
	static T instance;
	return instance;
}

template <typename T>
inline MeyersSingleton<T>::MeyersSingleton(): NonCopyable() {}

template <typename T>
inline MeyersSingleton<T>::~MeyersSingleton() {}




template <typename T>
volatile T& Singleton<T>::GetInstance() {
#if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	// Double-checked locking does not work in C++!!!
	// Please see: http://erdani.com/publications/DDJ_Jul_Aug_2004_revised.pdf
	DefaultLockGuard lockGuard(mtxInstance_);
	if (pInstance_ == NULL) {
		pInstance_ = new volatile T();
		if (::atexit(&DeleteInstance) != 0) {
			delete pInstance_;
			pInstance_ = NULL;
			throw ::std::runtime_error("Singleton: Failed to register with atexit()");
		}
	}
	return *pInstance_;
#else
	static volatile T instance;
	return instance;
#endif
}

template <typename T>
inline Singleton<T>::Singleton(): NonCopyable() {}

template <typename T>
inline Singleton<T>::~Singleton() {}

#if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)

template <typename T>
inline void Singleton<T>::DeleteInstance() {
	delete pInstance_;
}


template <typename T>
volatile T* volatile Singleton<T>::pInstance_ = NULL;

template <typename T>
DefaultMutex Singleton<T>::mtxInstance_;

#endif




template <typename T>
T& PhoenixSingleton<T>::GetInstance() {
	if (pInstance_ == NULL) {
		if (destroyed_) {
			ReCreate();
		}
		else {
			Create();
		}
	}
	return *pInstance_;
}

template <typename T>
inline PhoenixSingleton<T>::PhoenixSingleton(): NonCopyable() {}

template <typename T>
inline PhoenixSingleton<T>::~PhoenixSingleton() {
	pInstance_ = NULL;
	destroyed_ = true;
}

// NOTE: Do not inline!
template <typename T>
void PhoenixSingleton<T>::Create() {
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
#	if (CPPDEVTK_COMPILER_MSVC)
#	pragma warning(suppress: 4640)	// C4640: 'instance' : construction of local static object is not thread-safe
#	endif
#	endif
	static T instance;
	pInstance_ = &instance;
}

template <typename T>
void PhoenixSingleton<T>::ReCreate() {
	Create();
	new(pInstance_) T();
	if (::atexit(&Destroy) != 0) {
		Destroy();
		throw ::std::runtime_error("PhoenixSingleton: Failed to register with atexit()");
	}
	destroyed_ = false;
}

template <typename T>
inline void PhoenixSingleton<T>::Destroy() {
	pInstance_->~T();
}


template <typename T>
T* PhoenixSingleton<T>::pInstance_ = NULL;

template <typename T>
bool PhoenixSingleton<T>::destroyed_ = false;




template <typename T>
volatile T& ThreadSafePhoenixSingleton<T>::GetInstance() {
	DefaultLockGuard lockGuard(GetMutexRef());
	if (pInstance_ == NULL) {
		if (destroyed_) {
			ReCreate();
		}
		else {
			Create();
		}
	}
	return *pInstance_;
}

template <typename T>
inline ThreadSafePhoenixSingleton<T>::ThreadSafePhoenixSingleton(): NonCopyable() {}

template <typename T>
inline ThreadSafePhoenixSingleton<T>::~ThreadSafePhoenixSingleton() {
	pInstance_ = NULL;
	destroyed_ = true;
}

// NOTE: Do not inline!
template <typename T>
void ThreadSafePhoenixSingleton<T>::Create() {
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
#	if (CPPDEVTK_COMPILER_MSVC)
#	pragma warning(suppress: 4640)	// C4640: 'instance' : construction of local static object is not thread-safe
#	endif
#	endif
	static volatile T instance;
	pInstance_ = &instance;
}

template <typename T>
void ThreadSafePhoenixSingleton<T>::ReCreate() {
	Create();
	new(const_cast<T*>(pInstance_)) T();
	if (::atexit(&Destroy) != 0) {
		Destroy();
		throw ::std::runtime_error("ThreadSafePhoenixSingleton: Failed to register with atexit()");
	}
	destroyed_ = false;
}

template <typename T>
inline void ThreadSafePhoenixSingleton<T>::Destroy() {
	pInstance_->~T();
}

// NOTE: Do not inline!
template <typename T>
DefaultMutex& ThreadSafePhoenixSingleton<T>::GetMutexRef() {
#	if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
	return mtx_;
#	else
	static DefaultMutex mtx;
	return mtx;
#	endif
}


template <typename T>
volatile T* volatile ThreadSafePhoenixSingleton<T>::pInstance_ = NULL;

template <typename T>
volatile bool ThreadSafePhoenixSingleton<T>::destroyed_ = false;

#if (!CPPDEVTK_COMPILER_HAVE_LOCAL_STATIC_VAR_INIT_TS)
template <typename T>
DefaultMutex ThreadSafePhoenixSingleton<T>::mtx_;
#endif


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_SINGLETONS_HPP_INCLUDED_
