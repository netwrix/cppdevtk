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


#if (!(defined(CPPDEVTK_DETAIL_BUILD) || defined(CPPDEVTK_BASE_MUTEX_HPP_INCLUDED_)))
#	error "Do not include directly (non-std file); please include <cppdevtk/base/mutex.hpp> instead!!!"
#endif


#ifndef CPPDEVTK_BASE_GENERIC_LOCKING_ALGORITHMS_HPP_INCLUDED_
#define CPPDEVTK_BASE_GENERIC_LOCKING_ALGORITHMS_HPP_INCLUDED_


#include "config.hpp"
#include "locks.hpp"
#include "lock_exception.hpp"
#include "deadlock_exception.hpp"
#include "cassert.hpp"


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup generic_locking_algorithms Generic locking algorithms
/// \sa C++ 11, 30.4.3 Generic locking algorithms
/// @{


template <class TL1, class TL2>
int TryLock(TL1& l1, TL2& l2);

template <class TL1, class TL2, class TL3>
int TryLock(TL1& l1, TL2& l2, TL3& l3);

template <class TL1, class TL2, class TL3, class TL4>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4);

template <class TL1, class TL2, class TL3, class TL4, class TL5>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9, class TL10>
int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9, TL10& l10);




template <class TL1, class TL2>
void Lock(TL1& l1, TL2& l2);

template <class TL1, class TL2, class TL3>
void Lock(TL1& l1, TL2& l2, TL3& l3);

template <class TL1, class TL2, class TL3, class TL4>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4);

template <class TL1, class TL2, class TL3, class TL4, class TL5>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9, class TL10>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9, TL10& l10);




namespace detail {


template <class TL1, class TL2>
int LockHelper(TL1& l1, TL2& l2);

template <class TL1, class TL2, class TL3>
int LockHelper(TL1& l1, TL2& l2, TL3& l3);

template <class TL1, class TL2, class TL3, class TL4>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4);

template <class TL1, class TL2, class TL3, class TL4, class TL5>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9);

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9, class TL10>
int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9, TL10& l10);


}	// namespace detail


/// @}	// generic_locking_algorithms
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

template <class TL1, class TL2>
inline int TryLock(TL1& l1, TL2& l2) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		if (l2.TryLock()) {
			uniqueLock.Release();
			return -1;
		}
		return 1;
	}
	return 0;
}

template <class TL1, class TL2, class TL3>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4, class TL5>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4, l5);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4, l5, l6);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4, l5, l6, l7);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4, l5, l6, l7, l8);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4, l5, l6, l7, l8, l9);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9, class TL10>
inline int TryLock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9, TL10& l10) {
	UniqueLock<TL1> uniqueLock(l1, tryToLock);
	if (uniqueLock.OwnsLock()) {
		const int kIdx = TryLock(l2, l3, l4, l5, l6, l7, l8, l9, l10);
		if (kIdx == -1) {
			uniqueLock.Release();
			return kIdx;
		}
		return (kIdx + 1);
	}
	return 0;
}




template <class TL1, class TL2>
void Lock(TL1& l1, TL2& l2) {
	const int kNumLockables = 2;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3>
void Lock(TL1& l1, TL2& l2, TL3& l3) {
	const int kNumLockables = 3;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4) {
	const int kNumLockables = 4;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5) {
	const int kNumLockables = 5;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4, l5);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l5, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l5, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l5, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			case 4:
				lockableIdx = detail::LockHelper(l5, l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 4) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6) {
	const int kNumLockables = 6;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4, l5, l6);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l5, l6, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l5, l6, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l5, l6, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			case 4:
				lockableIdx = detail::LockHelper(l5, l6, l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 4) % kNumLockables;
				break;
			case 5:
				lockableIdx = detail::LockHelper(l6, l1, l2, l3, l4, l5);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 5) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7) {
	const int kNumLockables = 7;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4, l5, l6, l7);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l5, l6, l7, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l5, l6, l7, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l5, l6, l7, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			case 4:
				lockableIdx = detail::LockHelper(l5, l6, l7, l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 4) % kNumLockables;
				break;
			case 5:
				lockableIdx = detail::LockHelper(l6, l7, l1, l2, l3, l4, l5);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 5) % kNumLockables;
				break;
			case 6:
				lockableIdx = detail::LockHelper(l7, l1, l2, l3, l4, l5, l6);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 6) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8) {
	const int kNumLockables = 8;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4, l5, l6, l7, l8);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l5, l6, l7, l8, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l5, l6, l7, l8, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l5, l6, l7, l8, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			case 4:
				lockableIdx = detail::LockHelper(l5, l6, l7, l8, l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 4) % kNumLockables;
				break;
			case 5:
				lockableIdx = detail::LockHelper(l6, l7, l8, l1, l2, l3, l4, l5);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 5) % kNumLockables;
				break;
			case 6:
				lockableIdx = detail::LockHelper(l7, l8, l1, l2, l3, l4, l5, l6);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 6) % kNumLockables;
				break;
			case 7:
				lockableIdx = detail::LockHelper(l8, l1, l2, l3, l4, l5, l6, l7);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 7) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9) {
	const int kNumLockables = 9;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4, l5, l6, l7, l8, l9);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l5, l6, l7, l8, l9, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l5, l6, l7, l8, l9, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l5, l6, l7, l8, l9, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			case 4:
				lockableIdx = detail::LockHelper(l5, l6, l7, l8, l9, l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 4) % kNumLockables;
				break;
			case 5:
				lockableIdx = detail::LockHelper(l6, l7, l8, l9, l1, l2, l3, l4, l5);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 5) % kNumLockables;
				break;
			case 6:
				lockableIdx = detail::LockHelper(l7, l8, l9, l1, l2, l3, l4, l5, l6);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 6) % kNumLockables;
				break;
			case 7:
				lockableIdx = detail::LockHelper(l8, l9, l1, l2, l3, l4, l5, l6, l7);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 7) % kNumLockables;
				break;
			case 8:
				lockableIdx = detail::LockHelper(l9, l1, l2, l3, l4, l5, l6, l7, l8);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 8) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9, class TL10>
void Lock(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9, TL10& l10) {
	const int kNumLockables = 10;
	int lockableIdx = 0;
	do {
		switch (lockableIdx) {
			case 0:
				lockableIdx = detail::LockHelper(l1, l2, l3, l4, l5, l6, l7, l8, l9, l10);
				if (lockableIdx == -1) {
					return;
				}
				break;
			case 1:
				lockableIdx = detail::LockHelper(l2, l3, l4, l5, l6, l7, l8, l9, l10, l1);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 1) % kNumLockables;
				break;
			case 2:
				lockableIdx = detail::LockHelper(l3, l4, l5, l6, l7, l8, l9, l10, l1, l2);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 2) % kNumLockables;
				break;
			case 3:
				lockableIdx = detail::LockHelper(l4, l5, l6, l7, l8, l9, l10, l1, l2, l3);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 3) % kNumLockables;
				break;
			case 4:
				lockableIdx = detail::LockHelper(l5, l6, l7, l8, l9, l10, l1, l2, l3, l4);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 4) % kNumLockables;
				break;
			case 5:
				lockableIdx = detail::LockHelper(l6, l7, l8, l9, l10, l1, l2, l3, l4, l5);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 5) % kNumLockables;
				break;
			case 6:
				lockableIdx = detail::LockHelper(l7, l8, l9, l10, l1, l2, l3, l4, l5, l6);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 6) % kNumLockables;
				break;
			case 7:
				lockableIdx = detail::LockHelper(l8, l9, l10, l1, l2, l3, l4, l5, l6, l7);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 7) % kNumLockables;
				break;
			case 8:
				lockableIdx = detail::LockHelper(l9, l10, l1, l2, l3, l4, l5, l6, l7, l8);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 8) % kNumLockables;
				break;
			case 9:
				lockableIdx = detail::LockHelper(l10, l1, l2, l3, l4, l5, l6, l7, l8, l9);
				if (lockableIdx == -1) {
					return;
				}
				lockableIdx = (lockableIdx + 9) % kNumLockables;
				break;
			default:	// silence compiler warning about missing default in switch
				CPPDEVTK_ASSERT(0 && "we should never get here");
				break;
		}
	}
	while (true);
}




namespace detail {


template <class TL1, class TL2>
inline int LockHelper(TL1& l1, TL2& l2) {
	UniqueLock<TL1> uniqueLock(l1);
	if (l2.TryLock()) {
		uniqueLock.Release();
		return -1;
	}
	return 1;
}

template <class TL1, class TL2, class TL3>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4, l5);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4, l5, l6);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4, l5, l6, l7);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4, l5, l6, l7, l8);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4, l5, l6, l7, l8, l9);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}

template <class TL1, class TL2, class TL3, class TL4, class TL5, class TL6, class TL7, class TL8, class TL9, class TL10>
inline int LockHelper(TL1& l1, TL2& l2, TL3& l3, TL4& l4, TL5& l5, TL6& l6, TL7& l7, TL8& l8, TL9& l9, TL10& l10) {
	UniqueLock<TL1> uniqueLock(l1);
	const int kIdx = TryLock(l2, l3, l4, l5, l6, l7, l8, l9, l10);
	if (kIdx == -1) {
		uniqueLock.Release();
		return kIdx;
	}
	return (kIdx + 1);
}


}	// namespace detail
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_GENERIC_LOCKING_ALGORITHMS_HPP_INCLUDED_
