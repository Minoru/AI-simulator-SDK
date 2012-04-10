#include "NetworkingManager.h"

NetworkingManager::NetworkingManager(quint16 port)
{
    socket = new QUdpSocket();
    this->port = port;
}

NetworkingManager::~NetworkingManager()
{
    socket->~QUdpSocket();
}

void NetworkingManager::send(Message *msg)
{
    // Ensure port is correct
    msg->port = port;

    QByteArray *datagram = NULL;

    switch(msg->type) {
    case MsgMove:
        datagram = serializeMessageMove(static_cast<MessageMove *>(msg));
        break;

    case MsgTurn:
        datagram = serializeMessageTurn(static_cast<MessageTurn *>(msg));
        break;

    case MsgChangeSize:
        datagram = serializeMessageChangeSize(static_cast<MessageChangeSize *>(msg));
        break;

    case MsgChangeColor:
        datagram = serializeMessageChangeColor(static_cast<MessageChangeColor *>(msg));
        break;

    case MsgWhoIsThere:
        datagram = serializeMessageWhoIsThere(static_cast<MessageWhoIsThere *>(msg));
        break;

    case MsgParameterReport:
        datagram = serializeMessageParameterReport(static_cast<MessageParameterReport *>(msg));
        break;
    default:
        // FIXME: stick qDebug in here
        // That message type is unhandled
        break;
    }

    if(datagram) {
        socket->writeDatagram(datagram->constData(), datagram->size(),
                QHostAddress::LocalHost, port);
    }
}

bool NetworkingManager::hasPendingMessages()
{
    return socket->hasPendingDatagrams();
}

MessageType NetworkingManager::receive(Message *msg)
{
    // TODO: implement that method
    return MsgUndefined;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
