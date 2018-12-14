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


#ifndef CPPDEVTK_BASE_THROWABLE_HPP_INCLUDED_
#define CPPDEVTK_BASE_THROWABLE_HPP_INCLUDED_


#include "config.hpp"

#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(push)
#	pragma warning(disable: 4265)	// C4265: 'class' : class has virtual functions, but destructor is not virtual
#endif


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract base class for throwable classes.
class CPPDEVTK_BASE_API Throwable {
public:
	/// \attention If DoThrow() does not throw an exception, or if thrown exception is not of expected type
	/// calls \c ::std::terminate() (or throws \c ::std::runtime_error if CPPDEVTK_THROWABLE_DISABLE_TERMINATE=1)
	void Throw() const;
protected:
	Throwable();
	Throwable(const Throwable&);
	/* virtual */ ~Throwable();	///< \remark Not virtual.
	Throwable& operator=(const Throwable&);
	
	/// \attention Do not forget to override in each derived class.
	/// Example:
	/// \code
	/// class Derived: public Throwable {
	///	//...
	/// protected:
	/// 	virtual void DoThrow() const { throw *this; }
	/// //...
	/// };
	/// \endcode
	virtual void DoThrow() const = 0;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline Throwable::Throwable() {}

inline Throwable::Throwable(const Throwable&) {}

inline Throwable::~Throwable() {}

inline Throwable& Throwable::operator=(const Throwable&) {
	return *this;
}


}	// namespace base
}	// namespace cppdevtk


#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS && CPPDEVTK_COMPILER_MSVC)
#	pragma warning(pop)
#endif


#endif	// CPPDEVTK_BASE_THROWABLE_HPP_INCLUDED_
