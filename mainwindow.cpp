#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polygondialog.h"
#include "curvedialog.h"
#include "linedialog.h"
#include "resetcanvasdialog.h"
#include "myalgorithm.h"
#include "clipdialog.h"
#include "QDebug"
#include <QPen>
#include <QPoint>
#include <cmath>
#include <QLineEdit>
#include <QSlider>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <stdio.h>
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

bool operator<(const QPoint &p1, const QPoint &p2)
{
    if (p1.x() < p2.x())
    {
        return true;
    }
    else if (p1.x() > p2.x())
    {
        return false;
    }
    else if (p1.y() < p2.y())
    {
        return true;
    }
    else
        return false;
}

Shape *MainWindow::search(int id)
{
    for (Shape *shape : shapes)
    {
        if (shape->id == id)
        {
            return shape;
        }
    }
    return nullptr;
}

void MainWindow::parseLine(QString line)
{
    //解析一行
    QStringList words = line.split(' ');
    QString command = words.at(0);
    if (command == "resetCanvas")
    {
        int w = words.at(1).toInt();
        int h = words.at(2).toInt();
        resetCanvas(w, h);
    }
    else if (command == "saveCanvas")
    {
        saveCanvas(saveDir + words.at(1));
    }
    else if (command == "setColor")
    {
        int r = words.at(1).toInt();
        int g = words.at(2).toInt();
        int b = words.at(3).toInt();
        setColor(r, g, b, true);
    }
    else if (command == "drawLine")
    {
        int id = words.at(1).toInt();
        int x1 = words.at(2).toInt();
        int y1 = words.at(3).toInt();
        int x2 = words.at(4).toInt();
        int y2 = words.at(5).toInt();
        QPoint startPosition(x1, y1);
        QPoint endPosition(x2, y2);
        int type = words.at(6) == "DDA" ? DDA : Bresenham;
        drawLine(startPosition, endPosition, type, id); //只需要添加本图像
    }
    else if (command == "drawPolygon")
    {
        int id = words.at(1).toInt();
        int n = words.at(2).toInt();
        int type = words.at(3) == "DDA" ? DDA : Bresenham;
        QVector<QPoint> points;
        for (int i = 0; i < n; ++i)
        {
            int x = words.at(4 + i * 2).toInt();
            int y = words.at(5 + i * 2).toInt();
            points.append(QPoint(x, y));
        }
        drawPolygon(points, type, id);
    }
    else if (command == "drawCurve")
    {
        int id = words.at(1).toInt();
        int n = words.at(2).toInt();
        int type = words.at(3) == "Bezier" ? Bezier : BSpline;
        QVector<QPoint> points;
        for (int i = 0; i < n; ++i)
        {
            int x = words.at(4 + i * 2).toInt();
            int y = words.at(5 + i * 2).toInt();
            points.append(QPoint(x, y));
        }
        drawCurve(points, type, id);
    }
    else if (command == "drawEllipse")
    {
        int id = words.at(1).toInt();
        int x1 = words.at(2).toInt();
        int y1 = words.at(3).toInt();
        int rx = words.at(4).toInt();
        int ry = words.at(5).toInt();
        QPoint center(x1, y1);
        drawEllipse(center, rx, ry, id); //只需要添加本图像
    }
    else if (command == "translate")
    {
        int id = words.at(1).toInt();
        int dx = words.at(2).toInt();
        int dy = words.at(3).toInt();
        Shape *shape = search(id);
        translate(shape, dx, dy);
    }
    else if (command == "rotate")
    {
        int id = words.at(1).toInt();
        int x = words.at(2).toInt();
        int y = words.at(3).toInt();
        int r = words.at(4).toInt();
        QPoint center(x, y);
        Shape *shape = search(id);
        r = 360 - r;
        rotate(shape, center, r);
    }
    else if (command == "clip")
    {
        int id = words.at(1).toInt();
        int x1 = words.at(2).toInt();
        int y1 = words.at(3).toInt();
        int x2 = words.at(4).toInt();
        int y2 = words.at(5).toInt();
        QPoint rect1(x1, y1);
        QPoint rect2(x2, y2);
        int type = words.at(6) == "Cohen-Sutherland" ? Cohen : Liang;
        Shape *shape = search(id);
        clip(shape, rect1, rect2, type);
    }
}

void MainWindow::drawLastToCanvas()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (shapes.empty() == true)
    {
        return;
    }
    Shape *shape = shapes.last();
    drawShape(shape);
}

void MainWindow::drawAllToCanvas()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    for (Shape *shape : shapes)
    {
        drawShape(shape);
    }
}

void MainWindow::redrawAllShape()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    for (Shape *shape : shapes)
    {
        drawShape(shape);
    }
}

void MainWindow::drawShape(Shape *shape)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //设置画笔颜色
    canvasPainter->setPen(shape->color); //这样可以吗，不经过QPen
    if (shape != nullptr)
    {
        int count = 0;
        bool dotted = shape->getDotted();
        for (QPoint point : shape->points)
        {
            count++;
            if (dotted && count % 2 == 0)
                continue;
            canvasPainter->drawPoint(point);
        }
    }
}

//only draw points
void MainWindow::drawPoints(QVector<QPoint> &points)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    canvasPainter->setPen(Qt::black);
    for (QPoint &point : points)
    {
        canvasPainter->drawPoint(point);
    }
}

void MainWindow::drawMouseEventPoint(QMouseEvent *event) //在画布上画出红点，改变clickPoint
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    QPoint curPoint = event->pos(); //得到在MainWindow的位置，要转化成在canvas的位置-150,-150
    fixPoint(curPoint);
    clickPoint.append(curPoint);
    //特殊点红色画出
    QPen prePen = canvasPainter->pen();
    QPen curPen(Qt::red, 5);
    canvasPainter->setPen(curPen);
    canvasPainter->drawPoint(curPoint);
    canvasPainter->setPen(prePen);
    update(); //更新一下
}

void MainWindow::eraseMouseEventPoint() //从画布上抹去红点,不改变clickPoint
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    QPen prePen = canvasPainter->pen();
    QPen tempPen(Qt::white, 5);
    canvasPainter->setPen(tempPen);
    for (QPoint point : clickPoint)
    {
        canvasPainter->drawPoint(point);
    }
    canvasPainter->setPen(prePen);
    update();
    //    clickPoint.clear();
}

void MainWindow::clearCanvas()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    canvasPainter->eraseRect(canvas->rect());
}

void MainWindow::fixPoint(QPoint &point)
{
    point.setX(point.x() - RIGHT_SPACE);
    point.setY(point.y() - UP_SPACE);
}

void MainWindow::updatePointIdMap()
{
    Shape *shape = shapes.last(); //把最后一个图元的点加入
    QVector<QPoint> &points = shape->points;
    for (QPoint point : points)
    {
        auto it = pointIdMap.find(point);
        if (it == pointIdMap.end())
        {
            //找不到
            QVector<int> ids;
            ids.append(shape->id);
            pointIdMap.insert(point, ids);
        }
        else
        {
            if (it->contains(shape->id) == false)
                it->append(shape->id); //点对应的vector添加这个id
        }
    }
}

void MainWindow::deleteShapeFromMap(Shape *shape)
{
    QVector<QPoint> &points = shape->points;
    for (QPoint point : points)
    {
        auto it = pointIdMap.find(point);
        if (it != pointIdMap.end())
        {
            it->removeOne(shape->id); //删除此id
            if (it->empty())
            {
                //如果这个点已经没有图元了
                //删除这个apair
                pointIdMap.remove(point);
            }
        }
    }
}

void MainWindow::addShapeFromMap(Shape *shape)
{
    QVector<QPoint> points = shape->points;
    for (QPoint point : points)
    {
        auto it = pointIdMap.find(point);
        if (it == pointIdMap.end())
        {
            //找不到
            QVector<int> ids;
            ids.append(shape->id);
            pointIdMap.insert(point, ids);
        }
        else
        {
            if (it->contains(shape->id) == false)
                it->append(shape->id); //点对应的vector添加这个id
        }
    }
}

Shape *MainWindow::searchIdInMap(QPoint &point)
{
    //fprintf(fd,"in searchIdInMap(),before find()\n");
    auto it = pointIdMap.find(point);
    //fprintf(fd,"in searchIdInMap(),after find()\n");
    if (it == pointIdMap.end())
    {
        //        return nullptr;
        //稍微寻找一些point周围的点
        //fprintf(fd,"in searchIdInMap(),before neighbor\n");
        QVector<QPoint> neighbors = pointNeighbors(point);
        for (QPoint p : neighbors)
        {
            it = pointIdMap.find(p);
            if (it != pointIdMap.end())
            {
                //fprintf(fd,"in searchIdInMap(),found 2 layer neighbor\n");
                //                if(it->empty()){
                //                    //fprintf(fd,"it is empty!!\n");
                //                    return nullptr;//正常不会发生
                //                }
                return search(it->first()); //有可能是empty???
            }
        }
        //再扩大一圈
        neighbors = pointNeighbors2(point);
        for (QPoint p : neighbors)
        {
            it = pointIdMap.find(p);
            if (it != pointIdMap.end())
            {
                //fprintf(fd,"in searchIdInMap(),found 2 layer neighbor\n");
                //                if(it->empty()){
                //                    //fprintf(fd,"it is empty!!\n");
                //                    return nullptr;//正常不会发生
                //                }
                return search(it->first()); //有可能是empty???
            }
        }
    }
    //fprintf(fd,"in searchIdInMap(),no suitable point return null\n");
    return nullptr;
}

void MainWindow::removeHalfPoints(Shape *shape)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    QPen prePen = canvasPainter->pen();
    int i = 0;
    for (QPoint point : shape->points)
    {
        //先把原来的画白
        if (i % 2 == 0)
        {
            canvasPainter->setPen(Qt::red);
            canvasPainter->drawPoint(point);
        }
        else
        {
            canvasPainter->setPen(Qt::white);
            canvasPainter->drawPoint(point);
        }
        ++i;
    }
    canvasPainter->setPen(prePen);
    update();
}

QVector<QPoint> MainWindow::pointNeighbors(QPoint &point)
{
    //外一圈
    int x = point.x();
    int y = point.y();
    QVector<QPoint> points;
    points.append(QPoint(x - 1, y));
    points.append(QPoint(x + 1, y));
    points.append(QPoint(x, y + 1));
    points.append(QPoint(x, y - 1));
    points.append(QPoint(x - 1, y - 1));
    points.append(QPoint(x - 1, y + 1));
    points.append(QPoint(x + 1, y + 1));
    points.append(QPoint(x + 1, y - 1));
    return points;
}

QVector<QPoint> MainWindow::pointNeighbors2(QPoint &point)
{
    //再外一圈
    int x = point.x();
    int y = point.y();
    QVector<QPoint> points;
    for (int i = 0; i < 4; ++i)
    {
        points.append(QPoint(x - 1 + i, y + 2));
        points.append(QPoint(x + 2, y + 1 - i));
        points.append(QPoint(x + 1 - i, y - 2));
        points.append(QPoint(x - 2, y - 1 + i));
    }
    return points;
}

void MainWindow::getCommandFromFile(QString fileName)
{
    QFile file(fileName);
    if (!file.exists())
    {
        //TODO 报错文件不存在
        printf("file not found!\n");
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //打开错误
        printf("file open error!\n");
        //报错
        return;
    }
    QTextStream stream(&file); //用文本流读取文件
    QString line;
    printf("start\n");
    while (!stream.atEnd())
    {
        line = stream.readLine();
        if (line.mid(0, 11) == "drawPolygon" || line.mid(0, 9) == "drawCurve")
        { //还有其他分行的情况 TODO
            //把下一行加上,因为draw Polygon是分行的(需要在手册中说明)
            QString nextLine = stream.readLine();
            line = line + " " + nextLine;
        }

        //解析
        parseLine(line);
    }
    file.close();
}

void MainWindow::setSaveDir(QString dir)
{
    this->saveDir = dir;
}

void MainWindow::resetSelected()
{
    if (selected == true || selectedShape != nullptr)
    {
        selected = false;
        selectedShape->setUndotted();
        clearCanvas();
        drawAllToCanvas();
        update();
    }
}

void MainWindow::deleteShape(Shape *shape)
{
    //shape is valid
    //search shape in shapes,must can found
    //delete from map
    deleteShapeFromMap(shape);
    for (auto it = shapes.begin(); it != shapes.end(); ++it)
    {
        if (*it == shape)
        {
            shapes.erase(it);
            break;
        }
    }
    //redraw
    clearCanvas();
    redrawAllShape();
    update();
}

void MainWindow::drawTempRect(QPoint &rectPoint1, QPoint &rectPoint2)
{
    clearCanvas();
    redrawAllShape();
    int xmin = rectPoint1.x() < rectPoint2.x() ? rectPoint1.x() : rectPoint2.x();
    int xmax = rectPoint1.x() > rectPoint2.x() ? rectPoint1.x() : rectPoint2.x();
    int ymin = rectPoint1.y() < rectPoint2.y() ? rectPoint1.y() : rectPoint2.y();
    int ymax = rectPoint1.y() > rectPoint2.y() ? rectPoint1.y() : rectPoint2.y();
    for (int i = xmin + 1; i < xmax; ++i)
    {
        canvasPainter->drawPoint(QPoint(i, ymin));
        canvasPainter->drawPoint(QPoint(i, ymax));
    }
    for (int i = ymin + 1; i < ymax; ++i)
    {
        canvasPainter->drawPoint(QPoint(xmin, i));
        canvasPainter->drawPoint(QPoint(xmax, i));
    }
    update();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800, 800);
    canvas = nullptr;
    canvasPainter = nullptr;
    currentColor = Qt::black;
    state = NoInput;
    drawntempLine = false;
    drawntempEllipse = false;
    currentMinId = -1;

    pressed = false;
    selected = false;
    selectedShape = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetCanvas(int w, int h)
{
    if (canvas != nullptr)
    {
        delete canvasPainter; //注意两个delete的先后顺序
        delete canvas;
    }
    canvas = new QPixmap(w, h);
    canvas->fill(Qt::white);
    canvasPainter = new QPainter(canvas);
    //release all shapes
    for (Shape *shape : shapes)
    {
        delete shape;
    }
    shapes.clear();
    currentMinId = -1; //全没了
    update();
    pointIdMap.clear(); //清空 map
}

void MainWindow::saveCanvas(QString name)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    canvas->save(name + ".bmp");
}

void MainWindow::setColor(int r, int g, int b, bool setByFile)
{
    currentColor.setRgb(r, g, b);
    if(setByFile){
        int w = 20;
        int h =20;
        QPixmap curColorPix(w,h);
        QPainter painter(&curColorPix);
        painter.fillRect(0,0,w,h,currentColor);
        QIcon icon(curColorPix);
        ui->actionCurrentColor->setIcon(icon);
    }
}

void MainWindow::drawLine(QPoint startPosition, QPoint endPosition, int type, int id, bool drawAll, bool updateMap)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    shapes.append(new Line(startPosition, endPosition, type, id, currentColor));
    if (drawAll)
    {
        drawAllToCanvas(); //是只画新添加的还是全画
    }
    else
        drawLastToCanvas();
    update();
    if (updateMap == true)
        updatePointIdMap();
}

void MainWindow::drawEllipse(QPoint center, int rx, int ry, int id, bool drawAll, bool updateMap)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    shapes.append(new Ellipse(center, rx, ry, id, currentColor));
    if (drawAll)
    {
        drawAllToCanvas(); //是只画新添加的还是全画
    }
    else
        drawLastToCanvas();
    update();
    if (updateMap == true)
        updatePointIdMap();
}

void MainWindow::drawPolygon(QVector<QPoint> &points, int type, int id, bool updateMap)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    shapes.append(new Polygon(points, type, id, currentColor));
    redrawAllShape();//may cause some space white
    update();
    if (updateMap == true)
        updatePointIdMap();
}

void MainWindow::drawCurve(QVector<QPoint> &points, int type, int id, bool updateMap)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    shapes.append(new Curve(points, type, id, currentColor));
    redrawAllShape();
    update();
    if (updateMap == true)
        updatePointIdMap();
}

void MainWindow::translate(Shape *shape, int dx, int dy)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (shape != nullptr)
    {
        deleteShapeFromMap(shape);
        shape->translateShape(dx, dy);
        //清空、重画所有图形
        //        canvasPainter->eraseRect(canvas->rect());//清空
        clearCanvas();
        redrawAllShape();
        update(); //dont forget it
        addShapeFromMap(shape);
    }
}

//in fact,pass in Shape* is enough,angel
void MainWindow::rotate(Shape *shape, QPoint &center, int r, bool needDelete)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (shape != nullptr)
    {
        if (needDelete)
            deleteShapeFromMap(shape);
        shape->rotateShape(center, r);//将关键点旋转，再重新计算所有的像素点
        //清空、重画所有图形
        clearCanvas();
        redrawAllShape();
        update(); //dont forget it
        addShapeFromMap(shape);
    }
}

void MainWindow::simpleRotate(Shape *shape, QPoint &center, double r)
{
    //should handle original points:save original points
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    canvasPainter->setPen(Qt::white);
    if (shape != nullptr)
    {
        shape->points = originalPoints;
        myRotateArc(shape->points, center, r);//简单地将所有像素点旋转，关键点没有旋转
        clearCanvas();
        drawAllToCanvas();
        update();
    }
}

void MainWindow::scale(Shape *shape, QPoint &center, double s)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (shape != nullptr)
    {
        deleteShapeFromMap(shape);
//        shape->points = originalPoints; //写错了，这个没啥用，应该缩放关键点
        shape->scale(center, s);        //缩放关键点后再重新画
        //清空、重画所有图形
        //        canvasPainter->eraseRect(canvas->rect());//清空
        clearCanvas();
        redrawAllShape();
        update(); //dont forget it
        addShapeFromMap(shape);
    }
}

void MainWindow::clip(Shape *shape, QPoint &rect1, QPoint &rect2, int algorithm)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (shape != nullptr)
    {
        deleteShapeFromMap(shape);
        bool accept = shape->clip(rect1, rect2, algorithm);
        if (accept == false)
        {
            //remove entire shape
            deleteShape(shape);
        }
        clearCanvas();
        redrawAllShape();
        update(); //dont forget it
        addShapeFromMap(shape);
    }
}

void MainWindow::on_actionDrawPolygon_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //    if (this->state == DrawPolygon)
    //    {
    //        this->state = NoInput; //两次点击，取消
    //        qDebug()<<"press drawPolygon twice,canceled";
    //        return;
    //    }
    //当初窗口输入边数，进行检查
    PolygonDialog polygonDialog;
    if (polygonDialog.exec() == QDialog::Accepted)
    {
        lineNum = polygonDialog.getLineNum();
        polygonType = polygonDialog.getType() == "DDA" ? DDA : Bresenham;
        state = DrawPolygon;
        qDebug() << "enter drawEllipse mode";
    }
    else
    {
        qDebug() << "canceled";
    }
}

void MainWindow::on_actionDrwaCurve_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //    if (this->state == DrawCurve)
    //    {
    //        this->state = NoInput; //两次点击，取消
    //        qDebug()<<"press drawCurve twice,canceled";
    //        return;
    //    }
    CurveDialog curveDialog;
    if (curveDialog.exec() == QDialog::Accepted)
    {
        curvePtsNum = curveDialog.getCtlNum();
        curveType = curveDialog.getCurveType() == "Bezier" ? Bezier : BSpline;
        state = DrawCurve;
        qDebug() << "enter drawCurve mode";
    }
    else
    {
        qDebug() << "canceled";
    }
}

void MainWindow::on_actionDrawLine_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //    if (this->state == DrawLine)
    //    {
    //        this->state = NoInput; //两次点击，取消
    //        qDebug()<<"press drawLine twice,canceled";
    //        return;
    //    }
    LineDialog lineDialog;
    if (lineDialog.exec() == QDialog::Accepted)
    {
        lineType = lineDialog.getLineType() == "DDA" ? DDA : Bresenham;
        //应该把之前的特殊红色点删除
        eraseMouseEventPoint();
        clickPoint.clear();
        resetSelected();
        this->state = DrawLine; //切换到画线状态
        qDebug() << "enter DrawLine mode";
    }
    else
    {
        qDebug() << "canceled";
    }
}

void MainWindow::on_actionresetCanvas_triggered()
{
    //弹出对话框输入画布尺寸
    resetCanvasDialog dlog;
    if (dlog.exec() == QDialog::Accepted)
    {
        int w = dlog.getWidth();
        int h = dlog.getHeight();
        qDebug() << "press ok,w=" << w << "h=" << h;
        resetCanvas(w, h);
    }
    else
    {
        qDebug() << "canceled";
    }
}

void MainWindow::on_actionDrawEllipse_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    eraseMouseEventPoint();
    clickPoint.clear();
    resetSelected();
    state = DrawEllipse;
    qDebug() << "enter drawEllipse mode";
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (canvas == nullptr)
    { //还没有画布
        return;
    }
    QPainter painter(this);
    painter.drawPixmap(RIGHT_SPACE, UP_SPACE, *canvas); //把画布内容画到窗口
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (event->button() == Qt::RightButton)
    { //不响应右键
        return;
    }
    pressed = true;
    switch (this->state)
    {
    case FromFile:
        break; //TODO:modify
    case DrawLine:
    {
        QPoint curPoint = event->pos(); //得到在Widget的位置，要转化成在canvas的位置-150,-150
        fixPoint(curPoint);
        clickPoint.append(curPoint);
    }
    break;
    case DrawEllipse:
    {
        QPoint curPoint = event->pos(); //得到在Widget的位置，要转化成在canvas的位置-150,-150
        fixPoint(curPoint);
        clickPoint.append(curPoint);
    }
    break;
    case DrawPolygon:
    {
        //这时候lineNum应该是合理的 >=3
        drawMouseEventPoint(event);
        //        clickPoint.append(event->pos());
        if (clickPoint.length() == lineNum)
        {
            //画够点了
            eraseMouseEventPoint();
            currentMinId--;
            drawPolygon(clickPoint, polygonType, currentMinId);
            clickPoint.clear();
        }
    }
    break;
    case TransLate:
    {
        //获取点击位置,决定是哪个id
        QPoint point = event->pos();
        fixPoint(point);
        Shape *shape = searchIdInMap(point);
        if (shape != nullptr)
        {
            selected = true;
            translateStart = point; //平移起点
            selectedShape = shape;
            removeHalfPoints(shape);    //按下的时候不需要全部重绘,只要覆盖一半就行了
            selectedShape->setDotted(); //虚线显示
        }
    }
    break;
    case Rotate:
    {
        QPoint point = event->pos();
        fixPoint(point);
        Shape *shape = searchIdInMap(point);
        if (shape != nullptr && shape->rotatable)
        {
            state = RotateSelected;
            selectedShape = shape;
            removeHalfPoints(shape);
            selectedShape->setDotted(); //to early??
                                        //            deleteShapeFromMap(selectedShape); //对应的像素点从map中删除
            //if delete from map and then cancle rotate? this is a bug
            //delete it later?
        }
    }
    break;
    case RotateSelected:
    {
        //set center
        rotateCenter = event->pos();
        fixPoint(rotateCenter);
        state = GotCenter;
        //draw this center in red
        clearCanvas(); //after selected,remove selected mark(half red)
        redrawAllShape();
        drawMouseEventPoint(event);
    }
    break;
    case GotCenter:
    {
        //get start point
        rotateStartPoint = event->pos();
        //        drawMouseEventPoint(event);
        fixPoint(rotateStartPoint);
        if (rotateStartPoint == rotateCenter)
        {
            return; //will caouse r == nan
        }
        //then into moveing part
        //save original points first
        originalPoints = selectedShape->points; //set original points
        state = GotStart;
        deleteShapeFromMap(selectedShape); //对应的像素点从map中删除
    }
    break;
    case Scale:
    {
        QPoint point = event->pos();
        fixPoint(point);
        if (selected == false)
        {
            Shape *shape = searchIdInMap(point);
            if (shape != nullptr)
            {
                selected = true;
                //                rotateCenter = point; //name should be change,not rotate but scale,or just center
                selectedShape = shape;
                removeHalfPoints(shape);                //按下的时候不需要全部重绘,只要覆盖一半就行了
                selectedShape->setDotted();             //虚线显示
//                originalPoints = selectedShape->points; // 这些原始的点作为scale的对象
                state = ScaleSelected;
            }
        }
    }
    break;
    case ScaleSelected:
    {
        //set center
        QPoint point = event->pos();
        fixPoint(point);
        rotateCenter = point;
        //        selectedShape->setUndotted();
        drawMouseEventPoint(event); //draw center in red
        state = ScaleFinished;
    }
    break;
    case ScaleFinished:
    { //finished and click: back to scale
        selectedShape->setUndotted();
        selected = false;
        selectedShape = nullptr;
        state = Scale;
        clearCanvas();
        drawAllToCanvas();
        update();
    }
    break;
    case DeleteShape:
    {
        QPoint point = event->pos();
        fixPoint(point);
        Shape *shape = searchIdInMap(point);
        if (shape != nullptr)
        {
            //delete
            deleteShape(shape);
        }
    }
    break;
    case Clip:
    {
        QPoint point = event->pos();
        fixPoint(point);
        Shape *shape = searchIdInMap(point);
        if (shape != nullptr && shape->clipable)
        {
            state = ClipSelected;
            selectedShape = shape;
            removeHalfPoints(shape);
            selectedShape->setDotted();
        }
    }
    break;
    case ClipSelected:
    {
        QPoint point = event->pos();
        fixPoint(point);
        clickPoint.append(point);
        drawMouseEventPoint(event);
        state = ClipOneRectSelected;
    }
    break;
    case DrawCurve:
    {
        drawMouseEventPoint(event);
        if (clickPoint.length() == curvePtsNum)
        {
            //画够点了
            eraseMouseEventPoint();
            currentMinId--; //如果先鼠标画，后文件，会不会重复？？有重复的话，修改鼠标的，暂时不考虑这个问题
            drawCurve(clickPoint, curveType, currentMinId);
            clickPoint.clear();
        }
    }
    break;
    default:
    {
    }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (event->button() == Qt::RightButton)
    { //不响应右键
        return;
    }
    if (pressed == false)
    {
        return;
    }
    pressed = false;
    switch (state)
    {
    case DrawLine:
    {
        QPoint point = event->pos();
        fixPoint(point);
        //删除临时线,考虑写成函数?
        if (drawntempLine == true) //可能动都不动就松开，防止那种清空
        {
            delete shapes.last();
            shapes.remove(shapes.length() - 1);
        }

        clearCanvas();
        currentMinId--; //如果先鼠标画，后文件，会不会重复？？有重复的话，修改鼠标的，暂时不考虑这个问题
        drawLine(clickPoint.first(), point, lineType, currentMinId, true);
        drawntempLine = false;
        clickPoint.clear();
    }
    break;
    case DrawEllipse:
    {
        QPoint point = event->pos();
        fixPoint(point);
        //删除临时线,考虑写成函数?
        if (drawntempEllipse == true) //可能动都不动就松开，防止那种清空
        {
            delete shapes.last();
            shapes.remove(shapes.length() - 1);
        }

        clearCanvas();
        int x1 = clickPoint[0].x();
        int x2 = point.x();
        int y1 = clickPoint[0].y();
        int y2 = point.y();
        int rx = abs(x1 - x2) / 2;
        int ry = abs(y1 - y2) / 2;
        QPoint ellicenter((x1 + x2) / 2, (y1 + y2) / 2);
        currentMinId--;
        drawEllipse(ellicenter, rx, ry, currentMinId, true);
        drawntempEllipse = false;
        clickPoint.clear();
    }
    break;
    case TransLate:
    {
        if (selected == false)
            return;
        selected = false;
        QPoint point = event->pos();
        fixPoint(point);
        int dx = point.x() - translateStart.x();
        int dy = point.y() - translateStart.y();
        selectedShape->setUndotted();
        translate(selectedShape, dx, dy);
        selectedShape = nullptr;
    }
    break;
    case GotStart:
    {
        QPoint endPoint = event->pos();
        fixPoint(endPoint);
        int r = calculateRotateAngel(rotateCenter, rotateStartPoint, endPoint); //angel
        selectedShape->setUndotted();
        rotate(selectedShape, rotateCenter, r, false); //set map
        state = Rotate;
        selectedShape = nullptr;
        selected = false;
    }
    break;
    case ClipOneRectSelected:
    {
        QPoint point = event->pos();
        fixPoint(point);
        selectedShape->setUndotted();
        clip(selectedShape, clickPoint[0], point, clipType); //need add a comboboxS
        state = Clip;
        clickPoint.clear();
    }
    break;
    default:
    {
    }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    if (event->button() == Qt::RightButton)
    { //不响应右键
        return;
    }
    if (pressed == false)
    {
        return;
    }
    //判断state,如果是画线状态并且已经点击了一下，就不断地画
    switch (state)
    {
    case DrawLine:
    {
        if (clickPoint.length() == 1)
        {
            //已经点击了一下
            QPoint point = event->pos();
            fixPoint(point);
            //先删除之前的临时线，再画，会不会太慢?
            if (drawntempLine == true)
            {
                //抹去,上一个图形一定是shapes的最后一个
                delete shapes.last();
                shapes.remove(shapes.length() - 1);
            }
            drawntempLine = true;
            clearCanvas();
            drawLine(clickPoint.first(), point, lineType, -1, true, false); //暂时的图形ID都是-1
            //如果是鼠标移动过程中，drawXXX就不需要修改pointIdMap，因此可以加一个默认参数，决定要不要调用updatePointIDmap函数
        }
    }
    break;
    case DrawEllipse:
    {
        if (clickPoint.length() == 1)
        { //这应该是恒成立的
            //已经点击了一下
            QPoint point = event->pos();
            fixPoint(point);
            //先删除之前的临时线，再画，会不会太慢?
            if (drawntempEllipse == true)
            {
                //抹去,上一个图形一定是shapes的最后一个
                delete shapes.last();
                shapes.remove(shapes.length() - 1);
            }
            drawntempEllipse = true;
            clearCanvas();
            int x1 = clickPoint[0].x();
            int x2 = point.x();
            int y1 = clickPoint[0].y();
            int y2 = point.y();
            int rx = abs(x1 - x2) / 2;
            int ry = abs(y1 - y2) / 2;
            QPoint ellicenter((x1 + x2) / 2, (y1 + y2) / 2);
            drawEllipse(ellicenter, rx, ry, -1, true, false);
        }
    }
    break;
    case TransLate:
    {
        if (selected == false)
            return;
        QPoint point = event->pos();
        fixPoint(point);
        int dx = point.x() - translateStart.x();
        int dy = point.y() - translateStart.y();
        translateStart = point;           //迭代
                                          //        simpleTransLate(selectedShape->id, dx, dy);
        translate(selectedShape, dx, dy); //change subclass's parameter
    }
    break;
    case GotStart:
    {
        //hav got start point
        QPoint tmpPoint = event->pos();
        fixPoint(tmpPoint);
        //calculate r
        double r = calculateRotateArc(rotateCenter, rotateStartPoint, tmpPoint);
        //refer to code above(translate)
        simpleRotate(selectedShape, rotateCenter, r); //rotate originalPoints
        //没有修改map,也没有修改subClass内部的参数
    }
    break;
    case ClipOneRectSelected:
    {
        QPoint tmpPoint = event->pos();
        fixPoint(tmpPoint);
        drawTempRect(clickPoint[0], tmpPoint);
    }
    break;
    default:
    {
    }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (canvas == nullptr || state != ScaleFinished || selected == false || selectedShape == nullptr)
    { //没有画布或者不是缩放模式或者还没选中图形
        return;
    }
    if (event->delta() > 0)
    {
        scale(selectedShape, rotateCenter, 1.2);
    }
    else
    {
        scale(selectedShape, rotateCenter, 0.8);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //    fclose(fd);
}

void MainWindow::on_actionmove_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //应该把之前的特殊红色点删除
    eraseMouseEventPoint();
    clickPoint.clear();
    resetSelected();
    state = TransLate;
    qDebug() << "enter translate mode";
}

void MainWindow::on_actionRotate_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //应该把之前的特殊红色点删除
    eraseMouseEventPoint();
    clickPoint.clear();
    resetSelected();
    state = Rotate;
    qDebug() << "enter rotate mode";
}

void MainWindow::on_actionScale_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //应该把之前的特殊红色点删除
    eraseMouseEventPoint();
    clickPoint.clear();
    resetSelected();
    state = Scale;
    qDebug() << "enter scale mode";
}

void MainWindow::on_actionClipLine_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //应该把之前的特殊红色点删除
    eraseMouseEventPoint();
    clickPoint.clear();
    resetSelected();
    clipDialog dlog;
    if (dlog.exec() == QDialog::Accepted)
    {
        clipType = dlog.getClipTyoe() == "Cohen-Sutherland" ? Cohen : Liang;
        state = Clip;
        qDebug() << "enter clip mode";
    }
    else
    {
        qDebug() << "canceled";
    }
}

void MainWindow::on_actionOpenCommandFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "open file", ".");
    if (fileName.isEmpty())
        return;
    getCommandFromFile(fileName);
}

void MainWindow::on_actionSaveCanvas_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    resetSelected();
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", ".", ".bmp");
    if (fileName.isEmpty())
        return;
    saveCanvas(fileName);
}

void MainWindow::on_actionSetColor_triggered()
{
    QColorDialog qColorDialog;
    QColor color = qColorDialog.getColor();
    if(color.isValid()){
        setColor(color.red(),color.green(),color.blue());
        qDebug()<<"color changed";
        //要将这个颜色显示到一个Qaction上
        int w = 20;
        int h =20;
        QPixmap curColorPix(w,h);
        QPainter painter(&curColorPix);
        painter.fillRect(0,0,w,h,color);
        QIcon icon(curColorPix);
        ui->actionCurrentColor->setIcon(icon);
    }
    else{
        qDebug()<<"canceled";
    }
}

void MainWindow::on_actionCurrentColor_triggered()
{
    QColorDialog qColorDialog;
    QColor color = qColorDialog.getColor();
    if(color.isValid()){
        setColor(color.red(),color.green(),color.blue());
        qDebug()<<"color changed";
        //要将这个颜色显示到一个Qaction上
        int w = 20;
        int h =20;
        QPixmap curColorPix(w,h);
        QPainter painter(&curColorPix);
        painter.fillRect(0,0,w,h,color);
        QIcon icon(curColorPix);
        ui->actionCurrentColor->setIcon(icon);
    }
    else{
        qDebug()<<"canceled";
    }
}

void MainWindow::on_actiondeleteShape_triggered()
{
    if (canvas == nullptr)
    { //没有画布
        return;
    }
    //应该把之前的特殊红色点删除
    eraseMouseEventPoint();
    clickPoint.clear();
    resetSelected();
    this->state = DeleteShape;
    qDebug()<<"enter delete mode";
}
