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
    // calculate new orientation
    double angle = acos(fabs(y - coords.second)
                        / sqrt(  pow(x - coords.first, 2)
                               + pow(y - coords.second, 2)));
    angle *= 180 / PI;

    if (y > coords.second && x > coords.first)
        angle = 180 - angle;
    else if (y > coords.second && x <= coords.first)
        angle = 180 + angle;
    else if (y <= coords.second && x <= coords.first)
        angle = 360 - angle;

    if(orientation != angle) {
        MessageTurn mTurn;
        mTurn.envObjID = objectId;
        mTurn.degrees = angle;
        network->send(&mTurn);
        orientation = angle;
    }

    MessageMove m;
    m.envObjID = objectId;
    m.coordX = x;
    m.coordY = y;
    network->send(&m);

    //TODO: implement bump
    coords = std::pair<int, int>(x, y);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
