#ifndef NETWORKINGMANAGER_H
#define NETWORKINGMANAGER_H

#include <QtCore/QtGlobal>
#include <QtNetwork/QUdpSocket>

#include "constants.h"
#include "messages.h"
#include "messages-serializers.h"

class NetworkingManager
{
public:
    NetworkingManager(quint16);
    ~NetworkingManager();

    void send(Message *);
    bool hasPendingMessages();
    MessageType receive(Message *);
    bool waitForReadyRead(int);

private:
    quint16 port;
    QUdpSocket *socket;
};

#endif

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
