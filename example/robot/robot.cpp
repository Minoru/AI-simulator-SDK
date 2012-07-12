#include "robot.h"

Robot::Robot():
    portNumber(0),
    color(255, 255, 255),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0),
    state(Started)
{
    //default constructor
    parameters = new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        char symbol = static_cast<char>(65 + i);
        parameters[i] = std::pair<std::string, double>(&symbol, 0);
    }
    network = NULL;
}

/* Returns true if move was sucessful, false if we bumped into something */
bool Robot::move(int x, int y)
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
        mTurn.envObjID = 0;
        mTurn.degrees = angle;
        network->send(&mTurn);
        orientation = angle;
    }

    MessageMove m;
    m.envObjID = 0;
    m.coordX = x;
    m.coordY = y;
    network->send(&m);

    Message *msg = NULL;
    msg = waitForMessage();
    if (msg && msg->type == MsgBump) {
        MessageBump *m = static_cast<MessageBump *>(msg);
        coords = std::pair<int, int>(m->coordX, m->coordY);
        return false;
    } else {
        delete msg;
        coords = std::pair<int, int>(x, y);
        return true;
    }
}

void Robot::turn(double degrees)
{
    MessageTurn m;
    m.envObjID = 0;
    m.degrees = degrees;
    network->send(&m);

    orientation = degrees;
}

void Robot::changeDiameter(unsigned int diameter)
{
    MessageChangeSize m;
    m.envObjID = 0;
    m.diameter = diameter;
    network->send(&m);

    size = diameter;
}

void Robot::changeColor(char red, char green, char blue)
{
    MessageChangeColor m;
    m.envObjID = 0;
    m.red = red;
    m.green = green;
    m.blue = blue;
    network->send(&m);

    color = QColor(red, green, blue);
}

std::vector<MessageObject> Robot::whoIsThere(unsigned int x, unsigned int y, unsigned int radius)
{
    MessageWhoIsThere m;
    m.envObjID = 0;
    m.coordX = x;
    m.coordY = y;
    m.radius = radius;
    network->send(&m);

    Message *msg = NULL;
    std::vector<MessageObject> results;
    msg = waitForMessage();
    if (msg && msg->type == MsgThereYouSee) {
        MessageThereYouSee *m = static_cast<MessageThereYouSee *>(msg);
        results.resize(m->objects.size());
        std::vector<MessageObject>::iterator it;
        for(it = m->objects.begin(); it < m->objects.end(); it++)
            results.push_back(*it);
    }
    return results;
}

void Robot::reportParameter(char id, int integral, unsigned int real)
{
    MessageParameterReport m;
    m.envObjID = 0;
    m.id = id;
    m.integral = integral;
    m.real = real;
    network->send(&m);
}

bool Robot::isStart()
{
    checkForStateChanges();
    return (state == Started);
}

bool Robot::isPause()
{
    checkForStateChanges();
    return (state == Paused);
}

void Robot::checkForStateChanges()
{
    Message *msg = network->receive();
    if(msg && msg->type == MsgStart) {
        state = Started;
    } else if(msg && msg->type == MsgPause) {
        state = Paused;
    } else if(msg && msg->type == MsgBump) {
        MessageBump *m = static_cast<MessageBump *>(msg);
        coords = std::pair<int, int>(m->coordX, m->coordY);
    }
    delete msg;
}

Message* Robot::waitForMessage()
{
    Message *m = NULL;
    while(network->waitForReadyRead(ROBOT_TIMEOUT)) {
        m = network->receive();
        if(m && m->type == MsgStart) {
            state = Started;
            delete m;
        } else if(m && m->type == MsgPause) {
            state = Paused;
            delete m;
        } else {
            return m;
        }
    }
    return NULL;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
