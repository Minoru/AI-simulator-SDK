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

void Robot::move(int x, int y)
{
    MessageMove m;
    m.coordX = x;
    m.coordY = y;
    network->send(&m);

    coords = std::pair<int, int>(x, y);

    // FIXME: should we wait for MsgBump and return some bool to indicate success or fail?
}

void Robot::turn(double degrees)
{
    MessageTurn m;
    m.degrees = degrees;
    network->send(&m);

    // TODO: update robot's internal state
}

void Robot::changeDiameter(unsigned int diameter)
{
    MessageChangeSize m;
    m.diameter = diameter;
    network->send(&m);

    // TODO: update robot's internal state
}

void Robot::changeColor(char red, char green, char blue)
{
    MessageChangeColor m;
    m.red = red;
    m.green = green;
    m.blue = blue;
    network->send(&m);

    // TODO: update robot's internal state
}

void Robot::whoIsThere(unsigned int x, unsigned int y, unsigned int radius)
{
    MessageWhoIsThere m;
    m.coordX = x;
    m.coordY = y;
    m.radius = radius;
    network->send(&m);

    // TODO: update robot's internal state

    // FIXME: should that thing wait for ThereYouSee message and return array of objects?
}

void Robot::reportParameter(char id, int integral, unsigned int real)
{
    MessageParameterReport m;
    m.id = id;
    m.integral = integral;
    m.real = real;
    network->send(&m);

    // TODO: update robot's internal state
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

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
