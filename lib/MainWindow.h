#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <assert.h>
#include <QDebug>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QTimer>

#include "cvWidget.h"
#include "serialportcontrol.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>


class MainWindow : public QDialog
{
    Q_OBJECT
private:
    void createLayout(void);
    void createWidgets(void);

    QLabel *coordX;
    QLabel *coordY;

    SerialPortControl *spwidget;
    cvWidget *cvwidget;
    CvCapture *cam;

    QVBoxLayout *vbLayout;
    QLabel *lbImage;
    QPixmap *pxImage;
    QImage qimg;

    QTimer *tmr;
        
public:
    MainWindow(const char *windowTitle);
    ~MainWindow(void);

    QImage *qim;
         
protected:

public slots:
    void tmrHandler(void);
};


#endif //MAINWINDOW_H
