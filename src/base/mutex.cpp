/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#define CPPDEVTK_BASE_MUTEX_CPP


#include <cppdevtk/base/mutex.hpp>


namespace cppdevtk {
namespace base {


template class CPPDEVTK_BASE_API LockGuard<Mutex>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API LockGuard<ErrorCheckingMutex>;
#endif
template class CPPDEVTK_BASE_API LockGuard<RecursiveMutex>;

template class CPPDEVTK_BASE_API UniqueLock<TimedMutex>;
#if (CPPDEVTK_HAVE_PTHREADS)
template class CPPDEVTK_BASE_API UniqueLock<ErrorCheckingTimedMutex>;
#endif
template class CPPDEVTK_BASE_API UniqueLock<RecursiveTimedMutex>;


}	// namespace base
}	// namespace cppdevtk
