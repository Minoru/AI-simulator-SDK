#ifndef NETWORKINGMANAGER_H
#define NETWORKINGMANAGER_H

#include <QtCore/QtGlobal>
#include <QtNetwork/QUdpSocket>

#include "messages.h"
#include "messages-serializers.h"

class NetworkingManager
{
public:
    static NetworkingManager& getInstance(quint16 port)
    {
        static NetworkingManager instance(port);
        return instance;
    }

    void send(Message *);
    bool hasPendingMessages();
    MessageType receive(Message *);

private:
    quint16 port;
    QUdpSocket *socket;

    NetworkingManager(quint16);
    NetworkingManager(NetworkingManager const&);
    ~NetworkingManager();

    void operator=(NetworkingManager const&);
};

#endif

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
