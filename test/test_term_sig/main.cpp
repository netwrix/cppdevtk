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
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(disable: 4459)	// C4459: declaration of 'item' hides global declaration
#endif

#include "init_resources.hpp"
#include "application.hpp"
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/on_block_exit.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/unused.hpp>

#include <QtCore/QString>

#include <cstdlib>


using ::cppdevtk::base::Exception;
using ::std::exception;


#if (CPPDEVTK_PLATFORM_ANDROID)
__attribute__((visibility("default")))
#endif
int main(int argc, char* argv[]) try {
	using ::cppdevtk::test_term_sig::Application;
	
	
	::cppdevtk::test_term_sig::InitResources();
	
	Application::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			CPPDEVTK_SHORT_NAME_SANITIZED, CPPDEVTK_VERSION_STRING, CPPDEVTK_TEST_TERMINATION_SIGNALS_SHORT_NAME_SANITIZED);
	
	Application application(argc, argv);
	CPPDEVTK_ASSERT(!application.GetQuitOnTerminationSignals());
	application.SetQuitOnTerminationSignals(true);
	CPPDEVTK_ASSERT(application.GetQuitOnTerminationSignals());
	
#	if (BOOST_VERSION >= 105000)
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN(void) {
#	else
	CPPDEVTK_ON_BLOCK_EXIT_BEGIN() {
#	endif
		try {
			static_cast<Application*>(QCoreApplication::instance())->SetQuitOnTerminationSignals(false);
		}
		CPPDEVTK_ON_BLOCK_EXIT_CATCH
	}
	CPPDEVTK_ON_BLOCK_EXIT_END
	
	CPPDEVTK_COUT << "Press CTRL+C to Quit" << endl;
	
	const int kRetCode = application.exec();
	
	CPPDEVTK_COUT << "Done! Exiting with retCode: " << kRetCode << endl;
	return kRetCode;
}
catch (const exception& exc) {
	const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
			exc.what(), Exception::GetDetailedInfo(exc));
	CPPDEVTK_LOG_ERROR(kErrMsg);
	CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
	
	CPPDEVTK_ASSERT((dynamic_cast<const ::cppdevtk::base::LogicException*>(&exc) != NULL)
			&& (dynamic_cast<const ::std::logic_error*>(&exc) != NULL));
	
	return EXIT_FAILURE;
}
catch (...) {
	const QString kErrMsg("caught unknown exception!!!");
	CPPDEVTK_LOG_ERROR(kErrMsg);
	CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
