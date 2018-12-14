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


#include <cppdevtk/gui/power_notifier.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif

#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/verify.h>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace cppdevtk {
namespace gui {


using base::SuppressUnusedWarning;


PowerNotifier::WmPowerBroadcastWidget::WmPowerBroadcastWidget(PowerNotifier& powerNotifier):
		InvisibleWidget(), powerNotifier_(powerNotifier) {}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
bool PowerNotifier::WmPowerBroadcastWidget::nativeEvent(const QByteArray& eventType, void* pVMsg, long* pResult) {
	CPPDEVTK_ASSERT(eventType == "windows_generic_MSG");
	SuppressUnusedWarning(eventType);
	
	MSG* pMsg = static_cast<MSG*>(pVMsg);
#else
bool PowerNotifier::WmPowerBroadcastWidget::winEvent(MSG* pMsg, long* pResult) {
#endif
	CPPDEVTK_ASSERT(pMsg != NULL);
	SuppressUnusedWarning(pResult);
	
	if (pMsg->message == WM_POWERBROADCAST) {
		//CPPDEVTK_LOG_TRACE("got WM_POWERBROADCAST");
		switch (pMsg->wParam) {
			case PBT_APMSUSPEND:
				//CPPDEVTK_LOG_DEBUG("got PBT_APMSUSPEND");
				Q_EMIT powerNotifier_.Sleeping();
				break;
			case PBT_APMRESUMEAUTOMATIC:
				//CPPDEVTK_LOG_DEBUG("got PBT_APMRESUMEAUTOMATIC");
				Q_EMIT powerNotifier_.Resuming();
				break;
			default:
				break;
		}
	}
	
	return false;
}


void PowerNotifier::Uninit() {
	if (!wmPowerBroadcastWidget_.isHidden()) {
		CPPDEVTK_VERIFY(wmPowerBroadcastWidget_.close());
		CPPDEVTK_ASSERT(wmPowerBroadcastWidget_.isHidden());
	}
}

PowerNotifier::PowerNotifier(): QObject(), ::cppdevtk::base::MeyersSingleton<PowerNotifier>(),
		wmPowerBroadcastWidget_(*this) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	wmPowerBroadcastWidget_.show();
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

PowerNotifier::~PowerNotifier() {
	CPPDEVTK_ASSERT(wmPowerBroadcastWidget_.isHidden() && "Call Uninit() before leaving main()");
}


}	// namespace gui
}	// namespace cppdevtk
