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


#include <cppdevtk/base/string_conv.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/invalid_string_conversion_exception.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/QTextCodec>

#include <cstddef>


using ::std::string;


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API ::std::wstring A2W(const char* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
	return A2Q(str).toStdWString();
}

CPPDEVTK_BASE_API QString A2Q(const ::std::string& str) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return QString::fromLatin1(str.c_str());
#	else
	return QString::fromStdString(str);
#	endif
}

CPPDEVTK_BASE_API QString A2Q(const char* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return QString::fromLatin1(str);
#	else
	return QString(str);
#	endif
}

CPPDEVTK_BASE_API ::std::string A2Utf8(const char* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
	return string(str);
}

CPPDEVTK_BASE_API ::std::string W2A(const wchar_t* str, bool strict) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
	if (strict) {
		QTextCodec* pTextCodec = QTextCodec::codecForName("latin1");
		if (pTextCodec == NULL) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("latin1 text codec not found!");
		}
		const QString kUnicode = QString::fromWCharArray(str);
		QTextCodec::ConverterState converterState;
		const QByteArray kLatin1 = pTextCodec->fromUnicode(kUnicode.unicode(), kUnicode.length(), &converterState);
		if (converterState.invalidChars != 0) {
			throw CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION("unicode", "latin1");
		}
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return kLatin1.toStdString();
#		else
		return string(kLatin1.constData());
#		endif
	}
	else {
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return W2Q(str).toLatin1().toStdString();
#		else
		return string(W2Q(str).toLatin1().constData());
#		endif
	}
}

CPPDEVTK_BASE_API ::std::string W2A(const ::std::wstring& str, bool strict) {
	if (strict) {
		QTextCodec* pTextCodec = QTextCodec::codecForName("latin1");
		if (pTextCodec == NULL) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("latin1 text codec not found!");
		}
		const QString kUnicode = QString::fromStdWString(str);
		QTextCodec::ConverterState converterState;
		const QByteArray kLatin1 = pTextCodec->fromUnicode(kUnicode.unicode(), kUnicode.length(), &converterState);
		if (converterState.invalidChars != 0) {
			throw CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION("unicode", "latin1");
		}
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return kLatin1.toStdString();
#		else
		return string(kLatin1.constData());
#		endif
	}
	else {
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return W2Q(str).toLatin1().toStdString();
#		else
		return string(W2Q(str).toLatin1().constData());
#		endif
	}
}

CPPDEVTK_BASE_API QString W2Q(const wchar_t* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
	return QString::fromWCharArray(str);
}

CPPDEVTK_BASE_API ::std::string W2Utf8(const wchar_t* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return W2Q(str).toStdString();
#	else
	return string(W2Q(str).toUtf8().constData());
#	endif
}

CPPDEVTK_BASE_API ::std::string W2Utf8(const ::std::wstring& str) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return W2Q(str).toStdString();
#	else
	return string(W2Q(str).toUtf8().constData());
#	endif
}

CPPDEVTK_BASE_API ::std::string Q2A(const QString& str, bool strict) {
	if (strict) {
		QTextCodec* pTextCodec = QTextCodec::codecForName("latin1");
		if (pTextCodec == NULL) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("latin1 text codec not found!");
		}
		QTextCodec::ConverterState converterState;
		const QByteArray kLatin1 = pTextCodec->fromUnicode(str.unicode(), str.length(), &converterState);
		if (converterState.invalidChars != 0) {
			throw CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION("unicode", "latin1");
		}
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return kLatin1.toStdString();
#		else
		return string(kLatin1.constData());
#		endif
	}
	else {
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return str.toLatin1().toStdString();
#		else
		return string(str.toLatin1().constData());
#		endif
	}
}

CPPDEVTK_BASE_API ::std::string Q2Utf8(const QString& str) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return str.toStdString();
#	else
	return string(str.toUtf8().constData());
#	endif
}

CPPDEVTK_BASE_API ::std::string Utf82A(const char* str, bool strict) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
	if (strict) {
		QTextCodec* pTextCodec = QTextCodec::codecForName("latin1");
		if (pTextCodec == NULL) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("latin1 text codec not found!");
		}
		const QString kUnicode =
#				if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
				QString(str)
#				else
				QString::fromUtf8(str);
#				endif
		;
		QTextCodec::ConverterState converterState;
		const QByteArray kLatin1 = pTextCodec->fromUnicode(kUnicode.unicode(), kUnicode.length(), &converterState);
		if (converterState.invalidChars != 0) {
			throw CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION("unicode", "latin1");
		}
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return kLatin1.toStdString();
#		else
		return string(kLatin1.constData());
#		endif
	}
	else {
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return Utf82Q(str).toLatin1().toStdString();
#		else
		return string(Utf82Q(str).toLatin1().constData());
#		endif
	}
}

CPPDEVTK_BASE_API ::std::string Utf82A(const ::std::string& str, bool strict) {
	if (strict) {
		QTextCodec* pTextCodec = QTextCodec::codecForName("latin1");
		if (pTextCodec == NULL) {
			throw CPPDEVTK_RUNTIME_EXCEPTION("latin1 text codec not found!");
		}
		const QString kUnicode =
#				if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
				QString::fromStdString(str)
#				else
				QString::fromUtf8(str.c_str());
#				endif
		;
		QTextCodec::ConverterState converterState;
		const QByteArray kLatin1 = pTextCodec->fromUnicode(kUnicode.unicode(), kUnicode.length(), &converterState);
		if (converterState.invalidChars != 0) {
			throw CPPDEVTK_INVALID_STRING_CONVERSION_EXCEPTION("unicode", "latin1");
		}
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return kLatin1.toStdString();
#		else
		return string(kLatin1.constData());
#		endif
	}
	else {
#		if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		return Utf82Q(str).toLatin1().toStdString();
#		else
		return string(Utf82Q(str.c_str()).toLatin1().constData());
#		endif
	}
}

CPPDEVTK_BASE_API ::std::wstring Utf82W(const char* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
	return Utf82Q(str).toStdWString();
}

CPPDEVTK_BASE_API QString Utf82Q(const char* str) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(str);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return QString(str);
#	else
	return QString::fromUtf8(str);
#	endif
}

CPPDEVTK_BASE_API QString Utf82Q(const ::std::string& str) {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	return QString::fromStdString(str);
#	else
	return QString::fromUtf8(str.c_str());
#	endif
}


}	// namespace base
}	// namespace cppdevtk
