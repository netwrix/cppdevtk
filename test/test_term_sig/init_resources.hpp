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


#ifndef CPPDEVTK_TEST_TERMINATION_SIGNALS_INIT_RESOURCES_HPP_INCLUDED_
#define CPPDEVTK_TEST_TERMINATION_SIGNALS_INIT_RESOURCES_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/base/init_resources.hpp>
#include <cppdevtk/util/init_resources.hpp>

#include <QtCore/QDir>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize resources for cppdevtk/test_term_sig subproject.
/// \remark Not in namespace.
void CppDevTkTestTermSigInitResources();


namespace cppdevtk {
namespace test_term_sig {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize all resources for cppdevtk/test_term_sig subproject.
void InitResources();


}	// namespace test_term_sig
}	// namespace cppdevtk




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline void CppDevTkTestTermSigInitResources() {
#	ifndef CPPDEVTK_SHARED
	Q_INIT_RESOURCE(cppdevtk_test_term_sig);
	Q_INIT_RESOURCE(cppdevtk_test_term_sig_customization);
#	endif
}


namespace cppdevtk {
namespace test_term_sig {


inline void InitResources() {
	CppDevTkBaseInitResources();
	CppDevTkUtilInitResources();
	
	CppDevTkTestTermSigInitResources();
}


}	// namespace test_term_sig
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_TERMINATION_SIGNALS_INIT_RESOURCES_HPP_INCLUDED_
