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


#include "config.hpp"
#include "init_resources.hpp"
#include <cppdevtk/util/core_application.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/util/filesystem_exception.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/exception.hpp>

#include <QtCore/QString>
#include <QtCore/QtDebug>

#include <cstdlib>


#if (!CPPDEVTK_PLATFORM_ANDROID)
#define CPPDEVTK_COUT ::cppdevtk::base::qcout
#define CPPDEVTK_CERR ::cppdevtk::base::qcerr
#else
#define CPPDEVTK_COUT qDebug()
#define CPPDEVTK_CERR qInfo()
#endif


using ::cppdevtk::base::qcerr;
using ::cppdevtk::base::qcout;
using ::cppdevtk::base::Exception;
using ::std::exception;

using namespace ::cppdevtk::util;


#if (CPPDEVTK_PLATFORM_ANDROID)
__attribute__((visibility("default")))
#endif
int main(int argc, char* argv[]) try {
	using ::cppdevtk::util::CoreApplication;
	
	
	::cppdevtk::test_util::InitResources();
	
	CoreApplication::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			CPPDEVTK_SHORT_NAME_SANITIZED, CPPDEVTK_VERSION_STRING, CPPDEVTK_TEST_UTIL_SHORT_NAME_SANITIZED);
	
	CoreApplication coreApplication(argc, argv);
	
	CPPDEVTK_MAKE_FILESYSTEM_EXCEPTION_W_EC_WA_SRC_DST(fsExc, ::cppdevtk::base::ErrorCode(),
			"testing fs exc", "aSrcPath", "aDstPath");
	CPPDEVTK_COUT << "fsExc.What(): " << fsExc.What() << endl;
	CPPDEVTK_COUT << "fsExc.ToString(): " << fsExc.ToString() << endl;
	
	
	/*
	
	#if (CPPDEVTK_PLATFORM_WINDOWS)
	QString srcPath("d:/cososys/x86/third_party/nss_nspr");
	QString dstPath1("d:/tmp/testfs/1");
	QString dstPath2("d:/tmp/testfs/2");
	#else
	QString srcPath("/opt/cososys/third_party/nss_nspr");
	QString dstPath1("/tmp/testfs/1");
	QString dstPath2("/tmp/testfs/2");
	#endif
	QString dstPath1abc(dstPath1 + "/a/b/c");
	
	::cppdevtk::base::ErrorCode errorCode;
	
	if (!CopyDirectoryRecursively(srcPath, dstPath1abc, false, false, errorCode)) {
		CPPDEVTK_LOG_ERROR("copy " << dstPath1abc << " error code: " << errorCode.ToString());
		return EXIT_FAILURE;
	}
	if (!CopyDirectoryRecursively(srcPath, dstPath2, false, false, errorCode)) {
		CPPDEVTK_LOG_ERROR("copy " << dstPath2 << " error code: " << errorCode.ToString());
		return EXIT_FAILURE;
	}
	if (!RemoveDirectoryRecursively(dstPath1, true, errorCode)) {
		CPPDEVTK_LOG_ERROR("remove " << dstPath1 << " error code: " << errorCode.ToString());
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
	
	*/
	
	/*
	::cppdevtk::base::ErrorCode errorCode;
	QStringList mountPoints = GetMountPoints(true, errorCode);
	if (errorCode) {
		CPPDEVTK_CERR << "GetMountPoints() failed; errorCode: " << errorCode << endl;
		return EXIT_FAILURE;
	}
	CPPDEVTK_COUT << "mountPoints: " << mountPoints.join(' ') << endl;
	
	mountPoints = GetMountPointsFromPath("/media/canita/43CB-0B4D", errorCode);
	if (errorCode) {
		CPPDEVTK_CERR << "GetMountPointsFromPath() failed; errorCode: " << errorCode << endl;
		return EXIT_FAILURE;
	}
	CPPDEVTK_COUT << "mountPointsFromPath: " << mountPoints.join(' ') << endl;
	
	mountPoints = GetMountPointsFromDeviceName("/dev/sdb1");
	CPPDEVTK_COUT << "mountPointsFromDeviceName: " << mountPoints.join(' ') << endl;
	
	QString deviceName = GetDeviceNameFromMountPoint("/media/canita/43CB-0B4D/", errorCode);
	if (errorCode) {
		CPPDEVTK_CERR << "GetDeviceNameFromMountPoint() failed; errorCode: " << errorCode << endl;
		return EXIT_FAILURE;
	}
	CPPDEVTK_COUT << "deviceName: " << deviceName << endl;
	*/
	
#	if 1
	CPPDEVTK_COUT << "done!" << endl;
	return EXIT_SUCCESS;
#	else
	try {
		return coreApplication.exec();
	}
	catch (const exception& exc) {
		const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
				exc.what(), Exception::GetDetailedInfo(exc));
		CPPDEVTK_LOG_ERROR(kErrMsg);
		CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
	catch (...) {
		const QString kErrMsg("caught unknown exception!!!");
		CPPDEVTK_LOG_ERROR(kErrMsg);
		CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
#	endif
}
catch (const exception& exc) {
	const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
			exc.what(), Exception::GetDetailedInfo(exc));
	CPPDEVTK_LOG_FATAL(kErrMsg);
	CPPDEVTK_CERR << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
catch (...) {
	const QString kErrMsg("caught unknown exception!!!");
	CPPDEVTK_LOG_FATAL(kErrMsg);
	CPPDEVTK_CERR << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
