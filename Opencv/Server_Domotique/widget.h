#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void nouveauClient();
    void sockDisconnected();
    void clientMeParle();
    QString QRCode();
private:
    QTcpServer* mServeur;
    QList<QTcpSocket*> mListeSocks;
};
#endif // WIDGET_H
