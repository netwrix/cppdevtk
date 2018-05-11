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


// NOTE: On Windows the only screensaver notification is SC_SCREENSAVE that works only for top-level active window so we use timer...


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
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QSysInfo>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif


static BOOL CALLBACK StopScreenSaverWndProc(HWND hwnd, LPARAM lParam);


namespace cppdevtk {
namespace gui {


using base::ErrorCode;
using base::GetLastSystemErrorCode;
using base::MakeSystemErrorCode;
using base::Exception;
using ::std::exception;


void ScreenSaver::Uninit() {
	timer_.stop();
}

bool ScreenSaver::Lock() {
	restore_ = GetScreenSaveSecure(isScreenSaverSecure_);
	bool commit = false;
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commit)(&restore_)) {
		if (!commit) {
			restore_ = false;
		}
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	if (!SetScreenSaveSecure(TRUE)) {
		CPPDEVTK_LOG_ERROR("SetScreenSaveSecure() failed");
		return false;
	}
	
	if (!SetActive(true)) {
		CPPDEVTK_LOG_ERROR("failed to activate screensaver");
		return false;
	}
	
	commit = true;
	return true;
}

bool ScreenSaver::SetActive(bool active) {
	if (active) {
#		if 0
		const LRESULT kLResult = SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_SCREENSAVE, 0);
		if (kLResult == 0) {
			CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_SYSCOMMAND + SC_SCREENSAVE to HWND_BROADCAST"
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			return false;
		}
#		else
		// GetDesktopWindow() is not documented to fail
		const LRESULT kLResult = SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
		if (kLResult != 0) {
			CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_SYSCOMMAND + SC_SCREENSAVE to DesktopWindow; kLResult: " << kLResult
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
			return false;
		}
#		endif
	}
	else {
		// Please see:
		// - How to force a screen saver to close once started:
		// https://support.microsoft.com/en-us/help/140723/how-to-force-a-screen-saver-to-close-once-started-in-windows-nt-window
		// - How do I start, detect and stop screen savers?: http://bobmoore.mvps.org/win32/w32tip22.htm
		
		HDESK hScreenSaverDesktop = OpenDesktop(CPPDEVTK_T("Screen-saver"), 0, FALSE,
				(DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS));
		if (hScreenSaverDesktop != NULL) {
			CPPDEVTK_LOG_DEBUG("the screensaver runs on its own desktop");
			
			CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hScreenSaverDesktop)) {
				if (!CloseDesktop(hScreenSaverDesktop)) {
					CPPDEVTK_LOG_WARN("Failed to CloseDesktop; errorCode: " << GetLastSystemErrorCode().ToString());
				}
			}
			CPPDEVTK_ON_BLOCK_EXIT_END
			
			if (!EnumDesktopWindows(hScreenSaverDesktop, StopScreenSaverWndProc, 0)) {
				CPPDEVTK_LOG_ERROR("failed to EnumDesktopWindows() for hScreenSaverDesktop; errorCode: "
						<< GetLastSystemErrorCode().ToString());
				return false;
			}
		}
		else {
			try {
				if (!IsActive()) {
					CPPDEVTK_LOG_DEBUG("screensaver already not active");
					return true;
				}
			}
			catch (const ::std::runtime_error& exc) {
				CPPDEVTK_LOG_ERROR("IsActive() failed; exc: " << Exception::GetDetailedInfo(exc));
				return false;
			}
			
			CPPDEVTK_LOG_DEBUG("the screensaver runs on the default desktop");
			
			HWND hForegroundWindow = GetForegroundWindow();
			if (hForegroundWindow != NULL) {
				const LRESULT kLResult = SendMessage(hForegroundWindow, WM_CLOSE, 0, 0);
				if (kLResult != 0) {
					CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_CLOSE to ForegroundWindow; kLResult: " << kLResult
							<< "; errorCode: " << GetLastSystemErrorCode().ToString());
					return false;
				}
			}
			else {
				CPPDEVTK_LOG_DEBUG("screensaver window lost activation (screensaver already deactivated)");
			}
		}
	}
	
	Refresh();
	
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
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_ERROR("IsActive() failed; exc: " << Exception::GetDetailedInfo(exc));
		return;
	}
	
	if (isActive_ != wasActive) {
		if (!isActive_) {
			if (restore_) {
				if (!SetScreenSaveSecure(isScreenSaverSecure_)) {
					CPPDEVTK_LOG_WARN("failed to restore isScreenSaverSecure_");
				}
				restore_ = false;
			}
		}
		
		Q_EMIT ActiveChanged(isActive_);
	}
}

ScreenSaver::ScreenSaver(): QObject(), ::cppdevtk::base::MeyersSingleton<ScreenSaver>(), timer_(), isActive_(IsActive()),
		restore_(false), isScreenSaverSecure_(FALSE) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	timer_.setInterval(1000);
	timer_.setSingleShot(false);
	CPPDEVTK_VERIFY(connect(&timer_, SIGNAL(timeout()), SLOT(Refresh())));
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
	timer_.start();
}

ScreenSaver::~ScreenSaver() {
	CPPDEVTK_ASSERT(!timer_.isActive() && "Call Uninit() before leaving main()");
}

bool ScreenSaver::SetScreenSaveSecure(BOOL value) {
#	ifndef SPI_SETSCREENSAVESECURE
#	define SPI_SETSCREENSAVESECURE 0x0077
#	endif
	
	CPPDEVTK_ASSERT((value == TRUE) || (value == FALSE));
	
	if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA) {
		if (!SystemParametersInfo(SPI_SETSCREENSAVESECURE, value, NULL, (SPIF_UPDATEINIFILE | SPIF_SENDCHANGE))) {
			CPPDEVTK_LOG_ERROR("SystemParametersInfo() failed for SPI_SETSCREENSAVESECURE; errorCode: "
					<< GetLastSystemErrorCode().ToString());
			return false;
		}
	}
	else {
		HKEY hKey = NULL;
		LONG retCode = RegOpenKeyEx(HKEY_CURRENT_USER, CPPDEVTK_T("Control Panel\\Desktop"), 0, KEY_WRITE, &hKey);
		if (retCode != ERROR_SUCCESS) {
			CPPDEVTK_LOG_ERROR("Failed to open Desktop registry key for writing; errorCode: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hKey)) {
			LONG retCode = RegCloseKey(hKey);
			if (retCode != ERROR_SUCCESS) {
				CPPDEVTK_LOG_WARN("Failed to close Desktop registry key; errorCode: "
						<< MakeSystemErrorCode(retCode).ToString());
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		const base::tstring kNumber = CPPDEVTK_Q2T(QString::number(value));
		CPPDEVTK_ASSERT(kNumber.length() == 1);
		retCode = RegSetValueEx(hKey, CPPDEVTK_T("ScreenSaverIsSecure"), 0, REG_SZ,
				(BYTE*)kNumber.c_str(), (DWORD)((kNumber.length() + 1) * sizeof(tchar)));
		if (retCode != ERROR_SUCCESS) {
			CPPDEVTK_LOG_ERROR("Failed to write ScreenSaverIsSecure registry value; errorCode: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
		}
	}
	
	return true;
}

bool ScreenSaver::GetScreenSaveSecure(BOOL& isScreenSaverSecure) {
#	ifndef SPI_GETSCREENSAVESECURE
#	define SPI_GETSCREENSAVESECURE 0x0076
#	endif
	
	if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA) {
		if (!SystemParametersInfo(SPI_GETSCREENSAVESECURE, 0, &isScreenSaverSecure, 0)) {
			CPPDEVTK_LOG_ERROR("SystemParametersInfo() failed for SPI_GETSCREENSAVESECURE; errorCode: "
					<< GetLastSystemErrorCode().ToString());
			return false;
		}
	}
	else {
		HKEY hKey = NULL;
		LONG retCode = RegOpenKeyEx(HKEY_CURRENT_USER, CPPDEVTK_T("Control Panel\\Desktop"), 0, KEY_READ, &hKey);
		if (retCode != ERROR_SUCCESS) {
			CPPDEVTK_LOG_ERROR("Failed to open Desktop registry key for reading; errorCode: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&hKey)) {
			LONG retCode = RegCloseKey(hKey);
			if (retCode != ERROR_SUCCESS) {
				CPPDEVTK_LOG_WARN("Failed to close Desktop registry key; errorCode: "
						<< MakeSystemErrorCode(retCode).ToString());
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		DWORD valueType = 0;
		BYTE buf[((1 + 1) + 1) * sizeof(tchar)] = {0};
		DWORD cbData = sizeof(buf) - sizeof(tchar);
		retCode = RegQueryValueEx(hKey, CPPDEVTK_T("ScreenSaverIsSecure"), NULL, &valueType, buf, &cbData);
		if (retCode != ERROR_SUCCESS) {
			CPPDEVTK_LOG_ERROR("Failed to read ScreenSaverIsSecure registry value; errorCode: "
					<< MakeSystemErrorCode(retCode).ToString());
			return false;
		}
		if (valueType != REG_SZ) {
			CPPDEVTK_LOG_ERROR("ScreenSaverIsSecure registry value does not have expected type");
			return false;
		}
		
		const QString kScreenSaverIsSecure = CPPDEVTK_T2Q((tchar*)buf);
		bool convOk = false;
		isScreenSaverSecure = kScreenSaverIsSecure.toInt(&convOk);
		if (!convOk) {
			CPPDEVTK_LOG_ERROR("Failed to convert ScreenSaverIsSecure registry value '" << kScreenSaverIsSecure << "' to BOOL");
			return false;
		}
	}
	
	CPPDEVTK_ASSERT((isScreenSaverSecure == TRUE) || (isScreenSaverSecure == FALSE));
	return true;
}


}	// namespace gui
}	// namespace cppdevtk


BOOL CALLBACK StopScreenSaverWndProc(HWND hwnd, LPARAM lParam) {
	::cppdevtk::base::SuppressUnusedWarning(lParam);
	
	if (IsWindowVisible(hwnd)) {
		const LRESULT kLResult = SendMessage(hwnd, WM_CLOSE, 0, 0);
		if (kLResult != 0) {
			CPPDEVTK_LOG_ERROR("failed to SendMessage() WM_CLOSE to hwnd: " << hwnd << "; kLResult: " << kLResult
					<< "; errorCode: " << ::cppdevtk::base::GetLastSystemErrorCode().ToString());
			return FALSE;
		}
	}
	return TRUE;
}
