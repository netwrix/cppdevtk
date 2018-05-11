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
private:
	Q_DISABLE_COPY(Application)
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline Application::~Application() {
	CPPDEVTK_COUT << "Application dtor" << endl;
}

inline Application::Application(int& argc, char** argv): ::cppdevtk::util::CoreApplication(argc, argv) {
	CPPDEVTK_COUT << "Application ctor" << endl;
}


}	// namespace test_term_sig
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_TERMINATION_SIGNALS_APPLICATION_HPP_INCLUDED_
