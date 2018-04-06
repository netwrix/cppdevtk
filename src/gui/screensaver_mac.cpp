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


#include <cppdevtk/gui/screensaver.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/string_conv.hpp>

#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QDesktopServices>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <CoreFoundation/CFPreferences.h>
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFNumber.h>
#include <CoreServices/CoreServices.h>
#if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)
#include <libproc.h>
#else
#include <cppdevtk/base/unused.hpp>
#endif

#include <csignal>
#include <cstring>
#include <string>


#define CPPDEVTK_SCREEN_SAVER_ENGINE_APP_PATH_SIERRA "/System/Library/Frameworks/ScreenSaver.framework/Resources/ScreenSaverEngine.app"
#define CPPDEVTK_SCREEN_SAVER_ENGINE_APP_PATH_HIGH_SIERRA "/System/Library/CoreServices/ScreenSaverEngine.app"
#define CPPDEVTK_SCREEN_SAVER_PREF_APP_ID "com.apple.screensaver"
#define CPPDEVTK_SCREEN_SAVER_PREF_ASK_FOR_PASSWORD_KEY "askForPassword"
#define CPPDEVTK_SCREEN_SAVER_PREF_ASK_FOR_PASSWORD_DELAY_KEY "askForPasswordDelay"


namespace cppdevtk {
namespace gui {


using ::cppdevtk::base::Exception;
using ::cppdevtk::base::GetLastSystemErrorCode;
using ::std::exception;
using ::std::string;
using ::std::vector;


bool ScreenSaver::Lock() {
	SetAskForPassword(afpTrue);
	SetAskForPasswordDelay(0);
	
	if (!SetActive(true)) {
		CPPDEVTK_LOG_ERROR("failed to activate screensaver");
		return false;
	}
	
	return true;
}

bool ScreenSaver::SetActive(bool active) {
	if (active) {
		const char* pScreenSaverEngineAppPath = GetScreenSaverEngineAppPath();
		if (pScreenSaverEngineAppPath == NULL) {
			CPPDEVTK_LOG_ERROR("could not find ScreenSaverEngine app; errorCode: " << GetLastSystemErrorCode().ToString());
			return false;
		}
		
		const QUrl kScreenSaverEngineUrl = QUrl::fromLocalFile(pScreenSaverEngineAppPath);
		CPPDEVTK_ASSERT(kScreenSaverEngineUrl.isValid());
		if (!QDesktopServices::openUrl(kScreenSaverEngineUrl)) {
			CPPDEVTK_LOG_ERROR("failed to start ScreenSaverEngine: " << kScreenSaverEngineUrl.toLocalFile());
			return false;
		}
		
		//CPPDEVTK_LOG_DEBUG("started ScreenSaverEngine: " << kScreenSaverEngineUrl.toLocalFile());
		return true;
	}
	
	CPPDEVTK_ASSERT(!active);
#	if 0
	vector<int> screenSaverEnginePIds;
	if (!GetScreenSaverEnginePIds(screenSaverEnginePIds)) {
		CPPDEVTK_LOG_ERROR("failed to get ScreenSaverEnginePIds; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	
	//CPPDEVTK_LOG_DEBUG("found " << screenSaverEnginePIds.size() << " ScreenSaverEnginePIds");
	bool retValue = true;
	for (vector<int>::iterator iter = screenSaverEnginePIds.begin(); iter != screenSaverEnginePIds.end(); ++iter) {
		if (kill(*iter, SIGTERM) != 0) {
			CPPDEVTK_LOG_ERROR("failed to kill ScreenSaverEngine with pid " << *iter
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			retValue = false;
		}
	}
	return retValue;
#	else
	const OSErr kOSErr = UpdateSystemActivity(UsrActivity);
	if (kOSErr != noErr) {
			CPPDEVTK_LOG_ERROR("failed to UpdateSystemActivity(UsrActivity); kOSErr: " << kOSErr);
			return false;
	}
	return true;
#	endif
}

bool ScreenSaver::IsActive() const {
	vector<int> screenSaverEnginePIds;
	if (!GetScreenSaverEnginePIds(screenSaverEnginePIds)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "failed to get ScreenSaverEngine pids");
	}
	return !screenSaverEnginePIds.empty();
}

void ScreenSaver::Refresh() {
	bool wasActive = isActive_;
	
	try {
		isActive_ = IsActive();
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_ERROR("IsActive() failed; exc: " << Exception::GetDetailedInfo(exc));
		return;
	}
	
	if (isActive_ != wasActive) {
		Q_EMIT ActiveChanged(isActive_);
	}
}

ScreenSaver::ScreenSaver(): QObject(), ::cppdevtk::base::MeyersSingleton<ScreenSaver>(), timer_(), isActive_(IsActive()),
		askForPassword_(GetAskForPassword()), askForPasswordDelay_(GetAskForPasswordDelay()) {
	CPPDEVTK_ASSERT(qApp != NULL);
	
	//CPPDEVTK_LOG_DEBUG("askForPassword_: " << askForPassword_);
	//CPPDEVTK_LOG_DEBUG("askForPasswordDelay_: " << askForPasswordDelay_);
	
	timer_.setInterval(1000);
	timer_.setSingleShot(false);
	CPPDEVTK_VERIFY(connect(&timer_, SIGNAL(timeout()), SLOT(Refresh())));
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), &timer_, SLOT(stop())));
	timer_.start();
}

ScreenSaver::~ScreenSaver() {
	SetAskForPassword(askForPassword_);
	SetAskForPasswordDelay(askForPasswordDelay_);
}

const char* ScreenSaver::GetScreenSaverEngineAppPath() {
	if (QDir(CPPDEVTK_SCREEN_SAVER_ENGINE_APP_PATH_HIGH_SIERRA).exists()) {
		return CPPDEVTK_SCREEN_SAVER_ENGINE_APP_PATH_HIGH_SIERRA;
	}
	
	if (QDir(CPPDEVTK_SCREEN_SAVER_ENGINE_APP_PATH_SIERRA).exists()) {
		return CPPDEVTK_SCREEN_SAVER_ENGINE_APP_PATH_SIERRA;
	}
	
	CPPDEVTK_ASSERT(0 && "could not find ScreenSaverEngine app");	// maybe it will be changed again on > High Sierra
	errno = ENOENT;
	return NULL;
}

bool ScreenSaver::GetScreenSaverEnginePIds(::std::vector<int>& pids) {
#	if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)
	const char* pScreenSaverEngineAppPath = GetScreenSaverEngineAppPath();
	if (pScreenSaverEngineAppPath == NULL) {
		CPPDEVTK_LOG_ERROR("could not find ScreenSaverEngine app; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	
	int numPIds = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
	if (numPIds <= 0) {
		CPPDEVTK_LOG_ERROR("proc_listpids() failed; numPIds: " << numPIds
				<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	//CPPDEVTK_LOG_DEBUG("suggested numPIds: " << numPIds);
	
	// increase size in case new processes appeared since proc_listpids() call
	vector<int> listedPIds((numPIds + (numPIds / 4)), -1);
	numPIds = proc_listpids(PROC_ALL_PIDS, 0, &listedPIds[0], (listedPIds.size() * sizeof(listedPIds[0])));
	if (numPIds <= 0) {
		CPPDEVTK_LOG_ERROR("proc_listpids() failed; numPIds: " << numPIds
				<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		return false;
	}
	//CPPDEVTK_LOG_DEBUG("actual numPIds: " << numPIds);
	
	listedPIds.resize(numPIds);
	pids.clear();
	const QString kScreenSaverEngineBin = QString(pScreenSaverEngineAppPath) + "/Contents/MacOS/ScreenSaverEngine";
	char pidPathInfo[PROC_PIDPATHINFO_MAXSIZE] = {0};
	for (vector<int>::iterator iter = listedPIds.begin(); iter != listedPIds.end(); ++iter) {
		if (*iter == 0) {
			continue;	// kernel task
		}
		if (*iter < 0) {
			continue;	// not a real process
		}
		
		memset(pidPathInfo, 0, sizeof(pidPathInfo));
		if (proc_pidpath(*iter, pidPathInfo, sizeof(pidPathInfo)) <= 0) {
			if (errno != ESRCH) {	// process ended since proc_listpids
				CPPDEVTK_LOG_WARN("proc_pidpath() failed for pid " << *iter
						<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			}
			continue;
		}
		
		CPPDEVTK_ASSERT(pidPathInfo[0] != 0);
		CPPDEVTK_ASSERT(pidPathInfo[sizeof(pidPathInfo) - 1] == 0);
		if (QFileInfo(base::Utf82Q(pidPathInfo)).canonicalFilePath() == QFileInfo(kScreenSaverEngineBin).canonicalFilePath()) {
			pids.push_back(*iter);
		}
	}
	
	return true;
#	else	// (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)
	// TODO: Mac OS X 10.4 port
	::cppdevtk::base::SuppressUnusedWarning(pids);
	CPPDEVTK_COMPILER_WARNING("GetScreenSaverEnginePIds() not ported on Mac OS X 10.4");
	return false;
#	endif
}

void ScreenSaver::SetAskForPassword(AskForPassword value) {
	if (value == afpUnknown) {
		return;
	}
	
	CFPreferencesSetAppValue(CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_ASK_FOR_PASSWORD_KEY),
			((value == afpTrue) ? CFBooleanRef(kCFBooleanTrue) : CFBooleanRef(kCFBooleanFalse)),
			CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_APP_ID));
	
	if (!CFPreferencesAppSynchronize(CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_APP_ID))) {
		CPPDEVTK_LOG_WARN("CFPreferencesAppSynchronize() failed for screensaver");
	}
}

ScreenSaver::AskForPassword ScreenSaver::GetAskForPassword() {
	AskForPassword askForPassword = afpUnknown;
	
	Boolean keyExistsAndHasValidFormat = FALSE;
	Boolean cfAskForPassword = CFPreferencesGetAppBooleanValue(CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_ASK_FOR_PASSWORD_KEY),
			CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_APP_ID), &keyExistsAndHasValidFormat);
	if (!cfAskForPassword && !keyExistsAndHasValidFormat) {
		CPPDEVTK_LOG_WARN("askForPassword key does not exist or has invalid format");
	}
	else {
		CPPDEVTK_ASSERT((cfAskForPassword == TRUE) || (cfAskForPassword == FALSE));
		askForPassword = static_cast<AskForPassword>(cfAskForPassword);
	}
	
	return askForPassword;
}

void ScreenSaver::SetAskForPasswordDelay(int value) {
	if (value < 0) {
		return;
	}
	
	CFPreferencesSetAppValue(CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_ASK_FOR_PASSWORD_DELAY_KEY),
			CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &value), CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_APP_ID));
	
	if (!CFPreferencesAppSynchronize(CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_APP_ID))) {
		CPPDEVTK_LOG_WARN("CFPreferencesAppSynchronize() failed for screensaver");
	}
}

int ScreenSaver::GetAskForPasswordDelay() {
	int askForPasswordDelay = -1;
	
	Boolean keyExistsAndHasValidFormat = FALSE;
	CFIndex cfAskForPasswordDelay = CFPreferencesGetAppIntegerValue(CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_ASK_FOR_PASSWORD_DELAY_KEY),
			CFSTR(CPPDEVTK_SCREEN_SAVER_PREF_APP_ID), &keyExistsAndHasValidFormat);
	if ((cfAskForPasswordDelay == 0) && !keyExistsAndHasValidFormat) {
		CPPDEVTK_LOG_WARN("askForPasswordDelay key does not exist or has invalid format");
	}
	else {
		askForPasswordDelay = cfAskForPasswordDelay;
	}
	
	return askForPasswordDelay;
}


}	// namespace gui
}	// namespace cppdevtk
