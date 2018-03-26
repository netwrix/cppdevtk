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
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Unix specific!!!"
#endif
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>

#include <QtCore/QCoreApplication>
#include <QtCore/QMetaObject>
#include <QtCore/QString>


namespace cppdevtk {
namespace util {


static QString GetControlSignalName(DWORD dwCtrlType);


void CoreApplicationBase::SetQuitOnTerminationSignals(bool value) {
	if (GetQuitOnTerminationSignals() == value) {
		return;
	}
	
	if (!SetConsoleCtrlHandler(ConsoleCtrlHandler, (value ? TRUE : FALSE))) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(base::GetLastSystemErrorCode(), "failed to (un)set console ctrl handler");
	}
	
	quitOnTerminationSignals_ = value;
}

BOOL WINAPI CoreApplicationBase::ConsoleCtrlHandler(_In_ DWORD dwCtrlType) {
	CPPDEVTK_ASSERT(qApp != NULL);
	
	CPPDEVTK_LOG_INFO("got control signal " << GetControlSignalName(dwCtrlType));
	
	switch (dwCtrlType) {
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
			if (!QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection)) {
				CPPDEVTK_LOG_ERROR("failed to request app to quit");
				return FALSE;
			}
			CPPDEVTK_LOG_INFO("requested app to quit");
			return TRUE;
		default:
			return FALSE;
	}
}


QString GetControlSignalName(DWORD dwCtrlType) {
	switch (dwCtrlType) {
		case CTRL_C_EVENT:
			return "CTRL_C_EVENT";
		case CTRL_BREAK_EVENT:
			return "CTRL_BREAK_EVENT";
		case CTRL_CLOSE_EVENT:
			return "CTRL_CLOSE_EVENT";
		default:
			return QString::number(dwCtrlType);
	}
}


}	// namespace util
}	// namespace cppdevtk
