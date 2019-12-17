#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(argc == 3){//first is dir if current
        QString saveDir = QString(argv[2]) + "\\";//保存路径
        w.setSaveDir(saveDir);
        w.getCommandFromFile(QString(argv[1]));
    }
    else{
        w.show();
    }
    return a.exec();
}
