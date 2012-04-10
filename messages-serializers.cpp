#include "messages-serializers.h"

void setupHeader(Message *msg, QDataStream &stream)
{
}

QByteArray *serializeMessageMove(MessageMove *msg)
{
}

QByteArray *serializeMessageTurn(MessageTurn *msg)
{
}

QByteArray *serializeMessageChangeSize(MessageChangeSize *msg)
{
}

QByteArray *serializeMessageChangeColor(MessageChangeColor *msg)
{
}

QByteArray *serializeMessageWhoIsThere(MessageWhoIsThere *msg)
{
}

QByteArray *serializeMessageParameterReport(MessageParameterReport *msg)
{
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
