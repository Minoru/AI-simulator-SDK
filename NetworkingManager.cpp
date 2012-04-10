#include "NetworkingManager.h"

MessageThereYouSee *parseMessageThereYouSee(QDataStream &stream)
{
    MessageThereYouSee *msg = new MessageThereYouSee();
    quint32 count;
    stream >> count;
    msg->objects.reserve(count);
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

        msg->objects.push_back(obj);
    }
    return msg;
}

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
    if(! socket->hasPendingDatagrams()) {
        msg = NULL;
        return MsgUndefined;
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
    if(version != 1) {
        // FIXME: stick qDebug in here
        // VERSION MISMATCH, CAN'T HANDLE THAT
        return MsgUndefined;
    }

    quint32 seq_num;
    quint16 port;
    quint8 msg_type;
    stream >> seq_num >> port >> msg_type;

    switch(static_cast<MessageType>(msg_type)) {
    case MsgBump:
        msg = new MessageBump();
        quint32 x, y;
        stream >> x >> y;
        (static_cast<MessageBump *>(msg))->coordX = x;
        (static_cast<MessageBump *>(msg))->coordY = y;
        break;

    case MsgThereYouSee:
        msg = parseMessageThereYouSee(stream);
        break;

    default:
        // FIXME: stick qDebug in here
        // That message type is unhandled
        break;
    }

    msg->num = seq_num;
    msg->port = port;
    msg->type = static_cast<MessageType>(msg_type);

    return msg->type;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
