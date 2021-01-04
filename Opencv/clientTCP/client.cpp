#include "client.h"

client::client(QObject *parent) : QObject(parent)
{
    QTcpSocket* sock = new QTcpSocket(this);
    connect(sock,SIGNAL(readyRead()),this,SLOT(serveurMeParle()));
    connect(sock, SIGNAL(disconnected()), sock, SLOT(deleteLater()));
    sock->connectToHost("localhost",8585);
    if (sock->waitForConnected(3000) == true)
    {
        sock->write("Connexion établie");
    }
}

void client::serveurMeParle()
{
    QTcpSocket * sock = (QTcpSocket*)sender();
    qDebug() << sock->readAll(); //Lecture renvoyé par le serveur
    sock->close();
}
