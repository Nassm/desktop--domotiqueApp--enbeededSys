#ifndef ROOMSTM_H
#define ROOMSTM_H

#include <QString>
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLCDNumber>
#include <QDir>
#include "rommstmmodel.h"
#include "myserialport.h"

class RoomStm : public QObject
{
    Q_OBJECT
public:
    explicit RoomStm(QString name, QObject *parent = nullptr);
    ~RoomStm();

    QWidget* widget;
    QLabel getLabelTemp();
    QLabel getLabelPression();
    QWidget* getWidget();
    MySerialPort* getSerialPort();
    QString getName();

private slots:
    void setChambre(RoomStmModel* chambre);

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
    RoomStmModel* chambre;
    MySerialPort* serial;
};

#endif // ROOMSTM_H
