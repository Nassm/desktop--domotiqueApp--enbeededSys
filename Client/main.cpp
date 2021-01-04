#include <QApplication>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QIcon>
#include <QAction>
#include "burgermenu.h"
#include "roomstm.h"
#include "myserialport.h"
#include <QDebug>
#include <QTimer>
#include "qrcode.h"


/* static QString path= "/home/greg/Bureau/Domotique_Client_Integrated/images/"; */
static QString path= "/home/sadikou/domotique_2a_g10/Client/images";

int main(int argc, char *argv[])
{
    bool doorOpen = false;
    qrCode *thread = new qrCode();
    thread->start();
    QObject::connect(thread, SIGNAL(finished()),thread, SLOT(deleteLater()));
    QApplication a(argc, argv);
    QWidget window;
    QWidget* wContenu = new QWidget();
    window.setMinimumSize(700, 700);
    window.setWindowTitle("Interface domotique maison");

    BurgerMenu* menu = new BurgerMenu();

    wContenu->setLayout(new QVBoxLayout());
    window.setLayout(new QHBoxLayout());
    window.layout()->setContentsMargins(0,0,0,0);
    window.layout()->setSpacing(0);
    window.layout()->addWidget(menu);
    window.layout()->addWidget(wContenu);

    QWidget* wCam = new QWidget();
    wCam->setLayout(new QHBoxLayout());
    QLabel* label = new QLabel();
    wCam->layout()->addWidget(label);
    window.layout()->addWidget(wCam);
    wCam->setVisible(false);

    menu->setMenuWidth(100);
    menu->setBurgerIcon(QIcon(path + "burger.png"));
    menu->addMenuAction(QIcon(path + "Porte_ferme.png"),      "Porte");


    window.setStyleSheet("title                       { background-color: #2C3E50; }"
                         "BurgerMenu                  { background-color: #2C3E50; }"
                         "#BurgerMenu                 { background-color: #2C3E50; }"
                         "#BurgerButton               { background-color: #F39C12; color: white; font-size: 18px; }"
                         "#BurgerButton:hover         { background-color: #FAD7A0; }"
                         "#BurgerButton:checked       { background-color: #D68910; }"
                         "#BurgerButton:checked:hover { background-color: #D68910; }"
                         "#MainBurgerButton           { background-color: #2C3E50; border: none; }"
                         "#MainBurgerButton:hover     { background-color: #34495E; } "
                         );


    /* Détection des ports COM */
    bool allOpened = false;
    QList<RoomStm*> rooms;
    while(!allOpened){
        RoomStm *temp;
        temp= new RoomStm("");
        if(!temp->getSerialPort()->isEnabled())
            allOpened = true;
        else
        {
            wContenu->layout()->addWidget(temp->getWidget());
            temp->getWidget()->setVisible(false);
            rooms.append(temp);
            QTimer::singleShot(1000, a.thread(), [=]() {
                if(temp->getName().contains("Sejour") || temp->getName().contains("Chambre"))
                    menu->addMenuAction(QIcon(path + temp->getName() + ".png"), temp->getName());
                else
                    menu->addMenuAction(QIcon(path + "all.png"), temp->getName());

            });
        }
    }
    menu->addMenuAction(QIcon(path + "all.png"),      "Tous");


    /* Se déclenche lorsqu'on appuie sur un burger bouton, et permet d'afficher les bons widgets. */
    QObject::connect(menu, &BurgerMenu::triggered, [&](QAction* action)
    {
        if(action->iconText() == "Tous")
        {
            doorOpen = false;
            wCam->setVisible(false);
            for(int i =0; i < rooms.length(); i++)
            {
                rooms.value(i)->getWidget()->setVisible(true);

            }
        }
        else if (action->iconText() == "Porte")
        {
            doorOpen = true;
            wCam->setVisible(true);
            for(int i =0; i < rooms.length(); i++)
            {
                rooms.value(i)->getWidget()->setVisible(false);
            }
        }
        else
        {
            doorOpen = false;
            wCam->setVisible(false);
            for(int i =0; i < rooms.length(); i++)
            {
                if(rooms.value(i)->getName() == action->iconText())
                    rooms.value(i)->getWidget()->setVisible(true);
                else
                    rooms.value(i)->getWidget()->setVisible(false);
            }
        }
    });


    /* Se déclenche avec un qrCode "Open" et change le logo entre porte ouverte et porte fermée */
    QObject::connect(thread, &qrCode::sendResultQRCode, [&](bool openDoor)
    {
        menu->actions().at(0)->setIcon(QIcon(path + "Porte_ouverte.png"));
        QTimer::singleShot(3000, a.thread(), [=]() {
            menu->actions().at(0)->setIcon(QIcon(path + "Porte_ferme.png"));
        });
    });


    /* Récupère l'image de la caméra et la transforme en label afin de l'intégré au widget */
    QObject::connect(thread, &qrCode::sendWidgetVideo, [&](cv::Mat frame)
    {
            cv::Mat temp(frame.cols,frame.rows,frame.type()); // make the same cv::Mat
            cvtColor(frame, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
            QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
            QPixmap qm = QPixmap::fromImage(dest);
            label->setPixmap(qm);
    });

    window.show();
    return a.exec();
}



