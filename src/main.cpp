#include <QApplication>
#include <QWidget>
#include "cvWidget.h"
#include "MainWindow.h"

MainWindow *mainWin;
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    mainWin = new MainWindow("DrawFaces");
    mainWin->show();    
    int res = app.exec();
    
    return res;
}

