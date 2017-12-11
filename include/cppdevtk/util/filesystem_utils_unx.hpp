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


#ifndef CPPDEVTK_UTIL_FILESYSTEM_UTILS_UNX_HPP_INCLUDED_
#define CPPDEVTK_UTIL_FILESYSTEM_UTILS_UNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include "filesystem_exception.hpp"

#include <QtCore/QStringList>
#include <QtCore/QString>

#include <climits>


#define CPPDEVTK_NAME_MAX NAME_MAX	// NAME_MAX does not include the terminating null
#define CPPDEVTK_PATH_MAX PATH_MAX	// PATH_MAX does include the terminating null


namespace cppdevtk {
namespace util {


enum DeviceType {
	dtDisk,
	dtCd
};


typedef int fhandle_t;


static const fhandle_t kInvalidFileHandle = -1;


/// \throw ::cppdevtk::base::RuntimeException
/// \throw ::cppdevtk::util::FilesystemException
/// \return mount points that end with '/' (empty list if no error and no mount point found)
CPPDEVTK_UTIL_API QStringList GetMountPoints(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId);

/// \throw ::cppdevtk::base::RuntimeException
/// \throw ::cppdevtk::util::FilesystemException
/// \arg deviceName
/// - On Linux has the form /dev/sr1 (includes _PATH_DEV (/dev))
/// - On Mac OS X has the form disk2 (BSD name)
/// \return mount points that end with '/' (empty list if no error and no mount point found)
CPPDEVTK_UTIL_API QStringList GetMountPointsFromDeviceName(const QString& deviceName);

/// \throw ::cppdevtk::base::RuntimeException
/// \return
/// - On Linux has the form /dev/sr1 (includes _PATH_DEV (/dev))
/// - On Mac OS X has the form disk2 (BSD name)
/// - empty string if no error and no device name found
CPPDEVTK_UTIL_API QString GetDeviceName(DeviceType deviceType, const QString& serialNumber,
		unsigned long vendorId, unsigned long productId);

/// \throw ::cppdevtk::util::FilesystemException
/// \return
/// - On Linux has the form /dev/sr1 (includes _PATH_DEV (/dev))
/// - On Mac OS X has the form disk2 (BSD name)
/// - empty string if no error and no device name found
CPPDEVTK_UTIL_API QString GetDeviceNameFromMountPoint(const QString& mountPoint);

/// \return
/// - On Linux has the form /dev/sr1 (includes _PATH_DEV (/dev))
/// - On Mac OS X has the form disk2 (BSD name)
/// - empty string: check \a errorCode to see if no error or no device name found
CPPDEVTK_UTIL_API QString GetDeviceNameFromMountPoint(const QString& mountPoint, ::cppdevtk::base::ErrorCode& errorCode);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline CPPDEVTK_UTIL_API QString GetDeviceNameFromMountPoint(const QString& mountPoint,
		::cppdevtk::base::ErrorCode& errorCode) {
	errorCode = ::cppdevtk::base::ErrorCode();
	try {
		return GetDeviceNameFromMountPoint(mountPoint);
	}
	catch (const FilesystemException& exc) {
		errorCode = exc.ErrorCodeRef();
		return QString();
	}
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_FILESYSTEM_UTILS_UNX_HPP_INCLUDED_
