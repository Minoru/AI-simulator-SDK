#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}


struct Object {
    unsigned int objectId;
    std::pair<int, int> startPosition;
    unsigned int size;
    unsigned int intersectionType;
    bool movable;
    double orientation;
    unsigned int velocity;
    QString color;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_addObjectPushButton_clicked();

    void on_generateProfilePushButton_clicked();

    void on_clearAllPushButton_clicked();

    void on_generateCheckBox_clicked();

    void on_movableNoRadioButton_clicked();

    void on_movableYesRadioButton_clicked();

private:
    Ui::MainWindow *ui;

    QVector<Object> *objects;

    void clearForm();
};

#endif // MAINWINDOW_H
