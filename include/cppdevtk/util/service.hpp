/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>\n
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


#ifndef CPPDEVTK_UTIL_SERVICE_HPP_INCLUDED_
#define CPPDEVTK_UTIL_SERVICE_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_ENABLE_QT_SOLUTIONS)
#error "This file require QtSolutions enabled!!!"
#endif

#include "service_base.hpp"
#include "core_application.hpp"

#include <cppdevtk/base/static_assert.hpp>
#if (CPPDEVTK_PLATFORM_UNIX)
#include <cppdevtk/base/posix_signals_watcher_unx.hpp>
#include <cppdevtk/base/verify.h>
#endif

#include <cstddef>
#include <new>
#include CPPDEVTK_TR1_HEADER(type_traits)


namespace cppdevtk {
namespace util {


template <class TApplication>
class Service: public ServiceBase {
public:
	typedef TApplication ApplicationType;
	
	
    Service(int argc, char** argv, const QString& name);
protected:
    TApplication* application() const;
    virtual void createApplication(int& argc, char** argv);
    virtual int executeApplication();
private:
	Q_DISABLE_COPY(Service)
	
	
    TApplication* pApplication_;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

template <class TApplication>
inline Service<TApplication>::Service(int argc, char** argv, const QString& name): ServiceBase(argc, argv, name),
		pApplication_(NULL) {}

template <class TApplication>
inline TApplication* Service<TApplication>::application() const {
	return pApplication_;
}

template <class TApplication>
void Service<TApplication>::createApplication(int& argc, char** argv) {
	CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_TR1_NS::is_base_of<CoreApplication, TApplication>::value),
			"CoreApplication must be a base class of TApplication");
	
	pApplication_ = new TApplication(argc, argv);
	
#	if (CPPDEVTK_PLATFORM_UNIX)
	
	const ServiceFlags kServiceFlags = serviceFlags();
	::cppdevtk::base::PosixSignalsWatcher& thePosixSignalsWatcher = ::cppdevtk::base::PosixSignalsWatcher::GetInstance();
	
	if (!(kServiceFlags & CannotBeStopped)) {
		CPPDEVTK_VERIFY(connect(&thePosixSignalsWatcher, SIGNAL(SigTerm()), SLOT(StopAndQuit())));
		CPPDEVTK_VERIFY(thePosixSignalsWatcher.Watch(SIGTERM));
		
		CPPDEVTK_VERIFY(connect(&thePosixSignalsWatcher, SIGNAL(SigInt()), SLOT(StopAndQuit())));
		CPPDEVTK_VERIFY(thePosixSignalsWatcher.Watch(SIGINT));
	}
	
	if (kServiceFlags & CanBeSuspended) {
		CPPDEVTK_VERIFY(connect(&thePosixSignalsWatcher, SIGNAL(SigHUp()), SLOT(reloadConfig())));
		CPPDEVTK_VERIFY(thePosixSignalsWatcher.Watch(SIGHUP));
	}
	
#	endif	// (CPPDEVTK_PLATFORM_UNIX)
}

template <class TApplication>
inline int Service<TApplication>::executeApplication() {
	return TApplication::exec();
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Templates explicit instantiation declaration.

#ifndef CPPDEVTK_UTIL_SERVICE_CPP

CPPDEVTK_UTIL_TMPL_EXPL_INST template class CPPDEVTK_UTIL_API Service<CoreApplication>;

#endif


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_SERVICE_HPP_INCLUDED_
