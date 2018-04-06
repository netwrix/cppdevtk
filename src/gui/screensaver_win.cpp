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
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/tchar.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/on_block_exit.hpp>

#include <QtCore/QSysInfo>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>


namespace cppdevtk {
namespace gui {


using base::ErrorCode;
using base::GetLastSystemErrorCode;
using base::MakeSystemErrorCode;
using base::Exception;
using ::std::exception;


bool ScreenSaver::Lock() {
	try {
		SetScreenSaveSecure(TRUE);
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_ERROR("failed to set ScreenSaveSecure; exc: " << Exception::GetDetailedInfo(exc));
		return false;
	}
	
	if (!SetActive(true)) {
		CPPDEVTK_LOG_ERROR("failed to activate screensaver");
		return false;
	}
	
	return true;
}

bool ScreenSaver::SetActive(bool active) {
	if (active == IsActive()) {
		return true;
	}
	
	if (active) {
#		if 0
		const LRESULT kLResult = SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_SCREENSAVE, 0);
		if (kLResult == 0) {
			CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_SYSCOMMAND + SC_SCREENSAVE to HWND_BROADCAST"
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			return false;
		}
#		else
		const LRESULT kLResult = SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
		if (kLResult != 0) {
			CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_SYSCOMMAND + SC_SCREENSAVE to DesktopWindow; kLResult: " << kLResult
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			return false;
		}
#		endif
	}
	else {
		const LRESULT kLResult = SendMessage(GetForegroundWindow(), WM_CLOSE, 0, 0);
		if (kLResult != 0) {
			CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_CLOSE to ForegroundWindow; kLResult: " << kLResult
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			return false;
		}
	}
	
	return true;
}

bool ScreenSaver::IsActive() const {
	BOOL isScreenSaverRunning = FALSE;
	if (!SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &isScreenSaverRunning, 0)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
				"SystemParametersInfo() failed for SPI_GETSCREENSAVERRUNNING");
	}
	return isScreenSaverRunning;
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
		isScreenSaverSecure_(GetScreenSaveSecure()) {
	CPPDEVTK_ASSERT(qApp != NULL);
	
	timer_.setInterval(1000);
	timer_.setSingleShot(false);
	CPPDEVTK_VERIFY(connect(&timer_, SIGNAL(timeout()), SLOT(Refresh())));
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), &timer_, SLOT(stop())));
	timer_.start();
}

ScreenSaver::~ScreenSaver() {
	try {
		SetScreenSaveSecure(isScreenSaverSecure_);
	}
	catch (const exception& exc) {
		CPPDEVTK_LOG_WARN("failed to restore isScreenSaverSecure_; exc: " << Exception::GetDetailedInfo(exc));
	}
}

void ScreenSaver::SetScreenSaveSecure(BOOL value) {
#	ifndef SPI_SETSCREENSAVESECURE
#	define SPI_SETSCREENSAVESECURE 0x0077
#	endif
	
	CPPDEVTK_ASSERT((value == TRUE) || (value == FALSE));
	
	if (value == GetScreenSaveSecure()) {
		return;
	}
	
	if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA) {
		if (!SystemParametersInfo(SPI_SETSCREENSAVESECURE, value, NULL, (SPIF_UPDATEINIFILE | SPIF_SENDCHANGE))) {
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
					"SystemParametersInfo() failed for SPI_SETSCREENSAVESECURE");
		}
	}
	else {
		HKEY hKey = NULL;
		LONG retCode = RegOpenKeyEx(HKEY_CURRENT_USER, CPPDEVTK_T("Control Panel\\Desktop"), 0, KEY_WRITE, &hKey);
		if (retCode != ERROR_SUCCESS) {
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
					"Failed to open Desktop registry key for writing");
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hKey)) {
			LONG retCode = RegCloseKey(hKey);
			if (retCode != ERROR_SUCCESS) {
				CPPDEVTK_LOG_WARN("Failed to close Desktop registry key; errorCode: "
						<< GetLastSystemErrorCode().ToString());
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		const base::tstring kNumber = CPPDEVTK_Q2T(QString::number(value));
		CPPDEVTK_ASSERT(kNumber.length() == 1);
		retCode = RegSetValueEx(hKey, CPPDEVTK_T("ScreenSaverIsSecure"), 0, REG_SZ,
				(BYTE*)kNumber.c_str(), ((kNumber.length() + 1) * sizeof(tchar)));
		if (retCode != ERROR_SUCCESS) {
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode),
					"Failed to write ScreenSaverIsSecure registry value");
		}
	}
}

BOOL ScreenSaver::GetScreenSaveSecure() {
#	ifndef SPI_GETSCREENSAVESECURE
#	define SPI_GETSCREENSAVESECURE 0x0076
#	endif
	
	BOOL screenSaveSecure = FALSE;
	if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA) {
		if (!SystemParametersInfo(SPI_GETSCREENSAVESECURE, 0, &screenSaveSecure, 0)) {
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
					"SystemParametersInfo() failed for SPI_GETSCREENSAVESECURE");
		}
	}
	else {
		HKEY hKey = NULL;
		LONG retCode = RegOpenKeyEx(HKEY_CURRENT_USER, CPPDEVTK_T("Control Panel\\Desktop"), 0, KEY_READ, &hKey);
		if (retCode != ERROR_SUCCESS) {
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(),
					"Failed to open Desktop registry key for reading");
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hKey)) {
			LONG retCode = RegCloseKey(hKey);
			if (retCode != ERROR_SUCCESS) {
				CPPDEVTK_LOG_WARN("Failed to close Desktop registry key; errorCode: "
						<< GetLastSystemErrorCode().ToString());
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		DWORD valueType = 0;
		BYTE buf[((1 + 1) + 1) * sizeof(tchar)] = {0};
		DWORD cbData = sizeof(buf) - sizeof(tchar);
		retCode = RegQueryValueEx(hKey, CPPDEVTK_T("ScreenSaverIsSecure"), NULL, &valueType, buf, &cbData);
		if (retCode != ERROR_SUCCESS) {
			throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(MakeSystemErrorCode(retCode),
					"Failed to read ScreenSaverIsSecure registry value");
		}
		if (valueType != REG_SZ) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("ScreenSaverIsSecure registry value does not have expected type");
		}
		
		const QString kScreenSaverIsSecure = CPPDEVTK_T2Q((tchar*)buf);
		bool convOk = false;
		screenSaveSecure = kScreenSaverIsSecure.toInt(&convOk);
		if (!convOk) {
			throw CPPDEVTK_RUNTIME_EXCEPTION(
					QString("Failed to convert ScreenSaverIsSecure registry value '%1' to BOOL").arg(kScreenSaverIsSecure));
		}
	}
	
	CPPDEVTK_ASSERT((screenSaveSecure == TRUE) || (screenSaveSecure == FALSE));
	return screenSaveSecure;
}


}	// namespace gui
}	// namespace cppdevtk
