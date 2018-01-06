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


#ifndef CPPDEVTK_BASE_SAFE_DELETE_HPP_INCLUDED_
#define CPPDEVTK_BASE_SAFE_DELETE_HPP_INCLUDED_


#include "config.hpp"
#include "static_assert.hpp"

#include <new>
#include <cstddef>


namespace cppdevtk {
namespace base {


template <typename T>
void DeleteAndSetToNull(T*& ptr);

template <typename T>
void DeleteArrayAndSetToNull(T*& ptr);

template <class T>
void DeleteCompleteType(T* ptr);

template <class T>
void DeleteCompleteTypeArray(T* ptr);

template <class T>
void DeleteCompleteTypeAndSetToNull(T*& ptr);

template <class T>
void DeleteCompleteTypeArrayAndSetToNull(T*& ptr);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions / template definitions.

template <typename T>
inline void DeleteAndSetToNull(T*& ptr) {
	delete ptr;
	ptr = NULL;
}

template <typename T>
inline void DeleteArrayAndSetToNull(T*& ptr) {
	delete[] ptr;
	ptr = NULL;
}

template <class T>
inline void DeleteCompleteType(T* ptr) {
	CPPDEVTK_STATIC_ASSERT_W_MSG(sizeof(T), "T must be a complete type");
	
	delete ptr;
}

template <class T>
inline void DeleteCompleteTypeArray(T* ptr) {
	CPPDEVTK_STATIC_ASSERT_W_MSG(sizeof(T), "T must be a complete type");
	
	delete[] ptr;
}

template <class T>
inline void DeleteCompleteTypeAndSetToNull(T*& ptr) {
	CPPDEVTK_STATIC_ASSERT_W_MSG(sizeof(T), "T must be a complete type");
	
	delete ptr;
	ptr = NULL;
}

template <class T>
inline void DeleteCompleteTypeArrayAndSetToNull(T*& ptr) {
	CPPDEVTK_STATIC_ASSERT_W_MSG(sizeof(T), "T must be a complete type");
	
	delete[] ptr;
	ptr = NULL;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_SAFE_DELETE_HPP_INCLUDED_
