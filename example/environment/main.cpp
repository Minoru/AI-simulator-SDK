#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QRegExp>
#include <iostream>
#include "manager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QString(argv[1]).contains(QRegExp("^(\\d){4}(\\.)env$"))) {
        std::cout << "Error! Invalid profile name.\n";
        return 0;
    }

    bool ok = true;
    std::pair<unsigned int, unsigned int> mapSize = std::pair<unsigned int, unsigned int>();
    mapSize.first = QString(argv[2]).toInt(&ok);
    mapSize.second = QString(argv[3]).toInt(&ok);
    if (!ok || mapSize.first == 0 || mapSize.second == 0) {
        std::cout << "Error! Invalid map size.\n";
        return 0;
    }

    Manager *manager = new Manager(&a, QString(argv[1]), mapSize);
    QObject::connect(manager, SIGNAL(stop()), &a, SLOT(quit()));
    QTimer::singleShot(0, manager, SLOT(run()));

    return a.exec();
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
