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


#include <cppdevtk/base/error_code.hpp>
#include <cppdevtk/base/error_category.hpp>


namespace cppdevtk {
namespace base {


ErrorCode::ErrorCode() CPPDEVTK_NOEXCEPT: value_(sys_err::success), pErrorCategory_(&SystemCategoryRef()) {}

void ErrorCode::Clear() CPPDEVTK_NOEXCEPT {
	value_ = sys_err::success;
	pErrorCategory_ = &SystemCategoryRef();
}

const ErrorCategory& ErrorCode::CategoryRef() const CPPDEVTK_NOEXCEPT {
	return *pErrorCategory_;
}

ErrorCondition ErrorCode::GetDefaultErrorCondition() const CPPDEVTK_NOEXCEPT {
	return pErrorCategory_->GetDefaultErrorCondition(value_);
}

QString ErrorCode::GetMessage() const {
	return pErrorCategory_->GetMessage(value_);
}

QString ErrorCode::ToString() const {
	return QString("category: %1; error: %2 (%3)").arg(pErrorCategory_->GetName(), QString::number(value_), GetMessage());
}


namespace sys_err {


CPPDEVTK_BASE_API ErrorCode MakeErrorCode(sys_err_t sysErr) CPPDEVTK_NOEXCEPT {
	return ErrorCode(sysErr, SystemCategoryRef());
}


}	// namespace sys_err


namespace errc {


CPPDEVTK_BASE_API ErrorCode MakeErrorCode(errc_t errCond) CPPDEVTK_NOEXCEPT {
	return ErrorCode(errCond, GenericCategoryRef());
}


}	// namespace errc


CPPDEVTK_BASE_API ErrorCode MakeSystemErrorCode(int value) CPPDEVTK_NOEXCEPT {
	return ErrorCode(value, SystemCategoryRef());
}


CPPDEVTK_BASE_API bool operator==(const ErrorCode& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT {
	return (lhs.CategoryRef() == rhs.CategoryRef()) && (lhs.GetValue() == rhs.GetValue());
}

CPPDEVTK_BASE_API bool operator<(const ErrorCode& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT {
	return (lhs.CategoryRef() < rhs.CategoryRef())
			|| ((lhs.CategoryRef() == rhs.CategoryRef()) && (lhs.GetValue() < rhs.GetValue()));
}


CPPDEVTK_BASE_API bool operator==(const ErrorCode& lhs, const ErrorCondition& rhs) CPPDEVTK_NOEXCEPT {
	return lhs.CategoryRef().IsEquivalent(lhs.GetValue(), rhs)
			|| rhs.CategoryRef().IsEquivalent(lhs, rhs.GetValue());
}

CPPDEVTK_BASE_API bool operator==(const ErrorCondition& lhs, const ErrorCode& rhs) CPPDEVTK_NOEXCEPT {
	return lhs.CategoryRef().IsEquivalent(rhs, lhs.GetValue())
			|| rhs.CategoryRef().IsEquivalent(rhs.GetValue(), lhs);
}


}	// namespace base
}	// namespace cppdevtk
