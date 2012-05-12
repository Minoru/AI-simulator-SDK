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

/* Returns true if we bumped into something */
bool Robot::move(int x, int y)
{
    MessageMove m;
    m.coordX = x;
    m.coordY = y;
    network->send(&m);

    coords = std::pair<int, int>(x, y);

    Message *msg = NULL;
    MessageType type = waitForMessage(msg);
    if (type == MsgBump) {
        MessageBump *m = static_cast<MessageBump *>(msg);
        coords = std::pair<int, int>(m->coordX, m->coordY);
        return true;
    } else {
        return false;
    }
}

void Robot::turn(double degrees)
{
    MessageTurn m;
    m.degrees = degrees;
    network->send(&m);

    orientation = degrees;
}

void Robot::changeDiameter(unsigned int diameter)
{
    MessageChangeSize m;
    m.diameter = diameter;
    network->send(&m);

    size = diameter;
}

void Robot::changeColor(char red, char green, char blue)
{
    MessageChangeColor m;
    m.red = red;
    m.green = green;
    m.blue = blue;
    network->send(&m);

    color = QColor(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));
}

std::vector<MessageObject> Robot::whoIsThere(unsigned int x, unsigned int y, unsigned int radius)
{
    MessageWhoIsThere m;
    m.coordX = x;
    m.coordY = y;
    m.radius = radius;
    network->send(&m);

    Message *msg = NULL;
    std::vector<MessageObject> results;
    MessageType type = waitForMessage(msg);
    if (type == MsgThereYouSee) {
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

bool Robot::isStop()
{
    checkForStateChanges();
    return (state == Stopped);
}

void Robot::checkForStateChanges()
{
    Message *msg = NULL;
    MessageType type = network->receive(msg);
    if(type == MsgStart) {
        state = Started;
    } else if(type == MsgStop) {
        state = Stopped;
    } else if(type == MsgPause) {
        state = Paused;
    }
}

MessageType Robot::waitForMessage(Message *msg)
{
    Message *m = NULL;
    MessageType type;
    while(network->waitForReadyRead(ROBOT_TIMEOUT)) {
        type = network->receive(m);
        if(type == MsgStart) {
            state = Started;
            delete m;
        } else if (type == MsgPause) {
            state = Paused;
            delete m;
        } else if (type == MsgStop) {
            state = Stopped;
            delete m;
        } else {
            msg = m;
            return type;
        }
    }
    // dead code needed to satisfy compiler
    msg = NULL;
    return MsgUndefined;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
