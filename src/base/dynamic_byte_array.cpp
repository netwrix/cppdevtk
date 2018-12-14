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


#include <cppdevtk/base/dynamic_byte_array.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QTextStream>

#include <algorithm>
#include <cstddef>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API QString Stringize(const DynamicByteArray& dynamicByteArray, int len, int base) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(((-1 <= len) && ((::std::size_t)len <= dynamicByteArray.size())),
			"argument len has invalid value");
	CPPDEVTK_DBC_CHECK_ARGUMENT(((base == 0) || (base == 2) || (base == 8) || (base == 10) || (base == 16)),
			"argument base has invalid value");
	
	QString stringized;
	
	{
		QTextStream textStream(&stringized);
		const DynamicByteArray::size_type kDynamicByteArraySize = dynamicByteArray.size();
		textStream << "size: " << kDynamicByteArraySize << '\n';
		const DynamicByteArray::size_type kMaxIdx = (len == -1) ? kDynamicByteArraySize : len;
		if (kMaxIdx > 0) {
			if (kMaxIdx == kDynamicByteArraySize) {
				textStream << "all elements:\n[\n";
			}
			else {
				textStream << "first " << kMaxIdx << " elements:\n[\n";
			}
			
			for (DynamicByteArray::size_type idx = 0; idx < kMaxIdx; ++idx) {
				textStream << "byte " << idx << ": ";
				textStream.setIntegerBase(base);
				if ((base != 10) && (base != 0)) {
					textStream << showbase;
				}
				textStream << dynamicByteArray[idx] << '\n';
				if ((base != 10) && (base != 0)) {
					textStream << noshowbase;
				}
				textStream << dec;
			}
			
			textStream << "]";
		}
		
		CPPDEVTK_ASSERT(textStream.status() == QTextStream::Ok);
	}
	
	CPPDEVTK_ASSERT(!stringized.isEmpty());
	return stringized;
}


}	// namespace base
}	// namespace cppdevtk
