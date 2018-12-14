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


#ifndef CPPDEVTK_UTIL_SERVICE_BASE_HPP_INCLUDED_
#define CPPDEVTK_UTIL_SERVICE_BASE_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_ENABLE_QT_SOLUTIONS)
#error "This file require QtSolutions enabled!!!"
#endif

#include <cppdevtk/QtSolutions/QtService/QtServiceBase>


namespace cppdevtk {
namespace util {


class CPPDEVTK_UTIL_API ServiceBase: public ::cppdevtk::qtsol::QtServiceBase {
	Q_OBJECT
public:
	 ServiceBase(int argc, char** argv, const QString& name);
	 
	 void LogError(const QString& message, const QByteArray& data = QByteArray());
	 void LogWarn(const QString& message, const QByteArray& data = QByteArray());
	 void LogInfo(const QString& message, const QByteArray& data = QByteArray());
	 
	 
	 static ServiceBase* Instance();
protected Q_SLOTS:
	 void StopAndQuit();
private:
	Q_DISABLE_COPY(ServiceBase)
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline ServiceBase::ServiceBase(int argc, char** argv, const QString& name):
		::cppdevtk::qtsol::QtServiceBase(argc, argv, name) {}

inline void ServiceBase::LogError(const QString& message, const QByteArray& data) {
	logMessage(message, Error, 0, 0, data);
}

inline void ServiceBase::LogWarn(const QString& message, const QByteArray& data) {
	logMessage(message, Warning, 0, 0, data);
}

inline void ServiceBase::LogInfo(const QString& message, const QByteArray& data) {
	logMessage(message, Information, 0, 0, data);
}

inline ServiceBase* ServiceBase::Instance() {
	return static_cast<ServiceBase*>(instance());
}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_SERVICE_BASE_HPP_INCLUDED_
