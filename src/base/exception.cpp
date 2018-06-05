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


#include <cppdevtk/base/exception.hpp>
#include <cppdevtk/base/tstring_conv.hpp>
#if (!(!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE))
#include <cppdevtk/base/unused.hpp>
#endif


namespace cppdevtk {
namespace base {


Exception::Exception(const SourceCodeInfo& throwPoint) /* CPPDEVTK_NOEXCEPT */: Throwable(), Cloneable(),
		QStringizable(), QtException(), pStdWhatMsg_(new ::std::string()), pStackTrace_(new StackTrace(
#		if (!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_STACK_TRACE_IN_RELEASE)
		true
#		else
		false
#		endif
		)),
		throwPoint_(
#		if (!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE)
		throwPoint
#		else
		SourceCodeInfo()
#		endif
		), pCause_() {
#	if (!(!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE))
	SuppressUnusedWarning(throwPoint);
#	endif
}

Exception::Exception(const SourceCodeInfo& throwPoint, const Exception& cause) /* CPPDEVTK_NOEXCEPT */:
		Throwable(), Cloneable(), QStringizable(), QtException(), pStdWhatMsg_(new ::std::string()),
		pStackTrace_(new StackTrace(
#		if (!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_STACK_TRACE_IN_RELEASE)
		true
#		else
		false
#		endif
		)), throwPoint_(
#		if (!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE)
		throwPoint
#		else
		SourceCodeInfo()
#		endif
		), pCause_(cause.Clone().release()) {
#	if (!(!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE))
	SuppressUnusedWarning(throwPoint);
#	endif
}

QString Exception::ToString() const {
	QString stringized;
	
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
	
	stringized = QString("Runtime type: ") + demangledName + "\nMessage: " + DoOwnWhat();
	
#	if (!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_THROW_POINT_IN_RELEASE)
	stringized += "\nThrow point: " + throwPoint_.ToString();
#	endif

#	if (!defined(NDEBUG) || CPPDEVTK_EXCEPTION_ENABLE_STACK_TRACE_IN_RELEASE)
	if (pStackTrace_) {
		stringized += "\nStack trace:\n" + pStackTrace_->ToString();
	}
	else {
		stringized += "\nStack trace not available!";
	}
#	endif
	
	if (pCause_) {
		stringized += "\nCaused by: " + pCause_->ToString();
	}
	
	return stringized;
}

const char* Exception::what() const CPPDEVTK_NOEXCEPT {
	try {
#		if (CPPDEVTK_DISABLE_UNICODE)
		*pStdWhatMsg_ = Q2A(What());
#		else
		*pStdWhatMsg_ = Q2Utf8(What());
#		endif
		return pStdWhatMsg_->c_str();
	}
	catch (...) {
		//pStdWhatMsg_->clear();
		// return "::cppdevtk::base::Exception";
		return typeid(*this).name();
	}
}

Exception::CausePtrType Exception::GetInitialCause() const {
	CausePtrType pInitialCause(pCause_);
	
	if (pInitialCause) {
		for (;;) {
			CausePtrType pNextCause(pInitialCause->GetCause());
			if (pNextCause) {
				pInitialCause = pNextCause;
			}
			else {
				break;
			}
		}
	}
	
	return pInitialCause;
}

QString Exception::GetDetailedInfo(const ::std::exception& exc) {
	QString detailedInfo;
	
	const Exception* pException = dynamic_cast<const Exception*>(&exc);
	if (pException != NULL) {
		detailedInfo = pException->ToString();
	}
	else {
		const QString kTypeInfoName = typeid(exc).name();
		QString demangledName;
		if (!kTypeInfoName.isEmpty()) {
			if (IsMangled(kTypeInfoName)) {
				demangledName = Demangle(kTypeInfoName);
			}
			if (demangledName.isEmpty()) {
				demangledName = kTypeInfoName;
			}
		}
		
		detailedInfo = QString("Runtime type: ") + demangledName + "\nMessage: " + Utf82Q(exc.what());
	}
	
	return detailedInfo;
}


}	// namespace base
}	// namespace cppdevtk
