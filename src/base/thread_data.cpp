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


#include "thread_data.hpp"
#include "thread_local_data_ptr.hpp"


namespace cppdevtk {
namespace base {


namespace detail {


}	// namespace detail


namespace this_thread {
namespace detail {


#if (!CPPDEVTK_DETAIL_DISABLE_GET_THREAD_LOCAL_DATA_PTR)
#if (CPPDEVTK_HAVE_THREAD_STORAGE)

::cppdevtk::base::detail::ThreadData* GetThreadLocalDataPtr() {
	return pThreadLocalData;
}

#endif
#endif


}	// namespace detail
}	// namespace this_thread


}	// namespace base
}	// namespace cppdevtk
