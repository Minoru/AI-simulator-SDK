#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <math.h>
#include "manager.h"

Manager::Manager(QObject *parent, QString configurationFile) :
    QObject(parent)
{
    robot = new Robot();

    configurationLoaded = false;
    loadConfiguration(configurationFile);
}

void Manager::run()
{
    if (configurationLoaded) {
        //TODO: at this point we must process incoming messages from the simulator
        action();
        QTimer::singleShot(PERFORM_ACTION_FREQUENCY, this, SLOT(run()));
    } else {
        std::cout << "Robot configuration is not loaded. " <<
                     "Put robot's profile and this binary to /robots directory\n";
        emit stop();
    }
}

void Manager::action()
{
    //TODO: replace the code below by yours

    srand(static_cast<unsigned>(time(0)));

    if (fabs(robot->getCoords().first - robot->getParameter(0)) <
            robot->getParameter(2)
            && fabs(robot->getCoords().second - robot->getParameter(1)) <
            robot->getParameter(2)) {
        robot->setParameter(0, 300 + rand() % 23400);
        robot->setParameter(1, 300 + rand() % 23400);
    } else {
        double x1 = robot->getCoords().first;
        double y1 = robot->getCoords().second;
        double x2 = robot->getParameter(0);
        double y2 = robot->getParameter(1);
        double y;
        double part = fabs(robot->getParameter(2)) / sqrt(pow((x2 - x1) / (y2 - y1), 2) + 1);
        if (part + y1 < y2)
            y = part + y1;
        else
            y = y1 - part;
        double x = (x2 - x1) * (y - y1) / (y2 - y1) + x1;
        robot->move(static_cast<int>(x), static_cast<int>(y));
    }
}

void Manager::loadConfiguration(QString configurationFile)
{  
    // Load file contents (without commented strings) to configStringList
    QFile config(QString("robots/") + configurationFile);
    QStringList configStringList = QStringList();
    if (!config.open(QFile::ReadOnly)) {
        std::cout << "Cannot open configuration file\n";
        return;
    }
    QTextStream stream (&config);
    QString line;
    while(!stream.atEnd()) {
        line = stream.readLine();
        if (!line.contains(QRegExp("^(//)")))
            configStringList.append(line);
    }
    config.close();

    // Check if port in filename is equals to port in file body
    int portFilename = configurationFile.left(4).toInt();
    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        std::cout << "Ports in profile name and profile body aren't equals\n";
        return;
    }

    // Check start position
    QString pos = configStringList.at(2);
    if (!pos.contains(QRegExp("^(\\d)+;(\\d)+$"))) {
        std::cout << "Invalid start position\n";
        return;
    }
    bool ok = true;
    int x = pos.split(";").at(0).toInt(&ok);
    if (!ok) {
        std::cout << "Invalid start position\n";
        return;
    }
    int y = pos.split(";").at(1).toInt(&ok);
    if (!ok) {
        std::cout << "Invalid start position\n";
        return;
    }

    // Check if size is a number and is over than zero
    int size = configStringList.at(3).toInt();
    if (size <= 0) {
        std::cout << "Invalid size\n";
        return;
    }

    int type = configStringList.at(4).toInt(&ok);
    if (!ok || (type != 0 && type != 1)) {
        std::cout << "Invalid robot type";
        return;
    }

    // Check visibility radius
    int visibilityRadius = configStringList.at(5).toInt(&ok);
    if (!ok || visibilityRadius < 0) {
        std::cout << "Invalid visibilty radius";
        return;
    }

    // Check intersection type
    QString intersection = configStringList.at(8);
    if (intersection != "0" && intersection != "1" && intersection != "2") {
        std::cout << "Invalid intersection type\n";
        return;
    }

    // Check orientation
    double orientation = configStringList.at(9).toDouble(&ok);
    if (!ok) {
        std::cout << "Invalid orientation\n";
        return;
    }

    // Check color
    QColor color = QColor(configStringList.at(10));
    if (!color.isValid()) {
        std::cout << "Invalid color\n";
        return;
    }

    // Check all custom parameters
    std::pair<std::string, double> *parameters =
            new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        QString line = configStringList.at(11+i);
        if (!line.contains(QRegExp("^(\\d|\\.)+;(\\w|\\s)+$"))) {
            std::cout << "Invalid parameter " << i+1 << " \n";
            return;
        }
        double value = line.left(line.indexOf(QString(";"))).toDouble(&ok);
        if (!ok) {
            std::cout << "Invalid value of parameter" << i+1 << "\n";
            return;
        }
        std::string name = line.mid(line.indexOf(QString(";")) + 1).toStdString();
        parameters[i] = std::pair<std::string, double>(name, value);
    }

    robot->setPortNumber(portFilename);
    robot->move(x, y);
    robot->turn(orientation);
    robot->changeDiameter(size);
    robot->changeColor(color.red(), color.green(), color.blue());

    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        robot->setParameter(i, parameters[i].second);
    }

    //robot->setCoords(x, y);
    //robot->setSize(size);
    //robot->setOrientation(orientation);
    //robot->setColor(color);
    robot->setIntersection(static_cast<Intersection>(intersection.toInt()));
    //robot->setParameters(parameters);

    configurationLoaded = true;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
