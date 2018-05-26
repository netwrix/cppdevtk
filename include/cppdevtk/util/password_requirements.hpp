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


#ifndef CPPDEVTK_UTIL_PASSWORD_REQUIREMENTS_HPP_INCLUDED_
#define CPPDEVTK_UTIL_PASSWORD_REQUIREMENTS_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif

#include <cppdevtk/base/stringizable.hpp>

#include <QtCore/QCoreApplication>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \note
/// - For int members values <= 0 means no requirement.
/// - Length is in characters, not bytes.
/// - Length do not include terminating null.
/// - Hint similarity is the distance (string metric) between hint and password. Damerau-Levenshtein distance is used.
/// \sa
/// - <a href=http://en.wikipedia.org/wiki/String_metric">String metric</a>
/// - <a href=http://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance">Damerau?Levenshtein distance</a>
class CPPDEVTK_UTIL_API PasswordRequirements: public ::cppdevtk::base::QStringizable {
	Q_DECLARE_TR_FUNCTIONS(PasswordRequirements)
public:
	static const int kQLineEditMaxLength_ = 32767;
	
	static const bool kDefUnicode_ = true;
	static const int kPwdDefMaxLen_ = 0;	///< \pre <= kQLineEditMaxLength_
	/// \pre
	/// - ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_))
	/// >= (kPwdDefMinLetters_ + kPwdDefMinDigits_ + kPwdDefMinSymbols_)
	static const int kPwdDefMinLen_ = 8;
	/// \pre
	/// - ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_) && (<= kPwdDefMinLen_))
	/// - >= (kPwdDefMinUpper_ + kPwdDefMinLower_)
	static const int kPwdDefMinLetters_ = 2;
	/// \pre ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_) && (<= kPwdDefMinLen_) && (<= kPwdDefMinLetters_))
	static const int kPwdDefMinUpper_ = 1;
	/// \pre ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_) && (<= kPwdDefMinLen_) && (<= kPwdDefMinLetters_))
	static const int kPwdDefMinLower_ = 1;
	static const int kPwdDefMinDigits_ = 1;	///< \pre ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_) && (<= kPwdDefMinLen_))
	static const int kPwdDefMinSymbols_ = 1;	///< \pre ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_) && (<= kPwdDefMinLen_))
	
	static const int kHintDefMaxLen_ = 0;	///< \pre <= kQLineEditMaxLength_
	static const int kHintDefMinLen_ = 4;	///< \pre ((<= kQLineEditMaxLength_) && (<= kPwdDefMaxLen_))
	static const bool kHintDefNotEqual_ = true;
	static const int kHintDefMinSimilarity_ = 2;
	
	static const bool kNewOldDefNotEqual_ = true;
	static const int kNewOldDefMinSimilarity_ = 4;
	
	
	PasswordRequirements(bool unicode = kDefUnicode_, int pwdMaxLen = kPwdDefMaxLen_, int pwdMinLen = kPwdDefMinLen_,
			int pwdMinLetters = kPwdDefMinLetters_, int pwdMinUpper = kPwdDefMinUpper_, int pwdMinLower = kPwdDefMinLower_,
			int pwdMinDigits = kPwdDefMinDigits_, int pwdMinSymbols = kPwdDefMinSymbols_, int hintMaxLen = kHintDefMaxLen_,
			int hintMinLen = kHintDefMinLen_, bool hintNotEqual = kHintDefNotEqual_, int hintMinSimilarity = kHintDefMinSimilarity_,
			bool newOldNotEqual = kNewOldDefNotEqual_, int newOldMinSimilarity = kNewOldDefMinSimilarity_);
	
	bool GetUnicode() const;
	int GetPwdMaxLen() const;
	int GetPwdMinLen() const;
	int GetPwdMinLetters() const;
	int GetPwdMinUpper() const;
	int GetPwdMinLower() const;
	int GetPwdMinDigits() const;
	int GetPwdMinSymbols() const;
	int GetHintMaxLen() const;
	int GetHintMinLen() const;
	bool GetHintNotEqual() const;
	int GetHintMinSimilarity() const;
	bool GetNewOldNotEqual() const;
	int GetNewOldMinSimilarity() const;
	
	void Set(bool unicode = kDefUnicode_, int pwdMaxLen = kPwdDefMaxLen_, int pwdMinLen = kPwdDefMinLen_,
		int pwdMinLetters = kPwdDefMinLetters_, int pwdMinUpper = kPwdDefMinUpper_, int pwdMinLower = kPwdDefMinLower_,
		int pwdMinDigits = kPwdDefMinDigits_, int pwdMinSymbols = kPwdDefMinSymbols_, int hintMaxLen = kHintDefMaxLen_,
		int hintMinLen = kHintDefMinLen_, bool hintNotEqual = kHintDefNotEqual_, int hintMinSimilarity = kHintDefMinSimilarity_,
		bool newOldNotEqual = kNewOldDefNotEqual_, int newOldMinSimilarity = kNewOldDefMinSimilarity_); ///< \pre Validate() arguments.
	
	void Clear();
	void ClearHint();
	void ClearNew();
	
	bool HasRequirements() const;
	
	virtual QString ToString() const;
	
	
	static bool Validate(bool unicode, int pwdMaxLen, int pwdMinLen, int pwdMinLetters, int pwdMinUpper, int pwdMinLower,
		int pwdMinDigits, int pwdMinSymbols, int hintMaxLen, int hintMinLen, bool hintNotEqual, int hintMinSimilarity,
		bool newOldNotEqual, int newOldMinSimilarity);
private:
	bool unicode_;
	
	int pwdMaxLen_;
	int pwdMinLen_;
	int pwdMinLetters_;
	int pwdMinUpper_;
	int pwdMinLower_;
	int pwdMinDigits_;
	int pwdMinSymbols_;
	
	int hintMaxLen_;
	int hintMinLen_;
	bool hintNotEqual_;
	int hintMinSimilarity_;
	
	bool newOldNotEqual_;
	int newOldMinSimilarity_;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline bool PasswordRequirements::GetUnicode() const {
	return unicode_;
}

inline int PasswordRequirements::GetPwdMaxLen() const {
	return pwdMaxLen_;
}

inline int PasswordRequirements::GetPwdMinLen() const {
	return pwdMinLen_;
}

inline int PasswordRequirements::GetPwdMinLetters() const {
	return pwdMinLetters_;
}

inline int PasswordRequirements::GetPwdMinUpper() const {
	return pwdMinUpper_;
}

inline int PasswordRequirements::GetPwdMinLower() const {
	return pwdMinLower_;
}

inline int PasswordRequirements::GetPwdMinDigits() const {
	return pwdMinDigits_;
}

inline int PasswordRequirements::GetPwdMinSymbols() const {
	return pwdMinSymbols_;
}

inline int PasswordRequirements::GetHintMaxLen() const {
	return hintMaxLen_;
}

inline int PasswordRequirements::GetHintMinLen() const {
	return hintMinLen_;
}

inline bool PasswordRequirements::GetHintNotEqual() const {
	return hintNotEqual_;
}

inline int PasswordRequirements::GetHintMinSimilarity() const {
	return hintMinSimilarity_;
}

inline bool PasswordRequirements::GetNewOldNotEqual() const {
	return newOldNotEqual_;
}

inline int PasswordRequirements::GetNewOldMinSimilarity() const {
	return newOldMinSimilarity_;
}

inline void PasswordRequirements::Clear() {
	Set(true, 0, 0, 0, 0, 0, 0, 0, 0, 0, false, 0, false, 0);
}

inline void PasswordRequirements::ClearHint() {
	hintMaxLen_ = 0;
	hintMinLen_ = 0;
	hintNotEqual_ = false;
	hintMinSimilarity_ = 0;
}

inline void PasswordRequirements::ClearNew() {
	newOldNotEqual_ = false;
	newOldMinSimilarity_ = 0;
}

inline bool PasswordRequirements::HasRequirements() const {
	return (!unicode_ || (pwdMaxLen_ > 0) || (pwdMinLen_ > 0) || (pwdMinLetters_ > 0) || (pwdMinUpper_ > 0)
			|| (pwdMinLower_ > 0) || (pwdMinDigits_ > 0) || (pwdMinSymbols_ > 0) || (hintMaxLen_ > 0)
			|| (hintMinLen_ > 0) || hintNotEqual_ || (hintMinSimilarity_ > 0) || newOldNotEqual_ || (newOldMinSimilarity_ > 0));
}


}	// namespace util
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_UTIL_PASSWORD_REQUIREMENTS_HPP_INCLUDED_
