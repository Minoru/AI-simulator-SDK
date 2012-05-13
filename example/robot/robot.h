#ifndef ROBOT_H
#define ROBOT_H

#include <QColor>
#include <QUdpSocket>
#include <QVariant>
#include <math.h>
#include "../../NetworkingManager.h"
#include "../../constants.h"

class Robot
{
public:
    Robot();

private:
    unsigned int portNumber;                    //also it's robot's id
    QColor color;                               //object's color
    unsigned int size;                          //diameter in special pixels (1/60 of real pixel)
    double orientation;                         //orientation (in degrees)
    Intersection intersection;                  //type of intersection
    std::pair<int, int> coords;                 //robot coordinates, first - x, second - y;
    std::pair<std::string, double> *parameters; //custom robot parameters
    NetworkingManager *network;
    ModellingState state;

    void checkForStateChanges();
    Message* waitForMessage();

public:
    bool move(int x, int y);
    void turn(double degrees);
    void changeDiameter(unsigned int diameter);
    void changeColor(int red, int green, int blue);
    std::vector<MessageObject> whoIsThere(unsigned int x, unsigned int y, unsigned int radius);
private:
    void reportParameter(char id, int integral, unsigned int real);
public:
    bool isStart();
    bool isPause();

public:
    // getters

    unsigned int getPortNumber()
    {
        return portNumber;
    }

    QColor getColor()
    {
        return color;
    }

    unsigned int getSize()
    {
        return size;
    }

    double getOrientation()
    {
        return orientation;
    }

    Intersection getIntersection()
    {
        return intersection;
    }

    std::pair<int, int> getCoords()
    {
        return coords;
    }

    double getParameter(unsigned int parameter)
    {
        if (parameter < CUSTOM_PARAMETERS_QUANTITY)
            return parameters[parameter].second;
        else
            return 0;
    }

    ModellingState getState()
    {
        checkForStateChanges();
        return state;
    }

    // setters

    void setPortNumber(unsigned int port)
    {
        if (portNumber == 0) {
            portNumber = port;

            if (network != NULL)
                delete network;
            network = new NetworkingManager(port);
        }
    }

    void setIntersection(Intersection intersection)
    {
        this->intersection = intersection;
    }

    void setParameter(unsigned int parameter, double value)
    {
        if (parameter < CUSTOM_PARAMETERS_QUANTITY)
            parameters[parameter] =
                    std::pair<std::string, double>(parameters[parameter].first, value);

        // accuracy = 5
        reportParameter(static_cast<char>(parameter), static_cast<int>(value),
                        static_cast<int>((value - static_cast<int>(value)) * 100000) % 100000);
    }
};

#endif // ROBOT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
