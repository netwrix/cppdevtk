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


#ifndef CPPDEVTK_BASE_ARCHITECTURE_TYPES_H_INCLUDED_
#define CPPDEVTK_BASE_ARCHITECTURE_TYPES_H_INCLUDED_


#include "config.hpp"
#include "stdint.h"


/// \brief A byte (8 bits).
/// \note Ms Windows porting: \c BYTE.
/// \sa <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751">Windows Data Types</a>
typedef uint8_t byte;

/// \brief A 16-bit unsigned integer.
/// \note Ms Windows porting: \c WORD.
/// \sa <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751">Windows Data Types</a>
typedef uint16_t word;

/// \brief Processor word (32 bits on X86 and 64 bits on X86-64).
#if (CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_ARM)
typedef uint32_t word_t;
#elif (CPPDEVTK_ARCHITECTURE_X86_64 || CPPDEVTK_ARCHITECTURE_ARM64)
typedef uint64_t word_t;
#else
#	error "Unsupported architecture!!!"
#endif

/// \brief A 32-bit unsigned integer.
/// \note Ms Windows porting: \c DWORD32 or \c DWORD.
/// \sa <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751">Windows Data Types</a>
typedef uint32_t dword32;

/// \brief A 64-bit unsigned integer.
/// \note Ms Windows porting: \c DWORD64.
/// \sa <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751">Windows Data Types</a>
typedef uint64_t dword64;

/// \brief Architecture dependent dword (32 bits on X86 and 64 bits on X86-64).
/// \note Ms Windows porting: \c DWORD_PTR.
/// \sa <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751">Windows Data Types</a>
#if (CPPDEVTK_ARCHITECTURE_X86 || CPPDEVTK_ARCHITECTURE_ARM)
typedef dword32 dword_t;
#elif (CPPDEVTK_ARCHITECTURE_X86_64 || CPPDEVTK_ARCHITECTURE_ARM64)
typedef dword64 dword_t;
#else
#	error "Unsupported architecture!!!"
#endif


#endif	// CPPDEVTK_BASE_ARCHITECTURE_TYPES_H_INCLUDED_
