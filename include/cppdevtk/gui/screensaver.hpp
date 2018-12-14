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


#ifndef CPPDEVTK_GUI_SCREENSAVER_HPP_INCLUDED_
#define CPPDEVTK_GUI_SCREENSAVER_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_HAVE_SCREENSAVER)
#	error "ScreenSaver is not available!!!"
#endif

#include <cppdevtk/base/singletons.hpp>

#include <QtCore/QObject>

#if (CPPDEVTK_PLATFORM_LINUX)
#include <QtDBus/QDBusInterface>
#include <memory>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#include <QtCore/QTimer>
#include <Windows.h>
#elif (CPPDEVTK_PLATFORM_MACOSX)
#include <vector>
#include <Carbon/Carbon.h>
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
@class CocoaScreenSaver;
#else
typedef struct objc_object CocoaScreenSaver;
#endif
#endif
#endif


namespace cppdevtk {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation.

namespace gui {

class ScreenSaver;

}

namespace base {

#if (!defined(CPPDEVTK_GUI_SCREENSAVER_CPP) || CPPDEVTK_COMPILER_CLANG)
CPPDEVTK_GUI_TMPL_EXPL_INST
#endif
template class CPPDEVTK_GUI_API MeyersSingleton< ::cppdevtk::gui::ScreenSaver>;

}	// namespace base


namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note
/// - All functions, except slots and destructor, may throw (DBusException on Linux, RuntimeException + SystemException on Windows and Mac)
/// - Linux: supported screensavers (session bus): kde, cinnamon, mate, gnome, freedesktop
/// - Mac: On 10.13 screensaver API changed (there are no defaults for com.apple.screensaver askForPassword and askForPasswordDelay)
/// No time to investigate now so Lock() is in fact SetActive(true)...
/// \sa <a href=https://blog.kolide.com/screensaver-security-on-macos-10-13-is-broken-a385726e2ae2">Screensaver Security on macOS 10.13 is broken</a>
class CPPDEVTK_GUI_API ScreenSaver: public QObject, public ::cppdevtk::base::MeyersSingleton<ScreenSaver> {
	friend class ::cppdevtk::base::MeyersSingleton<ScreenSaver>;
	
	Q_OBJECT
Q_SIGNALS:
	void ActiveChanged(bool isActive);
public Q_SLOTS:
	void Uninit();	///< \attention Call before leaving main()
	
	// TODO: investigate why does not lock on Mac (no error reported but only starts screensaver even if in settings lock is enabled)
	bool Lock();	///< Activates screensaver and lock screen on resume
	bool SetActive(bool active);
public:
	bool IsActive() const;
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	static bool IsScreenSaverServiceRegistered();
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
#	elif (CPPDEVTK_PLATFORM_MACOSX)
#	ifdef QT_MAC_USE_COCOA
	void OnScreenSaverDidStart();
	void OnScreenSaverDidStop();
#	endif
#	else
#	error "Unsupported platform!!!"
#	endif
private Q_SLOTS:
	// if we put slot in ifdef guards moc will ommit slot...
	void Refresh();
private:
#	if (CPPDEVTK_PLATFORM_MACOSX)
	enum AskForPassword {
		afpFalse,
		afpTrue,
		afpUnknown
	};
#	endif
	
	
	Q_DISABLE_COPY(ScreenSaver);
	
	ScreenSaver();
	~ScreenSaver();

#	if (CPPDEVTK_PLATFORM_LINUX)	
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	static bool SetScreenSaveSecure(BOOL value);
	static bool GetScreenSaveSecure(BOOL& isScreenSaverSecure);
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	void CheckActiveChanged(bool wasActive);
	
	static const char* GetScreenSaverEngineAppPath();
	static bool GetScreenSaverEnginePIds(::std::vector<int>& pids);
	static OSErr GetProcessName(const ProcessSerialNumber& processSerialNumber, QString& processName);
	
	static void SetAskForPassword(AskForPassword value);
	static AskForPassword GetAskForPassword();
	
	static void SetAskForPasswordDelay(int value);
	static int GetAskForPasswordDelay();
	
#	ifndef QT_MAC_USE_COCOA
	static OSStatus OnAppFrontSwitched(EventHandlerCallRef nextHandler, EventRef theEvent, void* pUserData);
#	endif
#	else
#	error "Unsupported platform!!!"
#	endif
	
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	::std::auto_ptr<QDBusInterface> pScreenSaverInterface_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	QTimer timer_;
	bool isActive_;
	bool restore_;
	BOOL isScreenSaverSecure_;
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	bool isActive_;
	bool restore_;
	AskForPassword askForPassword_;
	int askForPasswordDelay_;
#	ifdef QT_MAC_USE_COCOA
	CocoaScreenSaver* pCocoaScreenSaver_;
#	else
	EventHandlerUPP eventHandlerUPP_;
#	endif
#	else
#	error "Unsupported platform!!!"
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SCREENSAVER_HPP_INCLUDED_
