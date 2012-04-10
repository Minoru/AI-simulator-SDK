#ifndef MESSAGES_SERIALIZERS_H
#define MESSAGES_SERIALIZERS_H

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

#include "messages.h"

QByteArray *serializeMessageMove(MessageMove *);
QByteArray *serializeMessageTurn(MessageTurn *);
QByteArray *serializeMessageChangeSize(MessageChangeSize *);
QByteArray *serializeMessageChangeColor(MessageChangeColor *);
QByteArray *serializeMessageWhoIsThere(MessageWhoIsThere *);
QByteArray *serializeMessageParameterReport(MessageParameterReport *);

#endif

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
