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


#ifndef CPPDEVTK_BASE_STRING_UTILS_HPP_INCLUDED_
#define CPPDEVTK_BASE_STRING_UTILS_HPP_INCLUDED_


#include "config.hpp"
#include "static_assert.hpp"

#include <QtCore/QString>
#include <QtCore/QChar>
#include <QtCore/QTextStream>

#include CPPDEVTK_TR1_HEADER(type_traits)


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API bool IsAscii(const QChar& chr);
CPPDEVTK_BASE_API bool IsAscii(const QString& str);

CPPDEVTK_BASE_API bool IsLatin1(const QChar& chr);
CPPDEVTK_BASE_API bool IsLatin1(const QString& str);


CPPDEVTK_BASE_API int CountNonAscii(const QString& str);
CPPDEVTK_BASE_API int CountNonLatin1(const QString& str);
CPPDEVTK_BASE_API int CountLetters(const QString& str);
CPPDEVTK_BASE_API int CountUpper(const QString& str);
CPPDEVTK_BASE_API int CountLower(const QString& str);
CPPDEVTK_BASE_API int CountDigits(const QString& str);
CPPDEVTK_BASE_API int CountSymbols(const QString& str);


/// \tparam TNum Must be a integral type (excluding \c bool).
template <typename TNum>
QString NumToHexStr(TNum num);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

inline CPPDEVTK_BASE_API bool IsLatin1(const QChar& chr) {
	return chr.toLatin1() != 0;
}

inline CPPDEVTK_BASE_API int CountNonAscii(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (!IsAscii(*kIter)) {
			count++;
		}
	}
	return count;
}

inline CPPDEVTK_BASE_API int CountNonLatin1(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (!IsLatin1(*kIter)) {
			count++;
		}
	}
	return count;
}


inline CPPDEVTK_BASE_API int CountLetters(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (kIter->isLetter()) {
			count++;
		}
	}
	return count;
}

inline CPPDEVTK_BASE_API int CountUpper(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (kIter->isUpper()) {
			count++;
		}
	}
	return count;
}

inline CPPDEVTK_BASE_API int CountLower(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (kIter->isLower()) {
			count++;
		}
	}
	return count;
}

inline CPPDEVTK_BASE_API int CountDigits(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (kIter->isDigit()) {
			count++;
		}
	}
	return count;
}

inline CPPDEVTK_BASE_API int CountSymbols(const QString& str) {
	int count = 0;
	for (QString::ConstIterator kIter = str.constBegin(); kIter < str.constEnd(); ++kIter) {
		if (kIter->isSymbol()) {
			count++;
		}
	}
	return count;
}

template <typename TNum>
inline QString NumToHexStr(TNum num) {
	CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_TR1_NS::is_integral<TNum>::value && !CPPDEVTK_TR1_NS::is_same<TNum, bool>::value),
			"invalid type");
	
	QString hexStr;
	{
		QTextStream hexTextStream(&hexStr, QIODevice::WriteOnly);
		hexTextStream << "0x" << noshowbase << uppercasedigits << hex
				/* << qSetPadChar('0') << qSetFieldWidth(sizeof(TNum) * 2) */ << num;
	}
	return hexStr;
}


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_STRING_UTILS_HPP_INCLUDED_
