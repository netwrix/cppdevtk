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


#include <cppdevtk/base/demangle.hpp>
#if (!(CPPDEVTK_COMPILER_GCC || CPPDEVTK_COMPILER_CLANG))
#	error "This file is gcc/clang specific!!!"
#endif

#include <cppdevtk/base/string_conv.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QString>

#if (CPPDEVTK_HAVE_CXXABI_H)
#include <cxxabi.h>
#else
#error "CPPDEVTK_HAVE_CXXABI_H required!!!"
#endif

#include CPPDEVTK_TR1_HEADER(memory)
#include <cstddef>
#include <cstdlib>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API QString Demangle(const QString& mangledName) {
	if (mangledName.isEmpty()
			/*
			|| !(mangledName.startsWith("_Z") || mangledName.startsWith("__Z")
			|| mangledName.startsWith("N")
			|| mangledName.startsWith("_GLOBAL_")
#			if (CPPDEVTK_PLATFORM_MACOSX)	// Swift (not supported by __cxa_demangle())
			// || mangledName.startsWith("_T") || mangledName.startsWith("__T")
#			endif
			)
			*/
			) {
		return "";
	}
	
	QString demangledName;
	int status = -1;
	
#	if (!CPPDEVTK_HAVE_ABI___CXA_DEMANGLE)
#		error "CPPDEVTK_HAVE_ABI___CXA_DEMANGLE required!!!"
#	endif
	CPPDEVTK_TR1_NS::shared_ptr<char> pDemangledName(::abi::__cxa_demangle(Q2A(mangledName).c_str(), NULL, NULL, &status),
			&::std::free);
	switch (status) {
		case -3:
			CPPDEVTK_LOG_ERROR("::abi::__cxa_demangle(): one of the arguments is invalid");
			CPPDEVTK_ASSERT(0 && "::abi::__cxa_demangle(): one of the arguments is invalid");
			CPPDEVTK_ASSERT(pDemangledName.get() == NULL);
			break;
		case -2:
			// CPPDEVTK_LOG_ERROR(): avoid too many unsignificant errors...
			CPPDEVTK_LOG_TRACE("::abi::__cxa_demangle(): argument mangledName is not a valid name under the C++ ABI mangling rules"
					<< "; mangledName: " << mangledName);
			CPPDEVTK_ASSERT(pDemangledName.get() == NULL);
			break;
		case -1:
			CPPDEVTK_LOG_ERROR("::abi::__cxa_demangle(): a memory allocation failure occurred");
			CPPDEVTK_ASSERT(pDemangledName.get() == NULL);
			break;
		case 0:
			CPPDEVTK_ASSERT(pDemangledName.get() != NULL);
			demangledName = A2Q(pDemangledName.get());
			break;
		default:
			CPPDEVTK_LOG_ERROR("::abi::__cxa_demangle() returned unknown code: " << status);
			CPPDEVTK_ASSERT(0 && "::abi::__cxa_demangle() returned unknown code");
			break;
	}
	
	CPPDEVTK_ASSERT((status != 0) || !demangledName.isEmpty());
	return demangledName;
}


}	// namespace base
}	// namespace cppdevtk
