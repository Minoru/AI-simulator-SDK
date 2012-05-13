#include "NetworkingManager.h"

NetworkingManager::NetworkingManager(quint16 port)
{
    socket = new QUdpSocket();
    socket->bind(QHostAddress::LocalHost, port);
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
                              QHostAddress::LocalHost, SIMULATOR_PORT);
    }
    
    delete datagram;
}

bool NetworkingManager::hasPendingMessages()
{
    return socket->hasPendingDatagrams();
}

Message* NetworkingManager::receive()
{
    if(! socket->hasPendingDatagrams()) {
        Message* m = new Message();
        m->num = 0;
        m->port = port;
        m->type = MsgUndefined;
        return m;
    }

    
    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(datagram.data(), datagram.size(),
                         &sender, &senderPort);

    QDataStream stream(&datagram, QIODevice::ReadOnly);

    quint8 version;
    stream >> version;
    if(version != 2) {
        // FIXME: stick qDebug in here
        // VERSION MISMATCH, CAN'T HANDLE THAT

        return NULL;
    }

    quint32 seq_num;
    quint8 envObjID;
    quint16 port;
    quint8 msg_type;
    stream >> seq_num >> envObjID >> port >> msg_type;

    switch(static_cast<MessageType>(msg_type)) {
    case MsgBump:
    {
        MessageBump *m = new MessageBump();

        quint32 x, y;
        stream >> x >> y;
        m->coordX = x;
        m->coordY = y;
        m->num = seq_num;
        m->port = port;
        m->type = static_cast<MessageType>(msg_type);
        return m;
    };
        break;

    case MsgThereYouSee:
    {
        MessageThereYouSee *m = new MessageThereYouSee();;

        quint32 count;
        stream >> count;
        m->objects.reserve(count);
        MessageObject obj;
        quint32 x, y, diameter, seconds;
        quint8 red, green, blue;
        for(unsigned int i = 0; i < count; i++) {
            stream >> x >> y >> diameter >> seconds;
            stream >> red >> green >> blue;

            obj.coordX = x;
            obj.coordY = y;
            obj.diameter = diameter;
            obj.degrees = static_cast<double>(seconds) / 3600;
            obj.red = red;
            obj.green = green;
            obj.blue = blue;

            m->objects.push_back(obj);
        }

        m->num = seq_num;
        m->port = port;
        m->type = static_cast<MessageType>(msg_type);
        return m;
    };
        break;

    case MsgStart:
    {
        Message* m = new Message();
        m->num = seq_num;
        m->port = port;
        m->type = static_cast<MessageType>(msg_type);
        return m;
    };
        break;
    case MsgPause:
    {
        Message* m = new Message();
        m->num = seq_num;
        m->port = port;
        m->type = static_cast<MessageType>(msg_type);
        return m;
    };
        break;

    default:
        // FIXME: stick qDebug in here
        // That message type is unhandled
        break;
    }

    Message* m = new Message();
    m->num = seq_num;
    m->port = port;
    m->type = static_cast<MessageType>(msg_type);

    return m;
}

bool NetworkingManager::waitForReadyRead(int msecs)
{
    return socket->waitForReadyRead(msecs);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
