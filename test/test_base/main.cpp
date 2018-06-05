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
#include <cppdevtk/base/condition_variable.hpp>
#include <cppdevtk/base/lockables.hpp>
#include <cppdevtk/base/unused.hpp>
#include <cppdevtk/base/tstring.hpp>
#include <cppdevtk/base/singletons.hpp>
#include <cppdevtk/base/thread.hpp>
#include <cppdevtk/base/thread_exception.hpp>
#include <cppdevtk/base/factory.hpp>
#include <cppdevtk/base/verify.h>
#include <cppdevtk/util/core_application.hpp>

#include "semaphores.hpp"
#include "waitconditions.hpp"
#include "boost_any_test.hpp"
#include "boost_test_optional/optional_test.hpp"
#include "boost_call_traits_test.hpp"

#include <QtCore/QString>
#include <QtCore/QtGlobal>
#include <QtCore/QTime>
#include <QtCore/QtDebug>
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


#if (!CPPDEVTK_PLATFORM_ANDROID)
#define CPPDEVTK_COUT ::cppdevtk::base::qcout
#define CPPDEVTK_CERR ::cppdevtk::base::qcerr
#else
#define CPPDEVTK_COUT qDebug()
#define CPPDEVTK_CERR qInfo()
#endif


class TestTmplExplInst: public ::cppdevtk::base::ObjectLevelBasicLockable {};
class IncompleteType;

class DummyDefaultObjectLevelBasicLockable: public ::cppdevtk::base::DefaultObjectLevelBasicLockable {};
class DummyClassLevelBasicLockable: public ::cppdevtk::base::ClassLevelBasicLockable {};

class Greetings: public ::cppdevtk::base::MeyersSingleton<Greetings> {
	friend class ::cppdevtk::base::MeyersSingleton<Greetings>;
public:
	void Welcome() {
		CPPDEVTK_COUT << "Hello World!" << endl;
	}
private:
	Greetings(): ::cppdevtk::base::MeyersSingleton<Greetings>() {}
	~Greetings() {}
};

class Logger: public ::cppdevtk::base::ThreadSafePhoenixSingleton<Logger> {
	friend class ::cppdevtk::base::ThreadSafePhoenixSingleton<Logger>;
public:
	void Log(const QString& msg) {
		CPPDEVTK_COUT << "log msg: " << msg << endl;
	}
private:
	Logger(): ::cppdevtk::base::ThreadSafePhoenixSingleton<Logger>() {
		CPPDEVTK_COUT << "logger created" << endl;
	}
	
	~Logger() {
		CPPDEVTK_COUT << "logger destroyed" << endl;
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
		CPPDEVTK_COUT << "LoggerUser created" << endl;
	}
	
	~LoggerUser() {
		CPPDEVTK_COUT << "LoggerUser destroyed" << endl;
	}
};


class DerivedFromStdRuntimeError: public ::std::runtime_error {
public:
	DerivedFromStdRuntimeError(const ::std::string& msg): runtime_error(msg) {}
};

class DerivedFromQtException: public QtException {
public:
	virtual DerivedFromQtException* clone() const {
		return new DerivedFromQtException(*this);
	}
	
	virtual void raise() const {
		throw *this;
	}
};


#if (CPPDEVTK_HAVE_THREAD_STORAGE)

::cppdevtk::base::DefaultMutex gQCOutMtx;

int ThreadMainFunctionHelloWorld() {
	::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
	CPPDEVTK_COUT << "Function Hello world from thread with id: " << ::cppdevtk::base::this_thread::GetId().ToString() << endl;
	return EXIT_SUCCESS;
}

class HelloWorldThread: public ::cppdevtk::base::Thread {
public:
	HelloWorldThread(): Thread() {}
protected:
	virtual int Main() {
		::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
		CPPDEVTK_COUT << "Thread Hello world from thread with id: " << ::cppdevtk::base::this_thread::GetId().ToString() << endl;
		return EXIT_SUCCESS;
	}
};

#if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)

class InterruptibleSleepHelloWorldThread: public ::cppdevtk::base::Thread {
public:
	InterruptibleSleepHelloWorldThread(): Thread() {}
protected:
	virtual int Main() {
		{
			::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
			CPPDEVTK_COUT << "Interruptible Sleep Thread Hello world entering main from thread with id: "
					<< ::cppdevtk::base::this_thread::GetId().ToString() << endl;
		}
		
		// parent thread waits 2.5 sec before requesting interrupt; child thread loops max 5 sec
		for (int cnt = 0; cnt < 50; ++cnt) {
			{
				::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
				CPPDEVTK_COUT << "Interruptible Sleep Thread Hello world from thread with id: "
						<< ::cppdevtk::base::this_thread::GetId().ToString() << "; sleepCnt: " << cnt << endl;
			}
			::cppdevtk::base::this_thread::SleepFor(100);
		}
		
		{
			::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
			CPPDEVTK_COUT << "Interruptible Sleep Thread Hello world leaving main from thread with id: "
					<< ::cppdevtk::base::this_thread::GetId().ToString() << endl;
		}
		
		return EXIT_SUCCESS;
	}
};

class InterruptibleCondVarHelloWorldThread: public ::cppdevtk::base::Thread {
public:
	InterruptibleCondVarHelloWorldThread(): Thread() {}
protected:
	virtual int Main() {
		{
			::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
			CPPDEVTK_COUT << "Interruptible Cond Var Thread Hello world entering main from thread with id: "
					<< ::cppdevtk::base::this_thread::GetId().ToString() << endl;
		}
		
		// parent thread waits 2.5 sec before requesting interrupt; child thread loops max 5 sec
		const int kMaxCnt = (5 * 1000) / CPPDEVTK_CHECK_INTERRUPT_REL_TIME;
		for (int cnt = 0; cnt < kMaxCnt; ++cnt) {
			{
				::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
				CPPDEVTK_COUT << "Interruptible Cond Var Thread Hello world from thread with id: "
						<< ::cppdevtk::base::this_thread::GetId().ToString() << "; sleepCnt: " << cnt << endl;
			}
			::cppdevtk::base::Mutex mtx;
			::cppdevtk::base::UniqueLock< ::cppdevtk::base::Mutex> uniqueLock(mtx);
			::cppdevtk::base::ConditionVariable condVar;
			condVar.WaitFor(uniqueLock, 100);	// no predicate so CPPDEVTK_CHECK_INTERRUPT_REL_TIME spurious wakeups
		}
		
		{
			::cppdevtk::base::DefaultLockGuard lockGuard(gQCOutMtx);
			CPPDEVTK_COUT << "Interruptible Cond Var Thread Hello world leaving main from thread with id: "
					<< ::cppdevtk::base::this_thread::GetId().ToString() << endl;
		}
		
		return EXIT_SUCCESS;
	}
};

#endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


class Pet {
public:
	virtual ~Pet() {}
	virtual void Speak() const = 0;
};

class Dog: public Pet {
public:
	virtual void Speak() const {
		CPPDEVTK_COUT << "Woof, I'm hungry" << endl;
	}
};

class Cat: public Pet {
public:
	virtual void Speak() const {
		CPPDEVTK_COUT << "Meow, I'm hungry" << endl;
	}
};

enum PetId {
	pidDog,
	pidCat
};


void PetDeleter(Pet* pPet) {
	delete pPet;
}

Pet* CreateDogFunction() {
	return new Dog();
}

Pet* CreateCatFunction() {
	return new Cat();
}

class CreatePetFunctionObject {
public:
	virtual ~CreatePetFunctionObject() {}
	virtual Pet* operator()() = 0;
};

class CreateDogFunctionObject: public CreatePetFunctionObject {
public:
	Pet* operator()() {
		return new Dog();
	}
};

class CreateCatFunctionObject: public CreatePetFunctionObject {
public:
	Pet* operator()() {
		return new Cat();
	}
};




bool TestSafeDelete();
bool TestSingleton();
bool TestStackTrace();
bool TestStdExceptions();
bool TestNonStdExceptions();
bool TestSystemException();
bool TestExceptionPropagation();
bool TestFactory();
bool TestAny();
bool TestMutex();
#if (CPPDEVTK_HAVE_THREAD_STORAGE)
bool TestThread();
#endif


void TestStackTraceCppHelper1(int dummy);
void TestStackTraceCppHelper2(int);

extern "C" void TestStackTraceCHelper1(int dummy);
extern "C" void TestStackTraceCHelper2(int);

void PrintTime(int milliseconds);


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
#if (CPPDEVTK_HAVE_THREAD_STORAGE)
using ::cppdevtk::base::Thread;
#endif
using ::cppdevtk::base::ErrorCode;
using ::cppdevtk::base::GetLastSystemErrorCode;
using ::cppdevtk::base::SuppressUnusedWarning;
using ::std::exception;


int main(int argc, char* argv[]) try {
	using ::cppdevtk::util::CoreApplication;
	
	
	::cppdevtk::test_base::InitResources();
	
	CoreApplication::SetInfo(CPPDEVTK_COMPANY_SHORT_NAME_SANITIZED, CPPDEVTK_COMPANY_HOMEPAGE,
			CPPDEVTK_SHORT_NAME_SANITIZED, CPPDEVTK_VERSION_STRING, CPPDEVTK_TEST_BASE_SHORT_NAME_SANITIZED);
	
	CoreApplication coreApplication(argc, argv);
	
#	if 0
	// make sure npos works (issue with msvc >= 2010 when exporting string from dll)
	const ::std::string::size_type npos = ::std::string::npos;
	const ::std::wstring::size_type wnpos = ::std::wstring::npos;
	SuppressUnusedWarning(npos);
	SuppressUnusedWarning(wnpos);
#	else
	// use kStdStringNPos
#	endif
	
	//TestTmplExplInst testTmplExplInst;
	
	try {
		CPPDEVTK_COUT << "testing SafeDelete..." << endl;
		if (!TestSafeDelete()) {
			CPPDEVTK_CERR << "SafeDelete test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "SafeDelete test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing Singleton..." << endl;
		if (!TestSingleton()) {
			CPPDEVTK_CERR << "Singleton test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "Singleton test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing StackTrace..." << endl;
		if (!TestStackTrace()) {
			CPPDEVTK_CERR << "StackTrace test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "StackTrace test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing StdExceptions..." << endl;
		if (!TestStdExceptions()) {
			CPPDEVTK_CERR << "StdExceptions test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "StdExceptions test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing NonStdExceptions..." << endl;
		if (!TestNonStdExceptions()) {
			CPPDEVTK_CERR << "NonStdExceptions test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "NonStdExceptions test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing SystemException..." << endl;
		if (!TestSystemException()) {
			CPPDEVTK_CERR << "SystemException test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "SystemException test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing ExceptionPropagation..." << endl;
		if (!TestExceptionPropagation()) {
			CPPDEVTK_CERR << "ExceptionPropagation test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "ExceptionPropagation test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing Factory..." << endl;
		if (!TestFactory()) {
			CPPDEVTK_CERR << "Factory test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "Factory test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing Any..." << endl;
		if (!TestAny()) {
			CPPDEVTK_CERR << "Any test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "Any test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing Optional..." << endl;
		if (!TestOptional()) {
			CPPDEVTK_CERR << "Optional test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		if (boost::test_errors_counter != 0) {
			CPPDEVTK_COUT << "Optional test_errors_counter: " << boost::test_errors_counter << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "Optional test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing CallTraits..." << endl;
		if (!TestCallTraits()) {
			CPPDEVTK_CERR << "CallTraits test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		if (boost::test_errors_counter != 0) {
			CPPDEVTK_COUT << "CallTraits test_errors_counter: " << boost::test_errors_counter << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "CallTraits test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing Mutex..." << endl;
		if (!TestMutex()) {
			CPPDEVTK_CERR << "Mutex test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "Mutex test: PASSED" << endl;
		
#		if (CPPDEVTK_HAVE_THREAD_STORAGE)
		
		CPPDEVTK_COUT << "testing Thread..." << endl;
		if (!TestThread()) {
			CPPDEVTK_CERR << "Thread test: FAILED!!!" << endl;
			return EXIT_FAILURE;
		}
		CPPDEVTK_COUT << "Thread test: PASSED" << endl;
		
#		endif
		
		CPPDEVTK_COUT << "testing semaphore..." << endl;
		test_sem();
		CPPDEVTK_COUT << "semaphore test: PASSED" << endl;
		
		CPPDEVTK_COUT << "testing condition variable..." << endl;
		test_cv();
		CPPDEVTK_COUT << "condition variable test: PASSED" << endl;
		
		CPPDEVTK_COUT << "done!" << endl;
		return EXIT_SUCCESS;
	}
	catch (const exception& exc) {
		const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
				exc.what(), Exception::GetDetailedInfo(exc));
		CPPDEVTK_LOG_ERROR(kErrMsg);
		CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
	catch (...) {
		const QString kErrMsg("caught unknown exception!!!");
		CPPDEVTK_LOG_ERROR(kErrMsg);
		CPPDEVTK_CERR << "Error: " << kErrMsg << endl;
		
		return EXIT_FAILURE;
	}
}
catch (const exception& exc) {
	const QString kErrMsg = QString("caught ::std::exception: %1\nDetails: %2").arg(
			exc.what(), Exception::GetDetailedInfo(exc));
	CPPDEVTK_LOG_FATAL(kErrMsg);
	CPPDEVTK_CERR << "Fatal Error: " << kErrMsg << endl;
	
	return EXIT_FAILURE;
}
catch (...) {
	const QString kErrMsg("caught unknown exception!!!");
	CPPDEVTK_LOG_FATAL(kErrMsg);
	CPPDEVTK_CERR << "Fatal Error: " << kErrMsg << endl;
	
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
			CPPDEVTK_COUT << "StackTrace auto captured:\n" << stackTrace.ToString() << endl;
		}
		
		void f_capture() {
			StackTrace stackTrace(false);
			stackTrace.Capture();
			CPPDEVTK_COUT << "StackTrace captured on demand:\n" << stackTrace.ToString() << endl;
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
		CPPDEVTK_CERR << "pCloneable.get() == NULL" << endl;
		return false;
	}
	
	auto_ptr<Exception> pException(exc.Clone());
	if (pException.get() == NULL) {
		CPPDEVTK_CERR << "pException.get() == NULL" << endl;
		return false;
	}
	
	auto_ptr<LogicException> pLogicException(exc.Clone());
	if (pLogicException.get() == NULL) {
		CPPDEVTK_CERR << "pLogicException.get() == NULL" << endl;
		return false;
	}
	
	
	CPPDEVTK_COUT << "pException->What(): " << pException->What() << endl;
	CPPDEVTK_COUT << "pException->ToString(): " << pException->ToString() << endl;
	
	
	bool catched = false;
	try {
		pException->Throw();
	}
	catch (LogicException&) {
		catched = true;
	}
	if (!catched) {
		CPPDEVTK_CERR << "failed to catch LogicException" << endl;
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
		CPPDEVTK_CERR << "nullArg1.What() != kNullArg2What; nullArg1.What(): " << nullArg1.What()
				<< "; kNullArg2What: " << kNullArg2What << endl;
		return false;
	}
	
	if (nullArg2.What() != kNullArg1What) {
		CPPDEVTK_CERR << "nullArg2.What() != kNullArg1What; nullArg2.What(): " << nullArg2.What()
				<< "; kNullArg1What: " << kNullArg1What << endl;
		return false;
	}
	
	
	try {
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (NullArgumentException& exc) {
			CPPDEVTK_COUT << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (NullPointerException& exc) {
			CPPDEVTK_COUT << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (InvalidArgumentException& exc) {
			CPPDEVTK_COUT << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (LogicException& exc) {
			CPPDEVTK_COUT << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (Exception& exc) {
			CPPDEVTK_COUT << "exc.What(): " << exc.What() << endl;
		}
		
		try {
			throw CPPDEVTK_NULL_ARGUMENT_EXCEPTION("dummy");
		}
		catch (exception& exc) {
			CPPDEVTK_COUT << "exc.what(): " << exc.what() << endl;
		}
	}
	catch (...) {
		return false;
	}
	
	return true;
}

bool TestSystemException() {
	::cppdevtk::base::SetLastSystemErrorCode(ErrorCode());
	
	CPPDEVTK_COUT << CPPDEVTK_SYSTEM_EXCEPTION_W_EC(::cppdevtk::base::GetLastSystemErrorCode()).What() << endl;
	
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
		}
		
		if (exceptionPtr != NULL) {
			try {
				RethrowException(exceptionPtr);
			}
			catch (const DerivedFromStdRuntimeError&) {
				CPPDEVTK_COUT << "exception propagation supports classes derived from std exception classes" << endl;
			}
			catch (const runtime_error&) {
				CPPDEVTK_COUT << "WARN: exception propagation does not support classes derived from std exception classes" << endl;
			}
			catch (const exception&) {
				CPPDEVTK_CERR << "exception propagation does not support std exception classes" << endl;
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
				CPPDEVTK_COUT << "exception propagation supports Qt exception classes" << endl;
			}
			catch (const exception&) {
				CPPDEVTK_CERR << "exception propagation does not support Qt exception classes" << endl;
				return false;
			}
		}
	}
	
	{
		ExceptionPtr exceptionPtr;
		try {
			throw CPPDEVTK_IOS_FAILURE_EXCEPTION_W_WA("some i/o exception occurred");
		}
		catch (const exception&) {
			exceptionPtr = CurrentException();
		}
		
		if (exceptionPtr != NULL) {
			try {
				RethrowException(exceptionPtr);
			}
			catch (const ::cppdevtk::base::IosFailureException&) {
				CPPDEVTK_COUT << "exception propagation supports cppdevtk exception classes" << endl;
			}
			catch (const exception&) {
				CPPDEVTK_CERR << "exception propagation does not support cppdevtk exception classes" << endl;
				return false;
			}
		}
	}
	
	return true;
}

bool TestFactory() {
	typedef ::cppdevtk::base::Factory<Pet, PetId, false> PetFactory;
	PetFactory& thePetFactory = PetFactory::GetInstance();
	
	if (!thePetFactory.IsEmpty()) {
		CPPDEVTK_CERR << "factory is not empty" << endl;
		return false;
	}
	if (thePetFactory.GetSize() != 0) {
		CPPDEVTK_CERR << "factory size is not 0" << endl;
		return false;
	}
	
	if (!thePetFactory.RegisterConcreteProduct(pidDog, CreateDogFunction)) {
		CPPDEVTK_CERR << "failed to RegisterConcreteProduct()" << endl;
		return false;
	}
	if (thePetFactory.RegisterConcreteProduct(pidDog, CreateDogFunction)) {
		CPPDEVTK_CERR << "double registration succeeded" << endl;
		return false;
	}
	if (!thePetFactory.IsConcreteProductRegistered(pidDog)) {
		CPPDEVTK_CERR << "IsConcreteProductRegistered() failed" << endl;
		return false;
	}
	if (thePetFactory.GetSize() != 1) {
		CPPDEVTK_CERR << "factory size is not 1" << endl;
		return false;
	}
	CPPDEVTK_TR1_NS::shared_ptr<Pet> pDog = thePetFactory.CreateConcreteProduct(pidDog);
	if (dynamic_cast<Dog*>(pDog.get()) == NULL) {
		CPPDEVTK_CERR << "ptr type is not Dog" << endl;
		return false;
	}
	pDog->Speak();
	if (!thePetFactory.UnregisterConcreteProduct(pidDog)) {
		CPPDEVTK_CERR << "UnregisterConcreteProduct() failed" << endl;
		return false;
	}
	if (thePetFactory.UnregisterConcreteProduct(pidDog)) {
		CPPDEVTK_CERR << "double unregistration succeeded" << endl;
		return false;
	}
	if (thePetFactory.IsConcreteProductRegistered(pidDog)) {
		CPPDEVTK_CERR << "IsConcreteProductRegistered() failed" << endl;
		return false;
	}
	if (thePetFactory.GetSize() != 0) {
		CPPDEVTK_CERR << "factory size is not 0" << endl;
		return false;
	}
	
	CPPDEVTK_VERIFY(thePetFactory.RegisterConcreteProduct(pidCat, CreateCatFunction));
	CPPDEVTK_VERIFY(!thePetFactory.RegisterConcreteProduct(pidCat, CreateCatFunction));
	CPPDEVTK_VERIFY(thePetFactory.IsConcreteProductRegistered(pidCat));
	CPPDEVTK_VERIFY(thePetFactory.GetSize() == 1);
	CPPDEVTK_TR1_NS::shared_ptr<Pet> pCat = thePetFactory.CreateConcreteProduct(pidCat, PetDeleter);
	CPPDEVTK_VERIFY(dynamic_cast<Cat*>(pCat.get()) != NULL);
	pCat->Speak();
	thePetFactory.Clear();
	CPPDEVTK_VERIFY(!thePetFactory.UnregisterConcreteProduct(pidCat));
	CPPDEVTK_VERIFY(!thePetFactory.IsConcreteProductRegistered(pidCat));
	CPPDEVTK_VERIFY(thePetFactory.GetSize() == 0);
	CPPDEVTK_VERIFY(thePetFactory.IsEmpty());
	
	
	CPPDEVTK_VERIFY(thePetFactory.RegisterConcreteProduct(pidDog, CreateDogFunctionObject()));
	CPPDEVTK_VERIFY(!thePetFactory.RegisterConcreteProduct(pidDog, CreateDogFunctionObject()));
	CPPDEVTK_VERIFY(thePetFactory.IsConcreteProductRegistered(pidDog));
	pDog = thePetFactory.CreateConcreteProduct(pidDog);
	if (dynamic_cast<Dog*>(pDog.get()) == NULL) {
		CPPDEVTK_CERR << "ptr type is not Dog" << endl;
		return false;
	}
	pDog->Speak();
	
	return true;
}

bool TestAny() {
    using namespace any_tests;
    tester<test_case_iterator> test_suite(begin, end);
    return test_suite() ? true : false;
}

bool TestMutex() {
	Mutex mutex;
	mutex.Lock();
	if (mutex.TryLock()) {
		CPPDEVTK_CERR << "mutex.TryLock() succeeded" << endl;
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
		CPPDEVTK_CERR << "timedMutex.TryLock() succeeded" << endl;
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
			CPPDEVTK_COUT << "errorCheckingMutex.Lock(): got wrong error code: " << exc.ErrorCodeRef().ToString() << endl;
		}
	}
	catch (const exception& exc) {
		CPPDEVTK_COUT << "errorCheckingMutex.Lock(): caught: " << Exception::GetDetailedInfo(exc) << endl;
	}
	catch (...) {
		CPPDEVTK_CERR << "errorCheckingMutex.Lock(): caught unknown exception" << endl;
	}
	if (!caught) {
		CPPDEVTK_CERR << "errorCheckingMutex.Lock(): !caught" << endl;
		return false;
	}
	errorCheckingMutex.Unlock();
	
	errorCheckingMutex.Lock();
	caught = false;
	try {
		if (errorCheckingMutex.TryLock()) {
			CPPDEVTK_CERR << "errorCheckingMutex.TryLock() succeeded" << endl;
			return false;
		}
		caught = true;
	}
	catch (const SystemException& exc) {
		if (exc.ErrorCodeRef() == ::cppdevtk::base::errc::resource_deadlock_would_occur) {
			caught = true;
		}
		else {
			CPPDEVTK_COUT << "errorCheckingMutex.TryLock(): got wrong error code: " << exc.ErrorCodeRef().ToString() << endl;
		}
	}
	catch (const exception& exc) {
		CPPDEVTK_COUT << "errorCheckingMutex.TryLock(): caught: " << Exception::GetDetailedInfo(exc) << endl;
	}
	catch (...) {
		CPPDEVTK_CERR << "errorCheckingMutex.TryLock(): caught unknown exception" << endl;
	}
	if (!caught) {
		CPPDEVTK_CERR << "errorCheckingMutex.TryLock(): !caught" << endl;
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
			CPPDEVTK_COUT << "errorCheckingTimedMutex.Lock(): got wrong error code: " << exc.ErrorCodeRef().ToString() << endl;
		}
	}
	catch (const exception& exc) {
		CPPDEVTK_COUT << "errorCheckingTimedMutex.Lock(): caught: " << Exception::GetDetailedInfo(exc) << endl;
	}
	catch (...) {
		CPPDEVTK_CERR << "errorCheckingTimedMutex.Lock(): caught unknown exception" << endl;
	}
	if (!caught) {
		CPPDEVTK_CERR << "errorCheckingTimedMutex.Lock(): !caught" << endl;
		return false;
	}
	errorCheckingTimedMutex.Unlock();
	
	errorCheckingTimedMutex.Lock();
	caught = false;
	try {
		if (errorCheckingTimedMutex.TryLock()) {
			CPPDEVTK_CERR << "errorCheckingTimedMutex.TryLock() succeeded" << endl;
			return false;
		}
		caught = true;
	}
	catch (const SystemException& exc) {
		if (exc.ErrorCodeRef() == ::cppdevtk::base::errc::resource_deadlock_would_occur) {
			caught = true;
		}
		else {
			CPPDEVTK_COUT << "errorCheckingTimedMutex.TryLock(): got wrong error code: " << exc.ErrorCodeRef().ToString() << endl;
		}
	}
	catch (const exception& exc) {
		CPPDEVTK_COUT << "errorCheckingTimedMutex.TryLock(): caught: " << Exception::GetDetailedInfo(exc) << endl;
	}
	catch (...) {
		CPPDEVTK_CERR << "errorCheckingTimedMutex.TryLock(): caught unknown exception" << endl;
	}
	if (!caught) {
		CPPDEVTK_CERR << "errorCheckingTimedMutex.TryLock(): !caught" << endl;
		return false;
	}
	errorCheckingTimedMutex.Unlock();
#endif
	
	
	DummyDefaultObjectLevelBasicLockable dummyDefaultObjectLevelBasicLockable;
	::cppdevtk::base::DefaultObjectLevelBasicLockableLockGuard defaultObjectLevelBasicLockableLockGuard(
			dummyDefaultObjectLevelBasicLockable);
	
	DummyClassLevelBasicLockable dummylassLevelBasicLockable;
	::cppdevtk::base::ClassLevelBasicLockableLockGuard classLevelBasicLockableLockGuard(dummylassLevelBasicLockable);
	
	SuppressUnusedWarning(caught);
	
	return true;
}

#if (CPPDEVTK_HAVE_THREAD_STORAGE)

bool TestThread() {
	using ::cppdevtk::base::DefaultLockGuard;
	
	
	CPPDEVTK_COUT << "hardwareConcurrency: " << Thread::GetHardwareConcurrency() << endl;
	
	int retCode = EXIT_FAILURE;
	Thread delegationThread(&ThreadMainFunctionHelloWorld);
	delegationThread.Start();
	delegationThread.Join(retCode);
	
	HelloWorldThread helloWorldThread;
	helloWorldThread.Start();
	helloWorldThread.Join(retCode);
	
	
#	if (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	QTime time;
	time.start();
	
	InterruptibleSleepHelloWorldThread interruptibleSleepHelloWorldThread;
	interruptibleSleepHelloWorldThread.Start();
	{
		DefaultLockGuard lockGuard(gQCOutMtx);
		CPPDEVTK_COUT << "letting child interruptibleSleepHelloWorldThread to work a little..." << endl;
	}
	::cppdevtk::base::this_thread::SleepFor(2500);
	{
		DefaultLockGuard lockGuard(gQCOutMtx);
		CPPDEVTK_COUT << "done waiting for child interruptibleSleepHelloWorldThread" << endl;
	}
	interruptibleSleepHelloWorldThread.RequestInterruption();
	bool caught = false;
	try {
		interruptibleSleepHelloWorldThread.Join(retCode);
	}
	catch (const ::cppdevtk::base::ThreadInterruptedException& exc) {
		caught = true;
		CPPDEVTK_COUT << "interruptibleSleepHelloWorldThread interrupted: " << exc.What() << endl;
	}
	if (!caught) {
		CPPDEVTK_CERR << "failed to catch thread interrupted for interruptibleSleepHelloWorldThread" << endl;
		return false;
	}
	
	CPPDEVTK_COUT << "interruptibleSleepHelloWorldThread took: ";
	PrintTime(time.elapsed());
	
	
	time.restart();
	
	InterruptibleCondVarHelloWorldThread interruptibleCondVarHelloWorldThread;
	interruptibleCondVarHelloWorldThread.Start();
	{
		DefaultLockGuard lockGuard(gQCOutMtx);
		CPPDEVTK_COUT << "letting child interruptibleCondVarHelloWorldThread to work a little..." << endl;
	}
	::cppdevtk::base::this_thread::SleepFor(2500);
	{
		DefaultLockGuard lockGuard(gQCOutMtx);
		CPPDEVTK_COUT << "done waiting for child interruptibleCondVarHelloWorldThread" << endl;
	}
	interruptibleCondVarHelloWorldThread.RequestInterruption();
	caught = false;
	try {
		interruptibleCondVarHelloWorldThread.Join(retCode);
	}
	catch (const ::cppdevtk::base::ThreadInterruptedException& exc) {
		caught = true;
		CPPDEVTK_COUT << "interruptibleCondVarHelloWorldThread interrupted: " << exc.What() << endl;
	}
	if (!caught) {
		CPPDEVTK_CERR << "failed to catch thread interrupted for interruptibleCondVarHelloWorldThread" << endl;
		return false;
	}
	
	CPPDEVTK_COUT << "interruptibleCondVarHelloWorldThread took: ";
	PrintTime(time.elapsed());
	
#	endif	// (CPPDEVTK_ENABLE_THREAD_INTERRUPTION)
	
	return true;
}

#endif	// (CPPDEVTK_HAVE_THREAD_STORAGE)


void TestStackTraceCppHelper1(int dummy) {
	TestStackTraceCppHelper2(dummy);
}

void TestStackTraceCppHelper2(int) {
	::cppdevtk::base::StackTrace stackTrace;
	CPPDEVTK_COUT << "StackTrace from C++ function:\n" << stackTrace.ToString() << endl;
}

void TestStackTraceCHelper1(int dummy) {
	TestStackTraceCHelper2(dummy);
}

void TestStackTraceCHelper2(int) {
	::cppdevtk::base::StackTrace stackTrace;
	CPPDEVTK_COUT << "StackTrace from C function:\n" << stackTrace.ToString() << endl;
}


void PrintTime(int milliseconds) {
	int seconds = milliseconds / 1000;
	milliseconds %= 1000;
	int minutes = seconds / 60;
	seconds %= 60;
	int hours = minutes / 60;
	minutes %= 60;
	CPPDEVTK_COUT << QTime(hours, minutes, seconds, milliseconds).toString("hh:mm:ss.zzz") << endl;
}
