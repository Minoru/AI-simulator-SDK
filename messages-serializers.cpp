#include "messages-serializers.h"

void setupHeader(Message *msg, QDataStream &stream)
{
    // version: 1
    stream << static_cast<quint8>(1);
    // other header infortmation
    stream << static_cast<quint32>(msg->num) << static_cast<quint16>(msg->port)
           << static_cast<quint8>(msg->type);
}

QByteArray *serializeMessageMove(MessageMove *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << static_cast<quint32>(msg->coordX) << static_cast<quint32>(msg->coordY);

    return arr;
}

QByteArray *serializeMessageTurn(MessageTurn *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << static_cast<quint32>(msg->degrees * 3600);

    return arr;
}

QByteArray *serializeMessageChangeSize(MessageChangeSize *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << static_cast<quint32>(msg->diameter);

    return arr;
}

QByteArray *serializeMessageChangeColor(MessageChangeColor *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << static_cast<quint8>(msg->red) << static_cast<quint8>(msg->green)
           << static_cast<quint8>(msg->blue);

    return arr;
}

QByteArray *serializeMessageWhoIsThere(MessageWhoIsThere *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << static_cast<quint32>(msg->coordX) << static_cast<quint32>(msg->coordY)
           << static_cast<quint32>(msg->radius);

    return arr;
}

QByteArray *serializeMessageParameterReport(MessageParameterReport *msg)
{
    QByteArray *arr = new QByteArray();
    QDataStream stream(arr, QIODevice::WriteOnly);

    setupHeader(msg, stream);
    stream << static_cast<quint8>(msg->id) << static_cast<quint32>(msg->integral)
           << static_cast<quint32>(msg->real);

    return arr;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
