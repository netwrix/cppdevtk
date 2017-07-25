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


#ifndef CPPDEVTK_JNI_EXCEPTIONS_HPP_INCLUDED_
#define CPPDEVTK_JNI_EXCEPTIONS_HPP_INCLUDED_


#include "config.hpp"

#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/unused.hpp>


namespace cppdevtk {
namespace jni {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JniException

#define CPPDEVTK_JNI_EXCEPTION(whatArg)	\
	::cppdevtk::jni::JniException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_JNI_EXCEPTION_W_CAUSE(whatArg, cause)	\
	::cppdevtk::jni::JniException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))

#define CPPDEVTK_MAKE_JNI_EXCEPTION(excName, whatArg)	\
	::cppdevtk::jni::JniException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_JNI_EXCEPTION_W_CAUSE(excName, whatArg, cause)	\
	::cppdevtk::jni::JniException excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (cause))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Thrown to indicate JNI error
class CPPDEVTK_JNI_API JniException: public virtual ::cppdevtk::base::RuntimeException {
public:
	JniException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg);
	JniException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg,
			const ::cppdevtk::base::Exception& cause);
	
	virtual ~JniException() throw();
	
	::std::auto_ptr<JniException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual JniException* clone() const;
#	endif
	
	void Swap(JniException& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual JniException* DoClone() const;
#	endif
	
	void SwapOwnData(JniException& other);
};


CPPDEVTK_JNI_API void swap(JniException& x, JniException& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OutOfMemoryError

#define CPPDEVTK_OUT_OF_MEMORY_ERROR(whatArg)	\
	::cppdevtk::jni::OutOfMemoryError(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_OUT_OF_MEMORY_ERROR(excName, whatArg)	\
	::cppdevtk::jni::OutOfMemoryError excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


class CPPDEVTK_JNI_API OutOfMemoryError: public JniException {
public:
	OutOfMemoryError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~OutOfMemoryError() throw();
	
	::std::auto_ptr<OutOfMemoryError> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual OutOfMemoryError* clone() const;
#	endif
	
	void Swap(OutOfMemoryError& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual OutOfMemoryError* DoClone() const;
#	endif
	
	void SwapOwnData(OutOfMemoryError& other);
};


CPPDEVTK_JNI_API void swap(OutOfMemoryError& x, OutOfMemoryError& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NoClassDefFoundError

#define CPPDEVTK_NO_CLASS_DEF_FOUND_ERROR(whatArg)	\
	::cppdevtk::jni::NoClassDefFoundError(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_NO_CLASS_DEF_FOUND_ERROR(excName, whatArg)	\
	::cppdevtk::jni::NoClassDefFoundError excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


class CPPDEVTK_JNI_API NoClassDefFoundError: public JniException {
public:
	NoClassDefFoundError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~NoClassDefFoundError() throw();
	
	::std::auto_ptr<NoClassDefFoundError> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual NoClassDefFoundError* clone() const;
#	endif
	
	void Swap(NoClassDefFoundError& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual NoClassDefFoundError* DoClone() const;
#	endif
	
	void SwapOwnData(NoClassDefFoundError& other);
};


CPPDEVTK_JNI_API void swap(NoClassDefFoundError& x, NoClassDefFoundError& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NoSuchMethodError

#define CPPDEVTK_NO_SUCH_METHOD_ERROR(whatArg)	\
	::cppdevtk::jni::NoSuchMethodError(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_NO_SUCH_METHOD_ERROR(excName, whatArg)	\
	::cppdevtk::jni::NoSuchMethodError excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


class CPPDEVTK_JNI_API NoSuchMethodError: public JniException {
public:
	NoSuchMethodError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~NoSuchMethodError() throw();
	
	::std::auto_ptr<NoSuchMethodError> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual NoSuchMethodError* clone() const;
#	endif
	
	void Swap(NoSuchMethodError& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual NoSuchMethodError* DoClone() const;
#	endif
	
	void SwapOwnData(NoSuchMethodError& other);
};


CPPDEVTK_JNI_API void swap(NoSuchMethodError& x, NoSuchMethodError& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NoSuchFieldError

#define CPPDEVTK_NO_SUCH_FIELD_ERROR(whatArg)	\
	::cppdevtk::jni::NoSuchFieldError(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))

#define CPPDEVTK_MAKE_NO_SUCH_FIELD_ERROR(excName, whatArg)	\
	::cppdevtk::jni::NoSuchFieldError excName(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg))


class CPPDEVTK_JNI_API NoSuchFieldError: public JniException {
public:
	NoSuchFieldError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg);
	
	virtual ~NoSuchFieldError() throw();
	
	::std::auto_ptr<NoSuchFieldError> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual NoSuchFieldError* clone() const;
#	endif
	
	void Swap(NoSuchFieldError& other);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual NoSuchFieldError* DoClone() const;
#	endif
	
	void SwapOwnData(NoSuchFieldError& other);
};


CPPDEVTK_JNI_API void swap(NoSuchFieldError& x, NoSuchFieldError& y);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline JniException::JniException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg) {}

inline JniException::JniException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg,
		const ::cppdevtk::base::Exception& cause): Exception(throwPoint, cause),
		RuntimeException(throwPoint, whatArg, cause) {}

inline JniException::~JniException() throw() {}

inline ::std::auto_ptr<JniException> JniException::Clone() const {
	return ::std::auto_ptr<JniException>(dynamic_cast<JniException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* JniException::clone() const {
#else
inline JniException* JniException::clone() const {
#endif
	return Clone().release();
}

inline void JniException::Swap(JniException& other) {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline void JniException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* JniException::DoClone() const {
#else
inline JniException* JniException::DoClone() const {
#endif
	return new JniException(*this);
}

inline void JniException::SwapOwnData(JniException& other) {
	::cppdevtk::base::SuppressUnusedWarning(other);
}


inline CPPDEVTK_JNI_API void swap(JniException& x, JniException& y) {
	x.Swap(y);
}




inline OutOfMemoryError::OutOfMemoryError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg), JniException(throwPoint, whatArg) {}

inline OutOfMemoryError::~OutOfMemoryError() throw() {}

inline ::std::auto_ptr<OutOfMemoryError> OutOfMemoryError::Clone() const {
	return ::std::auto_ptr<OutOfMemoryError>(dynamic_cast<OutOfMemoryError*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* OutOfMemoryError::clone() const {
#else
inline OutOfMemoryError* OutOfMemoryError::clone() const {
#endif
	return Clone().release();
}

inline void OutOfMemoryError::Swap(OutOfMemoryError& other) {
	if (this != &other) {
		JniException::Swap(other);
		SwapOwnData(other);
	}
}

inline void OutOfMemoryError::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* OutOfMemoryError::DoClone() const {
#else
inline OutOfMemoryError* OutOfMemoryError::DoClone() const {
#endif
	return new OutOfMemoryError(*this);
}

inline void OutOfMemoryError::SwapOwnData(OutOfMemoryError& other) {
	::cppdevtk::base::SuppressUnusedWarning(other);
}


inline CPPDEVTK_JNI_API void swap(OutOfMemoryError& x, OutOfMemoryError& y) {
	x.Swap(y);
}




inline NoClassDefFoundError::NoClassDefFoundError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg), JniException(throwPoint, whatArg) {}

inline NoClassDefFoundError::~NoClassDefFoundError() throw() {}

inline ::std::auto_ptr<NoClassDefFoundError> NoClassDefFoundError::Clone() const {
	return ::std::auto_ptr<NoClassDefFoundError>(dynamic_cast<NoClassDefFoundError*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* NoClassDefFoundError::clone() const {
#else
inline NoClassDefFoundError* NoClassDefFoundError::clone() const {
#endif
	return Clone().release();
}

inline void NoClassDefFoundError::Swap(NoClassDefFoundError& other) {
	if (this != &other) {
		JniException::Swap(other);
		SwapOwnData(other);
	}
}

inline void NoClassDefFoundError::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* NoClassDefFoundError::DoClone() const {
#else
inline NoClassDefFoundError* NoClassDefFoundError::DoClone() const {
#endif
	return new NoClassDefFoundError(*this);
}

inline void NoClassDefFoundError::SwapOwnData(NoClassDefFoundError& other) {
	::cppdevtk::base::SuppressUnusedWarning(other);
}


inline CPPDEVTK_JNI_API void swap(NoClassDefFoundError& x, NoClassDefFoundError& y) {
	x.Swap(y);
}




inline NoSuchMethodError::NoSuchMethodError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg), JniException(throwPoint, whatArg) {}

inline NoSuchMethodError::~NoSuchMethodError() throw() {}

inline ::std::auto_ptr<NoSuchMethodError> NoSuchMethodError::Clone() const {
	return ::std::auto_ptr<NoSuchMethodError>(dynamic_cast<NoSuchMethodError*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* NoSuchMethodError::clone() const {
#else
inline NoSuchMethodError* NoSuchMethodError::clone() const {
#endif
	return Clone().release();
}

inline void NoSuchMethodError::Swap(NoSuchMethodError& other) {
	if (this != &other) {
		JniException::Swap(other);
		SwapOwnData(other);
	}
}

inline void NoSuchMethodError::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* NoSuchMethodError::DoClone() const {
#else
inline NoSuchMethodError* NoSuchMethodError::DoClone() const {
#endif
	return new NoSuchMethodError(*this);
}

inline void NoSuchMethodError::SwapOwnData(NoSuchMethodError& other) {
	::cppdevtk::base::SuppressUnusedWarning(other);
}


inline CPPDEVTK_JNI_API void swap(NoSuchMethodError& x, NoSuchMethodError& y) {
	x.Swap(y);
}




inline NoSuchFieldError::NoSuchFieldError(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg):
		Exception(throwPoint), RuntimeException(throwPoint, whatArg), JniException(throwPoint, whatArg) {}

inline NoSuchFieldError::~NoSuchFieldError() throw() {}

inline ::std::auto_ptr<NoSuchFieldError> NoSuchFieldError::Clone() const {
	return ::std::auto_ptr<NoSuchFieldError>(dynamic_cast<NoSuchFieldError*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* NoSuchFieldError::clone() const {
#else
inline NoSuchFieldError* NoSuchFieldError::clone() const {
#endif
	return Clone().release();
}

inline void NoSuchFieldError::Swap(NoSuchFieldError& other) {
	if (this != &other) {
		JniException::Swap(other);
		SwapOwnData(other);
	}
}

inline void NoSuchFieldError::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* NoSuchFieldError::DoClone() const {
#else
inline NoSuchFieldError* NoSuchFieldError::DoClone() const {
#endif
	return new NoSuchFieldError(*this);
}

inline void NoSuchFieldError::SwapOwnData(NoSuchFieldError& other) {
	::cppdevtk::base::SuppressUnusedWarning(other);
}


inline CPPDEVTK_JNI_API void swap(NoSuchFieldError& x, NoSuchFieldError& y) {
	x.Swap(y);
}


}	// namespace jni
}	// namespace cppdevtk


#endif	// CPPDEVTK_JNI_EXCEPTIONS_HPP_INCLUDED_
