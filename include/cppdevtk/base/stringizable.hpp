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


#ifndef CPPDEVTK_BASE_STRINGIZABLE_HPP_INCLUDED_
#define CPPDEVTK_BASE_STRINGIZABLE_HPP_INCLUDED_


#include "config.hpp"
#include "static_assert.hpp"
#include "tstring.hpp"
#include "tostream.hpp"

#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include CPPDEVTK_TR1_HEADER(type_traits)


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract base class for stringizable classes; primary template.
/// \tparam TChar Must be char or wchar_t.
template <typename TChar>
class BasicStringizable {
private:
	CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_TR1_NS::is_same<char, TChar>::value || CPPDEVTK_TR1_NS::is_same<wchar_t, TChar>::value),
			"invalid type");
public:
	typedef ::std::basic_string<TChar> StringType;
	
	
	virtual StringType ToString() const = 0;
protected:
	BasicStringizable();
	BasicStringizable(const BasicStringizable&);
	/* virtual */ ~BasicStringizable();	///< \remark Not virtual.
	BasicStringizable& operator=(const BasicStringizable&);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Full (explicit) specialization for \c QChar.
template <>
class BasicStringizable<QChar> {
public:
	typedef QString StringType;
	
	
	virtual StringType ToString() const = 0;
protected:
	BasicStringizable();
	BasicStringizable(const BasicStringizable&);
	/* virtual */ ~BasicStringizable();	///< \remark Not virtual.
	BasicStringizable& operator=(const BasicStringizable&);
};


#if (CPPDEVTK_ENABLE_TMPL_EXPL_INST)
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicStringizable<char>;
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicStringizable<wchar_t>;
#if (!CPPDEVTK_COMPILER_MSVC || (_MSC_VER > 1500))	// msvc 2008: C2950: 'type' : cannot explicitly instantiate an explicit specialization
CPPDEVTK_BASE_TMPL_EXPL_INST template class CPPDEVTK_BASE_API BasicStringizable<QChar>;
#endif
#endif


typedef BasicStringizable<char> Stringizable;
typedef BasicStringizable<wchar_t> WStringizable;
typedef BasicStringizable<QChar> QStringizable;


template <typename TChar>
::std::basic_ostream<TChar>& operator<<(::std::basic_ostream<TChar>& os, const BasicStringizable<TChar>& stringizable);

CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, const QStringizable& stringizable);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions / Inline functions

template <typename TChar>
inline BasicStringizable<TChar>::BasicStringizable() {}

template <typename TChar>
inline BasicStringizable<TChar>::BasicStringizable(const BasicStringizable&) {}

template <typename TChar>
inline BasicStringizable<TChar>::~BasicStringizable() {}

template <typename TChar>
inline BasicStringizable<TChar>& BasicStringizable<TChar>::operator=(const BasicStringizable&) {
	return *this;
}


inline BasicStringizable<QChar>::BasicStringizable() {}

inline BasicStringizable<QChar>::BasicStringizable(const BasicStringizable&) {}

inline BasicStringizable<QChar>::~BasicStringizable() {}

inline BasicStringizable<QChar>& BasicStringizable<QChar>::operator=(const BasicStringizable&) {
	return *this;
}


template <typename TChar>
inline ::std::basic_ostream<TChar>& operator<<(::std::basic_ostream<TChar>& os, const BasicStringizable<TChar>& stringizable) {
	return os << stringizable.ToString();
}

inline CPPDEVTK_BASE_API QTextStream& operator<<(QTextStream& os, const QStringizable& stringizable) {
	return os << stringizable.ToString();
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_STRINGIZABLE_HPP_INCLUDED_
