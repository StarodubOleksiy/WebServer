/****************************************************************************
**
** Copyright (C) 2014 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "server.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

Server::Server(quint16 port, QObject *parent) :
    QObject(parent),
    webSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                           QWebSocketServer::NonSecureMode, this))//,

{
    if (webSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server listening on port" << port;
        connect(webSocketServer, &QWebSocketServer::newConnection,
                this, Server::creatingNewConnection);
        connect(webSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
}

Server::~Server()
{
   webSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
}

void Server::creatingNewConnection()
{
    QWebSocket *pSocket =webSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::finisihingTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    clients << pSocket;
}

void Server::finisihingTextMessage(QString message)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
     qDebug() << "Text Message received from client: " << message;
    QDateTime currentDateTime =  QDateTime::currentDateTime() ;
      if (client)
      {
      message.insert(0,currentDateTime.toString("yyyy-MM-dd hh:mm:ss")+": ") ;
     client->sendTextMessage(message);
      }
}


void Server::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    //if (m_debug)
        qDebug() << "socketDisconnected:" << client;
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
    }
}
