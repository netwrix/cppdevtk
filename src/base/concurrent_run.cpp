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
namespace detail {


void StartAndRunCancelableTask<void>::run() {
	if (isCanceled()) {
		ReportCanceled(CPPDEVTK_TASK_CANCELED_EXCEPTION_W_WA("task was canceled when started running"));
	}
	else {
		try {
			pCancelableTask_->Run(::std::auto_ptr<FutureInterfaceCancelable>(new FutureInterfaceCancelable(*this)));
		}
		catch (const TaskCanceledException& exc) {
			//CPPDEVTK_LOG_INFO("TaskCanceledException: " << Exception::GetDetailedInfo(exc));
			ReportCanceled(exc);
		}
		catch (const QtException& exc) {
			//CPPDEVTK_LOG_ERROR("task failed; QtException: " << Exception::GetDetailedInfo(exc));
			ReportException(exc);
		}
		catch (const ::std::exception& exc) {
			CPPDEVTK_LOG_ERROR("task failed; unhandled ::std::exception: " << Exception::GetDetailedInfo(exc));
			ReportUnhandledException();
		}
		catch (...) {
			CPPDEVTK_LOG_ERROR("task failed; unhandled unknown exception");
			ReportUnhandledException();
		}
	}
	
	ReportFinished();
}


}	// namespace detail
}	// namespace concurrent
}	// namespace base
}	// namespace cppdevtk
