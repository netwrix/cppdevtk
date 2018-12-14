/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_GUI_SESSION_HPP_INCLUDED_
#define CPPDEVTK_GUI_SESSION_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_HAVE_SESSION)
#	error "Session is not available!!!"
#endif

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QString>

#if (CPPDEVTK_PLATFORM_LINUX)
#include <memory>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#include "invisible_widget.hpp"
#include <windows.h>
#elif (CPPDEVTK_PLATFORM_MACOSX)
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Security/AuthSession.h>
#ifdef QT_MAC_USE_COCOA
#ifdef __OBJC__
#include <Foundation/Foundation.h>
#else
#include <objc/objc-runtime.h>
#endif
#endif
#else
#error "Unsupported platform!!!"
#endif


#if (CPPDEVTK_PLATFORM_MACOSX)
#ifdef QT_MAC_USE_COCOA
#ifdef __OBJC__
@class CocoaSession;
#else
typedef struct objc_object CocoaSession;
#endif
#endif
#endif


namespace cppdevtk {
namespace gui {
#if (CPPDEVTK_PLATFORM_LINUX)
namespace detail {


class LogindSession;
class ConsoleKitSession;

class LogindManager;
class ConsoleKitManager;


}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note On Linux Logind and ConsoleKit sessions are supported
class CPPDEVTK_GUI_API Session: public QObject {
	friend class SessionManager;
#	if (CPPDEVTK_PLATFORM_LINUX)
	friend class ::cppdevtk::gui::detail::LogindSession;
	friend class ::cppdevtk::gui::detail::ConsoleKitSession;
	friend class ::cppdevtk::gui::detail::LogindManager;
	friend class ::cppdevtk::gui::detail::ConsoleKitManager;
#	endif
	
	Q_OBJECT
Q_SIGNALS:
	void Activated();
	void Deactivated();
	
	void Locked();	///< \attention Not emitted on Mac OS X
	void Unlocked();	///< \attention Not emitted on Mac OS X
public Q_SLOTS:
	/// \attention On Mac require >= 10.7
	/// \remark On Linux when locking session from UI screensaver is used and we receive screensaver signals
	bool Lock();
public:
#	if (CPPDEVTK_PLATFORM_LINUX)
	typedef qulonglong IdleTime;
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	typedef unsigned long long int IdleTime;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	typedef DWORD IdleTime;
#	else
#	error "Unsupported platform!!!"
#	endif
	
	virtual ~Session();
	
	QString GetId() const;
	IdleTime GetIdleTime() const;	///< \return The time, in milliseconds, elapsed since the last input event.
	
	bool operator==(const Session& other) const;
	bool operator!=(const Session& other) const;
private:
#	if (CPPDEVTK_PLATFORM_LINUX)
#	if (__GNUC__ < 5)
	// workaround: although we declared friends we get: error: class cppdevtk::gui::Session::Impl is private
	public:
#	endif
	class Impl;
#	if (__GNUC__ < 5)
	private:
#	endif
#	endif
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	class WmWtSessionChange: public InvisibleWidget {
		// Error: Meta object features not supported for nested classes
		//Q_OBJECT
	public:
		explicit WmWtSessionChange(Session& session);
		virtual ~WmWtSessionChange();
	protected:
		virtual void showEvent(QShowEvent* pShowEvent);
		virtual void changeEvent(QEvent* pEvent);
		
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		virtual bool nativeEvent(const QByteArray& eventType, void* pVMsg, long* pResult);
#		else
		virtual bool winEvent(MSG* pMsg, long* pResult);
#		endif
	private:
		Q_DISABLE_COPY(WmWtSessionChange);
		
		HWND GetHWnd() const;
		
		
		Session& session_;
		HWND hWnd_;
	};
#	endif
	
	
	Q_DISABLE_COPY(Session);
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	Session(const QString& service, const QString& path);
#	else
	Session();
#	endif
	
	
#	if (CPPDEVTK_PLATFORM_MACOSX)
	static SecuritySessionId GetCallerSecuritySessionId();
#	ifndef QT_MAC_USE_COCOA
	static OSStatus OnUserSessionChanged(EventHandlerCallRef nextHandler, EventRef theEvent, void* pUserData);
#	endif
#	endif
	
	
	QString id_;
#	if (CPPDEVTK_PLATFORM_LINUX)
	::std::auto_ptr<Impl> pImpl_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	WmWtSessionChange wmWtSessionChange_;
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#	ifdef QT_MAC_USE_COCOA
	CocoaSession* pCocoaSession_;
#	else
	EventHandlerUPP eventHandlerUPP_;
#	endif
#	else
#	error "Unsupported platform!!!"
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline QString Session::GetId() const {
	return id_;
}

inline bool Session::operator==(const Session& other) const {
	return GetId() == other.GetId();
}

inline bool Session::operator!=(const Session& other) const {
	return GetId() != other.GetId();
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SESSION_HPP_INCLUDED_
