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


#ifndef CPPDEVTK_GUI_POWER_NOTIFIER_HPP_INCLUDED_
#define CPPDEVTK_GUI_POWER_NOTIFIER_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_HAVE_POWER_NOTIFIER)
#	error "PowerNotifier is not available!!!"
#endif

#include <cppdevtk/base/singletons.hpp>

#include <QtCore/QObject>

#if (CPPDEVTK_PLATFORM_LINUX)
#include <memory>
#elif (CPPDEVTK_PLATFORM_WINDOWS)
#include "invisible_widget.hpp"
#include <Windows.h>
#elif (CPPDEVTK_PLATFORM_MACOSX)
#include <IOKit/IOTypes.h>
#include <IOKit/IOKitLib.h>
#else
#error "Unsupported platform!!!"
#endif


namespace cppdevtk {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation.

namespace gui {

class PowerNotifier;

}

namespace base {

#if (!defined(CPPDEVTK_GUI_POWER_NOTIFIER_CPP) || CPPDEVTK_COMPILER_CLANG)
CPPDEVTK_GUI_TMPL_EXPL_INST
#endif
template class CPPDEVTK_GUI_API MeyersSingleton< ::cppdevtk::gui::PowerNotifier>;

}	// namespace base


namespace gui {
#if (CPPDEVTK_PLATFORM_LINUX)
namespace detail {


class LogindPowerNotifier;
class UPowerNotifier;


}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note On Linux UPower and logind are supported.
class CPPDEVTK_GUI_API PowerNotifier: public QObject, public ::cppdevtk::base::MeyersSingleton<PowerNotifier> {
	friend class ::cppdevtk::base::MeyersSingleton<PowerNotifier>;
#	if (CPPDEVTK_PLATFORM_LINUX)
	friend class ::cppdevtk::gui::detail::LogindPowerNotifier;
	friend class ::cppdevtk::gui::detail::UPowerNotifier;
#	endif
	
	Q_OBJECT
Q_SIGNALS:
	void Sleeping();
	void Resuming();
public Q_SLOTS:
	void Uninit();	///< \attention Call before leaving main()
public:
#	if (CPPDEVTK_PLATFORM_LINUX)
	static bool IsPowerNotifierServiceRegistered();
#	endif
private:
#	if (CPPDEVTK_PLATFORM_LINUX)
#	if (__GNUC__ < 5)
	// workaround: although we declared LogindPowerNotifier and UPowerNotifier friends we get:
	// error: class cppdevtk::gui::PowerNotifier::Impl is private
	public:
#	endif
	class Impl;
#	if (__GNUC__ < 5)
	private:
#	endif
#	endif
	
#	if (CPPDEVTK_PLATFORM_WINDOWS)
	class WmPowerBroadcastWidget: public InvisibleWidget {
		// Error: Meta object features not supported for nested classes
		//Q_OBJECT
	public:
		explicit WmPowerBroadcastWidget(PowerNotifier& powerNotifier);
	protected:
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		virtual bool nativeEvent(const QByteArray& eventType, void* pVMsg, long* pResult);
#		else
		virtual bool winEvent(MSG* pMsg, long* pResult);
#		endif
	private:
		Q_DISABLE_COPY(WmPowerBroadcastWidget);
		
		
		PowerNotifier& powerNotifier_;
	};
#	endif
	
	
	Q_DISABLE_COPY(PowerNotifier);
	
	PowerNotifier();
	~PowerNotifier();
	
	
#	if (CPPDEVTK_PLATFORM_MACOSX)
	static void IOSystemPowerCallback(void* pRefcon, io_service_t service, uint32_t messageType, void* pMessageArgument);
#	endif
	
	
#	if (CPPDEVTK_PLATFORM_LINUX)
	::std::auto_ptr<Impl> pImpl_;
#	elif (CPPDEVTK_PLATFORM_WINDOWS)
	WmPowerBroadcastWidget wmPowerBroadcastWidget_;
#	elif (CPPDEVTK_PLATFORM_MACOSX)
	io_connect_t spSession_;
	IONotificationPortRef spIONotificationPortRef_;
	io_object_t spNotifierObject_;
#	else
#	error "Unsupported platform!!!"
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_POWER_NOTIFIER_HPP_INCLUDED_
