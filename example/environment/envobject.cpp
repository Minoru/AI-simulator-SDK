#include "envobject.h"

unsigned int EnvObject::portNumber = 0;
NetworkingManager * EnvObject::network = NULL;

EnvObject::EnvObject():
    velocity(0),
    color(255, 255, 255),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    movable(false),
    coords(0, 0)
{
}

void EnvObject::move(int x, int y)
{
    //TODO: implement
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
