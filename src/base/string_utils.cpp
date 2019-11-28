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


#include <cppdevtk/base/string_utils.hpp>

#include <QtCore/QtGlobal>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API bool IsAscii(const QChar& chr) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return chr.toLatin1() != 0;
#	else
	return chr.toAscii() != 0;
#	endif
}

CPPDEVTK_BASE_API bool IsAscii(const QString& str) {
	const int kLength = str.length();
	for (int idx = 0; idx < kLength; ++idx) {
		if (!IsAscii(str.at(idx))) {
			return false;
		}
	}
	
	return true;
}

CPPDEVTK_BASE_API bool IsLatin1(const QString& str) {
	const int kLength = str.length();
	for (int idx = 0; idx < kLength; ++idx) {
		if (!IsLatin1(str.at(idx))) {
			return false;
		}
	}
	
	return true;
}


}	// namespace base
}	// namespace cppdevtk
