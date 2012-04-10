#ifndef MESSAGES_H
#define MESSAGES_H

/* This header is basically messages.h from the AI-simulator project, but direct access to members
 * is forbidden to enforce sanity checking
 */

#include <vector>

enum MessageType {
    MsgAcknowledge = 0,
    MsgMove,
    MsgTurn,
    MsgChangeSize,
    MsgChangeColor,
    MsgWhoIsThere,
    MsgBump,
    MsgThereYouSee,
    MsgParameterReport,
    MsgUndefined
};

class Message
{
public:
    int getNum();
    void setNum(int);

    int getPort();
    void setPort(int);

    MessageType getMessageType();
    void setMessageType(MessageType);

protected:
    MessageType type;

private:
    int num, port;
};

class MessageMove : public Message
{
public:
    MessageMove() { type = MsgMove; };

    unsigned int getCoordX();
    void setCoordX(unsigned int);

    unsigned int getCoordY();
    void setCoordY(unsigned int);

private:
    unsigned int coordX, coordY;
};

class MessageBump : public Message
{
public:
    MessageBump() { type = MsgBump; };

    unsigned int getCoordX();
    unsigned int getCoordY();

private:
    unsigned int coordX, coordY;
};

class MessageTurn : public Message
{
public:
    MessageTurn() { type = MsgTurn; };

    double getDegrees();
    void setDegrees(double);

private:
    double degrees; 
};

class MessageChangeSize : public Message
{
public:
    MessageChangeSize() { type = MsgChangeSize; };

    unsigned int getDiameter();
    void setDiameter(unsigned int);

private:
    unsigned int diameter; 
};

class MessageChangeColor : public Message
{
public:
    MessageChangeColor() { type = MsgChangeColor; };

    char getRed();
    void setRed(char);

    char getGreen();
    void setGreen(char);

    char getBlue();
    void setBlue(char);

private:
    char red, green, blue; 
};

class MessageWhoIsThere : public Message
{
public:
    MessageWhoIsThere() { type = MsgWhoIsThere; };

    unsigned int getCoordX();
    void setCoordX(unsigned int);

    unsigned int getCoordY();
    void setCoordY(unsigned int);

    unsigned int getRadius();
    void setRadius(unsigned int);

private:
    unsigned int coordX, coordY; 
    unsigned int radius;
};

class MessageParameterReport : public Message
{
public:
    MessageParameterReport() { type = MsgParameterReport; };

    char getId();
    void setId(char);

    int getIntegral();
    void setIntegral(int);

    unsigned int getReal();
    void setReal(unsigned int);

private:
    char id;
    unsigned int integral;
    int real; 
};

/* Excuse me for awkward name - I really couldn't come up with anything better */
class MessageObject
{
public:
    char getRed();
    char getGreen();
    char getBlue();
    unsigned int getDiameter();
    unsigned int getCoordX();
    unsigned int getCoordY();
    double getDegrees();

private:
    char red, green, blue;
    unsigned int diameter;
    unsigned int coordX, coordY;
    double degrees;
};

class MessageThereYouSee : public Message
{
public:
    MessageThereYouSee() { type = MsgThereYouSee; };

    std::vector<MessageObject> getObjects();

private:
    std::vector<MessageObject> objects;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
