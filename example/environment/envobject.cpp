#include "envobject.h"

unsigned int EnvObject::portNumber = 0;
NetworkingManager * EnvObject::network = NULL;
ModellingState EnvObject::state = Started;

EnvObject::EnvObject(unsigned int id, bool isMovable,
                     Intersection intersectionType, unsigned int speed):
    objectId(id),
    velocity(speed),
    color(255, 255, 255),
    size(0),
    orientation(0),
    intersection(intersectionType),
    movable(isMovable),
    coords(0, 0)
{
}

bool EnvObject::move(int x, int y)
{
    if (!movable)
        return false;

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
        mTurn.envObjID = objectId + 1;
        mTurn.degrees = angle;
        network->send(&mTurn);
        orientation = angle;
    }

    MessageMove m;
    m.envObjID = objectId + 1;
    m.coordX = x;
    m.coordY = y;
    network->send(&m);

    Message *msg = NULL;
    msg = waitForMessage();

    if (msg && msg->type == MsgBump && msg->envObjID == objectId + 1) {
        MessageBump *m = static_cast<MessageBump *>(msg);
        coords = std::pair<int, int>(m->coordX, m->coordY);
        return true;

    //TODO: apply bump message to corresponding envObject when msg->envObjID != objectId + 1
    } else {
        if(msg) delete msg;
        coords = std::pair<int, int>(x, y);
        return false;
    }
}

void EnvObject::turn(double degrees)
{
    MessageTurn m;
    m.envObjID = objectId + 1;
    m.degrees = degrees;
    network->send(&m);

    orientation = degrees;
}

void EnvObject::changeDiameter(unsigned int diameter)
{
    MessageChangeSize m;
    m.envObjID = objectId + 1;
    m.diameter = diameter;
    network->send(&m);

    size = diameter;
}

void EnvObject::changeColor(int red, int green, int blue)
{
    MessageChangeColor m;
    m.envObjID = objectId + 1;
    m.red = static_cast<char>(red);
    m.green = static_cast<char>(green);
    m.blue = static_cast<char>(blue);
    network->send(&m);

    color = QColor(red, green, blue);
}

Message* EnvObject::waitForMessage()
{
    Message *m = NULL;
    while(network->waitForReadyRead(ROBOT_TIMEOUT)) {
        m = network->receive();
        if(m && m->type == MsgStart) {
            state = Started;
            delete m;
        } else if (m && m->type == MsgPause) {
            state = Paused;
            delete m;
        } else {
            return m;
        }
    }
    return m;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
