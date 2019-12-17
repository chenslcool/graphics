#ifndef SHAPE_H
#define SHAPE_H
#include <QVector>
#include <QPoint>
#include <QColor>

class MainWindow;
class Shape{
protected:
    int id;
    QColor color;
    QVector<QPoint> points;
    bool dotted;//draw in dotted line
    bool rotatable;//ellipse is not rotatable
    bool clipable;
public:
    Shape();
    virtual void translateShape(int dx,int dy);
    virtual void rotateShape(QPoint&center,int r);//in angle
    virtual void rotateShapeArc(QPoint&center,double r);//in arc
    virtual void scale(QPoint&center,double s);//不同图像的scale不一样
    virtual bool clip(QPoint&rect1,QPoint&rect2,int algorithm);
    friend class MainWindow;
    void setDotted();
    void setUndotted();
    bool getDotted();
    virtual ~Shape(){}
};

class Line :public Shape{
private:
    QPoint startPoint;
    QPoint endPoint;
    int algorithm;
public:
    Line(int id=0);
    Line(QPoint startPoint,QPoint endPoint,int type,int id,QColor colr);
    void translateShape(int dx,int dy);//也许可以增加simpleTranslate，用originPoint辅助
    void scale(QPoint&center,double s);//start and end piont scale and then redraw
    void rotateShapeArc(QPoint&center,double r);
    void rotateShape(QPoint&center,int r);
    bool clip(QPoint&rect1,QPoint&rect2,int algorithm);
    virtual ~Line(){}
};

class Polygon :public Shape{
private:
    QVector<QPoint>lineStartEndPair;
    int algorithm;
public:
    Polygon(int id=0);
    Polygon(QVector<QPoint>& points,int type,int id,QColor colr);
    void translateShape(int dx,int dy);
    void scale(QPoint&center,double s);//scale each line and redraw
    void rotateShapeArc(QPoint&center,double r);
    void rotateShape(QPoint&center,int r);
    virtual ~Polygon(){}
};

class Ellipse :public Shape{
private:
    QPoint center;
    int rx;
    int ry;
public:
    Ellipse(int id=0);
    Ellipse(QPoint center,int rx,int ry,int id,QColor colr);
    void translateShape(int dx,int dy);
    void scale(QPoint&center,double s);//scale center and rx,ry,redraw
    virtual ~Ellipse(){}
};

//曲线支持平移、缩放、旋转
class Curve :public Shape{
private:
    const int DEFUALT_BSPLINE_K = 4;//默认三次B样条
    QVector<QPoint> ctlPoints;//控制点
    int algorithm;//曲线算法
public:
    Curve(int id = 0);
    Curve(QVector<QPoint>& points,int type,int id,QColor color);
    void translateShape(int dx,int dy);
    void scale(QPoint&center,double s);//scale center and rx,ry,redraw
    void rotateShapeArc(QPoint&center,double r);
    void rotateShape(QPoint&center,int r);
    virtual ~Curve(){}
};

#endif // SHAPE_H
