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


#include <cppdevtk/util/core_application_base.hpp>
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include <cppdevtk/base/posix_signals_watcher_unx.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QCoreApplication>

#include <csignal>
#include <cstddef>


namespace cppdevtk {
namespace util {


using ::cppdevtk::base::GetLastSystemErrorCode;


void CoreApplicationBase::SetQuitOnTerminationSignals(bool value) {
	if (GetQuitOnTerminationSignals() == value) {
		return;
	}
	
	bool commit = false;
	base::PosixSignalsWatcher& thePosixSignalsWatcher = base::PosixSignalsWatcher::GetInstance();
	CPPDEVTK_ASSERT(qApp != NULL);
	
	if (value) {
		if (!thePosixSignalsWatcher.IsWatched(SIGTERM)) {
			if (!thePosixSignalsWatcher.Watch(SIGTERM)) {
				throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to watch SIGTERM");
			}
			unwatchSigTerm_ = true;
		}
		qApp->connect(&thePosixSignalsWatcher, SIGNAL(SigTerm()),
				SLOT(quit()), static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection));
		
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&thePosixSignalsWatcher)(&unwatchSigTerm_)) {
			if (!commit) {
				CPPDEVTK_VERIFY(thePosixSignalsWatcher.disconnect(SIGNAL(SigTerm()), qApp, SLOT(quit())));
				if (unwatchSigTerm_) {
					CPPDEVTK_ASSERT(thePosixSignalsWatcher.IsWatched(SIGTERM));
					if (thePosixSignalsWatcher.Unwatch(SIGTERM)) {
						unwatchSigTerm_ = false;
					}
					else {
						CPPDEVTK_LOG_WARN("failed to unwatch SIGTERM; errorCode: " << GetLastSystemErrorCode());
					}
				}
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		if (!thePosixSignalsWatcher.IsWatched(SIGINT)) {
			if (!thePosixSignalsWatcher.Watch(SIGINT)) {
				throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to watch SIGINT");
			}
			unwatchSigInt_ = true;
		}
		qApp->connect(&thePosixSignalsWatcher, SIGNAL(SigInt()),
				SLOT(quit()), static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection));
	}
	else {
		thePosixSignalsWatcher.disconnect(SIGNAL(SigInt()), qApp, SLOT(quit()));
		if (unwatchSigInt_) {
			if (thePosixSignalsWatcher.IsWatched(SIGINT)) {
				if (!thePosixSignalsWatcher.Unwatch(SIGINT)) {
					throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to unwatch SIGINT");
				}
			}
			unwatchSigInt_ = false;
		}
		
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&thePosixSignalsWatcher)(&unwatchSigInt_)) {
			if (!commit) {
				CPPDEVTK_ASSERT(!unwatchSigInt_);
				CPPDEVTK_ASSERT(!thePosixSignalsWatcher.IsWatched(SIGINT));
				
				if (thePosixSignalsWatcher.Watch(SIGINT)) {
					CPPDEVTK_VERIFY(qApp->connect(&thePosixSignalsWatcher, SIGNAL(SigInt()),
							SLOT(quit()), static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection)));
					unwatchSigInt_ = true;
				}
				else {
					CPPDEVTK_LOG_WARN("failed to watch SIGINT; errorCode: " << GetLastSystemErrorCode());
				}
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		
		thePosixSignalsWatcher.disconnect(SIGNAL(SigTerm()), qApp, SLOT(quit()));
		if (unwatchSigTerm_) {
			if (thePosixSignalsWatcher.IsWatched(SIGTERM)) {
				if (!thePosixSignalsWatcher.Unwatch(SIGTERM)) {
					throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to unwatch SIGTERM");
				}
			}
			unwatchSigTerm_ = false;
		}
	}
	
	quitOnTerminationSignals_ = value;
	commit = true;
}


}	// namespace util
}	// namespace cppdevtk
