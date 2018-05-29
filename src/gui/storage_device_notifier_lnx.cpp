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


#include <cppdevtk/gui/storage_device_notifier.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif

#include "udisks2_device_notifier_lnx.hpp"
#include "udisks1_device_notifier_lnx.hpp"
#include <cppdevtk/util/dbus_exception.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/base/cassert.hpp>

#include <QtDBus/QDBusError>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <cstddef>
#include <new>

#include <paths.h>


namespace cppdevtk {
namespace gui {


using detail::UDisks2DeviceNotifier;
using detail::UDisks1DeviceNotifier;


void StorageDeviceNotifier::Uninit() {
	if (pImpl_.get() != NULL) {
		pImpl_.reset();
	}
}

bool StorageDeviceNotifier::IsStorageDeviceNotifierServiceRegistered() {
	return UDisks2DeviceNotifier::IsUDisks2ServiceRegistered()
			|| UDisks1DeviceNotifier::IsUDisks1ServiceRegistered();
}

QString StorageDeviceNotifier::GetStorageDeviceName(StorageDeviceId storageDeviceId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(storageDeviceId.path().isEmpty(), "storageDeviceId");
	
	if (UDisks2DeviceNotifier::IsUDisks2ServiceRegistered()) {
		return UDisks2DeviceNotifier::GetStorageDeviceName(storageDeviceId);
	}
	else {
		if (UDisks1DeviceNotifier::IsUDisks1ServiceRegistered()) {
			return UDisks1DeviceNotifier::GetStorageDeviceName(storageDeviceId);
		}
		else {
			throw CPPDEVTK_DBUS_EXCEPTION("None of UDisks2/UDisks1 services is registered",
					QDBusError(QDBusError::ServiceUnknown, "StorageDeviceNotifier ServiceUnknown"));
		}
	}
}

StorageDeviceNotifier::StorageDeviceId StorageDeviceNotifier::GetStorageDeviceId(const QString& storageDeviceName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(storageDeviceName.startsWith(_PATH_DEV), "storageDeviceName must start with _PATH_DEV");
	
	if (UDisks2DeviceNotifier::IsUDisks2ServiceRegistered()) {
		return UDisks2DeviceNotifier::GetStorageDeviceId(storageDeviceName);
	}
	else {
		if (UDisks1DeviceNotifier::IsUDisks1ServiceRegistered()) {
			return UDisks1DeviceNotifier::GetStorageDeviceId(storageDeviceName);
		}
		else {
			throw CPPDEVTK_DBUS_EXCEPTION("None of UDisks2/UDisks1 services is registered",
					QDBusError(QDBusError::ServiceUnknown, "StorageDeviceNotifier ServiceUnknown"));
		}
	}
}

StorageDeviceNotifier::StorageDeviceNotifier(): QObject(), ::cppdevtk::base::MeyersSingleton<StorageDeviceNotifier>(),
		pImpl_() {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	if (UDisks2DeviceNotifier::IsUDisks2ServiceRegistered()) {
		pImpl_.reset(new UDisks2DeviceNotifier());
	}
	else {
		if (UDisks1DeviceNotifier::IsUDisks1ServiceRegistered()) {
			pImpl_.reset(new UDisks1DeviceNotifier());
		}
		else {
			throw CPPDEVTK_DBUS_EXCEPTION("None of UDisks2/UDisks1 services is registered",
					QDBusError(QDBusError::ServiceUnknown, "ServiceUnknown"));
		}
	}
	
	CPPDEVTK_ASSERT(pImpl_.get() != NULL);
	
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(StorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
			SIGNAL(StorageDeviceAdded(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(StorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
			SIGNAL(StorageDeviceRemoved(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
			SIGNAL(StorageDeviceMounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
	CPPDEVTK_VERIFY(connect(pImpl_.get(), SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId)),
			SIGNAL(StorageDeviceUnmounted(::cppdevtk::gui::StorageDeviceNotifier::StorageDeviceId))));
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

StorageDeviceNotifier::~StorageDeviceNotifier() {
	CPPDEVTK_ASSERT((pImpl_.get() == NULL) && "Call Uninit() before leaving main()");
}


}	// namespace gui
}	// namespace cppdevtk
