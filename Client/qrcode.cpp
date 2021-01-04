#include "qrcode.h"



using namespace cv;
using namespace std;
using namespace zbar;

qrCode::qrCode(QObject *parent) :
    QThread(parent)
{
    setParent(0);
    moveToThread(this);
}


void qrCode::run()
{
    qrCode::QRCode();
    exec();
}


void qrCode::QRCode()
{
    VideoCapture cap(0); // open the video camera no. 0
    if (!cap.isOpened()) // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
    }

    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    label = new QLabel();
    widget = new QWidget();

    while (1)
    {
        Mat frame;



        bool bSuccess = cap.read(frame); // read a new frame from video
        emit sendWidgetVideo(frame);





        if (!bSuccess) //if not success, break loop
        {

            break;
        }

        Mat grey;
        cvtColor(frame, grey, CV_BGR2GRAY);

        int width = frame.cols;
        int height = frame.rows;
        uchar *raw = (uchar *)grey.data;

        // wrap image data
        Image image(unsigned(width), unsigned(height), "Y800", raw, unsigned(width * height));

        // scan the image for barcodes
        scanner.scan(image);

        // extract results
        for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
        {
            vector<Point> vp;

            String QRCode = "OPEN";

            if (symbol->get_data() == QRCode)
                emit sendResultQRCode(true);


            int n = symbol->get_location_size();
            for(int i=0;i<n;i++)
            {
                vp.push_back(Point(symbol->get_location_x(unsigned(i)), symbol->get_location_y(unsigned(i))));
            }

            RotatedRect r = minAreaRect(vp);
            Point2f pts[4];
            r.points(pts);

            for(int i=0;i<4;i++)
            {
                line(frame, pts[i], pts[(i+1)%4], Scalar(255,0,0), 3);
            }
        }

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            break;
        }
    }
}



