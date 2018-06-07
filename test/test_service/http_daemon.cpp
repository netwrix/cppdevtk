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


#include "http_daemon.hpp"
#include <cppdevtk/util/service_base.hpp>

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>


namespace cppdevtk {
namespace test_service {


using ::cppdevtk::util::ServiceBase;


HttpDaemon::HttpDaemon(quint16 port, QObject* parent): QTcpServer(parent), disabled_(false) {
	listen(QHostAddress::Any, port);
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void HttpDaemon::incomingConnection(qintptr socketDescriptor) {
#else
void HttpDaemon::incomingConnection(int socketDescriptor) {
#endif
	if (disabled_) {
		ServiceBase::Instance()->LogInfo("incomingConnection: HttpDaemon disabled");
		return;
	}

	// When a new client connects, the server constructs a QTcpSocket and all
	// communication with the client is done over this QTcpSocket. QTcpSocket
	// works asynchronously, this means that all the communication is done
	// in the two slots readClient() and discardClient().
	QTcpSocket* s = new QTcpSocket(this);
	connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
	s->setSocketDescriptor(socketDescriptor);

	ServiceBase::Instance()->LogInfo("New Connection");
}

void HttpDaemon::pause() {
	disabled_ = true;
}

void HttpDaemon::resume() {
	disabled_ = false;
}

void HttpDaemon::readClient() {
	if (disabled_) {
		ServiceBase::Instance()->LogInfo("readClient: HttpDaemon disabled");
		return;
	}

	// This slot is called when the client sent data to the server. The
	// server looks if it was a get request and sends a very simple HTML
	// document back.
	QTcpSocket* socket = (QTcpSocket*)sender();
	if (socket->canReadLine()) {
		QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
		if (tokens[0] == "GET") {
			QTextStream os(socket);
			os.setAutoDetectUnicode(true);
			os << "HTTP/1.0 200 Ok\r\n"
				"Content-Type: text/html; charset=\"utf-8\"\r\n"
				"\r\n"
				"<h1>Hello world from CppDevTk test HttpDaemon!</h1>\n"
				<< QDateTime::currentDateTime().toString() << "\n";
			socket->close();

			ServiceBase::Instance()->LogInfo("Wrote to client");

			if (socket->state() == QTcpSocket::UnconnectedState) {
				delete socket;
				ServiceBase::Instance()->LogInfo("Connection closed");
			}
		}
	}
}

void HttpDaemon::discardClient() {
	QTcpSocket* socket = (QTcpSocket*)sender();
	socket->deleteLater();

	ServiceBase::Instance()->LogInfo("Connection closed");
}


}	// namespace test_service
}	// namespace cppdevtk
