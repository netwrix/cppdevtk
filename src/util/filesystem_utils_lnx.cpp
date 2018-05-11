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


#include <cppdevtk/util/filesystem_utils.hpp>
#if (!CPPDEVTK_PLATFORM_LINUX)
#	error "This file is Linux specific!!!"
#endif
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>
#if (!CPPDEVTK_PLATFORM_ANDROID)
#	include <cppdevtk/util/libudev_lnx.hpp>
#endif
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/string_utils.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/zeroize.hpp>

#include <QtCore/QDir>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mntent.h>
#include <paths.h>
#if (CPPDEVTK_PLATFORM_ANDROID)
#include <android/api-level.h>
#endif

#include <cstdio>
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && !CPPDEVTK_PLATFORM_ANDROID)
#	if (CPPDEVTK_GNUC_VERSION_NUM >= CPPDEVTK_GNUC_VERSION_NUM_FROM_COMPONENTS(4, 2, 4))
#		pragma GCC diagnostic ignored "-Wunused-function"
#	endif
#endif


using ::cppdevtk::base::Exception;
using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;
using ::cppdevtk::base::GetSystemCategory;
using ::cppdevtk::base::SuppressUnusedWarning;
using ::std::auto_ptr;
using ::std::exception;


namespace cppdevtk {
namespace util {


namespace detail {


#if (!CPPDEVTK_PLATFORM_ANDROID)

static void PrintUDevDevice(const udev_device& udevDevice);
static void PrintUDevListEntry(const udev_list_entry& udevListEntry);

#endif

}	// namespace detail


CPPDEVTK_UTIL_API bool IsValidPath(const QString& path, bool ignorePathSeparator) {
	if (path.isEmpty()) {
		CPPDEVTK_LOG_ERROR("path is empty");
		return false;
	}
	if (path.contains('\\')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' is not in Qt format");
		return false;
	}
	
	return ignorePathSeparator ? true : !path.contains('/');
}

CPPDEVTK_UTIL_API QStringList GetMountPointsFromPath(const QString& path) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((IsValidPath(path)), "path must be valid");
	
#	if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 21))
	
	const ::std::string kNativePath = CPPDEVTK_Q2U(QDir::toNativeSeparators(path));
	struct stat statPath;
	//Zeroize(&statPath, sizeof(statPath));
	if (stat(kNativePath.c_str(), &statPath) != ESUCCESS) {
		CPPDEVTK_ASSERT(errno != EINTR);
		if (errno == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(path);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "stat() failed", path);
		}
	}
	
	FILE* pMTabFile = setmntent(_PATH_MOUNTED, "r");
	if (pMTabFile == NULL) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "setmntent() failed; (errno not documented to be set)");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pMTabFile)) {
		CPPDEVTK_ASSERT(pMTabFile != NULL);
		endmntent(pMTabFile);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	QStringList mountPoints;
	
	for (;;) {
		struct mntent mntEntry;
		// mntEntry.mnt_fsname: name of mounted file system; ex: rootfs
		// mntEntry.mnt_dir: file system path prefix; ex: /
		//Zeroize(&mntEntry, sizeof(mntEntry));
		
		// buf size assumptions:
		// mnt_fsname: CPPDEVTK_NAME_MAX
		// mnt_dir: CPPDEVTK_PATH_MAX
		// mnt_type: 32
		// mnt_opts: 64
		char buf[(CPPDEVTK_NAME_MAX + 1) + CPPDEVTK_PATH_MAX + 32 + 64] = {0};
		
		if (getmntent_r(pMTabFile, &mntEntry, buf, sizeof(buf)) == NULL) {
			CPPDEVTK_LOG_DEBUG("getmntent_r() returned NULL; assuming end of mntent");
			break;
		}
		CPPDEVTK_ASSERT(mntEntry.mnt_dir != NULL);
		
		struct stat statMntDir;
		//Zeroize(&statMntDir, sizeof(statMntDir));
		if (stat(mntEntry.mnt_dir, &statMntDir) != ESUCCESS) {
			CPPDEVTK_ASSERT(errno != EINTR);
			const ErrorCode kErrorCode = GetLastSystemErrorCode();
			CPPDEVTK_LOG_ERROR("stat() failed for: " << mntEntry.mnt_dir << "\nerror code: " << kErrorCode.ToString());
			continue;
		}
		
		if (statPath.st_dev == statMntDir.st_dev) {
			QString mountPoint = QDir::fromNativeSeparators(CPPDEVTK_U2Q(mntEntry.mnt_dir));
			CPPDEVTK_ASSERT(!mountPoint.isEmpty());
			if (!mountPoint.endsWith('/')) {
				mountPoint += '/';
			}
			
			mountPoints.append(mountPoint);
		}
	}
	
	return mountPoints;
	
#	else
	// TODO: Android port for API level < 21
	SuppressUnusedWarning(path);
	CPPDEVTK_COMPILER_WARNING("GetMountPointsFromPath(): Not working on Android < 5.0.1 (API level < 21)!");
	return QStringList();
#	endif
}

CPPDEVTK_UTIL_API QStringList GetMountPointsFromDeviceName(const QString& deviceName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(deviceName), "deviceName must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(deviceName.startsWith(QDir::fromNativeSeparators(CPPDEVTK_U2Q(_PATH_DEV))),
			"deviceName must start with _PATH_DEV (/dev/)");
	
#	if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 21))
	
	FILE* pMTabFile = setmntent(_PATH_MOUNTED, "r");
	if (pMTabFile == NULL) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "setmntent() failed; (errno not documented to be set)");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pMTabFile)) {
		CPPDEVTK_ASSERT(pMTabFile != NULL);
		endmntent(pMTabFile);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	QStringList mountPoints;
	
	for (;;) {
		struct mntent mntEntry;
		// mntEntry.mnt_fsname: name of mounted file system; ex: rootfs
		// mntEntry.mnt_dir: file system path prefix; ex: /
		//Zeroize(&mntEntry, sizeof(mntEntry));
		
		// buf size assumptions:
		// mnt_fsname: CPPDEVTK_NAME_MAX
		// mnt_dir: CPPDEVTK_PATH_MAX
		// mnt_type: 32
		// mnt_opts: 64
		char buf[(CPPDEVTK_NAME_MAX + 1) + CPPDEVTK_PATH_MAX + 32 + 64] = {0};
		
		if (getmntent_r(pMTabFile, &mntEntry, buf, sizeof(buf)) == NULL) {
			CPPDEVTK_LOG_DEBUG("getmntent_r() returned NULL; assuming end of mntent");
			break;
		}
		CPPDEVTK_ASSERT(mntEntry.mnt_fsname != NULL);
		CPPDEVTK_ASSERT(mntEntry.mnt_dir != NULL);
		
		if (!CPPDEVTK_U2Q(mntEntry.mnt_fsname).startsWith(CPPDEVTK_U2Q(_PATH_DEV))) {
			CPPDEVTK_LOG_TRACE("skipping mntEntry.mnt_fsname: " << CPPDEVTK_U2Q(mntEntry.mnt_fsname)
					<< "; it does not start with _PATH_DEV: " << CPPDEVTK_U2Q(_PATH_DEV));
			continue;
		}
		if (deviceName == QDir::fromNativeSeparators(CPPDEVTK_U2Q(mntEntry.mnt_fsname))) {
			QString mountPoint = QDir::fromNativeSeparators(CPPDEVTK_U2Q(mntEntry.mnt_dir));
			CPPDEVTK_ASSERT(!mountPoint.isEmpty());
			if (!mountPoint.endsWith('/')) {
				mountPoint += '/';
			}
			
			mountPoints.append(mountPoint);
		}
	}
	
	return mountPoints;
	
#	else
	// TODO: Android port for API level < 21
	SuppressUnusedWarning(deviceName);
	CPPDEVTK_COMPILER_WARNING("GetMountPointsFromDeviceName(): Not working on Android < 5.0.1 (API level < 21)!");
	return QStringList();
#	endif
}

CPPDEVTK_UTIL_API QString GetDeviceName(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(serialNumber.isEmpty(), "serialNumber");
	
#	if (!CPPDEVTK_PLATFORM_ANDROID)
	
	CPPDEVTK_LOG_DEBUG("deviceType: " << deviceType << "; serialNumber: " << serialNumber
			<< "; vendorId: " << base::NumToHexStr(vendorId) << "; productId: " << base::NumToHexStr(productId));
	
	auto_ptr<LibUDev> pLibUDev(NULL);
	try {
		pLibUDev.reset(new LibUDev1());
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_INFO("failed to load LibUDev1; trying LibUDev0; reason: " << Exception::GetDetailedInfo(exc));
		pLibUDev.reset(new LibUDev0());
	}
	CPPDEVTK_ASSERT(pLibUDev.get() != NULL);
	
	udev* pUDev = pLibUDev->udev_new();
	if (pUDev == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_new() failed");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pLibUDev)(&pUDev)) {
        CPPDEVTK_ASSERT(pUDev != NULL);
		pLibUDev->udev_unref(pUDev);
    }
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	udev_enumerate* pUDevEnumerate = pLibUDev->udev_enumerate_new(pUDev);
	if (pUDevEnumerate == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_new() failed");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pLibUDev)(&pUDevEnumerate)) {
        CPPDEVTK_ASSERT(pUDevEnumerate != NULL);
		pLibUDev->udev_enumerate_unref(pUDevEnumerate);
    }
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	int retCode = pLibUDev->udev_enumerate_add_match_subsystem(pUDevEnumerate, "block");
	if (retCode != 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_add_match_subsystem() failed");
	}
	
	retCode = pLibUDev->udev_enumerate_add_match_property(pUDevEnumerate, "ID_SERIAL_SHORT", serialNumber.toUtf8().constData());
	if (retCode != 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_add_match_property() failed");
	}
	
	retCode = pLibUDev->udev_enumerate_add_match_property(pUDevEnumerate, "ID_VENDOR_ID",
			QString::number(vendorId, 16).toLower().toUtf8().constData());
	if (retCode != 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_add_match_property() failed");
	}
	
	retCode = pLibUDev->udev_enumerate_add_match_property(pUDevEnumerate, "ID_MODEL_ID",
			QString::number(productId, 16).toLower().toUtf8().constData());
	if (retCode != 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_add_match_property() failed");
	}
	
	retCode = pLibUDev->udev_enumerate_scan_devices(pUDevEnumerate);
	if (retCode != 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_scan_devices() failed");
	}
	
	udev_list_entry* pUDevListEntry = pLibUDev->udev_enumerate_get_list_entry(pUDevEnumerate);
	if (pUDevListEntry == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("udev_enumerate_get_list_entry() failed");
	}
	
	QString deviceName;
	
	do {
		CPPDEVTK_ASSERT(pUDevListEntry != NULL);
		
		const char* pUDevListEntryName = pLibUDev->udev_list_entry_get_name(pUDevListEntry);
		if (pUDevListEntryName == NULL) {
			CPPDEVTK_LOG_ERROR("udev_list_entry_get_name() failed");
			continue;
		}
		CPPDEVTK_ASSERT(strlen(pUDevListEntryName) > 0);
		//CPPDEVTK_LOG_TRACE("pUDevListEntryName: " << pUDevListEntryName);
		
		udev_device* pUDevDevice = pLibUDev->udev_device_new_from_syspath(pUDev, pUDevListEntryName);
		if (pUDevDevice == NULL) {
			CPPDEVTK_LOG_ERROR("udev_device_new_from_syspath() failed for " << pUDevListEntryName);
			continue;
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pLibUDev)(&pUDevDevice)) {
	        CPPDEVTK_ASSERT(pUDevDevice != NULL);
			pLibUDev->udev_device_unref(pUDevDevice);
	    }
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		//PrintUDevDevice(*pUDevDevice);
		
		const char* devType = pLibUDev->udev_device_get_devtype(pUDevDevice);
		if (devType == NULL) {
			CPPDEVTK_LOG_ERROR("udev_device_get_devtype() failed for " << pUDevListEntryName);
			continue;
		}
		const char* expDevType = (deviceType == dtCd) ? "disk" : "partition";
		if (strcmp(devType, expDevType) != 0) {
			CPPDEVTK_LOG_TRACE("skipping " << pUDevListEntryName << " (devType: " << devType << ')');
			continue;
		}
		
		const char* propertyValue = pLibUDev->udev_device_get_property_value(pUDevDevice, "ID_FS_USAGE");
		if (propertyValue == NULL) {
			CPPDEVTK_LOG_ERROR("udev_device_get_property_value() failed for ID_FS_USAGE; " << pUDevListEntryName);
			continue;
		}
		const char* expectedPropertyValue = "filesystem";
		if (strcmp(propertyValue, expectedPropertyValue) != 0) {
			CPPDEVTK_LOG_TRACE("skipping " << pUDevListEntryName << " (ID_FS_USAGE: " << propertyValue << ')');
			continue;
		}
		
		propertyValue = pLibUDev->udev_device_get_property_value(pUDevDevice, "ID_TYPE");
		if (propertyValue == NULL) {
			CPPDEVTK_LOG_ERROR("udev_device_get_property_value() failed for ID_TYPE; " << pUDevListEntryName);
			continue;
		}
		expectedPropertyValue = (deviceType == dtCd) ? "cd" : "disk";
		if (strcmp(propertyValue, expectedPropertyValue) != 0) {
			CPPDEVTK_LOG_TRACE("skipping " << pUDevListEntryName << " (ID_TYPE: " << propertyValue << ')');
			continue;
		}
		
		/*
		if (deviceType == dtCd) {
			propertyValue = pLibUDev->udev_device_get_property_value(pUDevDevice, "ID_CDROM");
			if (propertyValue == NULL) {
			   CPPDEVTK_LOG_ERROR("udev_device_get_property_value() failed for ID_CDROM; " << pUDevListEntryName);
				continue;
			}
			expectedPropertyValue = "1";
			if (strcmp(propertyValue, expectedPropertyValue) != 0) {
				CPPDEVTK_LOG_TRACE("skipping " << pUDevListEntryName << " (ID_CDROM: " << propertyValue << ')');
				continue;
			}
		}
		*/
		
		const char* devNode = pLibUDev->udev_device_get_devnode(pUDevDevice);
		if (devNode == NULL) {
			CPPDEVTK_LOG_ERROR("udev_device_get_devnode() failed for " << pUDevListEntryName);
			continue;
		}
		
		deviceName = QDir::fromNativeSeparators(CPPDEVTK_U2Q(devNode));
		if (!deviceName.startsWith(QDir::fromNativeSeparators(CPPDEVTK_U2Q(_PATH_DEV)))) {
			CPPDEVTK_LOG_TRACE("skipping deviceName: " << deviceName
					<< "; it does not start with _PATH_DEV: " << QDir::fromNativeSeparators(CPPDEVTK_U2Q(_PATH_DEV)));
			
			deviceName.clear();
			
			continue;
		}
		
		break;
	}
	while ((pUDevListEntry = pLibUDev->udev_list_entry_get_next(pUDevListEntry)) != NULL);
	
	return deviceName;
	
#	else
	// TODO: Android port
	SuppressUnusedWarning(deviceType);
	SuppressUnusedWarning(serialNumber);
	SuppressUnusedWarning(vendorId);
	SuppressUnusedWarning(productId);
	CPPDEVTK_COMPILER_WARNING("GetDeviceName(): Not ported on Android!");
	return QString("");
#	endif
}

CPPDEVTK_UTIL_API QString GetDeviceNameFromMountPoint(const QString& mountPoint) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(mountPoint), "mountPoint must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(mountPoint.endsWith('/'), "mountPoint must end with /");
	
#	if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 21))
	
	FILE* pMTabFile = setmntent(_PATH_MOUNTED, "r");
	if (pMTabFile == NULL) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "setmntent() failed; (errno not documented to be set)");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pMTabFile)) {
		CPPDEVTK_ASSERT(pMTabFile != NULL);
		endmntent(pMTabFile);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	QString deviceName;
	
	for (;;) {
		struct mntent mntEntry;
		// mntEntry.mnt_fsname: name of mounted file system; ex: rootfs
		// mntEntry.mnt_dir: file system path prefix; ex: /
		//Zeroize(&mntEntry, sizeof(mntEntry));
		
		// buf size assumptions:
		// mnt_fsname: CPPDEVTK_NAME_MAX
		// mnt_dir: CPPDEVTK_PATH_MAX
		// mnt_type: 32
		// mnt_opts: 64
		char buf[(CPPDEVTK_NAME_MAX + 1) + CPPDEVTK_PATH_MAX + 32 + 64] = {0};
		
		if (getmntent_r(pMTabFile, &mntEntry, buf, sizeof(buf)) == NULL) {
			CPPDEVTK_LOG_DEBUG("getmntent_r() returned NULL; assuming end of mntent");
			break;
		}
		CPPDEVTK_ASSERT(mntEntry.mnt_fsname != NULL);
		CPPDEVTK_ASSERT(mntEntry.mnt_dir != NULL);
		
		QString mntDir = QDir::fromNativeSeparators(CPPDEVTK_U2Q(mntEntry.mnt_dir));
		CPPDEVTK_ASSERT(!mntDir.isEmpty());
		if (!mntDir.endsWith('/')) {
			mntDir.append('/');
		}
		if (mountPoint == mntDir) {
			deviceName = QDir::fromNativeSeparators(CPPDEVTK_U2Q(mntEntry.mnt_fsname));
			CPPDEVTK_ASSERT(!deviceName.isEmpty());
			if (!deviceName.startsWith(QDir::fromNativeSeparators(CPPDEVTK_U2Q(_PATH_DEV)))) {
				deviceName.clear();
				continue;
			}
			
			break;
		}
	}
	
	return deviceName;
	
#	else
	// TODO: Android port for API level < 21
	SuppressUnusedWarning(mountPoint);
	CPPDEVTK_COMPILER_WARNING("GetDeviceNameFromMountPoint(): Not working on Android < 5.0.1 (API level < 21)!");
	return QString();
#	endif
}




CPPDEVTK_UTIL_API QStringList GetMountPoints(bool ignoreSpecialFileSystems) {
#	if (!CPPDEVTK_PLATFORM_ANDROID || (__ANDROID_API__ >= 21))
	
	FILE* pMTabFile = setmntent(_PATH_MOUNTED, "r");
	if (pMTabFile == NULL) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "setmntent() failed; (errno not documented to be set)");
	}
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pMTabFile)) {
		CPPDEVTK_ASSERT(pMTabFile != NULL);
		endmntent(pMTabFile);
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	QStringList mountPoints;
	
	for (;;) {
		struct mntent mntEntry;
		// mntEntry.mnt_fsname: name of mounted file system; ex: rootfs
		// mntEntry.mnt_dir: file system path prefix; ex: /
		//Zeroize(&mntEntry, sizeof(mntEntry));
		
		// buf size assumptions:
		// mnt_fsname: CPPDEVTK_NAME_MAX
		// mnt_dir: CPPDEVTK_PATH_MAX
		// mnt_type: 32
		// mnt_opts: 64
		char buf[(CPPDEVTK_NAME_MAX + 1) + CPPDEVTK_PATH_MAX + 32 + 64] = {0};
		
		if (getmntent_r(pMTabFile, &mntEntry, buf, sizeof(buf)) == NULL) {
			CPPDEVTK_LOG_DEBUG("getmntent_r() returned NULL; assuming end of mntent");
			break;
		}
		CPPDEVTK_ASSERT(mntEntry.mnt_dir != NULL);
		
		if (ignoreSpecialFileSystems) {
			if (mntEntry.mnt_type != NULL) {
				if ((strcmp(mntEntry.mnt_type, MNTTYPE_IGNORE) == 0) || (strcmp(mntEntry.mnt_type, MNTTYPE_SWAP) == 0)) {
					CPPDEVTK_LOG_TRACE("skipping filesystem type: " << mntEntry.mnt_type);
					continue;
				}
			}
			
			if (mntEntry.mnt_fsname != NULL) {
				struct stat buf1;
				memset(&buf1, 0, sizeof(buf1));
				if (stat(mntEntry.mnt_fsname, &buf1) != 0) {
					CPPDEVTK_LOG_WARN("failed to stat " << mntEntry.mnt_fsname);
					continue;
				}
				if (!S_ISBLK(buf1.st_mode)) {
					CPPDEVTK_LOG_TRACE("skipping non-block device " << mntEntry.mnt_fsname);
					continue;
				}
			}
			
			struct stat buf2;
			memset(&buf2, 0, sizeof(buf2));
			if (stat(mntEntry.mnt_dir, &buf2) != 0) {
				CPPDEVTK_LOG_WARN("failed to stat " << mntEntry.mnt_dir);
				continue;
			}
			if (buf2.st_size == 0) {
				CPPDEVTK_LOG_TRACE("skipping special filesystem " << mntEntry.mnt_dir);
				continue;
			}
		}
		
		QString mountPoint = QDir::fromNativeSeparators(mntEntry.mnt_dir);
		CPPDEVTK_ASSERT(!mountPoint.isEmpty());
		if (!mountPoint.endsWith('/')) {
			mountPoint.append('/');
		}
		CPPDEVTK_LOG_DEBUG("found mount point: " << mountPoint);
		mountPoints.append(mountPoint);
	}
	
	return mountPoints;
	
#	else
	// TODO: Android port for API level < 21
	CPPDEVTK_COMPILER_WARNING("GetMountPoints(): Not working on Android < 5.0.1 (API level < 21)!");
	return QStringList();
#	endif
}




namespace detail {


#if (!CPPDEVTK_PLATFORM_ANDROID)

void PrintUDevDevice(const udev_device& udevDevice) {
	auto_ptr<LibUDev> pLibUDev(NULL);
	try {
		pLibUDev.reset(new LibUDev1());
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_INFO("failed to load LibUDev1; trying LibUDev0; reason: " << Exception::GetDetailedInfo(exc));
		pLibUDev.reset(new LibUDev0());
	}
	CPPDEVTK_ASSERT(pLibUDev.get() != NULL);
	
	struct udev_device* pUDevDevice = const_cast<struct udev_device*>(&udevDevice);
	
#if 1
	const char* devpath = pLibUDev->udev_device_get_devpath(pUDevDevice);
	CPPDEVTK_LOG_TRACE("devpath: " << ((devpath != NULL) ? devpath : "NULL"));
	SuppressUnusedWarning(devpath);
	
	const char* subsystem = pLibUDev->udev_device_get_subsystem(pUDevDevice);
	CPPDEVTK_LOG_TRACE("subsystem: " << ((subsystem != NULL) ? subsystem : "NULL"));
	SuppressUnusedWarning(subsystem);
	
	const char* devtype = pLibUDev->udev_device_get_devtype(pUDevDevice);
	CPPDEVTK_LOG_TRACE("devtype: " << ((devtype != NULL) ? devtype : "NULL"));
	SuppressUnusedWarning(devtype);
	
	const char* syspath = pLibUDev->udev_device_get_syspath(pUDevDevice);
	CPPDEVTK_LOG_TRACE("syspath: " << ((syspath != NULL) ? syspath : "NULL"));
	SuppressUnusedWarning(syspath);
	
	const char* sysname = pLibUDev->udev_device_get_sysname(pUDevDevice);
	CPPDEVTK_LOG_TRACE("sysname: " << ((sysname != NULL) ? sysname : "NULL"));
	SuppressUnusedWarning(sysname);
	
	const char* sysnum = pLibUDev->udev_device_get_sysnum(pUDevDevice);
	CPPDEVTK_LOG_TRACE("sysnum: " << ((sysnum != NULL) ? sysnum : "NULL"));
	SuppressUnusedWarning(sysnum);
	
	const char* devnode = pLibUDev->udev_device_get_devnode(pUDevDevice);
	CPPDEVTK_LOG_TRACE("devnode: " << ((devnode != NULL) ? devnode : "NULL"));
	SuppressUnusedWarning(devnode);
#endif
	
#if 1	
	udev_list_entry* pProperties = pLibUDev->udev_device_get_properties_list_entry(pUDevDevice);
	if (pProperties != NULL) {
		CPPDEVTK_LOG_TRACE("Properties:");
		PrintUDevListEntry(*pProperties);
	}
	else {
		CPPDEVTK_LOG_TRACE("No properties!!!");
	}
#endif
	
#if 1
	udev_list_entry* pSysAttrs = pLibUDev->udev_device_get_sysattr_list_entry(pUDevDevice);
	if (pSysAttrs != NULL) {
		CPPDEVTK_LOG_TRACE("SysAttrs:");
		PrintUDevListEntry(*pSysAttrs);
	}
	else {
		CPPDEVTK_LOG_TRACE("No SysAttrs!!!");
	}
#endif
	
#if 1
	udev_list_entry* pTags = pLibUDev->udev_device_get_tags_list_entry(pUDevDevice);
	if (pTags != NULL) {
		CPPDEVTK_LOG_TRACE("Tags:");
		PrintUDevListEntry(*pTags);
	}
	else {
		CPPDEVTK_LOG_TRACE("No Tags!!!");
	}
#endif
}

void PrintUDevListEntry(const udev_list_entry& udevListEntry) {
	auto_ptr<LibUDev> pLibUDev(NULL);
	try {
		pLibUDev.reset(new LibUDev1());
	}
	catch (const ::std::runtime_error& exc) {
		CPPDEVTK_LOG_INFO("failed to load LibUDev1; trying LibUDev0; reason: " << Exception::GetDetailedInfo(exc));
		pLibUDev.reset(new LibUDev0());
	}
	CPPDEVTK_ASSERT(pLibUDev.get() != NULL);
	
	udev_list_entry* pUDevListEntry = const_cast<udev_list_entry*>(&udevListEntry);
	do {
		CPPDEVTK_ASSERT(pUDevListEntry != NULL);
		const char* name = pLibUDev->udev_list_entry_get_name(pUDevListEntry);
		const char* value = pLibUDev->udev_list_entry_get_value(pUDevListEntry);
		CPPDEVTK_LOG_TRACE("name: " << ((name != NULL) ? name : " NULL")
				<< "; value: " << ((value != NULL) ? value : " NULL"));
		SuppressUnusedWarning(name);
		SuppressUnusedWarning(value);
	}
	while ((pUDevListEntry = pLibUDev->udev_list_entry_get_next(pUDevListEntry)) != NULL);
}

#endif	// !ANDROID

}	// namespace detail


}	// namespace util
}	// namespace cppdevtk
