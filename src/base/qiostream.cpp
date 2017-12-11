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


#include <cppdevtk/base/qiostream.hpp>

#include <QtCore/QIODevice>

#include <cstdio>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API QTextStream qcin(stdin, (QIODevice::ReadOnly | QIODevice::Text));
CPPDEVTK_BASE_API QTextStream qcout(stdout, (QIODevice::WriteOnly | QIODevice::Text));	// QIODevice::Append does not work...
CPPDEVTK_BASE_API QTextStream qcerr(stderr, (QIODevice::WriteOnly | QIODevice::Text));
CPPDEVTK_BASE_API QTextStream qclog(stderr, (QIODevice::WriteOnly | QIODevice::Text));


}	// namespace base
}	// namespace cppdevtk
