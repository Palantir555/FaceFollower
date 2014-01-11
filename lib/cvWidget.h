
#ifndef CVWIDGET_H
#define CVWIDGET_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <QPixmap>
#include <QLabel>
#include <QWidget>
#include <QImage>
#include <QDebug>
#include <QPoint>
#include <QList>

#define WIDTH 100
#define HEIGHT 100
class cvWidget : public QWidget {

private:
    QImage image;

    CvCapture*      _capture;
    IplImage*       _img;
    CvHaarClassifierCascade *_cascade;
    CvMemStorage*   _storage;
    QList<CvScalar>  _colors;

public:
    cvWidget(QWidget *parent = 0);
    ~cvWidget(void);
    QImage getQImage(IplImage *cvimage);
    void detectAndDraw(IplImage *cvimage);
    QPoint firstCenter;
    int drawnFacesNum;
    int getXcoordinate(IplImage *cvimage, int faceNum=0);
    int getYcoordinate(IplImage *cvimage, int faceNum=0);
    int getWidth(IplImage *cvimage, int faceNum=0);
    int getHeight(IplImage *cvimage, int faceNum=0);
    int countFaces(IplImage *cvimage); //Counts the faces in a IplImage.
                                       //    Does NOT update facesNum.
    IplImage* QImage2IplImage(QImage *qImage, int chNum=3); //Receives Format_RGB888
};

#endif
