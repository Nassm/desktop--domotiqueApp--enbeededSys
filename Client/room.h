#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLCDNumber>
#include <QDir>
#include "chambre.h"
#include "myserialport.h"

class Room
{

public:
    Room(QString name);
    QWidget* widget;
    QLabel getLabelTemp();
    QLabel getLabelPression();
    QWidget* getWidget();
    void initConnect();

private slots:
    void setPression(Chambre* chambre);

public:
    QVBoxLayout* hbox;
    QHBoxLayout* lTop;
    QHBoxLayout* lBottom;
    QHBoxLayout* lTemp;
    QHBoxLayout* lPress;
    QLabel* lblName;
    QLabel* lblTemp;
    QLabel* lblPress;
    QPixmap* iTemp;
    QPixmap* iPress;
    QLabel* iLbl;
    QLCDNumber* lcdTemp;
    QLCDNumber* lcdPress;
    Chambre* chambre;
    MySerialPort* serial;

};

#endif // ROOM_H
