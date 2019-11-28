/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif


#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/util/filesystem_utils.hpp>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <Dbt.h>

#include <cstddef>


namespace cppdevtk {
namespace gui {


using base::SuppressUnusedWarning;


StorageDeviceNotifier::WmDeviceChangeWidget::WmDeviceChangeWidget(StorageDeviceNotifier& storageDeviceNotifier):
		InvisibleWidget(), storageDeviceNotifier_(storageDeviceNotifier) {}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
bool StorageDeviceNotifier::WmDeviceChangeWidget::nativeEvent(const QByteArray& eventType, void* pVMsg, long* pResult) {
	CPPDEVTK_ASSERT(eventType == "windows_generic_MSG");
	SuppressUnusedWarning(eventType);
	
	MSG* pMsg = static_cast<MSG*>(pVMsg);
#else
bool StorageDeviceNotifier::WmDeviceChangeWidget::winEvent(MSG* pMsg, long* pResult) {
#endif
	CPPDEVTK_ASSERT(pMsg != NULL);
	SuppressUnusedWarning(pResult);
	
	if (pMsg->message == WM_DEVICECHANGE) {
		//CPPDEVTK_LOG_TRACE("got WM_DEVICECHANGE");
		switch (pMsg->wParam) {
			case DBT_DEVICEARRIVAL: {
					//CPPDEVTK_LOG_DEBUG("got DBT_DEVICEARRIVAL");
					DEV_BROADCAST_HDR* pDevBroadcastHdr = (DEV_BROADCAST_HDR*)pMsg->lParam;
					CPPDEVTK_ASSERT(pDevBroadcastHdr != NULL);
					if (pDevBroadcastHdr->dbch_devicetype == DBT_DEVTYP_VOLUME) {
						//CPPDEVTK_LOG_DEBUG("got DBT_DEVICEARRIVAL + DBT_DEVTYP_VOLUME");
						DEV_BROADCAST_VOLUME* pDevBroadcastVolume = (DEV_BROADCAST_VOLUME*)pDevBroadcastHdr;
						//CPPDEVTK_LOG_DEBUG("DBT_DEVICEARRIVAL: DBTF_MEDIA: " << (pDevBroadcastVolume->dbcv_flags & DBTF_MEDIA)
						//		<< "DBTF_NET: " << (pDevBroadcastVolume->dbcv_flags & DBTF_NET));
						storageDeviceNotifier_.IterateDeviceArrivalLogicalUnitMask(pDevBroadcastVolume->dbcv_unitmask);
					}
				}
				break;
			case DBT_DEVICEREMOVECOMPLETE: {
					//CPPDEVTK_LOG_DEBUG("got DBT_DEVICEREMOVECOMPLETE");
					DEV_BROADCAST_HDR* pDevBroadcastHdr = (DEV_BROADCAST_HDR*)pMsg->lParam;
					CPPDEVTK_ASSERT(pDevBroadcastHdr != NULL);
					if (pDevBroadcastHdr->dbch_devicetype == DBT_DEVTYP_VOLUME) {
						//CPPDEVTK_LOG_DEBUG("got DBT_DEVICEREMOVECOMPLETE + DBT_DEVTYP_VOLUME");
						DEV_BROADCAST_VOLUME* pDevBroadcastVolume = (DEV_BROADCAST_VOLUME*)pDevBroadcastHdr;
						//CPPDEVTK_LOG_DEBUG("DBT_DEVICEREMOVECOMPLETE: DBTF_MEDIA: " << (pDevBroadcastVolume->dbcv_flags & DBTF_MEDIA)
						//		<< "DBTF_NET: " << (pDevBroadcastVolume->dbcv_flags & DBTF_NET));
						storageDeviceNotifier_.IterateDeviceRemoveCompleteLogicalUnitMask(pDevBroadcastVolume->dbcv_unitmask);
					}
				}
				break;
			default:
				break;
		}
	}
	
	return false;
}


void StorageDeviceNotifier::Uninit() {
	if (!wmDeviceChangeWidget_.isHidden()) {
		CPPDEVTK_VERIFY(wmDeviceChangeWidget_.close());
		CPPDEVTK_ASSERT(wmDeviceChangeWidget_.isHidden());
	}
}

QString StorageDeviceNotifier::GetStorageDeviceName(StorageDeviceId storageDeviceId) {
	for (char cnt = 0; cnt < 26; ++cnt) {
		if (storageDeviceId & 0x1) {
			if (storageDeviceId >> 1) {
				throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION(
						QString("The logical unit mask storageDeviceId %1 contains multiple logical units").arg(
						QString::number(storageDeviceId, 2)));
			}
			return QString((char)(cnt + 'A')) + ":/";
		}
		storageDeviceId >>= 1;
	}
	throw CPPDEVTK_EMPTY_ARGUMENT_EXCEPTION("storageDeviceId");
}

StorageDeviceNotifier::StorageDeviceId StorageDeviceNotifier::GetStorageDeviceId(const QString& storageDeviceName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((storageDeviceName.length() == 3), "storageDeviceName.length() != 3");
	CPPDEVTK_DBC_CHECK_ARGUMENT(storageDeviceName.endsWith('/'), "!storageDeviceName.endsWith('/')");
	CPPDEVTK_DBC_CHECK_ARGUMENT((storageDeviceName[1] == ':'), "storageDeviceName[1] != ':'");
	CPPDEVTK_DBC_CHECK_ARGUMENT(storageDeviceName[0].isLetter(), "!storageDeviceName[0].isLetter()");
	
	const char kDriveLetter = storageDeviceName[0].toUpper().toLatin1();
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((('A' <= kDriveLetter) && (kDriveLetter <= 'Z')),
			"storageDeviceName has invalid drive letter");
	
	StorageDeviceId storageDeviceId = 1;
	for (char driveLetter = 'A'; driveLetter < kDriveLetter; ++driveLetter) {
		storageDeviceId <<= 1;
	}
	
	return storageDeviceId;
}

bool StorageDeviceNotifier::IsEqual(StorageDeviceId sdId1, StorageDeviceId sdId2) {
	return sdId1 == sdId2;
}

StorageDeviceNotifier::StorageDeviceNotifier(): QObject(), ::cppdevtk::base::MeyersSingleton<StorageDeviceNotifier>(),
		wmDeviceChangeWidget_(*this) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	wmDeviceChangeWidget_.show();
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

StorageDeviceNotifier::~StorageDeviceNotifier() {
	CPPDEVTK_ASSERT(wmDeviceChangeWidget_.isHidden() && "Call Uninit() before leaving main()");
}

void StorageDeviceNotifier::IterateDeviceArrivalLogicalUnitMask(DWORD dbcvUnitMask) {
	for (int cnt = 0; cnt < 26; ++cnt) {
		if (dbcvUnitMask & 0x1) {
			const DWORD kLogicalDrive = 1 << cnt;
			Q_EMIT StorageDeviceAdded(kLogicalDrive);
			Q_EMIT StorageDeviceMounted(kLogicalDrive);
		}
		dbcvUnitMask >>= 1;
	}
}

void StorageDeviceNotifier::IterateDeviceRemoveCompleteLogicalUnitMask(DWORD dbcvUnitMask) {
	for (int cnt = 0; cnt < 26; ++cnt) {
		if (dbcvUnitMask & 0x1) {
			const DWORD kLogicalDrive = 1 << cnt;
			Q_EMIT StorageDeviceUnmounted(kLogicalDrive);
			Q_EMIT StorageDeviceRemoved(kLogicalDrive);
		}
		dbcvUnitMask >>= 1;
	}
}


}	// namespace gui
}	// namespace cppdevtk
