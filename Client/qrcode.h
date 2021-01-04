#ifndef QRCODE_H
#define QRCODE_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>
#include <QImage>
#include <QPixmap>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class qrCode : public QThread
{
    Q_OBJECT
public:
  explicit qrCode(QObject *parent = 0);
protected:
    void run();

private:
    QLabel* label;
    QWidget *widget;

// On met le slot en private pour éviter de pouvoir
// l'appeler comme une fonction normale
private slots:
    void QRCode();

signals:
    void sendResultQRCode(bool);
    void sendWidgetVideo(cv::Mat frame);

public :
    QWidget* getWidget();
};
#endif // QRCODE_H
