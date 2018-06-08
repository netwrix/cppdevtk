/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Solutions component.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


// NOTE: taken from qt-solutions-master\qtservice\examples\server\main.cpp and adapted for CppDevTk


#include "http_service.hpp"

#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <cstddef>


namespace cppdevtk {
namespace test_service {


HttpService::HttpService(int argc, char** argv): ::cppdevtk::util::Service< ::cppdevtk::util::CoreApplication>(argc, argv,
		"CppDevTk HTTP Test Service"), pDaemon_(NULL) {
	setServiceDescription("A dummy HTTP service implemented with CppDevTk");
	setServiceFlags(ServiceFlags(CanBeSuspended) | ServiceFlags(NeedsStopOnShutdown));
}

void HttpService::start() {
	LogInfo("starting");
	
#if QT_VERSION < 0x040100
	quint16 port = (app->argc() > 1) ?
			QString::fromLocal8Bit(app->argv()[1]).toUShort() : 8080;
#else
	const QStringList arguments = ApplicationType::arguments();
	quint16 port = (arguments.size() > 1) ?
			arguments.at(1).toUShort() : 8080;
#endif
	
	ApplicationType* app = application();
	CPPDEVTK_ASSERT(app != NULL);
	
	CPPDEVTK_ASSERT(pDaemon_ == NULL);
	pDaemon_ = new HttpDaemon(port, app);

	if (!pDaemon_->isListening()) {
		LogError(QString("Failed to bind to port %1").arg(pDaemon_->serverPort()));
		app->quit();
	}
}

void HttpService::stop() {
	LogInfo("stopping");
	CPPDEVTK_ASSERT(pDaemon_ != NULL);
	delete pDaemon_;
	pDaemon_ = NULL;
}

void HttpService::pause() {
	LogInfo("pausing");
	CPPDEVTK_ASSERT(pDaemon_ != NULL);
	pDaemon_->pause();
}

void HttpService::resume() {
	LogInfo("resuming");
	CPPDEVTK_ASSERT(pDaemon_ != NULL);
	pDaemon_->resume();
}

void HttpService::reloadConfig() {
	LogInfo("reloading config");
	CPPDEVTK_ASSERT(pDaemon_ != NULL);
	//pDaemon_->reloadConfig();
}


}	// namespace test_service
}	// namespace cppdevtk
