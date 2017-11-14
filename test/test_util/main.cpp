/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2017 CoSoSys Ltd <info@cososys.com>\n
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


using ::cppdevtk::base::qcerr;
using ::cppdevtk::base::Exception;
using ::std::exception;

using namespace ::cppdevtk::util;


int main(int argc, char* argv[]) try {
	::cppdevtk::test_util::InitResources();
	
	
	CPPDEVTK_MAKE_FILESYSTEM_EXCEPTION_W_EC_WA_SRC_DST(fsExc, ::cppdevtk::base::MakeSystemErrorCode(0),
			"testing fs exc", "aSrcPath", "aDstPath");
	qDebug() << "fsExc.What(): " << fsExc.What();
	qDebug() << "fsExc.ToString(): " << fsExc.ToString();
	
	
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
	
	
	::cppdevtk::util::CoreApplication coreApplication(argc, argv);
	
	try {
		return coreApplication.exec();
	}
	catch (const exception& exc) {
		const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
				exc.what(), Exception::GetDetailedInfo(exc));
		CPPDEVTK_LOG_ERROR(kErrMsg);
		qcerr << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
	catch (...) {
		const QString kErrMsg("caught unknown exception!!!");
		CPPDEVTK_LOG_ERROR(kErrMsg);
		qcerr << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
}
catch (const exception& exc) {
	const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
			exc.what(), Exception::GetDetailedInfo(exc));
	CPPDEVTK_LOG_FATAL(kErrMsg);
	qcerr << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
catch (...) {
	const QString kErrMsg("caught unknown exception!!!");
	CPPDEVTK_LOG_FATAL(kErrMsg);
	qcerr << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
