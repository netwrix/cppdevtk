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


#include <cppdevtk/base/config.hpp>
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_CLANG)
#	pragma clang diagnostic ignored "-Wpotentially-evaluated-expression"
#endif

#include <cppdevtk/base/throwable.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/string_conv.hpp>
#include <cppdevtk/base/name_mangling.hpp>
#include <cppdevtk/base/typeinfo.hpp>

#include <QtCore/QString>

#include <exception>
#include <stdexcept>


#if (!CPPDEVTK_THROWABLE_DISABLE_TERMINATE)
#	define CPPDEVTK_THROWABLE_LOG_TERMINATE CPPDEVTK_LOG_FATAL
#else
#	define CPPDEVTK_THROWABLE_LOG_TERMINATE CPPDEVTK_LOG_ERROR
#endif


namespace cppdevtk {
namespace base {


void Throwable::Throw() const {
#	if 0	// be optimistic
	DoThrow();
#	else	// be pessimistic (aka realistic)
	using ::std::terminate;
	using ::std::runtime_error;
	
	try {
		DoThrow();
	}
	catch (const Throwable& exc) {
		if (typeid(*this) != typeid(exc)) {
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
			
			const QString kTypeInfoActualName = typeid(exc).name();
			QString demangledActualName;
			if (!kTypeInfoActualName.isEmpty()) {
				if (IsMangled(kTypeInfoActualName)) {
					demangledActualName = Demangle(kTypeInfoActualName);
				}
				if (demangledActualName.isEmpty()) {
					demangledActualName = kTypeInfoActualName;
				}
			}
			
			CPPDEVTK_THROWABLE_LOG_TERMINATE(QString("Throwable::DoThrow() not or incorrectly overridden (type mismatch)!!!"
					"\nExpected type: %1\nActual type: %2").arg(demangledExpectedName, demangledActualName));
			
			CPPDEVTK_ASSERT(0 && "Throwable::DoThrow() not or incorrectly overridden (type mismatch)!!!");
			
#			if (!CPPDEVTK_THROWABLE_DISABLE_TERMINATE)
			terminate();
#			else
			throw runtime_error(Q2A(excMsg));
#			endif
		}
		
		throw;
	}
	catch (...) {
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
		
		CPPDEVTK_THROWABLE_LOG_TERMINATE(QString("Throwable::DoThrow() not or incorrectly overridden (type mismatch)!!!"
				"\nExpected type: %1\nActual type: unknown exception").arg(demangledName));
		
		CPPDEVTK_ASSERT(0 && "Throwable::DoThrow() not or incorrectly overridden (type mismatch)!!!");
		
#		if (!CPPDEVTK_THROWABLE_DISABLE_TERMINATE)
		terminate();
#		else
		throw runtime_error(Q2A(excMsg));
#		endif
	}
	
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
	
	CPPDEVTK_THROWABLE_LOG_TERMINATE(
			QString("Throwable::DoThrow() did not throw!!! Not or incorrectly overridden in: %1").arg(demangledName));
	
	CPPDEVTK_ASSERT(0 && "Throwable::DoThrow() did not throw (not or incorrectly overridden).");
	
#	if (!CPPDEVTK_THROWABLE_DISABLE_TERMINATE)
	terminate();
#	else
	throw runtime_error(Q2A(excMsg));
#	endif
	
#	endif
}


}	// namespace base
}	// namespace cppdevtk
