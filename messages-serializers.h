#ifndef MESSAGES_SERIALIZERS_H
#define MESSAGES_SERIALIZERS_H

#include <QtCore/QDataStream>

#include "messages.h"

QDataStream &serializeMessageMove(MessageMove *);
QDataStream &serializeMessageTurn(MessageTurn *);
QDataStream &serializeMessageChangeSize(MessageChangeSize *);
QDataStream &serializeMessageChangeColor(MessageChangeColor *);
QDataStream &serializeMessageWhoIsThere(MessageWhoIsThere *);

#endif

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
