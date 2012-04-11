#include <QtCore/QCoreApplication>
#include <QTimer>
#include <iostream>
#include "manager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QString(argv[1]).contains(QRegExp("^(\\d){4}(\\.)(\\d)$"))) {
        std::cout << "Error! Invalid profile name.\n";
        return 0;
    }

    srand((unsigned int)argv);

    Manager *manager = new Manager(&a, QString(argv[1]));
    QObject::connect(manager, SIGNAL(stop()), &a, SLOT(quit()));
    QTimer::singleShot(0, manager, SLOT(run()));

    return a.exec();
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
