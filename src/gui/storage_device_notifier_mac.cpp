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
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/verify.h>

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include <CoreServices/CoreServices.h>

#include <cstddef>


namespace cppdevtk {
namespace gui {


void StorageDeviceNotifier::Uninit() {
	if (eventHandlerUPP_ != NULL) {
		DisposeEventHandlerUPP(eventHandlerUPP_);
		eventHandlerUPP_ = NULL;
	}
}

StorageDeviceNotifier::StorageDeviceNotifier(): QObject(), ::cppdevtk::base::MeyersSingleton<StorageDeviceNotifier>(),
		eventHandlerUPP_(NULL) {
	CPPDEVTK_DBC_CHECK_PRECONDITION_W_MSG((qApp != NULL), "qApp is NULL; please create app first");
	
	eventHandlerUPP_ = NewEventHandlerUPP(&VolumeEventsHandler);
	if (eventHandlerUPP_ == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("Failed to create event handler UPP");
	}
	
	const EventTypeSpec kEventTypeSpecs[] = {
		{kEventClassVolume, kEventVolumeMounted},
		{kEventClassVolume, kEventVolumeUnmounted}
	};
	const OSStatus kOSStatus = InstallApplicationEventHandler(eventHandlerUPP_,
			GetEventTypeCount(kEventTypeSpecs), kEventTypeSpecs, this, NULL);
	if (kOSStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(
				QString("Failed to install StorageDeviceNotifier application event handler; kOSStatus: %1").arg(kOSStatus));
	}
	
	CPPDEVTK_VERIFY(connect(qApp, SIGNAL(aboutToQuit()), SLOT(Uninit())));
}

StorageDeviceNotifier::~StorageDeviceNotifier() {
	CPPDEVTK_ASSERT((eventHandlerUPP_ == NULL) && "Call Uninit() before leaving main()");
}

OSStatus StorageDeviceNotifier::VolumeEventsHandler(EventHandlerCallRef nextHandler, EventRef theEvent, void* pUserData) {
	base::SuppressUnusedWarning(nextHandler);
	
	CPPDEVTK_ASSERT(GetEventClass(theEvent) == kEventClassVolume);
	const UInt32 kEventKind = GetEventKind(theEvent);
	CPPDEVTK_ASSERT((kEventKind == kEventVolumeMounted) || (kEventKind == kEventVolumeUnmounted));
	CPPDEVTK_ASSERT(pUserData != NULL);
	
	FSVolumeRefNum fsVolumeRefNum;
	const OSStatus kOSStatus = GetEventParameter(theEvent, kEventParamDirectObject, typeFSVolumeRefNum, NULL,
			sizeof(FSVolumeRefNum), NULL, &fsVolumeRefNum);
	if (kOSStatus != noErr) {
		CPPDEVTK_LOG_ERROR("Failed to get event parameter, typeFSVolumeRefNum; kOSStatus: " << kOSStatus);
		return eventNotHandledErr;
	}
	
	if (kEventKind == kEventVolumeMounted) {
		//CPPDEVTK_LOG_DEBUG("got kEventVolumeMounted");
		Q_EMIT static_cast<StorageDeviceNotifier*>(pUserData)->StorageDeviceAdded(fsVolumeRefNum);
		Q_EMIT static_cast<StorageDeviceNotifier*>(pUserData)->StorageDeviceMounted(fsVolumeRefNum);
	}
	else {
		//CPPDEVTK_LOG_DEBUG("got kEventVolumeUnmounted");
		Q_EMIT static_cast<StorageDeviceNotifier*>(pUserData)->StorageDeviceUnmounted(fsVolumeRefNum);
		Q_EMIT static_cast<StorageDeviceNotifier*>(pUserData)->StorageDeviceRemoved(fsVolumeRefNum);
	}
	
	return noErr;
}

QString StorageDeviceNotifier::GetStorageDeviceName(StorageDeviceId storageDeviceId) {
	GetVolParmsInfoBuffer getVolParmsInfoBuffer;
#	if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)
	const OSStatus kOSStatus = FSGetVolumeParms(storageDeviceId, &getVolParmsInfoBuffer, sizeof(getVolParmsInfoBuffer));
	if (kOSStatus != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("FSGetVolumeParms() failed for storageDeviceId %1; kOSStatus: %2").arg(
				QString::number(storageDeviceId), QString::number(kOSStatus)));
	}
#	else
	HParamBlockRec hParamBlockRec;
	hParamBlockRec.ioParam.ioNamePtr = NULL;
	hParamBlockRec.ioParam.ioVRefNum = storageDeviceId;
	hParamBlockRec.ioParam.ioBuffer = (Ptr)&getVolParmsInfoBuffer;
	hParamBlockRec.ioParam.ioReqCount = sizeof(getVolParmsInfoBuffer);
	const OSErr kOSErr = PBHGetVolParmsSync(&hParamBlockRec);
	if (kOSErr != noErr) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(QString("PBHGetVolParmsSync() failed for storageDeviceId %1; kOSErr: %2").arg(
				QString::number(storageDeviceId), QString::number(kOSErr)));
	}
	//CPPDEVTK_LOG_DEBUG("out infoSize: " << hParamBlockRec.ioParam.ioActCount);
#	endif
	if (getVolParmsInfoBuffer.vMVersion < 4) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("GetVolParmsInfoBuffer does not have vMDeviceID");
	}
	if (getVolParmsInfoBuffer.vMDeviceID == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION(
				QString("BSD Name not available for storageDeviceId (FSVolumeRefNum): %1").arg(storageDeviceId));
	}
	return QString::fromUtf8((const char*)getVolParmsInfoBuffer.vMDeviceID);
}


}	// namespace gui
}	// namespace cppdevtk
