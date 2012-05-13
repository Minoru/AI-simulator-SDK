#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <math.h>
#include "manager.h"

Manager::Manager(QObject *parent, QString configurationFile,
                 std::pair<unsigned int, unsigned int> mapSize) :
    QObject(parent)
{
    this->mapSize = mapSize;
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
        std::cout << "Environment configuration is not loaded. " <<
                     "Put environment's profile and this binary to /robots directory\n";
        emit stop();
    }
}

void Manager::action()
{
    for (unsigned int i = 0; i < envObjects.size(); i++) {

        //TODO: implement action for each object
        // it can be accessed by envObjects.at(i)
    }
}

void Manager::loadConfiguration(QString configurationFile)
{
    // Load file contents (without commented strings) to configStringList
    QFile config(QString("robots/") + configurationFile);
    QStringList configStringList = QStringList();
    if (!config.open(QFile::ReadOnly)) {
        std::cout << "Cannot open configuration file for the environment";
        return;
    }
    QTextStream stream (&config);
    QString line;
    while(!stream.atEnd()) {
        line = stream.readLine();
        if (!line.contains(QRegExp("^(//)")) && !line.isEmpty())
            configStringList.append(line);
    }
    config.close();

    // check launch command
    if (configStringList.at(0).isEmpty()) {
        std::cout << "Launch command is empty!";
        return;
    }

    // check port
    int portFilename = configurationFile.left(4).toInt();
    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        std::cout << "Ports in filename and file body aren't equal!";
        return;
    }
    EnvObject::setPortNumber(portFilename);

    // start parsing each object
    const int parametersQuantityPerObject = 8;      // See AI-simulator wiki
    QVector<int> indexes = QVector<int>();
    for (int obj = 0; obj < ENV_OBJECTS; obj++) {
        EnvObject *envObject = new EnvObject();

        if (configStringList.size() < 2 + (obj+1) * parametersQuantityPerObject)
            break;
        QStringList objectParams = QStringList();
        for (int i = 0; i < parametersQuantityPerObject; i++)
            objectParams.push_back(configStringList.at(2 + obj * parametersQuantityPerObject + i));
        bool ok = true;

        // Check object id
        int index = objectParams.at(0).toInt(&ok);
        if (!ok || index < 0) {
            std::cout << "Id must be non-negative integer number (object " << obj << " )";
            return;
        }
        if (indexes.contains(index)) {
            std::cout << "Object with same id already exists (object " << obj << " )";
            return;
        }

        // Check object start position
        QString pos = objectParams.at(1);
        if (!pos.contains(QRegExp("^(\\d)+;(\\d)+$")) && pos != QString("-1;-1")) {
            std::cout << "Invalid start position (object " << obj << " )";
            return;
        }
        int x, y;
        if (pos != QString("-1;-1"))
        {
            x = pos.split(";").at(0).toInt(&ok);
            if (!ok) {
                std::cout << "Invalid start position (object " << obj << " )";
                return;
            }
            y = pos.split(";").at(1).toInt(&ok);
            if (!ok) {
                std::cout << "Invalid start position (object " << obj << " )";
                return;
            }
            if (x < 0 || y < 0 || x >= mapSize.first * REAL_PIXEL_SIZE
                    || y >= mapSize.second * REAL_PIXEL_SIZE) {
                std::cout << "Start position is out of the map (object " << obj << " )";
                return;
            }
        } else {

            srand(static_cast<unsigned int>(time(0)));
            x = rand() % (mapSize.first * REAL_PIXEL_SIZE);
            y = rand() % (mapSize.second * REAL_PIXEL_SIZE);
            std::cout << "Object " << obj <<
                         " receives random coordinates ( " << x << ", " << y << " )";
        }

        // Check if size is a number and is over than zero
        int size = objectParams.at(2).toInt();
        if (size <= 0) {
            std::cout << "Invalid size (object " << obj << " )";
            return;
        }

        // Check intersection type
        QString intersection = objectParams.at(3);
        if (intersection != "0" && intersection != "1" && intersection != "2") {
            std::cout << "Invalid intersection type (object " << obj << " )";
            return;
        }

        bool movable;
        if (objectParams.at(4) == QString("0"))
            movable = false;
        else if (objectParams.at(4) == QString("1"))
            movable = true;
        else {
            std::cout << "Movable parameter can receive only 0 or 1 (object " << obj << " )";
            return;
        }

        // Check orientation
        double orientation = objectParams.at(5).toDouble(&ok);
        if (!ok || orientation < 0) {
            std::cout << "Invalid orientation (object " << obj << " )";
            return;
        }

        int velocity = objectParams.at(6).toInt();
        if (velocity <= 0) {
            std::cout << "Invalid velocity (object " << obj << " )";
            return;
        }

        // Check color
        QColor color = QColor(objectParams.at(7));
        if (!color.isValid()) {
            std::cout << "Invalid color (object " << obj << " )";
            return;
        }

        indexes.push_back(index);
        envObject->setObjectId(index);
        envObject->setCoords(x, y);
        envObject->setSize(size);
        envObject->setIntersection(static_cast<Intersection>(intersection.toInt()));
        envObject->setMovable(movable);
        envObject->setOrientation(orientation);
        envObject->setVelocity(velocity);
        envObject->setColor(color);
        envObjects.push_back(envObject);
    }

    configurationLoaded = true;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
