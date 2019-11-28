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


#include <cppdevtk/util/config.hpp>
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_PLATFORM_IOS)
#	pragma clang diagnostic ignored "-Wunused-function"
#endif

#include <cppdevtk/util/filesystem_utils.hpp>
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif
#include <cppdevtk/util/no_such_file_or_directory_exception.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/string_utils.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/zeroize.hpp>

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>

#if (!CPPDEVTK_PLATFORM_IOS)	// apps using IOKit on iOS are rejected from App Store
#include <IOKit/IOKitLib.h>
#include <IOKit/IOBSD.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/storage/IOCDMedia.h>
#include <IOKit/storage/IOPartitionScheme.h>
#endif
#if (!CPPDEVTK_PLATFORM_IOS)	// DiskArbitration not available on iOS?
#include <DiskArbitration/DiskArbitration.h>
#endif
#include <CoreFoundation/CoreFoundation.h>

#include <sys/param.h>
#include <sys/mount.h>
#include <sys/ucred.h>
#include <paths.h>

#include <cstddef>
#include <cstring>
#include <vector>


#ifndef kUSBSerialNumberString
#	define kUSBSerialNumberString "USB Serial Number"
#endif

// Buggy Mac OS X 10.11 workaround
#define kIOUSBDeviceClassNamePre1011 kIOUSBDeviceClassName
#define kIOUSBDeviceClassName1011 "IOUSBHostDevice"

// Buggy 65 devices workaround
// normally we should search for:
// - kIOCDMediaTypeKey for CDROM area
// - kIOMediaPartitionIDKey for private area
// This will work for 13 controler
// Unfortunately, this does not work for 65 controller (and others maybe).
// 65 controller has kIOMediaPartitionIDKey for both CDROM and private areas,
// and kIOCDMediaTypeKey is missing for CDROM area.
// So it seems kIOMediaPreferredBlockSizeKey is the way to go...
#define CPPDEVTK_ENABLE_PREFERRED_BLOCK_SIZE_WORKAROUND 1


using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;
using ::cppdevtk::base::SuppressUnusedWarning;


namespace cppdevtk {
namespace util {


static QString GetDeviceName(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId, const QString& iousbDeviceClassName);


CPPDEVTK_UTIL_API bool IsValidPath(const QString& path, bool ignorePathSeparator) {
	if (path.isEmpty()) {
		CPPDEVTK_LOG_ERROR("path is empty");
		return false;
	}
	if (path.contains('\\')) {
		CPPDEVTK_LOG_ERROR("path '" << path << "' is not in Qt format");
		return false;
	}
	
	/*
	if (path.contains(':')) {
		if (path.count(':') > 1) {
			CPPDEVTK_LOG_ERROR("path '" << path << "' contains " << path.count(':') << " ':' character");
			return false;
		}
		if (!path.startsWith(":/")) {
			CPPDEVTK_LOG_ERROR("path '" << path << "' contains ':' character and is not a Qt resource path");
			return false;
		}
	}
	*/
	
	return ignorePathSeparator ? true : !path.contains('/');
}

CPPDEVTK_UTIL_API QStringList GetMountPointsFromPath(const QString& path) {
	CPPDEVTK_DBC_CHECK_ARGUMENT((IsValidPath(path)), "path must be valid");
	
	const ::std::string kNativePath = CPPDEVTK_Q2U(QDir::toNativeSeparators(path));
	struct statfs statFs;
	// statFs.f_mntonname: directory on which mounted; ex: /
	// statFs.f_mntfromname: mounted file system; ex: /dev/disk0s2
	//Zeroize(&statFs, sizeof(statFs));
	if (statfs(kNativePath.c_str(), &statFs) != ESUCCESS) {
		CPPDEVTK_ASSERT(errno != EINTR);
		if (errno == ENOENT) {
			throw CPPDEVTK_NO_SUCH_FILE_OR_DIRECTORY_EXCEPTION_W_P(path);
		}
		else {
			throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA_SRC(GetLastSystemErrorCode(), "statfs() failed", path);
		}
	}
	
	CPPDEVTK_ASSERT(statFs.f_mntonname != NULL);
	QString mountPoint = QDir::fromNativeSeparators(CPPDEVTK_U2Q(statFs.f_mntonname));
	CPPDEVTK_ASSERT(!mountPoint.isEmpty());
	if (!mountPoint.endsWith('/')) {
		mountPoint += '/';
	}
	
	QStringList mountPoints;
	mountPoints.append(mountPoint);
	
	return mountPoints;
}

CPPDEVTK_UTIL_API QStringList GetMountPointsFromDeviceName(const QString& deviceName) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(deviceName), "deviceName must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(!deviceName.startsWith(_PATH_DEV),
			"deviceName must not start with _PATH_DEV (it must be BSD name)");
	CPPDEVTK_DBC_CHECK_ARGUMENT(!deviceName.startsWith('/'),
			"deviceName must not start with '/' (it must be BSD name)");
	CPPDEVTK_DBC_CHECK_ARGUMENT(!deviceName.endsWith('/'),
			"deviceName must not end with '/' (it must be BSD name)");
	
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	DASessionRef session = DASessionCreate(kCFAllocatorDefault);
	if (session == 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("DASessionCreate() failed");
	}
    CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&session)) {
		CFRelease(session);
    }
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	DADiskRef disk = DADiskCreateFromBSDName(kCFAllocatorDefault, session, CPPDEVTK_Q2U(deviceName).c_str());
	if (disk == 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("DADiskCreateFromBSDName() failed");
	}
    CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&disk)) {
		CFRelease(disk);
    }
	CPPDEVTK_ON_BLOCK_EXIT_END

	CFDictionaryRef diskDescription = DADiskCopyDescription(disk);
	if (diskDescription == 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("DADiskCopyDescription() failed");
	}
    CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&diskDescription)) {
		CFRelease(diskDescription);
    }
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	Boolean macRetCode = CFDictionaryContainsKey(diskDescription, kDADiskDescriptionVolumePathKey);
	if (!macRetCode) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("CFDictionaryContainsKey() failed");
	}
	
	CFURLRef pathUrl = NULL;
	macRetCode = CFDictionaryGetValueIfPresent(diskDescription, kDADiskDescriptionVolumePathKey, (const void**)&pathUrl);
	if (!macRetCode) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("CFDictionaryGetValueIfPresent() failed");
	}
	
	QStringList mountPoints;
	
	if (pathUrl != NULL) {
		CFRetain(pathUrl);
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pathUrl)) {
			CFRelease(pathUrl);
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		CFStringRef pathStr = CFURLCopyFileSystemPath(pathUrl, kCFURLPOSIXPathStyle);
		if (pathStr == 0) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("CFURLCopyFileSystemPath() failed");
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&pathStr)) {
			CFRelease(pathStr);
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
				
		const size_t kBufSize = MAXPATHLEN;
		char buf[kBufSize];
#		if (CPPDEVTK_DISABLE_UNICODE)
		macRetCode = CFStringGetCString(pathStr, buf, kBufSize, kCFStringEncodingASCII);
#		else
		macRetCode = CFStringGetCString(pathStr, buf, kBufSize, kCFStringEncodingUTF8);
#		endif
		if (!macRetCode) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("CFStringGetCString() failed");
		}
		
		QString mountPoint = QDir::fromNativeSeparators(CPPDEVTK_U2Q(buf));
		CPPDEVTK_ASSERT(!mountPoint.isEmpty());
		if (!mountPoint.endsWith('/')) {
			mountPoint += '/';
		}
		
		mountPoints.append(mountPoint);
	}
	
	return mountPoints;
	
#	else
	// TODO: iOS port
	SuppressUnusedWarning(deviceName);
	CPPDEVTK_COMPILER_WARNING("GetMountPointsFromDeviceName() not ported on iOS");
	return QStringList();
#	endif
}

CPPDEVTK_UTIL_API QString GetDeviceName(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId) {
	CPPDEVTK_DBC_CHECK_NON_EMPTY_ARGUMENT(serialNumber.isEmpty(), "serialNumber");
	
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	QString deviceName = GetDeviceName(deviceType, serialNumber, vendorId, productId, kIOUSBDeviceClassName1011);
	if (deviceName.isEmpty()) {
		deviceName = GetDeviceName(deviceType, serialNumber, vendorId, productId, kIOUSBDeviceClassNamePre1011);
	}
	return deviceName;
	
#	else
	// TODO: iOS port
	SuppressUnusedWarning(deviceType);
	SuppressUnusedWarning(serialNumber);
	SuppressUnusedWarning(vendorId);
	SuppressUnusedWarning(productId);
	CPPDEVTK_COMPILER_WARNING("GetDeviceName() not ported on iOS");
	return QString("");
#	endif
}

CPPDEVTK_UTIL_API QString GetDeviceNameFromMountPoint(const QString& mountPoint) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(mountPoint), "mountPoint must be valid");
	CPPDEVTK_DBC_CHECK_ARGUMENT(mountPoint.endsWith('/'), "mountPoint must end with /");
	
	
	// NOTE: getmntinfo() is not usable because it is not thread-safe... so we use getfsstat()
	
	int retCode = getfsstat(NULL, 0, MNT_WAIT);
	if (retCode == -1) {
		CPPDEVTK_ASSERT(errno != EINTR);
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "getfsstat() failed");
	}
	if (retCode == 0) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(MakeErrorCode(base::errc::io_error), "no mounted file system found");
	}
	
	CPPDEVTK_LOG_DEBUG("number of mounted file systems: " << retCode);
	
	QString deviceName;
	
	::std::vector<struct statfs> buf(retCode);
	memset(&buf[0], 0, (buf.size() * sizeof(struct statfs)));
	retCode = getfsstat(&buf[0], (buf.size() * sizeof(struct statfs)), MNT_WAIT);
	if (retCode == -1) {
		CPPDEVTK_ASSERT(errno != EINTR);
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "getfsstat() failed");
	}
	if (retCode == 0) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(MakeErrorCode(base::errc::io_error), "no mounted file system found");
	}
	
	for (int idx = 0; idx < retCode; ++idx) {
		CPPDEVTK_ASSERT(buf[idx].f_mntfromname != NULL);
		CPPDEVTK_ASSERT(buf[idx].f_mntonname != NULL);
		
		QString currentMountPoint(QDir::fromNativeSeparators(CPPDEVTK_U2Q(buf[idx].f_mntonname)));
		CPPDEVTK_ASSERT(!currentMountPoint.isEmpty());
		if (!currentMountPoint.endsWith('/')) {
			currentMountPoint += '/';
		}
		
		if (mountPoint == currentMountPoint) {
			deviceName = QDir::fromNativeSeparators(CPPDEVTK_U2Q(buf[idx].f_mntfromname));
			if (deviceName.startsWith(_PATH_DEV)) {
				deviceName.remove(0, qstrlen(_PATH_DEV));
			}
			if (deviceName.startsWith('/')) {
				deviceName.remove(0, 1);
			}
			if (deviceName.endsWith('/')) {
				deviceName.chop(1);
			}
			CPPDEVTK_ASSERT(!deviceName.isEmpty());
			
			break;
		}
	}
	
	return deviceName;
}


QString GetDeviceName(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId, const QString& iousbDeviceClassName) {
	CPPDEVTK_ASSERT(!serialNumber.isEmpty());
	CPPDEVTK_ASSERT(!iousbDeviceClassName.isEmpty());
	
	CPPDEVTK_LOG_DEBUG("deviceType: " << deviceType << "; serialNumber: " << serialNumber
			<< "; vendorId: " << base::NumToHexStr(vendorId) << "; productId: " << base::NumToHexStr(productId)
			<< "; iousbDeviceClassName: " << iousbDeviceClassName);
	
	
#	if (!CPPDEVTK_PLATFORM_IOS)
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Find device
	
	CFMutableDictionaryRef matchingServices = IOServiceMatching(::cppdevtk::base::Q2A(iousbDeviceClassName).c_str());
	if (matchingServices == 0) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("IOServiceMatching() failed");
	}
	bool commitMatchingServices = false;
    CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&commitMatchingServices)(&matchingServices)) {
        if(!commitMatchingServices) {
           CFRelease(matchingServices);
		}
    }
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	SInt32 vid = vendorId;
	CFDictionarySetValue(matchingServices, CFSTR(kUSBVendorID), CFNumberCreate(kCFAllocatorDefault,
			kCFNumberSInt32Type, &vid));
	
	SInt32 pid = productId;
	CFDictionarySetValue(matchingServices, CFSTR(kUSBProductID), CFNumberCreate(kCFAllocatorDefault,
			kCFNumberSInt32Type, &pid));
	
#	if (CPPDEVTK_DISABLE_UNICODE)
	CFStringRef macSerialNumber = CFStringCreateWithCString(kCFAllocatorDefault,
			base::Q2A(serialNumber).c_str(), kCFStringEncodingASCII);
#	else
	CFStringRef macSerialNumber = CFStringCreateWithCString(kCFAllocatorDefault,
			base::Q2Utf8(serialNumber).c_str(), kCFStringEncodingUTF8);
#	endif
	if (macSerialNumber == NULL) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("CFStringCreateWithCString() failed");
	}
	CFDictionarySetValue(matchingServices, CFSTR(kUSBSerialNumberString), macSerialNumber);
	
	io_iterator_t iterMatchingServices;
	kern_return_t kernRetCode = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingServices,
			&iterMatchingServices);
	commitMatchingServices = true;
	if (kernRetCode != kIOReturnSuccess) {
		throw CPPDEVTK_RUNTIME_EXCEPTION("IOServiceGetMatchingServices() failed");
	}
    CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&iterMatchingServices)) {
		const kern_return_t kKernRet = IOObjectRelease(iterMatchingServices);
		if (kKernRet != kIOReturnSuccess) {
			CPPDEVTK_LOG_WARN("IOObjectRelease() failed for iterMatchingServices; kKernRet: " << kKernRet);
		}
    }
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	
	QString deviceName;
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Find media
	
	io_object_t service = 0;
	while ((service = IOIteratorNext(iterMatchingServices)) != 0) {
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&service)) {
			const kern_return_t kKernRet = IOObjectRelease(service);
			if (kKernRet != kIOReturnSuccess) {
				CPPDEVTK_LOG_WARN("IOObjectRelease() failed for service; kKernRet: " << kKernRet);
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		io_iterator_t iterRegistryEntries;
		kernRetCode = IORegistryEntryCreateIterator(service, kIOServicePlane, kIORegistryIterateRecursively,
				&iterRegistryEntries);
		if (kernRetCode != kIOReturnSuccess) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("IORegistryEntryCreateIterator() failed");
		}
		CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&iterRegistryEntries)) {
			const kern_return_t kKernRet = IOObjectRelease(iterRegistryEntries);
			if (kKernRet != kIOReturnSuccess) {
				CPPDEVTK_LOG_WARN("IOObjectRelease() failed for iterRegistryEntries; kKernRet: " << kKernRet);
			}
		}
		CPPDEVTK_ON_BLOCK_EXIT_END
		
		io_object_t registryEntry = 0;
		while ((registryEntry = IOIteratorNext(iterRegistryEntries)) != 0) {
			CPPDEVTK_ON_BLOCK_EXIT_BEGIN((&registryEntry)) {
				const kern_return_t kKernRet = IOObjectRelease(registryEntry);
				if (kKernRet != kIOReturnSuccess) {
					CPPDEVTK_LOG_WARN("IOObjectRelease() failed for registryEntry; kKernRet: " << kKernRet);
				}
			}
			CPPDEVTK_ON_BLOCK_EXIT_END
			
			CFTypeRef cfRegistryEntryProperty = IORegistryEntrySearchCFProperty(registryEntry, kIOServicePlane,
					CFSTR(kIOMediaLeafKey), kCFAllocatorDefault, kIORegistryIterateRecursively);
			if (cfRegistryEntryProperty == 0) {
				CPPDEVTK_LOG_DEBUG("IORegistryEntrySearchCFProperty() failed for kIOMediaLeafKey");
				continue;
			}
			if (CFGetTypeID(cfRegistryEntryProperty) != CFBooleanGetTypeID()) {
				CFRelease(cfRegistryEntryProperty);
				throw CPPDEVTK_RUNTIME_EXCEPTION("kIOMediaLeafKey is not boolean");
			}
			if (cfRegistryEntryProperty != kCFBooleanTrue) {
				CPPDEVTK_LOG_DEBUG("kIOMediaLeafKey is false");
				CFRelease(cfRegistryEntryProperty);
				continue;
			}
			CFRelease(cfRegistryEntryProperty);
			
			Boolean macRetCode = FALSE;
			
#			if CPPDEVTK_ENABLE_PREFERRED_BLOCK_SIZE_WORKAROUND
			
			cfRegistryEntryProperty = IORegistryEntrySearchCFProperty(registryEntry, kIOServicePlane,
					CFSTR(kIOMediaPreferredBlockSizeKey), kCFAllocatorDefault, 0);
			if (cfRegistryEntryProperty == 0) {
				CPPDEVTK_LOG_DEBUG("IORegistryEntrySearchCFProperty() failed for kIOMediaPreferredBlockSizeKey");
				continue;
			}
			if (CFGetTypeID(cfRegistryEntryProperty) != CFNumberGetTypeID()) {
				CFRelease(cfRegistryEntryProperty);
				throw CPPDEVTK_RUNTIME_EXCEPTION("kIOMediaPreferredBlockSizeKey is not number");
			}
			int ioMediaPreferredBlockSize = 0;
			macRetCode = CFNumberGetValue((CFNumberRef)cfRegistryEntryProperty, kCFNumberIntType, &ioMediaPreferredBlockSize);
			if (!macRetCode) {
				CFRelease(cfRegistryEntryProperty);
				throw CPPDEVTK_RUNTIME_EXCEPTION("CFNumberGetValue() failed for ioMediaPreferredBlockSize");
			}
			CPPDEVTK_LOG_DEBUG("ioMediaPreferredBlockSize: " << base::NumToHexStr(ioMediaPreferredBlockSize));
			if (deviceType == dtCd) {
				if (ioMediaPreferredBlockSize != 0x800) {
					CPPDEVTK_LOG_DEBUG("ioMediaPreferredBlockSize is not 0x800");
					CFRelease(cfRegistryEntryProperty);
					continue;
				}
			}
			else {
				CPPDEVTK_ASSERT(deviceType == dtDisk);
				if (ioMediaPreferredBlockSize != 0x200) {
					CPPDEVTK_LOG_DEBUG("ioMediaPreferredBlockSize is not 0x200");
					CFRelease(cfRegistryEntryProperty);
					continue;
				}
			}
			CFRelease(cfRegistryEntryProperty);
			
#			else	// CPPDEVTK_ENABLE_PREFERRED_BLOCK_SIZE_WORKAROUND
			
			if (deviceType == dtCd) {
				cfRegistryEntryProperty = IORegistryEntrySearchCFProperty(registryEntry, kIOServicePlane,
						CFSTR(kIOCDMediaTypeKey), kCFAllocatorDefault, 0);
				if (cfRegistryEntryProperty == 0) {
					CPPDEVTK_LOG_DEBUG("IORegistryEntrySearchCFProperty() failed for kIOCDMediaTypeKey");
					continue;
				}
			}
			else {
				cfRegistryEntryProperty = IORegistryEntrySearchCFProperty(registryEntry, kIOServicePlane,
						CFSTR(kIOMediaPartitionIDKey), kCFAllocatorDefault, 0);
				if (cfRegistryEntryProperty == 0) {
					CPPDEVTK_LOG_DEBUG("IORegistryEntrySearchCFProperty() failed for kIOMediaPartitionIDKey");
					continue;
				}
			}
			CFRelease(cfRegistryEntryProperty);
			
#			endif	// CPPDEVTK_ENABLE_PREFERRED_BLOCK_SIZE_WORKAROUND
			
			cfRegistryEntryProperty = IORegistryEntrySearchCFProperty(registryEntry, kIOServicePlane, CFSTR(kIOBSDNameKey),
					kCFAllocatorDefault, 0);
			if (cfRegistryEntryProperty == 0) {
				CPPDEVTK_LOG_DEBUG("IORegistryEntrySearchCFProperty() failed for kIOBSDNameKey");
				continue;
			}
			if (CFGetTypeID(cfRegistryEntryProperty) != CFStringGetTypeID()) {
				CFRelease(cfRegistryEntryProperty);
				throw CPPDEVTK_RUNTIME_EXCEPTION("kIOBSDNameKey is not string");
			}
			
			const size_t kBufSize = MAXPATHLEN;
			char buf[kBufSize] = {0};
#			if (CPPDEVTK_DISABLE_UNICODE)
			macRetCode = CFStringGetCString((CFStringRef)cfRegistryEntryProperty, buf, kBufSize, kCFStringEncodingASCII);
#			else
			macRetCode = CFStringGetCString((CFStringRef)cfRegistryEntryProperty, buf, kBufSize, kCFStringEncodingUTF8);
#			endif
			CFRelease(cfRegistryEntryProperty);
			if (!macRetCode) {
				throw CPPDEVTK_RUNTIME_EXCEPTION("CFStringGetCString() failed for kIOBSDNameKey");
			}
			deviceName = CPPDEVTK_U2Q(buf);
			CPPDEVTK_ASSERT(!deviceName.isEmpty());
			CPPDEVTK_ASSERT(!deviceName.startsWith(_PATH_DEV));
			CPPDEVTK_ASSERT(!deviceName.startsWith('/'));
			CPPDEVTK_ASSERT(!deviceName.endsWith('/'));
			
			break;
		}
		
		if (!deviceName.isEmpty()) {
			break;
		}
	}
	
	return deviceName;
	
#	else
	// TODO: iOS port
	SuppressUnusedWarning(deviceType);
	SuppressUnusedWarning(serialNumber);
	SuppressUnusedWarning(vendorId);
	SuppressUnusedWarning(productId);
	SuppressUnusedWarning(iousbDeviceClassName);
	CPPDEVTK_COMPILER_WARNING("GetDeviceName() not ported on iOS");
	return QString("");
#	endif
}




CPPDEVTK_UTIL_API QStringList GetMountPoints(bool ignoreSpecialFileSystems) {
	// NOTE: getmntinfo() is not usable because it is not thread-safe... so we use getfsstat()
	
	int retCode = getfsstat(NULL, 0, MNT_WAIT);
	if (retCode == -1) {
		CPPDEVTK_ASSERT(errno != EINTR);
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "getfsstat() failed");
	}
	if (retCode == 0) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(MakeErrorCode(base::errc::io_error), "no mounted file system found");
	}
	CPPDEVTK_LOG_DEBUG("number of mounted file systems: " << retCode);
	
	::std::vector<struct statfs> buf(retCode);
	memset(&buf[0], 0, (buf.size() * sizeof(struct statfs)));
	retCode = getfsstat(&buf[0], (buf.size() * sizeof(struct statfs)), MNT_WAIT);
	if (retCode == -1) {
		CPPDEVTK_ASSERT(errno != EINTR);
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(GetLastSystemErrorCode(), "getfsstat() failed");
	}
	if (retCode == 0) {
		throw CPPDEVTK_FILESYSTEM_EXCEPTION_W_EC_WA(MakeErrorCode(base::errc::io_error), "no mounted file system found");
	}
	
	QStringList mountPoints;
	
	for (int idx = 0; idx < retCode; ++idx) {
		CPPDEVTK_ASSERT(buf[idx].f_mntfromname != NULL);
		CPPDEVTK_ASSERT(buf[idx].f_mntonname != NULL);
		
		if (ignoreSpecialFileSystems) {
			struct statfs buf1;
			memset(&buf1, 0, sizeof(buf1));
			if (statfs(buf[idx].f_mntfromname, &buf1) != 0) {
				CPPDEVTK_LOG_WARN("failed to stat " << buf[idx].f_mntfromname);
				continue;
			}
			
			memset(&buf1, 0, sizeof(buf1));
			if (statfs(buf[idx].f_mntonname, &buf1) != 0) {
				CPPDEVTK_LOG_WARN("failed to stat " << buf[idx].f_mntonname);
				continue;
			}
			if (buf1.f_blocks == 0) {
				CPPDEVTK_LOG_TRACE("skipping special filesystem " << buf[idx].f_mntonname);
				continue;
			}
		}
		
		QString mountPoint = QDir::fromNativeSeparators(CPPDEVTK_U2Q(buf[idx].f_mntonname));
		CPPDEVTK_ASSERT(!mountPoint.isEmpty());
		if (!mountPoint.endsWith('/')) {
			mountPoint += '/';
		}
		CPPDEVTK_LOG_DEBUG("found mount point: " << mountPoint);
		mountPoints.append(mountPoint);
	}
	
	return mountPoints;	
}


}	// namespace util
}	// namespace cppdevtk
