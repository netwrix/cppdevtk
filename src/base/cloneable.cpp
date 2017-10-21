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


#include <cppdevtk/base/cloneable.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/string_conv.hpp>
#include <cppdevtk/base/name_mangling.hpp>
#include <cppdevtk/base/typeinfo.hpp>

#include <QtCore/QString>

#include <exception>
#include <stdexcept>
#include <cstdlib>


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_CLANG)
#	pragma GCC diagnostic ignored "-Wpotentially-evaluated-expression"
#endif


#if (!CPPDEVTK_CLONEABLE_DISABLE_TERMINATE)
#	define CPPDEVTK_CLONEABLE_LOG_TERMINATE CPPDEVTK_LOG_FATAL
#else
#	define CPPDEVTK_CLONEABLE_LOG_TERMINATE CPPDEVTK_LOG_ERROR
#endif


namespace cppdevtk {
namespace base {


::std::auto_ptr<Cloneable> Cloneable::Clone() const {
#	if 0	// be optimistic
	return ::std::auto_ptr<Cloneable>(DoClone());
#	else	// be pessimistic (aka realistic)
	using ::std::abort;
	using ::std::runtime_error;
	
	::std::auto_ptr<Cloneable> pCloneable(DoClone());
	
	if (pCloneable.get() == NULL) {
		const QString kTypeInfoName = typeid(*this).name();
		QString demangledName;
		if (!kTypeInfoName.isEmpty()) {
			if (IsMangled(kTypeInfoName)) {
				demangledName = Demangle(kTypeInfoName);
			}
			if (demangledName.isEmpty()) {
				demangledName = kTypeInfoName;
			}
		}
		
		CPPDEVTK_CLONEABLE_LOG_TERMINATE(
				QString("Cloneable::DoClone() not or incorrectly overridden (returned NULL) in: %1").arg(demangledName));
		
		CPPDEVTK_ASSERT(0 && "Cloneable::DoClone() not or incorrectly overridden (returned NULL)!!!");
		
#		if (!CPPDEVTK_CLONEABLE_DISABLE_TERMINATE)
		abort();
#		else
		throw runtime_error(Q2A(excMsg));
#		endif
	}
	
	/*
	CPPDEVTK_LOG_DEBUG("typeid(*this): " << typeid(*this).name()
			<< "\ntypeid(*(pCloneable.get())): " << typeid(*(pCloneable.get())).name());
	*/
	
	if (typeid(*this) != typeid(*(pCloneable.get()))) {
		const QString kTypeInfoExpectedName = typeid(*this).name();
		QString demangledExpectedName;
		if (!kTypeInfoExpectedName.isEmpty()) {
			if (IsMangled(kTypeInfoExpectedName)) {
				demangledExpectedName = Demangle(kTypeInfoExpectedName);
			}
			if (demangledExpectedName.isEmpty()) {
				demangledExpectedName = kTypeInfoExpectedName;
			}
		}
		
		const QString kTypeInfoActualName = typeid(*(pCloneable.get())).name();
		QString demangledActualName;
		if (!kTypeInfoActualName.isEmpty()) {
			if (IsMangled(kTypeInfoActualName)) {
				demangledActualName = Demangle(kTypeInfoActualName);
			}
			if (demangledActualName.isEmpty()) {
				demangledActualName = kTypeInfoActualName;
			}
		}
		
		CPPDEVTK_CLONEABLE_LOG_TERMINATE(QString("Cloneable::DoClone() not or incorrectly overridden (type mismatch)!!!"
				 "\nExpected type: %1\nActual type: %2").arg(demangledExpectedName, demangledActualName));
		
		CPPDEVTK_ASSERT(0 && "Cloneable::DoClone() not or incorrectly overridden (type mismatch)!!!");
		
#		if (!CPPDEVTK_CLONEABLE_DISABLE_TERMINATE)
		abort();
#		else
		throw runtime_error(Q2A(excMsg));
#		endif
	}
	
	return pCloneable;
#	endif
}


}	// namespace base
}	// namespace cppdevtk
