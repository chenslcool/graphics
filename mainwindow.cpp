#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polygondialog.h"
#include "curvedialog.h"
#include "linedialog.h"
#include "resetcanvasdialog.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionDrawPolygon_triggered()
{
    //当初窗口输入边数，进行检查
    PolygonDialog polygonDialog;
    if(polygonDialog.exec() == QDialog::Accepted){
        int lineNum = polygonDialog.getLineNum();
        qDebug()<<"press ok,input:"<<lineNum;
    }
    else{
        qDebug()<<"canceled";
    }
}

void MainWindow::on_actionDrwaCurve_triggered()
{
    CurveDialog curveDialog;
    if(curveDialog.exec() == QDialog::Accepted){
        int ctlNum = curveDialog.getCtlNum();
        QString curveType = curveDialog.getCurveType();
        qDebug()<<"press ok,ctlNum"<<ctlNum<<",Type="+curveType;
    }
    else{
        qDebug()<<"canceled";
    }
}

void MainWindow::on_actionDrawLine_triggered()
{
    LineDialog lineDialog;
    if(lineDialog.exec() == QDialog::Accepted){
        QString lineType = lineDialog.getLineType();
        qDebug()<<"press ok,lineType"<<lineType;
    }
    else{
        qDebug()<<"canceled";
    }
}

void MainWindow::on_actionresetCanvas_triggered()
{
    //弹出对话框输入画布尺寸
    resetCanvasDialog dlog;
    if(dlog.exec() == QDialog::Accepted){
        int w = dlog.getWidth();
        int h = dlog.getHeight();
        qDebug()<<"press ok,w="<<w<<"h="<<h;
    }
    else{
        qDebug()<<"canceled";
    }
}
