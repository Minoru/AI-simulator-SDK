#ifndef ROBOT_H
#define ROBOT_H

#include <QColor>
#include <QUdpSocket>
#include <QVariant>
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

public:
    void move(int x, int y);
    void turn(double degrees);
    void changeDiameter(unsigned int diameter);
    void changeColor(char red, char green, char blue);
    void whoIsThere(unsigned int x, unsigned int y, unsigned int radius);
    void reportParameter(char id, int integral, unsigned int real);
    bool isStart();
    bool isPause();
    bool isStop();

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

    std::pair<std::string, double> * getParameters()
    {
        return parameters;
    }

    std::pair<std::string, double> getParameter(unsigned int parameter)
    {
        if (parameter < CUSTOM_PARAMETERS_QUANTITY)
            return parameters[parameter];
        else
            return std::pair<std::string, double>();
    }

    NetworkingManager * getNetwork()
    {
        return network;
    }

    // setters

    void setSize(int size)
    {
        this->size = size;
    }

    void setOrientation(double orientation)
    {
        if (orientation >= 360 || orientation < 0)
            this->orientation = 0;
        else
            this->orientation = orientation;
    }

    void setColor(QColor color)
    {
        this->color = color;
    }

    void setCoords(int x, int y)
    {
        coords = std::pair<int, int>(x, y);
    }

    void setPortNumber(unsigned int port)
    {
        portNumber = port;

        if (network != NULL)
           delete network;
        network = new NetworkingManager(port);
    }

    void setIntersection(Intersection intersection)
    {
        this->intersection = intersection;
    }

    void setParameters(std::pair<std::string, double> *params)
    {
        for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
            parameters[i] = params[i];
        }
    }

    void setParameter(unsigned int parameter, double value)
    {
        if (parameter < CUSTOM_PARAMETERS_QUANTITY)
            parameters[parameter] =
                    std::pair<std::string, double>(parameters[parameter].first, value);
    }
};

#endif // ROBOT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
