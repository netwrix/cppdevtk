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


#include <cppdevtk/util/password_requirements.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/unused.hpp>

#include <algorithm>


#define CPPDEVTK_PASSWORD_REQUIREMENTS_ENABLE_LOG_VALIDATION 1
#if (CPPDEVTK_PASSWORD_REQUIREMENTS_ENABLE_LOG_VALIDATION)
#	define CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR(msg)	\
			CPPDEVTK_LOG_ERROR(msg)
#else
#	define CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR(msg)	\
			((void)0)
#endif


namespace cppdevtk {
namespace util {


PasswordRequirements::PasswordRequirements(bool unicode, int pwdMaxLen, int pwdMinLen, int pwdMinLetters,
		int pwdMinUpper, int pwdMinLower, int pwdMinDigits, int pwdMinSymbols, int hintMaxLen, int hintMinLen,
		bool hintNotEqual, int hintMinSimilarity, bool newOldNotEqual, int newOldMinSimilarity) {
	CPPDEVTK_ASSERT(Validate(kDefUnicode_, kPwdDefMaxLen_, kPwdDefMinLen_, kPwdDefMinLetters_, kPwdDefMinUpper_,
			kPwdDefMinLower_, kPwdDefMinDigits_, kPwdDefMinSymbols_, kHintDefMaxLen_, kHintDefMinLen_, kHintDefNotEqual_,
			kHintDefMinSimilarity_, kNewOldDefNotEqual_, kNewOldDefMinSimilarity_));
	
	Set(unicode, pwdMaxLen, pwdMinLen, pwdMinLetters, pwdMinUpper, pwdMinLower, pwdMinDigits, pwdMinSymbols,
			hintMaxLen, hintMinLen, hintNotEqual, hintMinSimilarity, newOldNotEqual, newOldMinSimilarity);
}

void PasswordRequirements::Set(bool unicode, int pwdMaxLen, int pwdMinLen, int pwdMinLetters,
		int pwdMinUpper, int pwdMinLower, int pwdMinDigits, int pwdMinSymbols, int hintMaxLen, int hintMinLen,
		bool hintNotEqual, int hintMinSimilarity, bool newOldNotEqual, int newOldMinSimilarity) {
	CPPDEVTK_DBC_CHECK_ARGUMENT(Validate(unicode, pwdMaxLen, pwdMinLen, pwdMinLetters, pwdMinUpper, pwdMinLower, pwdMinDigits,
			pwdMinSymbols, hintMaxLen, hintMinLen, hintNotEqual, hintMinSimilarity, newOldNotEqual, newOldMinSimilarity),
			"Invalid requirements argument(s)");
	
	unicode_ = unicode;
	pwdMaxLen_ = pwdMaxLen;
	pwdMinLen_ = pwdMinLen;
	pwdMinLetters_ = pwdMinLetters;
	pwdMinUpper_ = pwdMinUpper;
	pwdMinLower_ = pwdMinLower;
	pwdMinDigits_ = pwdMinDigits;
	pwdMinSymbols_ = pwdMinSymbols;
	hintMaxLen_ = hintMaxLen;
	hintMinLen_ = hintMinLen;
	hintNotEqual_ = hintNotEqual;
	hintMinSimilarity_ = hintMinSimilarity;
	newOldNotEqual_ = newOldNotEqual;
	newOldMinSimilarity_ = newOldMinSimilarity;
}

QString PasswordRequirements::ToString() const {
	QString requirements;
	
	if (HasRequirements()) {
		if (!unicode_) {
			requirements += tr("- ASCII only\n");
		}
		
		if (pwdMaxLen_ > 0) {
			requirements += tr("- maximum length: %1\n").arg(pwdMaxLen_);
		}
		if (pwdMinLen_ > 0) {
			requirements += tr("- minimum length: %1\n").arg(pwdMinLen_);
		}
		if (pwdMinLetters_ > 0) {
			requirements += tr("- minimum number of letters: %1\n").arg(pwdMinLetters_);
		}
		if (pwdMinUpper_ > 0) {
			requirements += tr("- minimum number of upper case letters: %1\n").arg(pwdMinUpper_);
		}
		if (pwdMinLower_ > 0) {
			requirements += tr("- minimum number of lower case letters: %1\n").arg(pwdMinLower_);
		}
		if (pwdMinDigits_ > 0) {
			requirements += tr("- minimum number of digits: %1\n").arg(pwdMinDigits_);
		}
		if (pwdMinSymbols_ > 0) {
			requirements += tr("- minimum number of symbols: %1\n").arg(pwdMinSymbols_);
		}
		
		if (hintMaxLen_ > 0) {
			requirements += tr("- hint maximum length: %1\n").arg(hintMaxLen_);
		}
		if (hintMinLen_ > 0) {
			requirements += tr("- hint minimum length: %1\n").arg(hintMinLen_);
		}
		if (hintNotEqual_) {
			requirements += tr("- hint can not be equal with password\n");
		}
		if (hintMinSimilarity_ > 0) {
			requirements += tr("- hint minimum similarity: %1\n").arg(hintMinSimilarity_);
		}
		
		if (newOldNotEqual_) {
			requirements += tr("- new password can not be equal with old password\n");
		}
		if (newOldMinSimilarity_ > 0) {
			requirements += tr("- new password minimum similarity with old password: %1\n").arg(newOldMinSimilarity_);
		}
	}
	else {
		requirements = tr("There are no requirements.");
	}
	
	return requirements;
}

bool PasswordRequirements::Validate(bool unicode, int pwdMaxLen, int pwdMinLen, int pwdMinLetters, int pwdMinUpper,
		int pwdMinLower, int pwdMinDigits, int pwdMinSymbols, int hintMaxLen, int hintMinLen, bool hintNotEqual,
		int hintMinSimilarity, bool newOldNotEqual, int newOldMinSimilarity) {
	using ::std::max;
	
	
	base::SuppressUnusedWarning(unicode);
	base::SuppressUnusedWarning(hintNotEqual);
	base::SuppressUnusedWarning(newOldNotEqual);
	
	base::SuppressUnusedWarning(hintMinSimilarity);
	base::SuppressUnusedWarning(newOldMinSimilarity);
	
	if (pwdMaxLen > 0) {
		if (pwdMaxLen > kQLineEditMaxLength_) {
			CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMaxLen > kQLineEditMaxLength_");
			return false;
		}
	}
	else {
		pwdMaxLen = kQLineEditMaxLength_;
	}
	
	CPPDEVTK_ASSERT((pwdMaxLen > 0) && (pwdMaxLen <= kQLineEditMaxLength_));
	
	if (pwdMinLen > 0) {
		if (pwdMinLen > pwdMaxLen) {
			CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinLen > pwdMaxLen");
			return false;
		}
	}
	
	if (pwdMinLetters > 0) {
		if (pwdMinLen > 0) {
			if (pwdMinLetters > pwdMinLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinLetters > pwdMinLen");
				return false;
			}
		}
		else {
			if (pwdMinLetters > pwdMaxLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinLetters > pwdMaxLen");
				return false;
			}
		}
	}
	
	if (pwdMinUpper > 0) {
		if (pwdMinLetters > 0) {
			if (pwdMinUpper > pwdMinLetters) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinUpper > pwdMinLetters");
				return false;
			}
		}
		else {
			if (pwdMinLen > 0) {
				if (pwdMinUpper > pwdMinLen) {
					CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinUpper > pwdMinLen");
					return false;
				}
			}
			else {
				if (pwdMinUpper > pwdMaxLen) {
					CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinUpper > pwdMaxLen");
					return false;
				}
			}
		}
	}
	
	if (pwdMinLower > 0) {
		if (pwdMinLetters > 0) {
			if (pwdMinLower > pwdMinLetters) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinLower > pwdMinLetters");
				return false;
			}
		}
		else {
			if (pwdMinLen > 0) {
				if (pwdMinLower > pwdMinLen) {
					CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinLower > pwdMinLen");
					return false;
				}
			}
			else {
				if (pwdMinLower > pwdMaxLen) {
					CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinLower > pwdMaxLen");
					return false;
				}
			}
		}
	}
	
	if (pwdMinDigits > 0) {
		if (pwdMinLen > 0) {
			if (pwdMinDigits > pwdMinLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinDigits > pwdMinLen");
				return false;
			}
		}
		else {
			if (pwdMinDigits > pwdMaxLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinDigits > pwdMaxLen");
				return false;
			}
		}
	}
	
	if (pwdMinSymbols > 0) {
		if (pwdMinLen > 0) {
			if (pwdMinSymbols > pwdMinLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinSymbols > pwdMinLen");
				return false;
			}
		}
		else {
			if (pwdMinSymbols > pwdMaxLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("pwdMinSymbols > pwdMaxLen");
				return false;
			}
		}
	}
	
	
	if (hintMaxLen > 0) {
		if (hintMaxLen > kQLineEditMaxLength_) {
			CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("hintMaxLen > kQLineEditMaxLength_");
			return false;
		}
	}
	else {
		hintMaxLen = kQLineEditMaxLength_;
	}
	
	CPPDEVTK_ASSERT((hintMaxLen > 0) && (hintMaxLen <= kQLineEditMaxLength_));
	
	if (hintMinLen > 0) {
		if (hintMinLen > hintMaxLen) {
			CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("hintMinLen > hintMaxLen");
			return false;
		}
	}
	
	/*
	
	if (hintMinSimilarity > 0) {
		if (hintMinSimilarity > max(hintMaxLen, pwdMaxLen)) {
			CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("hintMinSimilarity > max(hintMaxLen, pwdMaxLen)");
			return false;
		}
	}
	
	if (newOldMinSimilarity > 0) {
		if (newOldMinSimilarity > pwdMaxLen) {
			CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("newOldMinSimilarity > pwdMaxLen");
			return false;
		}
	}
	
	*/
	
	const int kActualMinLetters = pwdMinUpper + pwdMinLower;
	if (kActualMinLetters > 0) {
		if (pwdMinLetters > 0) {
			if (kActualMinLetters > pwdMinLetters) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("kActualMinLetters > pwdMinLetters");
				return false;
			}
		}
		else {
			if (pwdMinLen > 0) {
				if (kActualMinLetters > pwdMinLen) {
					CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("kActualMinLetters > pwdMinLen");
					return false;
				}
			}
			else {
				if (kActualMinLetters > pwdMaxLen) {
					CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("kActualMinLetters > pwdMaxLen");
					return false;
				}
			}
		}
	}
	
	int actualMinLen = pwdMinDigits + pwdMinSymbols;
	if (pwdMinLetters > 0) {
		actualMinLen += pwdMinLetters;
	}
	else {
		actualMinLen += (pwdMinUpper + pwdMinLower);
	}
	if (actualMinLen > 0) {
		if (pwdMinLen > 0) {
			if (actualMinLen > pwdMinLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("actualMinLen > pwdMinLen");
				return false;
			}
		}
		else {
			if (actualMinLen > pwdMaxLen) {
				CPPDEVTK_PASSWORD_REQUIREMENTS_LOG_VALIDATION_ERROR("actualMinLen > pwdMaxLen");
				return false;
			}
		}
	}
	
	return true;
}


}	// namespace util
}	// namespace cppdevtk
