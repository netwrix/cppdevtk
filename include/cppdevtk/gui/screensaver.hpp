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


#ifndef CPPDEVTK_GUI_SCREENSAVER_HPP_INCLUDED_
#define CPPDEVTK_GUI_SCREENSAVER_HPP_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is not for Android!!!"
#endif
#if (CPPDEVTK_PLATFORM_IOS)
#	error "This file is not for iOS!!!"
#endif

#include <cppdevtk/base/singletons.hpp>

#include <QtCore/QObject>

#if (CPPDEVTK_PLATFORM_LINUX)
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusInterface>
#include <memory>
#endif
#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <QtCore/QTimer>
#include <Windows.h>
#endif
#if (CPPDEVTK_PLATFORM_MACOSX)
#include <QtCore/QTimer>
#include <vector>
#endif


namespace cppdevtk {
namespace gui {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note
/// - All functions, except slots and destructor, may throw (DBusException on Linux, SystemException on Windows and Mac)
/// - Linux: supported screensavers: kde, cinnamon, mate, gnome, freedesktop
/// - Linux: uses session bus.
/// - On Windows the only screensaver notification is SC_SCREENSAVE that works only for top-level active window so we use timer...
/// - Mac: Carbon has no screensaver support, Cocoa had com.apple.screensaver.didstart, com.apple.screensaver.didstop,
/// com.apple.screenIsLocked, and com.apple.screenIsUnlocked but the last two were removed in Snow Leopard...
/// So Mac implementation is based on starting/polling ScreenSaverEngine...
/// Also on 10.13 there are no defaults for com.apple.screensaver askForPassword and askForPasswordDelay
/// so Lock() is in fact SetActive(true)...
/// \sa <a href=https://blog.kolide.com/screensaver-security-on-macos-10-13-is-broken-a385726e2ae2">Screensaver Security on macOS 10.13 is broken</a>
class CPPDEVTK_GUI_API ScreenSaver: public QObject, public ::cppdevtk::base::MeyersSingleton<ScreenSaver> {
	friend class ::cppdevtk::base::MeyersSingleton<ScreenSaver>;
	
	Q_OBJECT
Q_SIGNALS:
	/// \attention On Windows uses QTimer (lighter than a dedicated monitoring thread) so needs qApp and event loop
	void ActiveChanged(bool isActive);
public Q_SLOTS:
	// TODO: investigate why does not lock on Mac (no error reported but only starts screensaver)
	bool Lock();	///< Activates screensaver and lock screen on resume
	bool SetActive(bool active);
public:
	bool IsActive() const;
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	QDBusError GetLastError() const;
	static bool IsScreenSaverServiceRegistered();
#	endif
private Q_SLOTS:
	void Refresh();	// if we put slot in ifdef guards moc will ommit slot...
private:
#	if (CPPDEVTK_PLATFORM_MACOSX)
	enum AskForPassword {
		afpFalse,
		afpTrue,
		afpUnknown
	};
#	endif
	
	
	ScreenSaver();
	~ScreenSaver();
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	static void SetScreenSaveSecure(BOOL value);
	static BOOL GetScreenSaveSecure();
#	endif
#	if (CPPDEVTK_PLATFORM_MACOSX)
	static const char* GetScreenSaverEngineAppPath();
	static bool GetScreenSaverEnginePIds(::std::vector<int>& pids);
	
	static void SetAskForPassword(AskForPassword value);
	static AskForPassword GetAskForPassword();
	
	static void SetAskForPasswordDelay(int value);
	static int GetAskForPasswordDelay();
#	endif
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	::std::auto_ptr<QDBusInterface> pScreenSaverInterface_;
#	endif
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	QTimer timer_;
	bool isActive_;
	BOOL isScreenSaverSecure_;
#	endif
#	if (CPPDEVTK_PLATFORM_MACOSX)
	QTimer timer_;
	bool isActive_;
	AskForPassword askForPassword_;
	int askForPasswordDelay_;
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SCREENSAVER_HPP_INCLUDED_
