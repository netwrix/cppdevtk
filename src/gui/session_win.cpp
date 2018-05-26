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


#include <cppdevtk/gui/session.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/get_current_process_id.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/zeroize.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <qpa/qplatformnativeinterface.h>
#else
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#endif
#include <QtGui/QWindow>

#include <cstddef>

#include <Wtsapi32.h>


namespace cppdevtk {
namespace gui {


using base::GetLastSystemErrorCode;
using base::ErrorCode;


Session::WmWtSessionChange::WmWtSessionChange(Session& session): InvisibleWidget(), session_(session), hWnd_(NULL) {}

Session::WmWtSessionChange::~WmWtSessionChange() {
	if (hWnd_ != NULL) {
		if (!WTSUnRegisterSessionNotification(hWnd_)) {
			CPPDEVTK_LOG_WARN("Failed to unregister session notification for hWnd_: " << hWnd_
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		}
	}
}

void Session::WmWtSessionChange::showEvent(QShowEvent* pShowEvent) {
	CPPDEVTK_ASSERT(pShowEvent != NULL);
	
	InvisibleWidget::showEvent(pShowEvent);
	
	if (hWnd_ != NULL) {
		return;
	}
	
	hWnd_ = GetHWnd();
	if (hWnd_ != NULL) {
		if (!WTSRegisterSessionNotification(hWnd_, NOTIFY_FOR_THIS_SESSION)) {
			CPPDEVTK_LOG_WARN("Failed to register session notification for hWnd_: " << hWnd_
					<< "; errorCode: " << GetLastSystemErrorCode().ToString());
		}
	}
	else {
		CPPDEVTK_LOG_WARN("Got NULL hWnd_");
	}
}

void Session::WmWtSessionChange::changeEvent(QEvent* pEvent) {
	CPPDEVTK_ASSERT(pEvent != NULL);
	
	InvisibleWidget::changeEvent(pEvent);
	
    switch (pEvent->type()) {
		case QEvent::WinIdChange:
			if (hWnd_ != NULL) {
				if (!WTSUnRegisterSessionNotification(hWnd_)) {
					CPPDEVTK_LOG_WARN("Failed to unregister session notification for hWnd_: " << hWnd_
							<< "; errorCode: " << GetLastSystemErrorCode().ToString());
				}
			}
			hWnd_ = GetHWnd();
			if (hWnd_ != NULL) {
				if (!WTSRegisterSessionNotification(hWnd_, NOTIFY_FOR_THIS_SESSION)) {
					CPPDEVTK_LOG_WARN("Failed to register session notification for hWnd_: " << hWnd_
							<< "; errorCode: " << GetLastSystemErrorCode().ToString());
				}
			}
			else {
				CPPDEVTK_LOG_WARN("Got NULL hWnd_");
			}
			break;
		default:
			break;
    }
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
bool Session::WmWtSessionChange::nativeEvent(const QByteArray& eventType, void* pVMsg, long* pResult) {
	base::SuppressUnusedWarning(eventType);
	
	MSG* pMsg = static_cast<MSG*>(pVMsg);
#else
bool Session::WmWtSessionChange::winEvent(MSG* pMsg, long* pResult) {
#endif
	CPPDEVTK_ASSERT(pMsg != NULL);
	base::SuppressUnusedWarning(pResult);
	
	// On switch user we get WTS_CONSOLE_DISCONNECT + WTS_SESSION_LOCK
	// and when switching back we get WTS_CONSOLE_CONNECT + WTS_SESSION_UNLOCK
	if (pMsg->message == WM_WTSSESSION_CHANGE) {
		//CPPDEVTK_LOG_TRACE("got WM_WTSSESSION_CHANGE");
		switch (pMsg->wParam) {
			case WTS_CONSOLE_CONNECT:
				//CPPDEVTK_LOG_DEBUG("got WTS_CONSOLE_CONNECT");
				Q_EMIT session_.Activated();
				break;
			case WTS_REMOTE_CONNECT:
				//CPPDEVTK_LOG_DEBUG("got WTS_REMOTE_CONNECT");
				Q_EMIT session_.Activated();
				break;
			case WTS_CONSOLE_DISCONNECT:
				//CPPDEVTK_LOG_DEBUG("got WTS_CONSOLE_DISCONNECT");
				Q_EMIT session_.Deactivated();
				break;
			case WTS_REMOTE_DISCONNECT:
				//CPPDEVTK_LOG_DEBUG("got WTS_REMOTE_DISCONNECT");
				Q_EMIT session_.Deactivated();
				break;
			case WTS_SESSION_LOGON:
				//CPPDEVTK_LOG_DEBUG("got WTS_SESSION_LOGON");
				//Q_EMIT session_.Activated();
				break;
			case WTS_SESSION_LOGOFF:
				//CPPDEVTK_LOG_DEBUG("got WTS_SESSION_LOGOFF");
				//Q_EMIT session_.Deactivated();
				break;
			case WTS_SESSION_LOCK:
				//CPPDEVTK_LOG_DEBUG("got WTS_SESSION_LOCK");
				Q_EMIT session_.Locked();
				break;
			case WTS_SESSION_UNLOCK:
				//CPPDEVTK_LOG_DEBUG("got WTS_SESSION_UNLOCK");
				Q_EMIT session_.Unlocked();
				break;
			default:
				break;
		}
	}
	
	return false;
}

HWND Session::WmWtSessionChange::GetHWnd() const {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	HWND hWnd = NULL;
	
	QWindow* pWindow = windowHandle();
	if (pWindow == NULL) {
		const QWidget* pWidget = nativeParentWidget();
		if (pWidget != NULL) {
			pWindow = pWidget->windowHandle();
		}
	}
	if (pWindow != NULL) {
		hWnd = static_cast<HWND>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(
				QByteArrayLiteral("handle"), pWindow));
	}
	
	return hWnd;
#	else
	return winId();
#	endif
}


bool Session::Lock() {
	if (!LockWorkStation()) {
		const ErrorCode kErrorCode = GetLastSystemErrorCode();
		CPPDEVTK_LOG_ERROR("LockWorkStation() failed; errorCode: " << kErrorCode.ToString());
		return false;
	}
	
	return true;
}

Session::~Session() {}

Session::IdleTime Session::GetIdleTime() const {
	LASTINPUTINFO lastInputInfo;
	base::Zeroize(&lastInputInfo, sizeof lastInputInfo);
	lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
	if (!GetLastInputInfo(&lastInputInfo)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "GetLastInputInfo() failed");
	}
	
	const DWORD kTickCount = GetTickCount();
	
	return (kTickCount - lastInputInfo.dwTime);
}

Session::Session(): QObject(), id_(), wmWtSessionChange_(*this) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	DWORD sessionId = 0;
	if (!ProcessIdToSessionId(base::GetCurrentProcessId(), &sessionId)) {
		throw CPPDEVTK_SYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "ProcessIdToSessionId() failed");
	}
	id_ = QString::number(sessionId);
	CPPDEVTK_ASSERT(!id_.isEmpty());
	
	wmWtSessionChange_.show();
}


}	// namespace gui
}	// namespace cppdevtk
