#include "messages-serializers.h"

void setupHeader(Message *msg, QDataStream &stream)
{
    // version: 1
    stream << (quint8)1;
    // other header infortmation
    stream << (quint32)msg->num << (quint16)msg->port << (quint8)msg->type;
}

QByteArray *serializeMessageMove(MessageMove *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << (quint32)msg->coordX << (quint32)msg->coordY;

    return arr;
}

QByteArray *serializeMessageTurn(MessageTurn *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << (quint32)(msg->degrees * 3600);

    return arr;
}

QByteArray *serializeMessageChangeSize(MessageChangeSize *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << (quint32)msg->diameter;

    return arr;
}

QByteArray *serializeMessageChangeColor(MessageChangeColor *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << (quint8)msg->red << (quint8)msg->green << (quint8)msg->blue;

    return arr;
}

QByteArray *serializeMessageWhoIsThere(MessageWhoIsThere *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << (quint32)msg->coordX << (quint32)msg->coordY << (quint32)msg->radius;

    return arr;
}

QByteArray *serializeMessageParameterReport(MessageParameterReport *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << (quint8)msg->id << (quint32)msg->integral << (quint32)msg->real;

    return arr;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
