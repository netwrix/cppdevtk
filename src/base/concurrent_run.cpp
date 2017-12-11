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


#include <cppdevtk/base/concurrent_run.hpp>


namespace cppdevtk {
namespace base {
namespace concurrent {


#if (CPPDEVTK_ENABLE_TMPL_EXPL_INST)

template class CPPDEVTK_BASE_API CancelableTask<void>;

namespace detail {

template class CPPDEVTK_BASE_API StartAndRunCancelableTaskBase<void>;
#if (!CPPDEVTK_COMPILER_MSVC || (_MSC_VER > 1500))	// msvc 2008: C2950: 'type' : cannot explicitly instantiate an explicit specialization
template class CPPDEVTK_BASE_API StartAndRunCancelableTask<void>;
#endif

}

#endif


namespace detail {


void StartAndRunCancelableTask<void>::run() {
	if (QFutureInterface<void>::isCanceled()) {
		QFutureInterface<void>::reportFinished();
		
		return;
	}
	
	::std::auto_ptr<StartAndRunCancelableTaskBase<void>::CancelableTaskType::CancelableType> pCancelable(
			new StartAndRunCancelableTaskBase<void>::CancelableTaskType::CancelableType(*this));
	try {
		StartAndRunCancelableTaskBase<void>::pCancelableTask_->Run(pCancelable);
	}
	catch (const CPPDEVTK_QT_EXCEPTION& exc) {
		QFutureInterface<void>::reportException(exc);
	}
	catch (...) {
		QFutureInterface<void>::reportException(CPPDEVTK_QT_UNHANDLED_EXCEPTION());
	}
	
	QFutureInterface<void>::reportFinished();
}


}	// namespace detail
}	// namespace concurrent
}	// namespace base
}	// namespace cppdevtk
