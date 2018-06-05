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


#ifndef CPPDEVTK_UTIL_DBUS_EXCEPTION_HPP_INCLUDED_
#define CPPDEVTK_UTIL_DBUS_EXCEPTION_HPP_INCLUDED_


#include "config.hpp"
#ifdef QT_NO_DBUS
#error "This file require QtDBus"
#endif

#include <cppdevtk/base/stdexcept.hpp>

#include <QtDBus/QDBusError>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DBusException

#define CPPDEVTK_DBUS_EXCEPTION(whatArg, dbusError)	\
	::cppdevtk::util::DBusException(CPPDEVTK_SOURCE_CODE_INFO(), (whatArg), (dbusError))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Thrown to indicate QtDBus error
class CPPDEVTK_UTIL_API DBusException: public ::cppdevtk::base::RuntimeException {
public:
	DBusException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg, const QDBusError& dbusError);
	virtual ~DBusException() CPPDEVTK_NOEXCEPT;
	
	::std::auto_ptr<DBusException> Clone() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual QtException* clone() const;
#	else
	virtual DBusException* clone() const;
#	endif
	
	void Swap(DBusException& other) CPPDEVTK_NOEXCEPT;
	
	QDBusError GetDBusError() const;
protected:
	virtual void DoThrow() const;
	
#	if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
	virtual ::cppdevtk::base::Cloneable* DoClone() const;
#	else
	virtual DBusException* DoClone() const;
#	endif
	
	void SwapOwnData(DBusException& other) CPPDEVTK_NOEXCEPT;
private:
	QDBusError dbusError_;
};


CPPDEVTK_UTIL_API void swap(DBusException& x, DBusException& y) CPPDEVTK_NOEXCEPT;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline DBusException::DBusException(const ::cppdevtk::base::SourceCodeInfo& throwPoint, const QString& whatArg,
		const QDBusError& dbusError): Exception(throwPoint),
		RuntimeException(throwPoint, QString("%1; dbusErrorType: %2; dbusErrorName: %3; dbusErrorMessage: %4").arg(
		whatArg, QDBusError::errorString(dbusError.type()), dbusError.name(), dbusError.message())), dbusError_(dbusError) {}

inline DBusException::~DBusException() CPPDEVTK_NOEXCEPT {}

inline ::std::auto_ptr<DBusException> DBusException::Clone() const {
	return ::std::auto_ptr<DBusException>(dynamic_cast<DBusException*>(Cloneable::Clone().release()));
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline QtException* DBusException::clone() const {
#else
inline DBusException* DBusException::clone() const {
#endif
	return Clone().release();
}

inline void DBusException::Swap(DBusException& other) CPPDEVTK_NOEXCEPT {
	if (this != &other) {
		RuntimeException::Swap(other);
		SwapOwnData(other);
	}
}

inline QDBusError DBusException::GetDBusError() const {
	return dbusError_;
}

inline void DBusException::DoThrow() const {
	throw *this;
}

#if (CPPDEVTK_COMPILER_HAVE_MVI_CRT_BUG)
inline ::cppdevtk::base::Cloneable* DBusException::DoClone() const {
#else
inline DBusException* DBusException::DoClone() const {
#endif
	return new DBusException(*this);
}

inline void DBusException::SwapOwnData(DBusException& other) CPPDEVTK_NOEXCEPT {
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	dbusError_.swap(other.dbusError_);
#	else
	using ::std::swap;
	swap(dbusError_, other.dbusError_);
#	endif
}


inline CPPDEVTK_UTIL_API void swap(DBusException& x, DBusException& y) CPPDEVTK_NOEXCEPT {
	x.Swap(y);
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_DBUS_EXCEPTION_HPP_INCLUDED_
