#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "../../constants.h"
#include "robot.h"

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0, QString configurationFile = "");
    
signals:
    void stop();
public slots:
    void run();
private:
    void action();
    void loadConfiguration(QString configurationFile);

    Robot *robot;
    bool configurationLoaded;
    bool bumped;
};

#endif // MANAGER_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
