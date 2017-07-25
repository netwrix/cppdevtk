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


#ifndef CPPDEVTK_UTIL_FILESYSTEM_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_UTIL_FILESYSTEM_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/base/ios.hpp>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \addtogroup exceptions
/// @{


#define CPPDEVTK_FS_EXC_W_EC(errorCode)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode)

#define CPPDEVTK_FS_EXC_W_EC_WA(errorCode, whatArg)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg)

#define CPPDEVTK_FS_EXC_W_EC_WA_SRC(errorCode, whatArg, srcPath)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath)

#define CPPDEVTK_FS_EXC_W_EC_WA_SRC_DST(errorCode, whatArg, srcPath, dstPath)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath, dstPath)


#define CPPDEVTK_FS_EXC_W_EC_CAUSE(errorCode, cause)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, cause)

#define CPPDEVTK_FS_EXC_W_EC_WA_CAUSE(errorCode, whatArg, cause)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, cause)

#define CPPDEVTK_FS_EXC_W_EC_WA_SRC_CAUSE(errorCode, whatArg, srcPath, cause)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath, cause)

#define CPPDEVTK_FS_EXC_W_EC_WA_SRC_DST_CAUSE(errorCode, whatArg, srcPath, dstPath, cause)	\
	::cppdevtk::util::FilesystemException(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath, dstPath, cause)


#define CPPDEVTK_MAKE_FS_EXC_W_EC(excName, errorCode)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode)

#define CPPDEVTK_MAKE_FS_EXC_W_EC_WA(excName, errorCode, whatArg)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg)

#define CPPDEVTK_MAKE_FS_EXC_W_EC_WA_SRC(excName, errorCode, whatArg, srcPath)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath)

#define CPPDEVTK_MAKE_FS_EXC_W_EC_WA_SRC_DST(excName, errorCode, whatArg, srcPath, dstPath)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath, dstPath)


#define CPPDEVTK_MAKE_FS_EXC_W_EC_CAUSE(excName, errorCode, cause)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, cause)

#define CPPDEVTK_MAKE_FS_EXC_W_EC_WA_CAUSE(excName, errorCode, whatArg, cause)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, cause)

#define CPPDEVTK_MAKE_FS_EXC_W_EC_WA_SRC_CAUSE(excName, errorCode, whatArg, srcPath, cause)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath, cause)

#define CPPDEVTK_MAKE_FS_EXC_W_EC_WA_SRC_DST_CAUSE(excName, errorCode, whatArg, srcPath, dstPath, cause)	\
	::cppdevtk::util::FilesystemException excName(CPPDEVTK_SOURCE_CODE_INFO(), errorCode, whatArg, srcPath, dstPath, cause)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for filesystem related exceptions.
class CPPDEVTK_UTIL_API FilesystemException: public virtual ::cppdevtk::base::IosFailureException {
public:
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode);
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const QString& whatArg);
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const QString& whatArg, const QString& srcPath);
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const QString& whatArg, const QString& srcPath, const QString& dstPath);
	
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const ::cppdevtk::base::Exception& cause);
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const QString& whatArg, const ::cppdevtk::base::Exception& cause);
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const QString& whatArg, const QString& srcPath, const ::cppdevtk::base::Exception& cause);
	FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const ::cppdevtk::base::ErrorCode& errorCode,
			const QString& whatArg, const QString& srcPath, const QString& dstPath, const ::cppdevtk::base::Exception& cause);
	
	
	FilesystemException(const FilesystemException& other) throw();
	
	virtual ~FilesystemException() throw();
	
	FilesystemException& operator=(const FilesystemException& other) throw();
	
	::std::auto_ptr<FilesystemException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual CPPDEVTK_QT_EXCEPTION* clone() const;
#	else
	virtual FilesystemException* clone() const;
#	endif
	
	void Swap(FilesystemException& other);
	
	QString GetSrcPath() const;
	void SetSrcPath(const QString& srcPath);
	
	QString GetDstPath() const;
	void SetDstPath(const QString& dstPath);
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual FilesystemException* DoClone() const;
#	endif
	
	virtual QString DoOwnWhat() const;
	
	void SwapOwnData(FilesystemException& other);
private:
	QString srcPath_;
	QString dstPath_;
};


CPPDEVTK_UTIL_API void swap(FilesystemException& x, FilesystemException& y);


/// @}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode): Exception(throwPoint), RuntimeException(throwPoint, ""),
		SystemException(throwPoint, errorCode), IosFailureException(throwPoint, "", errorCode), srcPath_(), dstPath_() {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const QString& whatArg): Exception(throwPoint),
		RuntimeException(throwPoint, whatArg), SystemException(throwPoint, errorCode, whatArg),
		IosFailureException(throwPoint, whatArg, errorCode), srcPath_(), dstPath_() {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const QString& whatArg, const QString& srcPath): Exception(throwPoint),
		RuntimeException(throwPoint, whatArg), SystemException(throwPoint, errorCode, whatArg),
		IosFailureException(throwPoint, whatArg, errorCode), srcPath_(srcPath), dstPath_() {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const QString& whatArg, const QString& srcPath,
		const QString& dstPath): Exception(throwPoint), RuntimeException(throwPoint, whatArg),
		SystemException(throwPoint, errorCode, whatArg), IosFailureException(throwPoint, whatArg, errorCode),
		srcPath_(srcPath), dstPath_(dstPath) {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const ::cppdevtk::base::Exception& cause):
		Exception(throwPoint, cause), RuntimeException(throwPoint, "", cause),
		SystemException(throwPoint, errorCode, cause), IosFailureException(throwPoint, "", errorCode, cause),
		srcPath_(), dstPath_() {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const QString& whatArg, const ::cppdevtk::base::Exception& cause):
		Exception(throwPoint, cause), RuntimeException(throwPoint, whatArg, cause),
		SystemException(throwPoint, errorCode, whatArg, cause), IosFailureException(throwPoint, whatArg, errorCode, cause),
		srcPath_(), dstPath_() {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const QString& whatArg, const QString& srcPath,
		const ::cppdevtk::base::Exception& cause): Exception(throwPoint, cause),
		RuntimeException(throwPoint, whatArg, cause), SystemException(throwPoint, errorCode, whatArg, cause),
		IosFailureException(throwPoint, whatArg, errorCode, cause), srcPath_(srcPath), dstPath_() {}

inline FilesystemException::FilesystemException(const ::cppdevtk::base::SourceCodeInfo& throwPoint,
		const ::cppdevtk::base::ErrorCode& errorCode, const QString& whatArg, const QString& srcPath, const QString& dstPath,
		const ::cppdevtk::base::Exception& cause): Exception(throwPoint, cause),
		RuntimeException(throwPoint, whatArg, cause), SystemException(throwPoint, errorCode, whatArg, cause),
		IosFailureException(throwPoint, whatArg, errorCode, cause), srcPath_(srcPath), dstPath_(dstPath) {}

inline FilesystemException::FilesystemException(const FilesystemException& other) throw(): Exception(other),
		RuntimeException(other), SystemException(other), IosFailureException(other),
		srcPath_(other.srcPath_), dstPath_(other.dstPath_) {}

inline FilesystemException::~FilesystemException() throw() {}

inline FilesystemException& FilesystemException::operator=(const FilesystemException& other) throw() {
	FilesystemException tmp(other);
	Swap(tmp);
	return *this;
}

inline ::std::auto_ptr<FilesystemException> FilesystemException::Clone() const {
	return ::std::auto_ptr<FilesystemException>(dynamic_cast<FilesystemException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline CPPDEVTK_QT_EXCEPTION* FilesystemException::clone() const {
#else
inline FilesystemException* FilesystemException::clone() const {
#endif
	return Clone().release();
}

inline void FilesystemException::Swap(FilesystemException& other) {
	if (this != &other) {
		IosFailureException::Swap(other);
		SwapOwnData(other);
	}
}

inline QString FilesystemException::GetSrcPath() const {
	return srcPath_;
}

inline void FilesystemException::SetSrcPath(const QString& srcPath) {
	srcPath_ = srcPath;
}

inline QString FilesystemException::GetDstPath() const {
	return dstPath_;
}

inline void FilesystemException::SetDstPath(const QString& dstPath) {
	dstPath_ = dstPath;
}

inline void FilesystemException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* FilesystemException::DoClone() const {
#else
inline FilesystemException* FilesystemException::DoClone() const {
#endif
	return new FilesystemException(*this);
}

inline void FilesystemException::SwapOwnData(FilesystemException& other) {
#	if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
	srcPath_.swap(other.srcPath_);
	dstPath_.swap(other.dstPath_);
#	else
	using ::std::swap;
	
	swap(srcPath_, other.srcPath_);
	swap(dstPath_, other.dstPath_);
#	endif
}


inline CPPDEVTK_UTIL_API void swap(FilesystemException& x, FilesystemException& y) {
	x.Swap(y);
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_FILESYSTEM_EXCEPTION_HPP_INCLUDED_
