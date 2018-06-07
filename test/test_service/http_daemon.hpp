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


#ifndef CPPDEVTK_TEST_SERVICE_HTTP_DAEMON_HPP_INCLUDED_
#define CPPDEVTK_TEST_SERVICE_HTTP_DAEMON_HPP_INCLUDED_


#include "config.hpp"

#include <QtNetwork/QTcpServer>
#include <QtCore/QObject>
#include <QtCore/QtGlobal>

#include <cstddef>


namespace cppdevtk {
namespace test_service {


// HttpDaemon is the the class that implements the simple HTTP server.
class HttpDaemon: public QTcpServer {
    Q_OBJECT
public:
    HttpDaemon(quint16 port, QObject* parent = NULL);
	
#	if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    virtual void incomingConnection(qintptr socketDescriptor);
#	else
	virtual void incomingConnection(int socketDescriptor);
#	endif
    void pause();
    void resume();
private Q_SLOTS:
    void readClient();
    void discardClient();
private:
	Q_DISABLE_COPY(HttpDaemon)
	
	
    bool disabled_;
};


}	// namespace test_service
}	// namespace cppdevtk


#endif	// CPPDEVTK_TEST_SERVICE_HTTP_DAEMON_HPP_INCLUDED_
