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


#ifndef CPPDEVTK_BASE_THREAD_LOCAL_DATA_PTR_HPP_INCLUDED_
#define CPPDEVTK_BASE_THREAD_LOCAL_DATA_PTR_HPP_INCLUDED_


#include <cppdevtk/base/thread_data.hpp>


namespace cppdevtk {
namespace base {
namespace this_thread {
namespace detail {


#if (CPPDEVTK_HAVE_THREAD_STORAGE)
extern CPPDEVTK_THREAD ::cppdevtk::base::detail::ThreadData* pThreadLocalData;	// Non-NULL only during execution of thread main function!
#endif


}	// namespace detail
}	// namespace this_thread
}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_THREAD_LOCAL_DATA_PTR_HPP_INCLUDED_
