#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "shape.h"
#include <QMap>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define UP_SPACE 50
#define RIGHT_SPACE 50

enum InputState{
    NoInput,//no execute any command
    FromFile,
    DrawLine,
    DrawPolygon,
    DrawEllipse,
    TransLate,//点击移动按钮
    Rotate,
    RotateSelected,//decide rotate which shape
    GotCenter,//decide center of rotate
    GotStart,
    Scale,
    ScaleSelected,
    ScaleFinished,
    DeleteShape,
    Clip,
    ClipSelected,
    ClipOneRectSelected,
    DrawCurve
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString saveDir;

    QVector<QPoint> tmpRotatePoints;

    QVector<QPoint> originalPoints;

    QPoint rotateCenter;

    QPoint rotateStartPoint;

    bool selected;//是否选中图形

    QPoint translateStart;//平移起点

    bool pressed;//左键是否按下

//    FILE*fd;

    Shape* selectedShape;

    QMap<QPoint,QVector<int>> pointIdMap;//一个点可能对应多个图元

    int lineType;

    int polygonType;

    int clipType;

    int curveType;


    int lineNum;//多边形边数

    int curvePtsNum;

    bool drawntempLine;//判断之前是不是画过临时线

    bool drawntempEllipse;

    QVector<QPoint> clickPoint;//点击的位置

    QString defaultStyleSheet;

    InputState state;

    QColor currentColor;//当前画笔颜色

    QVector<Shape*> shapes;

    QPixmap* canvas;//由于画布会关闭、重建，所以设置为指针，通过new delete创建删除,在resetCanvas中设置

    QPainter* canvasPainter;//对应canvas的产生

    int currentMinId;//当前小ID,用于鼠标创建图形，分配ID，总是分配当前最小ID-1
    //防止和正数的文件输入ID冲突

    Shape* search(int id);

    void parseLine(QString line);

    void drawLastToCanvas();//把最后一个图元叠加到canvas

    void drawAllToCanvas();

    void redrawAllShape();

    void drawShape(Shape*shape);

    void drawPoints(QVector<QPoint>&points);

    void drawMouseEventPoint(QMouseEvent*event);//用红色画出点击的坐标

    void eraseMouseEventPoint();

    void clearCanvas();//清空画布，但是shapes没有改变

    void fixPoint(QPoint&point);//窗口和画布位置不一样，设置一下

    void updatePointIdMap();//将新图元的点-id对加入到map

    void deleteShapeFromMap(Shape*shape);

    void addShapeFromMap(Shape*shape);

    Shape *searchIdInMap(QPoint&point);//查看这个point属于哪个图元(返回第一个)

    void removeHalfPoints(Shape*shape);//把一半的点画成白点

    QVector<QPoint> pointNeighbors(QPoint&point);//返回邻近的点

    QVector<QPoint> pointNeighbors2(QPoint&point);

    void resetSelected();//if pre action  interrupted,reset selected = false and selectedShape -> unditted

    void deleteShape(Shape* shape);

    void drawTempRect(QPoint &p1,QPoint&p2);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resetCanvas(int w,int h);//重置画布

    void saveCanvas(QString name);//保存画布到指定路径

    void setColor(int r,int g,int b,bool setByFile=false);//设置颜色，也就是改变pixmap对应painter的颜色

    void drawLine(QPoint startPosition,QPoint endPosition,int type,int id,bool drawAll=false,bool updateMap=true);//new 一个 Line，添加到shapes

    void drawEllipse(QPoint center,int rx,int ry,int id,bool drawAll=false,bool updateMap=true);

    void drawPolygon(QVector<QPoint>&points,int type,int id,bool updateMap=true);

    void drawCurve(QVector<QPoint>&points,int type,int id,bool updateMap=true);
    //change map and canvas
    void translate(Shape*shape,int dx,int dy);

    //only modify canvas,erase previous and draw current. also change points
//    void simpleTransLate(int id,int dx,int dy);

    //change map and canvas
    void rotate(Shape*shape,QPoint& center,int r,bool needDelete = true);

    //only modify canvas,erase previous and draw current. also change points
    void simpleRotate(Shape* shape,QPoint&center,double r);//in arc

    void scale(Shape* shape,QPoint&center,double s);//change map

//    void simpleScale(Shape* shape,QPoint&center,double s);//not change map

    void clip(Shape*shape,QPoint&rect1,QPoint&rect2,int algorithm);

    void getCommandFromFile(QString fileName);

    void setSaveDir(QString dir);
private slots:

    void on_actionDrawPolygon_triggered();

    void on_actionDrwaCurve_triggered();

    void on_actionDrawLine_triggered();

    void on_actionresetCanvas_triggered();

    void on_actionDrawEllipse_triggered();

    void on_actionmove_triggered();

    void on_actionRotate_triggered();

    void on_actionScale_triggered();

    void on_actionClipLine_triggered();

    void on_actionOpenCommandFile_triggered();

    void on_actionSaveCanvas_triggered();

    void on_actionSetColor_triggered();

    void on_actionCurrentColor_triggered();

    void on_actiondeleteShape_triggered();

private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void closeEvent(QCloseEvent *event);

};
#endif // MAINWINDOW_H
