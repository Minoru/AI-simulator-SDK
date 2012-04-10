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
    // TODO: implement that method

    // Ensure port is correct
    msg->port = port;
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
