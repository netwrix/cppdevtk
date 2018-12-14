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


#ifndef CPPDEVTK_UTIL_DYNAMIC_LOADER_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_UTIL_DYNAMIC_LOADER_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/unused.hpp>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


#define CPPDEVTK_DYNAMIC_LOADER_EXCEPTION(whatArg)	\
	::cppdevtk::util::DynamicLoaderException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


#define CPPDEVTK_MAKE_DYNAMIC_LOADER_EXCEPTION(excName, whatArg)	\
	::cppdevtk::util::DynamicLoaderException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


class CPPDEVTK_UTIL_API DynamicLoaderException: public ::cppdevtk::base::RuntimeException {
public:
	DynamicLoaderException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~DynamicLoaderException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<DynamicLoaderException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual QtException* clone() const;
#	else
	virtual DynamicLoaderException* clone() const;
#	endif
		
	void Swap(DynamicLoaderException& other) CPPDEVTK_NOEXCEPT;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual DynamicLoaderException* DoClone() const;
#	endif
		
	void SwapOwnData(DynamicLoaderException& other) CPPDEVTK_NOEXCEPT;
private:
	static QString GetOsDynLdErrMsg();
};


CPPDEVTK_UTIL_API void swap(DynamicLoaderException& x, DynamicLoaderException& y) CPPDEVTK_NOEXCEPT;


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline DynamicLoaderException::DynamicLoaderException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const QString& whatArg): Exception(throwPoint),
		RuntimeException(throwPoint, (whatArg + "; dynamic loader error message: " + GetOsDynLdErrMsg())) {}

inline DynamicLoaderException::~DynamicLoaderException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<DynamicLoaderException> DynamicLoaderException::Clone() const {
	return ::std::auto_ptr<DynamicLoaderException>(dynamic_cast<DynamicLoaderException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline QtException* DynamicLoaderException::clone() const {
#else
inline DynamicLoaderException* DynamicLoaderException::clone() const {
#endif
	return Clone().release();
}

inline void DynamicLoaderException::Swap(DynamicLoaderException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void DynamicLoaderException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* DynamicLoaderException::DoClone() const {
#else
inline DynamicLoaderException* DynamicLoaderException::DoClone() const {
#endif
	return new DynamicLoaderException(*this);
}

inline void DynamicLoaderException::SwapOwnData(DynamicLoaderException& other) CPPDEVTK_NOEXCEPT {
	::cppdevtk::base::SuppressUnusedWarning(other);
}


inline CPPDEVTK_UTIL_API void swap(DynamicLoaderException& x, DynamicLoaderException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_DYNAMIC_LOADER_EXCEPTION_HPP_INCLUDED_
