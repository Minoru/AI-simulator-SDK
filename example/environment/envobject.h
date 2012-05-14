#ifndef ENVOBJECT_H
#define ENVOBJECT_H

#include <QColor>
#include "math.h"
#include "../../constants.h"
#include "../../NetworkingManager.h"

class EnvObject
{
public:
    EnvObject();

private:
    unsigned int objectId;              //as one controller controls all envObjects,
                                        //each of them must has own id
    static unsigned int portNumber;     //controller's port number
    unsigned int velocity;              //object's velocity
    QColor color;                        //object's color
    unsigned int size;                  //diameter in special pixels (1/60 of real pixel)
    double orientation;                 //orientation (in degrees)
    Intersection intersection;          //type of intersection
    bool movable;                       //is object movable
    std::pair<int, int> coords;         //object's coordinates, first - x, second - y;

    static NetworkingManager *network;
    ModellingState state;

public:
    void move(int x, int y);

public:

    unsigned int getObjectId()
    {
        return objectId;
    }

    bool isMovable()
    {
        return movable;
    }

    unsigned int getPortNumber()
    {
        return portNumber;
    }

    unsigned int getVelocity()
    {
        return velocity;
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

    // Setters

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

    void setIntersection(Intersection intersection)
    {
        this->intersection = intersection;
    }

    void setMovable(bool movable)
    {
        this->movable = movable;
    }

    void setObjectId(unsigned int id)
    {
        objectId = id;
    }

    void setVelocity(unsigned int velocity)
    {
        this->velocity = velocity;
    }

    static void setPortNumber(unsigned int port)
    {
        if (portNumber == 0) {
            portNumber = port;

            if (network != NULL)
                delete network;
            network = new NetworkingManager(port);
        }
    }
};

#endif // ENVOBJECT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
