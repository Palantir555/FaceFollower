#include "MainWindow.h"

MainWindow::MainWindow(const char *windowTitle)
{
    this->setWindowTitle(windowTitle);

    cam = cvCreateCameraCapture(0);
    /*for(int i=0; i<1000; i++)
    {
        if(cvCreateCameraCapture(i))
            qDebug() << "llllllllllllllllllllllllllllllllllllllllllllllll" << i;
    }*/
    assert(cam);
    qDebug() << "cam asserted";
    IplImage *image = cvQueryFrame(cam);
    assert(image);
    qDebug() << "image asserted";

    qDebug() << "Image depth = " <<  image->depth;
    qDebug() << "Image nChannels = " << image->nChannels;

    cvwidget = new cvWidget(this);
    spwidget = new SerialPortControl();
    spwidget->set_portName("/dev/tty.usbmodem621");
    if(!spwidget->openPort())
        qDebug() << "Failed to open the port";

    this->createLayout();
    this->createWidgets();
}

MainWindow::~MainWindow()
{
    cvReleaseCapture(&cam);
}

void MainWindow::createLayout()
{
    vbLayout = new QVBoxLayout(this);
    this->setLayout(vbLayout);
}

void MainWindow::createWidgets()
{
    coordX = new QLabel(tr("Coordinate X: -"));
    coordY = new QLabel(tr("Coordinate Y: -"));
    lbImage = new QLabel;
    pxImage = new QPixmap;
    tmr = new QTimer(this);
    qim = new QImage();

    vbLayout->addWidget(lbImage);
    vbLayout->addWidget(coordX);
    vbLayout->addWidget(coordY);
    QImage imgDefault(100, 100, QImage::Format_RGB32);
    for (int x = 0; x < 100; x ++)
    {
        for (int y =0; y < 100; y++)
            imgDefault.setPixel(x,y,qRgb(x, y, y));
    }
    lbImage->setPixmap(QPixmap::fromImage(imgDefault));

    tmr->setSingleShot(true);
    tmr->start(16);

    connect(tmr, SIGNAL(timeout()), this, SLOT(tmrHandler()));
}

void MainWindow::tmrHandler(void)
{
    IplImage *iplImage = cvQueryFrame(cam);
    cvwidget->detectAndDraw(iplImage);

    QString serial_str = "";

    int num = cvwidget->drawnFacesNum;
    float X = cvwidget->firstCenter.x();
    float Y = cvwidget->firstCenter.y();
    float imgWidth = iplImage->width;
    float imgHeight= iplImage->height;

    qimg = cvwidget->getQImage(iplImage);
    lbImage->setPixmap(QPixmap::fromImage(qimg));

    if((X==-1) || (Y==-1))
    {
        serial_str.append('HSVS\n'); //No faces->stop
        spwidget->sendQString(serial_str);
        tmr->start(1);
        return;
    }
/*
    //____________________X___________________
    if(X < imgWidth/3.0)
        serial_str.append("60,"); //move right
    else if(X > imgWidth*(2.0/3.0))
        serial_str.append("40,"); //move left
    else
        serial_str.append("0,"); //don't move
    //____________________Y___________________
    if(Y < imgHeight/3.0)
        serial_str.append("40"); //move up
    else if(Y > imgHeight*(2.0/3.0))
        serial_str.append("60"); //move down
    else
        serial_str.append("0"); //don't move

    serial_str.append('\n');
    spwidget->sendQString(serial_str);
*/
    //____________________X___________________
    serial_str.append("H");
    if(X < imgWidth/3.0)
        serial_str.append("L"); //move right
    else if(X > imgWidth*(2.0/3.0))
        serial_str.append("R"); //move left
    else
        serial_str.append("S"); //don't move
    //____________________Y___________________
    serial_str.append("V");
    if(Y < imgHeight/3.0)
        serial_str.append("U"); //move up
    else if(Y > imgHeight*(2.0/3.0))
        serial_str.append("D"); //move down
    else
        serial_str.append("S"); //don't move

    serial_str.append('\n');
    spwidget->sendQString(serial_str);
    tmr->start(1);
}
