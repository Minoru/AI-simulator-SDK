#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    objects = new QVector<Object>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addObjectPushButton_clicked()
{
    bool ok = true;

    // check color
    if (!QColor::isValidColor(ui->colorLineEdit->text())) {
        QMessageBox::warning(this, tr("Error!"), tr("Invalid color"));
        return;
    }

    // check id
    int id = ui->objectIdLineEdit->text().toInt(&ok);
    if (!ok || id < 0) {
        QMessageBox::warning(this, tr("Error!"), tr("Invalid object id"));
        return;
    }
    for (int i = 0; i < objects->size(); i++) {
        if (objects->at(i).objectId == static_cast<unsigned int>(id)) {
            QMessageBox::warning(this, tr("Error!"), tr("Object with this id already exists"));
            return;
        }
    }

    // check start position
    int x, y;
    if (ui->generateCheckBox->isChecked()) {
        x = -1;
        y = -1;
    } else {
        x = ui->startPositionXLineEdit->text().toInt(&ok);
        y = ui->startPositionYLineEdit->text().toInt(&ok);
        if (!ok || x < 0 || y < 0) {
            QMessageBox::warning(this, tr("Error!"), tr("Invalid start position"));
            return;
        }
    }

    // check size
    int size = ui->sizeLineEdit->text().toInt(&ok);
    if (!ok || size <= 0) {
        QMessageBox::warning(this, tr("Error!"), tr("Invalid size"));
        return;
    }

    // intersection
    int intersection = ui->intersectionComboBox->currentIndex();

    // check movable, orientation, velocity
    bool movable;
    double orientation;
    int velocity;

    if (ui->movableYesRadioButton->isChecked()) {
        orientation = ui->orientationLineEdit->text().toDouble(&ok);
        if (!ok || orientation < 0 || orientation > 360) {
            QMessageBox::warning(this, tr("Error!"), tr("Invalid orientation"));
            return;
        }
        velocity = ui->velocityLineEdit->text().toInt(&ok);
        if (!ok || velocity < 0) {
            QMessageBox::warning(this, tr("Error!"), tr("Invalid velocity"));
            return;
        }
        movable = true;
    } else {
        orientation = 0;
        velocity = 100;
        movable = false;
    }

    Object obj;
    obj.objectId = id;
    obj.color = ui->colorLineEdit->text();
    obj.startPosition = std::pair<int, int>(x, y);
    obj.size = size;
    obj.intersectionType = intersection;
    obj.movable = movable;
    obj.orientation = orientation;
    obj.velocity = velocity;

    objects->append(obj);
    clearForm();
    QMessageBox::information(this, tr("Success"), tr("Object was added to environment"));
}

void MainWindow::on_generateProfilePushButton_clicked()
{
    QString port = ui->portNumberLineEdit->text();
    if (port.isEmpty() || !port.contains(QRegExp("^(\\d){4}$"))) {
        QMessageBox::warning(this, tr("Error!"), tr("Invalid port number"));
        return;
    }

    if (ui->launchCommandLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error!"), tr("Invalid launch command"));
        return;
    }

    QStringList text;

    text << ui->launchCommandLineEdit->text();
    text << port;

    foreach (Object iter, *objects) {
        text << "";
        text << QString("%1").arg(iter.objectId);
        text << QString("%1;%2").arg(iter.startPosition.first).arg(iter.startPosition.second);
        text << QString("%1").arg(iter.size);
        text << QString("%1").arg(iter.intersectionType);
        text << QString("%1").arg(static_cast<int>(iter.movable));
        text << QString("%1").arg(iter.orientation);
        text << QString("%1").arg(iter.velocity);
        text << QString("%1").arg(iter.color);
    }

    QFile file(port + ".env");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    foreach (QString str, text)
        out << str << endl;
    file.close();

    clearForm();
    QMessageBox::information(this, tr("Success!"),
                 tr("Environment profile generated. Get it from the directory of this applcation"));
}

void MainWindow::on_clearAllPushButton_clicked()
{
    delete objects;
    objects = new QVector<Object>();
    clearForm();
}

void MainWindow::on_generateCheckBox_clicked()
{
    if (ui->generateCheckBox->isChecked())
    {
        ui->startPositionXLineEdit->setEnabled(false);
        ui->startPositionYLineEdit->setEnabled(false);
    } else {
        ui->startPositionXLineEdit->setEnabled(true);
        ui->startPositionYLineEdit->setEnabled(true);
    }
}

void MainWindow::on_movableNoRadioButton_clicked()
{
    ui->orientationLineEdit->setEnabled(false);
    ui->velocityLineEdit->setEnabled(false);
}

void MainWindow::on_movableYesRadioButton_clicked()
{
    ui->orientationLineEdit->setEnabled(true);
    ui->velocityLineEdit->setEnabled(true);
}

void MainWindow::clearForm()
{
    ui->objectIdLineEdit->setText("0");
    ui->generateCheckBox->setChecked(false);
    ui->startPositionXLineEdit->setText("2000");
    ui->startPositionYLineEdit->setText("2000");
    ui->sizeLineEdit->setText("300");
    ui->intersectionComboBox->setCurrentIndex(0);
    ui->movableYesRadioButton->setChecked(true);
    ui->movableNoRadioButton->setChecked(false);
    ui->velocityLineEdit->setText("100");
    ui->orientationLineEdit->setText("45");
    ui->colorLineEdit->setText("#ff00ff");

    ui->orientationLineEdit->setEnabled(true);
    ui->velocityLineEdit->setEnabled(true);
    ui->startPositionXLineEdit->setEnabled(true);
    ui->startPositionYLineEdit->setEnabled(true);
}
