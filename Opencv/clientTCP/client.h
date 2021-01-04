#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = nullptr);

signals:

public slots:
private slots:
    void serveurMeParle();
};

#endif // CLIENT_H
