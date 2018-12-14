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


#ifndef CPPDEVTK_BASE_CLONEABLE_HPP_INCLUDED_
#define CPPDEVTK_BASE_CLONEABLE_HPP_INCLUDED_


#include "config.hpp"

#include CPPDEVTK_TR1_HEADER(memory)


namespace cppdevtk {
namespace base {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract base class for cloneable classes.
class CPPDEVTK_BASE_API Cloneable {
public:
	Cloneable();
	virtual ~Cloneable();
	
	/// AKA virtual copy constructor.
	/// \attention If DoClone() is not or incorrectly overriden calls std::abort()
	/// (or throws std::runtime_error if CPPDEVTK_CLONEABLE_DISABLE_TERMINATE=1)
	/// \attention Do not forget to overload in each derived class to simulate covariant return types.
	/// Example:
	/// \code
	/// class Derived: public Cloneable {
	///	//...
	/// public:
	/// 	::std::auto_ptr<Derived> Clone() const {
	/// 			return ::std::auto_ptr<Derived>(static_cast<Derived*>(::cppdevtk::base::Cloneable::Clone().release()));
	/// 	}
	/// //...
	/// };
	/// \endcode
	::std::auto_ptr<Cloneable> Clone() const;
protected:
	Cloneable(const Cloneable&);
	Cloneable& operator=(const Cloneable&);
	
	/// \attention Do not forget to override using covariant return types in each derived class.
	/// Example:
	/// \code
	/// class Derived: public Cloneable {
	///	//...
	/// protected:
	/// 	virtual Derived* DoClone() const { return new Derived(*this); }
	/// //...
	/// };
	/// \endcode
	/// \attention MSVC has a nasty bug when corariant return types are used with multiple virtual inheritance.
	/// Use CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG to check for bug presence.
	/// Please see: https://connect.microsoft.com/VisualStudio/feedback/details/590625/visual-c-incorrectly-reports-ambiguity-when-covariance-is-used-with-virtual-inheritance
	virtual Cloneable* DoClone() const = 0;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions.

inline Cloneable::Cloneable() {}

inline Cloneable::~Cloneable() {}

inline Cloneable::Cloneable(const Cloneable&) {}

inline Cloneable& Cloneable::operator=(const Cloneable&) {
	return *this;
}

/*
inline Cloneable* Cloneable::DoClone() const {
	return new Cloneable(*this);
}
*/


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_CLONEABLE_HPP_INCLUDED_
