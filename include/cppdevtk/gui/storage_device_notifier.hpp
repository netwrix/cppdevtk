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


#ifndef CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_HPP_INCLUDED_
#define CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_HAVE_STORAGE_DEVICE_NOTIFIER)
#	error "StorageDeviceNotifier is not available!!!"
#endif

#include <cppdevtk/base/singletons.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>

#if (CPPDEVTK_PLATFORM_LINUX)
#include <QtDBus/QDBusObjectPath>
#include <memory>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#include "invisible_widget.hpp"
#include <Windows.h>
#elif (CPPDEVTK_PLATFORM_MACOSX)
#include <Carbon/Carbon.h>
#else
#error "Unsupported platform!!!"
#endif


namespace cppdevtk {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation.

namespace gui {

class StorageDeviceNotifier;

}

namespace base {

#if (!defined(CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_CPP) || CPPDEVTK_COMPILER_CLANG)
CPPDEVTK_GUI_TMPL_EXPL_INST
#endif
template class CPPDEVTK_GUI_API MeyersSingleton< ::cppdevtk::gui::StorageDeviceNotifier>;

}	// namespace base


namespace gui {
#if (CPPDEVTK_PLATFORM_LINUX)
namespace detail {


class UDisks2DeviceNotifier;
class UDisks1DeviceNotifier;


}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note On Linux UDisks2 and UDisks1 are supported.
class CPPDEVTK_GUI_API StorageDeviceNotifier: public QObject, public ::cppdevtk::base::MeyersSingleton<StorageDeviceNotifier> {
	friend class ::cppdevtk::base::MeyersSingleton<StorageDeviceNotifier>;
#	if (CPPDEVTK_PLATFORM_LINUX)
	friend class ::cppdevtk::gui::detail::UDisks2DeviceNotifier;
	friend class ::cppdevtk::gui::detail::UDisks1DeviceNotifier;
#	endif
	
	Q_OBJECT
public:
#	if (CPPDEVTK_PLATFORM_LINUX)
	typedef QDBusObjectPath StorageDeviceId;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	typedef DWORD StorageDeviceId;
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	typedef FSVolumeRefNum StorageDeviceId;
#	else
#	error "Unsupported platform!!!"
#	endif
Q_SIGNALS:
	void StorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void StorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	
	void StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
	void StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId storageDeviceId);
public Q_SLOTS:
	void Uninit();	///< \attention Call before leaving main()
public:
#	if (CPPDEVTK_PLATFORM_LINUX)
	static bool IsStorageDeviceNotifierServiceRegistered();
#	endif
	
	/// Storage device name has the form:
	/// - Linux: device name (includes _PATH_DEV (/dev/)); ex: /dev/sr1
	/// - Windows: logical drive string (uppercase, ends with /); ex: D:/
	/// - Mac OS X: BSD name; ex: disk2
	/// \return Empty string if no error and no device name found.
	static QString GetStorageDeviceName(StorageDeviceId storageDeviceId);
	
	/// Storage device name has the form:
	/// - Linux: device name (includes _PATH_DEV (/dev/)); ex: /dev/sr1
	/// - Windows: logical drive string (uppercase, ends with /); ex: D:/
	/// - Mac OS X: BSD name; ex: disk2
	static StorageDeviceId GetStorageDeviceId(const QString& storageDeviceName);
	
	static bool IsEqual(StorageDeviceId sdId1, StorageDeviceId sdId2);
private:
#	if (CPPDEVTK_PLATFORM_LINUX)
#	if (__GNUC__ < 5)
	// workaround: although we declared UDisksXDeviceNotifier friends we get:
	// error: class cppdevtk::gui::StorageDeviceNotifier::Impl is private
	public:
#	endif
	class Impl;
#	if (__GNUC__ < 5)
	private:
#	endif
#	endif
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	class WmDeviceChangeWidget: public InvisibleWidget {
		// Error: Meta object features not supported for nested classes
		//Q_OBJECT
	public:
		explicit WmDeviceChangeWidget(StorageDeviceNotifier& storageDeviceNotifier);
	protected:
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		virtual bool nativeEvent(const QByteArray& eventType, void* pVMsg, long* pResult);
#		else
		virtual bool winEvent(MSG* pMsg, long* pResult);
#		endif
	private:
		Q_DISABLE_COPY(WmDeviceChangeWidget);
		
		
		StorageDeviceNotifier& storageDeviceNotifier_;
	};
#	endif
	
	
	Q_DISABLE_COPY(StorageDeviceNotifier);
	
	StorageDeviceNotifier();
	~StorageDeviceNotifier();
	
#	if (CPPDEVTK_PLATFORM_LINUX)
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	void IterateDeviceArrivalLogicalUnitMask(DWORD dbcvUnitMask);
	void IterateDeviceRemoveCompleteLogicalUnitMask(DWORD dbcvUnitMask);
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	static OSStatus VolumeEventsHandler(EventHandlerCallRef nextHandler, EventRef theEvent, void* pUserData);
#	else
#	error "Unsupported platform!!!"
#	endif
	
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	::std::auto_ptr<Impl> pImpl_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	WmDeviceChangeWidget wmDeviceChangeWidget_;
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	EventHandlerUPP eventHandlerUPP_;
#	else
#	error "Unsupported platform!!!"
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_STORAGE_DEVICE_NOTIFIER_HPP_INCLUDED_
