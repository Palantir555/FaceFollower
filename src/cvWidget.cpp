#include "cvWidget.h"

// Constructor
cvWidget::cvWidget(QWidget *parent) : QWidget(parent)
{
    _capture = cvCaptureFromCAM( 0 );
    _cascade = new CvHaarClassifierCascade();
    _cascade = (CvHaarClassifierCascade*)cvLoad("/Users/Jc/QtSDK/Projects/DrawFaces/resources/haarcascade_frontalface_alt2.xml");
    //_cascade = (CvHaarClassifierCascade*)cvLoad(":/cascade/frontalface.xml");
    _storage = cvCreateMemStorage(0);

    _colors << cvScalar(0.0,0.0,255.0) << cvScalar(0.0,128.0,255.0)
            << cvScalar(0.0,255.0,255.0) << cvScalar(0.0,255.0,0.0)
            << cvScalar(255.0,128.0,0.0) << cvScalar(255.0,255.0,0.0)
            << cvScalar(255.0,0.0,0.0) << cvScalar(255.0,0.0,255.0);
}

cvWidget::~cvWidget(void)
{
    
}

QImage cvWidget::getQImage(IplImage *cvimage)
{
    int cvIndex, cvLineStart;
    // switch between bit depths
    if((cvimage->nChannels == 3) && (cvimage->depth == IPL_DEPTH_8U))
    {
        if((cvimage->width != image.width()) || (cvimage->height != image.height()))
        {
            QImage temp(cvimage->width, cvimage->height, QImage::Format_RGB32);
            image = temp;
        }
        cvIndex = 0; cvLineStart = 0;
        for (int y = 0; y < cvimage->height; y++)
        {
            unsigned char red,green,blue;
            cvIndex = cvLineStart;
            for (int x = 0; x < cvimage->width; x++)
            {
                // DO it
                red = cvimage->imageData[cvIndex+2];
                green = cvimage->imageData[cvIndex+1];
                blue = cvimage->imageData[cvIndex+0];

                image.setPixel(x, y, qRgb(red, green, blue));
                cvIndex += 3;
            }
            cvLineStart += cvimage->widthStep;
        }
    }
    else
    {
        if(cvimage->depth != IPL_DEPTH_8U)
            qDebug() << "This type of IplImage is not implemented in cvWidget";
        if(cvimage->nChannels != 3)
            qDebug() << "This number of channels is not supported";
        qDebug() << QString::number(cvimage->nChannels);
    }
    return(image);
}

void cvWidget::detectAndDraw (IplImage *cvimage)
{
    if (!cvimage)
        return;
    //cv::cvtColor(cvimage->imageData, cvimage->imageData, CV_BGRA2BGR);

    cvClearMemStorage(_storage);
    CvSeq *objects = cvHaarDetectObjects(cvimage, _cascade, _storage, 1.1, 3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize(WIDTH, HEIGHT));

    int n = (objects ? objects->total : 0);
    drawnFacesNum = n;

    CvRect* r;
    if(n==0)
    {
        firstCenter.setX(-1);
        firstCenter.setY(-1);
    }
    // Loop through objects and draw boxes
    for(int i=0; i<n; i++)
    {
        r = (CvRect*)cvGetSeqElem( objects, i );
        cvRectangle( cvimage,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     _colors[i%8], 3 );
        if(i==0)
        {
            firstCenter.setX(r->x + r->width/2);
            firstCenter.setY(r->y + r->height/2);
        }
    }
}

int cvWidget::countFaces(IplImage *cvimage)
{
    if (!cvimage)
        return(-1);

    cvClearMemStorage( _storage );
    CvSeq *objects = cvHaarDetectObjects(cvimage, _cascade, _storage, 1.1, 3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize( WIDTH, HEIGHT ));

    int n = (objects ? objects->total : 0);
    return(n);
}

int cvWidget::getXcoordinate(IplImage *cvimage, int faceNum)
{
    if (!cvimage)
        return(-1);
    cvClearMemStorage( _storage );
    CvSeq *objects = cvHaarDetectObjects(cvimage, _cascade, _storage, 1.1, 3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize( WIDTH, HEIGHT ));

    int n = (objects ? objects->total : 0);
    if(n==0)
        return(-1);
    CvRect* r;
    r = (CvRect*)cvGetSeqElem( objects, faceNum );
    int X = r->x + ((r->width)/2);
    return(X);
}

int cvWidget::getYcoordinate(IplImage *cvimage, int faceNum)
{
    if (!cvimage)
        return(-1);
    cvClearMemStorage( _storage );
    CvSeq *objects = cvHaarDetectObjects(cvimage, _cascade, _storage, 1.1, 3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize( WIDTH, HEIGHT ));
    int n = (objects ? objects->total : 0);
    if(n==0)
        return(-1);

    CvRect* r;
    r = (CvRect*)cvGetSeqElem( objects, faceNum );
    int Y = r->y + ((r->height)/2);
    return(Y);
}

int cvWidget::getWidth(IplImage *cvimage, int faceNum)
{
    if (!cvimage)
        return(-1);
    cvClearMemStorage( _storage );
    CvSeq *objects = cvHaarDetectObjects(cvimage, _cascade, _storage, 1.1, 3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize( WIDTH, HEIGHT ));
    int n = (objects ? objects->total : 0);
    if(n==0)
        return(-1);

    CvRect* r;
    r = (CvRect*)cvGetSeqElem( objects, faceNum );
    return(r->width);
}

int cvWidget::getHeight(IplImage *cvimage, int faceNum)
{
    if (!cvimage)
        return(-1);
    cvClearMemStorage( _storage );
    CvSeq *objects = cvHaarDetectObjects(cvimage, _cascade, _storage, 1.1, 3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize( WIDTH, HEIGHT ));
    int n = (objects ? objects->total : 0);
    if(n==0)
        return(-1);

    CvRect* r;
    r = (CvRect*)cvGetSeqElem( objects, faceNum );
    return(r->height);
}

IplImage* cvWidget::QImage2IplImage(QImage *qImage, int chNum)
{
    int width = qImage->width();
    int height = qImage->height();
    IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, chNum);
    char* imgBuffer = img->imageData;
    //Remove alpha channel
    int jump = (qImage->hasAlphaChannel())? 4 : 3; //Need to change for chNum??
    for(int y=0;y<height;y++)
    {
        QByteArray a((const char*)qImage->scanLine(y), qImage->bytesPerLine());
        for(int i=0; i<a.size(); i+=jump)
        {
            //Swap from RGB to BGR
            imgBuffer[2]= a[i];
            imgBuffer[1]= a[i+1];
            imgBuffer[0]= a[i+2];
            imgBuffer+=3;
        }
    }
    return(img);
}
