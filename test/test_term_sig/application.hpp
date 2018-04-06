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


#ifndef CPPDEVTK_TEST_TERMINATION_SIGNALS_APPLICATION_HPP_INCLUDED_
#define CPPDEVTK_TEST_TERMINATION_SIGNALS_APPLICATION_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/util/core_application.hpp>
#if (!CPPDEVTK_PLATFORM_ANDROID)
#include <cppdevtk/base/qiostream.hpp>
#else
#include <QtCore/QtDebug>
#endif

// We use Application::exec() in this test, meaning event loop, so let's test here ConsoleKit/logind (no time for separate test app)
#if (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)
#include <cppdevtk/util/logind_manager_lnx.hpp>
#include <cppdevtk/util/logind_session_lnx.hpp>
#include <cppdevtk/util/console_kit_manager_lnx.hpp>
#include <cppdevtk/util/console_kit_session_lnx.hpp>
#endif


#if (!CPPDEVTK_PLATFORM_ANDROID)
#define CPPDEVTK_COUT ::cppdevtk::base::qcout
#define CPPDEVTK_CERR ::cppdevtk::base::qcerr
#else
#define CPPDEVTK_COUT qDebug()
#define CPPDEVTK_CERR qInfo()
#endif


namespace cppdevtk {
namespace test_term_sig {


class Application: public ::cppdevtk::util::CoreApplication {
	Q_OBJECT
public:
	Application(int& argc, char** argv);
	virtual ~Application();
public Q_SLOTS:
#	if (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)
	void OnActiveSessionChanged(bool isActive);
	void OnSessionLocked();
	void OnSessionUnlocked();
#	endif
private:
	Q_DISABLE_COPY(Application)
	
#	if (CPPDEVTK_PLATFORM_LINUX && ! CPPDEVTK_PLATFORM_ANDROID)
	::std::auto_ptr< ::cppdevtk::util::LogindSession> pLogindSession_;
	::std::auto_ptr< ::cppdevtk::util::ConsoleKitSession> pConsoleKitSession_;
#	endif
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline Application::~Application() {
	CPPDEVTK_COUT << "Application dtor" << endl;
}


}	// namespace test_term_sig
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_TERMINATION_SIGNALS_APPLICATION_HPP_INCLUDED_
