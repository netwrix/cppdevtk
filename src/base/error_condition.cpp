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


#include <cppdevtk/base/error_condition.hpp>
#include <cppdevtk/base/error_category.hpp>


namespace cppdevtk {
namespace base {


ErrorCondition::ErrorCondition() CPPDEVTK_NOEXCEPT: value_(errc::success), pErrorCategory_(&GenericCategoryRef()) {}

void ErrorCondition::Clear() CPPDEVTK_NOEXCEPT {
	value_ = errc::success;
	pErrorCategory_ = &GenericCategoryRef();
}

const ErrorCategory& ErrorCondition::CategoryRef() const CPPDEVTK_NOEXCEPT {
	return *pErrorCategory_;
}

QString ErrorCondition::GetMessage() const {
	return pErrorCategory_->GetMessage(value_);
}

QString ErrorCondition::ToString() const {
	return QString("category: %1; error: %2 (%3)").arg(pErrorCategory_->GetName(), QString::number(value_), GetMessage());
}


namespace errc {


CPPDEVTK_BASE_API ErrorCondition MakeErrorCondition(errc_t errCond) CPPDEVTK_NOEXCEPT {
	return ErrorCondition(errCond, GenericCategoryRef());
}


}	// namespace errc


CPPDEVTK_BASE_API ErrorCondition MakeGenericErrorCondition(int value) CPPDEVTK_NOEXCEPT {
	return ErrorCondition(value, GenericCategoryRef());
}


CPPDEVTK_BASE_API bool operator==(const ErrorCondition& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT {
	return (lhs.CategoryRef() == rhs.CategoryRef()) && (lhs.GetValue() == rhs.GetValue());
}

CPPDEVTK_BASE_API bool operator<(const ErrorCondition& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT {
	return (lhs.CategoryRef() < rhs.CategoryRef())
			|| ((lhs.CategoryRef() == rhs.CategoryRef()) && (lhs.GetValue() < rhs.GetValue()));
}


}	// namespace base
}	// namespace cppdevtk
