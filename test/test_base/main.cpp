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


#include "config.hpp"
#include "init_resources.hpp"
#include <cppdevtk/base/stack_trace.hpp>
#include <cppdevtk/base/qiostream.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/ios.hpp>
#include <cppdevtk/base/dbc_exceptions.hpp>
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/safe_delete.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/mutex.hpp>
#include <cppdevtk/base/lockables.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/tstring.hpp>
#include <cppdevtk/base/singletons.hpp>
#include <cppdevtk/util/core_application.hpp>

#include "semaphores.hpp"
#include "waitconditions.hpp"


#include <QtCore/QString>
#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtCore/QException>
#else
#include <QtCore/QtCore>
#endif

#include <cstdlib>
#include <cstddef>

#if (CPPDEVTK_PLATFORM_WINDOWS)
#include <windows.h>
#endif


class TestTmplExplInst: public ::cppdevtk::base::ObjectLevelBasicLockable {};
class IncompleteType;

class DummyDefaultObjectLevelBasicLockable: public ::cppdevtk::base::DefaultObjectLevelBasicLockable {};
class DummyClassLevelBasicLockable: public ::cppdevtk::base::ClassLevelBasicLockable {};

class Greetings: public ::cppdevtk::base::MeyersSingleton<Greetings> {
	friend class ::cppdevtk::base::MeyersSingleton<Greetings>;
public:
	void Welcome() {
		::cppdevtk::base::qcout << "Hello World!" << endl;
	}
private:
	Greetings(): ::cppdevtk::base::MeyersSingleton<Greetings>() {}
	~Greetings() {}
};

class Logger: public ::cppdevtk::base::ThreadSafePhoenixSingleton<Logger> {
	friend class ::cppdevtk::base::ThreadSafePhoenixSingleton<Logger>;
public:
	void Log(const QString& msg) {
		::cppdevtk::base::qcout << "log msg: " << msg << endl;
	}
private:
	Logger(): ::cppdevtk::base::ThreadSafePhoenixSingleton<Logger>() {
		::cppdevtk::base::qcout << "logger created" << endl;
	}
	
	~Logger() {
		::cppdevtk::base::qcout << "logger destroyed" << endl;
	}
};

class LoggerUser: public ::cppdevtk::base::Singleton<LoggerUser> {
	friend class ::cppdevtk::base::Singleton<LoggerUser>;
public:
	void UseLogger() {
		const_cast<Logger&>(Logger::GetInstance()).Log("foo");
	}
private:
	LoggerUser(): ::cppdevtk::base::Singleton<LoggerUser>() {
		::cppdevtk::base::qcout << "LoggerUser created" << endl;
	}
	
	~LoggerUser() {
		::cppdevtk::base::qcout << "LoggerUser destroyed" << endl;
	}
};


class DerivedFromStdRuntimeError: public ::std::runtime_error {
public:
	DerivedFromStdRuntimeError(const ::std::string& msg): runtime_error(msg) {}
};

class DerivedFromQtException: public CPPDEVTK_QT_EXCEPTION {
public:
	virtual DerivedFromQtException* clone() const {
		return new DerivedFromQtException(*this);
	}
	
	virtual void raise() const {
		throw *this;
	}
};


bool TestSafeDelete();
bool TestSingleton();
bool TestStackTrace();
bool TestStdExceptions();
bool TestNonStdExceptions();
bool TestSystemException();
bool TestExceptionPropagation();
bool TestMutex();


void TestStackTraceCppHelper1(int dummy);
void TestStackTraceCppHelper2(int);

extern "C" void TestStackTraceCHelper1(int dummy);
extern "C" void TestStackTraceCHelper2(int);


using ::cppdevtk::base::qcerr;
using ::cppdevtk::base::qcout;
using ::cppdevtk::base::Exception;
using ::cppdevtk::base::SystemException;
using ::cppdevtk::base::Mutex;
using ::cppdevtk::base::RecursiveMutex;
using ::cppdevtk::base::TimedMutex;
using ::cppdevtk::base::RecursiveTimedMutex;
#if (CPPDEVTK_HAVE_PTHREADS)
using ::cppdevtk::base::ErrorCheckingMutex;
using ::cppdevtk::base::ErrorCheckingTimedMutex;
#endif
using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;
using ::std::exception;


int main(int argc, char* argv[]) try {
	::cppdevtk::test_base::InitResources();
	
	::cppdevtk::util::CoreApplication coreApplication(argc, argv);
	
#	if 0
	// make sure npos works (issue with msvc >= 2010 when exporting string from dll)
	const ::std::string::size_type npos = ::std::string::npos;
	const ::std::wstring::size_type wnpos = ::std::wstring::npos;
	::cppdevtk::base::SuppressUnusedWarning(npos);
	::cppdevtk::base::SuppressUnusedWarning(wnpos);
#	else
	// use kStdStringNPos
#	endif
	
	TestTmplExplInst testTmplExplInst;
	
	try {
		qcout << "testing SafeDelete..." << endl;
		if (!TestSafeDelete()) {
			qcerr << "SafeDelete test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "SafeDelete test: PASSED" << endl;
		
		qcout << "testing Singleton..." << endl;
		if (!TestSingleton()) {
			qcerr << "Singleton test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "Singleton test: PASSED" << endl;
		
		qcout << "testing StackTrace..." << endl;
		if (!TestStackTrace()) {
			qcerr << "StackTrace test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "StackTrace test: PASSED" << endl;
		
		qcout << "testing StdExceptions..." << endl;
		if (!TestStdExceptions()) {
			qcerr << "StdExceptions test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "StdExceptions test: PASSED" << endl;
		
		qcout << "testing NonStdExceptions..." << endl;
		if (!TestNonStdExceptions()) {
			qcerr << "NonStdExceptions test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "NonStdExceptions test: PASSED" << endl;
		
		qcout << "testing SystemException..." << endl;
		if (!TestSystemException()) {
			qcerr << "SystemException test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "SystemException test: PASSED" << endl;
		
		qcout << "testing ExceptionPropagation..." << endl;
		if (!TestExceptionPropagation()) {
			qcerr << "ExceptionPropagation test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "ExceptionPropagation test: PASSED" << endl;
		
		qcout << "testing Mutex..." << endl;
		if (!TestMutex()) {
			qcerr << "Mutex test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		qcout << "Mutex test: PASSED" << endl;
		
		qcout << "testing semaphore..." << endl;
		test_sem();
		qcout << "semaphore test: PASSED" << endl;
		
		qcout << "testing condition variable..." << endl;
		test_cv();
		qcout << "condition variable test: PASSED" << endl;
		
		qcout << "done!" << endl;
		return EXIT_SUCCESS;
	}
	catch (const exception& exc) {
		const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
				exc.what(), Exception::GetDetailedInfo(exc));
		CPPDEVTK_LOG_ERROR(kErrMsg);
		qcerr << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
	catch (...) {
		const QString kErrMsg("caught unknown exception!!!");
		CPPDEVTK_LOG_ERROR(kErrMsg);
		qcerr << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
}
catch (const exception& exc) {
	const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
			exc.what(), Exception::GetDetailedInfo(exc));
	CPPDEVTK_LOG_FATAL(kErrMsg);
	qcerr << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
catch (...) {
	const QString kErrMsg("caught unknown exception!!!");
	CPPDEVTK_LOG_FATAL(kErrMsg);
	qcerr << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}


bool TestSafeDelete() {
	using ::cppdevtk::base::DeleteCompleteType;
	
	// tested and passed: does not compile
	/*
	IncompleteType* pIncompleteType = NULL;
	DeleteCompleteType(pIncompleteType);
	*/
	
	DeleteCompleteType(new QString());
	
	return true;
}

bool TestSingleton() {
	Greetings::GetInstance().Welcome();
	const_cast<LoggerUser&>(LoggerUser::GetInstance()).UseLogger();
	return true;
}

bool TestStackTrace() {
	using ::cppdevtk::base::StackTrace;
	
	class TestStackTraceClass {
	public:
		void f_ctor() {
			StackTrace stackTrace;
			qcout << "StackTrace auto captured:\n" << stackTrace.ToString() << endl;
		}
		
		void f_capture() {
			StackTrace stackTrace(false);
			stackTrace.Capture();
			qcout << "StackTrace captured on demand:\n" << stackTrace.ToString() << endl;
		}
	};
	
	TestStackTraceClass testStackTraceClass;
	testStackTraceClass.f_ctor();
	testStackTraceClass.f_capture();
	
	TestStackTraceCppHelper1(0);
	
	TestStackTraceCHelper1(0);
	
	return true;
}

bool TestStdExceptions() {
	using ::cppdevtk::base::Cloneable;
	using ::cppdevtk::base::Exception;
	using ::cppdevtk::base::LogicException;
	using ::cppdevtk::base::InvalidArgumentException;
	using ::cppdevtk::base::swap;
	using ::std::auto_ptr;
	using ::std::swap;
	
	
	CPPDEVTK_MAKE_LOGIC_EXCEPTION_W_CAUSE(exc, "a bug...", CPPDEVTK_DOMAIN_EXCEPTION("a domain bug..."));
	
	auto_ptr<Cloneable> pCloneable(exc.Clone());
	if (pCloneable.get() == NULL) {
		qcerr << "pCloneable.get() == NULL" << endl;
		return false;
	}
	
	auto_ptr<Exception> pException(exc.Clone());
	if (pException.get() == NULL) {
		qcerr << "pException.get() == NULL" << endl;
		return false;
	}
	
	auto_ptr<LogicException> pLogicException(exc.Clone());
	if (pLogicException.get() == NULL) {
		qcerr << "pLogicException.get() == NULL" << endl;
		return false;
	}
	
	
	qcout << "pException->What(): " << pException->What() << endl;
	qcout << "pException->ToString(): " << pException->ToString() << endl;
	
	
	bool catched = false;
	try {
		pException->Throw();
	}
	catch (LogicException&) {
		catched = true;
	}
	if (!catched) {
		qcerr << "failed to catch LogicException" << endl;
		return false;
	}
	
	
	CPPDEVTK_MAKE_LOGIC_EXCEPTION(logicException1, "aLogicException");
	LogicException logicException2 = logicException1;
	logicException1 = logicException2;
	
	CPPDEVTK_MAKE_INVALID_ARGUMENT_EXCEPTION(invalidArgumentException1, "arg1");
	InvalidArgumentException invalidArgumentException2 = invalidArgumentException1;
	invalidArgumentException1 = invalidArgumentException2;
	
	return true;
}

bool TestNonStdExceptions() {
	using ::cppdevtk::base::NullArgumentException;
	using ::cppdevtk::base::NullPointerException;
	using ::cppdevtk::base::InvalidArgumentException;
	using ::cppdevtk::base::LogicException;
	using ::cppdevtk::base::Exception;
	using ::std::exception;
	
	
	CPPDEVTK_MAKE_NULL_ARGUMENT_EXCEPTION(nullArgumentException1, "arg1");
	NullArgumentException nullArgumentException2 = nullArgumentException1;
	nullArgumentException1 = nullArgumentException2;
	
	
	CPPDEVTK_MAKE_NULL_ARGUMENT_EXCEPTION(nullArg1, "arg1");
	const QString kNullArg1What = nullArg1.What();
	
	CPPDEVTK_MAKE_NULL_ARGUMENT_EXCEPTION(nullArg2, "arg2");
	const QString kNullArg2What = nullArg2.What();
	
	swap(nullArg1, nullArg2);
	
	if (nullArg1.What() != kNullArg2What) {
		qcerr << "nullArg1.What() != kNullArg2What; nullArg1.What(): " << nullArg1.What()
				<< "; kNullArg2What: " << kNullArg2What << endl;
		return false;
	}
	
	if (nullArg2.What() != kNullArg1What) {
		qcerr << "nullArg2.What() != kNullArg1What; nullArg2.What(): " << nullArg2.What()
				<< "; kNullArg1What: " << kNullArg1What << endl;
		return false;
	}
	
	
	try {
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (NullArgumentException& exc) {
			qcout << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (NullPointerException& exc) {
			qcout << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (InvalidArgumentException& exc) {
			qcout << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (LogicException& exc) {
			qcout << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (Exception& exc) {
			qcout << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (exception& exc) {
			qcout << "exc.what(): " << exc.what() << endl;
		}
	}
	catch (...) {
		return false;
	}
	
	return true;
}

bool TestSystemException() {
	::cppdevtk::base::SetLastSystemErrorCode(::cppdevtk::base::MakeSystemErrorCode(ESUCCESS));
	
	qcout << CPPDEVTK_SYS_EXC_W_EC(::cppdevtk::base::GetLastSystemErrorCode()).What() << endl;
	
	return true;
}

bool TestExceptionPropagation() {
	using ::std::runtime_error;
	using ::cppdevtk::base::ExceptionPtr;
	using ::cppdevtk::base::CurrentException;
	using ::cppdevtk::base::RethrowException;
	
	{
		ExceptionPtr exceptionPtr;
		try {
			throw DerivedFromStdRuntimeError("some derived runtime error occurred");
		}
		catch (const exception&) {
			exceptionPtr = CurrentException();
			CPPDEVTK_ASSERT(exceptionPtr != NULL);
		}
		
		if (exceptionPtr != NULL) {
			try {
				RethrowException(exceptionPtr);
			}
			catch (const DerivedFromStdRuntimeError&) {
				qcout << "exception propagation supports classes derived from std exception classes" << endl;
			}
			catch (const runtime_error&) {
				qcout << "WARN: exception propagation does not support classes derived from std exception classes" << endl;
			}
			catch (const exception&) {
				qcerr << "exception propagation does not support std exception classes" << endl;
				return false;
			}
		}
	}
	
	{
		ExceptionPtr exceptionPtr;
		try {
			throw DerivedFromQtException();
		}
		catch (const exception&) {
			exceptionPtr = CurrentException();
		}
		
		if (exceptionPtr != NULL) {
			try {
				RethrowException(exceptionPtr);
			}
			catch (const DerivedFromQtException&) {
				qcout << "exception propagation supports Qt exception classes" << endl;
			}
			catch (const exception&) {
				qcerr << "exception propagation does not support Qt exception classes" << endl;
				return false;
			}
		}
	}
	
	{
		ExceptionPtr exceptionPtr;
		try {
			throw CPPDEVTK_IOS_FAILURE_EXC_W_WA("some i/o exception occurred");
		}
		catch (const exception&) {
			exceptionPtr = CurrentException();
		}
		
		if (exceptionPtr != NULL) {
			try {
				RethrowException(exceptionPtr);
			}
			catch (const ::cppdevtk::base::IosFailureException&) {
				qcout << "exception propagation supports cppdevtk exception classes" << endl;
			}
			catch (const exception&) {
				qcerr << "exception propagation does not support cppdevtk exception classes" << endl;
				return false;
			}
		}
	}
	
	return true;
}

bool TestMutex() {
	Mutex mutex;
	mutex.Lock();
	if (mutex.TryLock()) {
		qcout << "mutex.TryLock() succeeded" << endl;
		return false;
	}
	mutex.Unlock();
	
	
	bool caught = false;
	
	RecursiveMutex recursiveMutex;
	recursiveMutex.Lock();
	recursiveMutex.Lock();
	recursiveMutex.TryLock();
	recursiveMutex.TryLock();
	recursiveMutex.Unlock();
	recursiveMutex.Unlock();
	recursiveMutex.Unlock();
	recursiveMutex.Unlock();	
	
	
	TimedMutex timedMutex;
	timedMutex.Lock();
	if (timedMutex.TryLock()) {
		qcout << "timedMutex.TryLock() succeeded" << endl;
		return false;
	}
	timedMutex.Unlock();
	
	
	RecursiveTimedMutex recursiveTimedMutex;
	recursiveTimedMutex.Lock();
	recursiveTimedMutex.Lock();
	recursiveTimedMutex.TryLock();
	recursiveTimedMutex.TryLock();
	recursiveTimedMutex.Unlock();
	recursiveTimedMutex.Unlock();
	recursiveTimedMutex.Unlock();
	recursiveTimedMutex.Unlock();
	
	
#	if (CPPDEVTK_HAVE_PTHREADS)
	ErrorCheckingMutex errorCheckingMutex;
	errorCheckingMutex.Lock();
	caught = false;
	try {
		errorCheckingMutex.Lock();
	}
	catch (const SystemException& exc) {
		if (exc.ErrorCodeRef() == ::cppdevtk::base::errc::resource_deadlock_would_occur) {
			caught = true;
		}
		else {
			qcout << "errorCheckingMutex.Lock(): got wrong error code: " << exc.ErrorCodeRef().ToString() << endl;
		}
	}
	if (!caught) {
		qcout << "errorCheckingMutex.Lock(): !caught" << endl;
		return false;
	}
	errorCheckingMutex.Unlock();
	
	errorCheckingMutex.Lock();
	if (errorCheckingMutex.TryLock()) {
		qcout << "errorCheckingMutex.TryLock() succeeded" << endl;
		return false;
	}
	errorCheckingMutex.Unlock();
	
	
	ErrorCheckingTimedMutex errorCheckingTimedMutex;
	errorCheckingTimedMutex.Lock();
	caught = false;
	try {
		errorCheckingTimedMutex.Lock();
	}
	catch (const SystemException& exc) {
		if (exc.ErrorCodeRef() == ::cppdevtk::base::errc::resource_deadlock_would_occur) {
			caught = true;
		}
		else {
			qcout << "errorCheckingTimedMutex.Lock(): got wrong error code: " << exc.ErrorCodeRef().ToString() << endl;
		}
	}
	if (!caught) {
		qcout << "errorCheckingTimedMutex.Lock(): !caught" << endl;
		return false;
	}
	errorCheckingTimedMutex.Unlock();
	
	errorCheckingTimedMutex.Lock();
	if (errorCheckingTimedMutex.TryLock()) {
		qcout << "errorCheckingTimedMutex.TryLock() succeeded" << endl;
		return false;
	}
	errorCheckingTimedMutex.Unlock();
#endif
	
	
	DummyDefaultObjectLevelBasicLockable dummyDefaultObjectLevelBasicLockable;
	::cppdevtk::base::DefaultObjectLevelBasicLockableLockGuard defaultObjectLevelBasicLockableLockGuard(
			dummyDefaultObjectLevelBasicLockable);
	
	DummyClassLevelBasicLockable dummylassLevelBasicLockable;
	::cppdevtk::base::ClassLevelBasicLockableLockGuard classLevelBasicLockableLockGuard(dummylassLevelBasicLockable);
	
	::cppdevtk::base::SuppressUnusedWarning(caught);
	
	return true;
}


void TestStackTraceCppHelper1(int dummy) {
	TestStackTraceCppHelper2(dummy);
}

void TestStackTraceCppHelper2(int) {
	::cppdevtk::base::StackTrace stackTrace;
	qcout << "StackTrace from C++ function:\n" << stackTrace.ToString() << endl;
}

void TestStackTraceCHelper1(int dummy) {
	TestStackTraceCHelper2(dummy);
}

void TestStackTraceCHelper2(int) {
	::cppdevtk::base::StackTrace stackTrace;
	qcout << "StackTrace from C function:\n" << stackTrace.ToString() << endl;
}
