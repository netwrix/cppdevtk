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
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/qiostream.hpp>

#include <QtCore/QString>
#include <QtCore/QtDebug>

#include <cstdlib>


#if (!CPPDEVTK_PLATFORM_ANDROID)
#define CPPDEVTK_COUT ::cppdevtk::base::qcout
#define CPPDEVTK_CERR ::cppdevtk::base::qcerr
#else
#define CPPDEVTK_COUT qDebug()
#define CPPDEVTK_CERR qCritical()
#endif


using ::cppdevtk::base::qcerr;
using ::cppdevtk::base::qcout;
using ::cppdevtk::base::Exception;
using ::std::exception;


#if (CPPDEVTK_PLATFORM_ANDROID)
__attribute__((visibility("default")))
#endif
int main(int argc, char* argv[]) try {
	using ::cppdevtk::util::CoreApplication;
	
	
	::cppdevtk::test_config::InitResources();
	
	CoreApplication::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			CPPDEVTK_SHORT_NAME_SANITIZED, CPPDEVTK_VERSION_STRING, CPPDEVTK_TEST_CONFIG_SHORT_NAME_SANITIZED);
	
	CoreApplication coreApplication(argc, argv);
	
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
