#include "shape.h"
#include "myalgorithm.h"
Shape::Shape()
{
    id=0;
    color=Qt::black;
    dotted=false;
    rotatable = true;
    clipable = false;
}

void Shape::translateShape(int dx, int dy)
{
    translate(points,dx,dy);//modify by referrence
}

void Shape::rotateShape(QPoint &center, int r)
{
    myRotate(points,center,r);
}

void Shape::rotateShapeArc(QPoint &center, double r)
{
    myRotateArc(points,center,r);
}

void Shape::scale(QPoint &center, double s)
{
    myScale(points,center,s);
}

bool Shape::clip(QPoint &rect1, QPoint &rect2,int algorithm)
{
    //do nothing
    return true;//accept
}

void Shape::setDotted()
{
    dotted=true;
}

void Shape::setUndotted()
{
    dotted=false;
}

bool Shape::getDotted()
{
    return dotted;
}

void Shape::setColor(QColor color)
{
    this->color = color;
}

QColor Shape::getColor()
{
    return this->color;
}


Line::Line(int id)
{
    this->algorithm = DDA;
    this->id=id;
}

Line::Line(QPoint startPoint, QPoint endPoint,int algorithm,int id,QColor color)
{
    this->clipable = true;
    this->algorithm = algorithm;
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    this->id=id;
    this->color=color;
    if(algorithm==DDA){
        this->points=drawLineDda(startPoint,endPoint);
    }
    else{
        this->points=drawLineBresenham(startPoint,endPoint);
    }
}

void Line::translateShape(int dx, int dy)
{
    //先调用父类transLate在修改start,end
    translate(points,dx,dy);
    startPoint.setX(startPoint.x()+dx);
    startPoint.setY(startPoint.y()+dy);
    endPoint.setX(endPoint.x()+dx);
    endPoint.setY(endPoint.y()+dy);
}

void Line::scale(QPoint &center, double s)
{
    myScaleSinglePoint(startPoint,center,s);
    myScaleSinglePoint(endPoint,center,s);
    points.clear();
    if(this->algorithm == DDA){
         points = drawLineDda(startPoint,endPoint);
    }
    else
        points = drawLineBresenham(startPoint,endPoint);
}

void Line::rotateShapeArc(QPoint &center, double r)
{
    //rotate two end and redraw
    myRotatePointArc(startPoint,center,r);
    myRotatePointArc(endPoint,center,r);
    points.clear();
    if(this->algorithm == DDA){
         points = drawLineDda(startPoint,endPoint);
    }
    else
        points = drawLineBresenham(startPoint,endPoint);
}

void Line::rotateShape(QPoint &center, int r)
{
    myRotatePoint(startPoint,center,r);
    myRotatePoint(endPoint,center,r);
    points.clear();
    if(this->algorithm == DDA){
         points = drawLineDda(startPoint,endPoint);
    }
    else
        points = drawLineBresenham(startPoint,endPoint);
}

bool Line::clip(QPoint &rect1, QPoint &rect2, int algorithm)
{
    if(algorithm == Cohen){
        bool accepted = myClipCoHen(rect1,rect2,startPoint,endPoint);
        if(accepted)//some points left,or delete entire shape
        {
            points.clear();
            if(this->algorithm == DDA){
                 points = drawLineDda(startPoint,endPoint);
            }
            else
                points = drawLineBresenham(startPoint,endPoint);
        }
        return accepted;
    }
    else{
        bool accepted = myClipLiang(rect1,rect2,startPoint,endPoint);
        if(accepted)//some points left,or delete entire shape
        {
            points.clear();
            if(this->algorithm == DDA){
                 points = drawLineDda(startPoint,endPoint);
            }
            else
                points = drawLineBresenham(startPoint,endPoint);
        }
        return accepted;
    }
}

Polygon::Polygon(int id)
{
    this->algorithm = DDA;
    this->id=id;
}

Polygon::Polygon(QVector<QPoint> &points, int algorithm,int id,QColor color)
{
    this->algorithm = algorithm;
    this->lineStartEndPair = points;//复制构造
    this->id=id;
    this->color=color;
    if(algorithm==DDA){
        this->points=drawPolygonDda(points);
    }
    else{
        this->points=drawPolygonBresenham(points);
    }
}

void Polygon::translateShape(int dx, int dy)
{
    translate(points,dx,dy);
    translate(lineStartEndPair,dx,dy);
}

void Polygon::scale(QPoint &center, double s)
{
    myScale(lineStartEndPair,center,s);
    points.clear();
    if(this->algorithm == DDA){
         points = drawPolygonDda(lineStartEndPair);
    }
    else
        points = drawPolygonBresenham(lineStartEndPair);
}

void Polygon::rotateShapeArc(QPoint &center, double r)
{
    myRotateArc(lineStartEndPair,center,r);
    points.clear();
    if(this->algorithm == DDA){
         points = drawPolygonDda(lineStartEndPair);
    }
    else
        points = drawPolygonBresenham(lineStartEndPair);
}

void Polygon::rotateShape(QPoint &center, int r)
{
    myRotate(lineStartEndPair,center,r);
    points.clear();
    if(this->algorithm == DDA){
         points = drawPolygonDda(lineStartEndPair);
    }
    else
        points = drawPolygonBresenham(lineStartEndPair);
}

Ellipse::Ellipse(int id)
{
    this->id=id;
    rotatable = false;
}

Ellipse::Ellipse(QPoint center,int rx,int ry,int id,QColor color)
{
    this->center = center;
    this->rx = rx;
    this->ry = ry;
    this->id=id;
    this->color=color;
    this->points=drawEllipseAlgorithm(center,rx,ry);
    this->rotatable = false;
}

void Ellipse::translateShape(int dx, int dy)
{
    translate(points,dx,dy);
    center.setX(center.x()+dx);
    center.setY(center.y()+dy);
}

void Ellipse::scale(QPoint &center, double s)
{
    myScaleSinglePoint(this->center,center,s);
    rx = (int)(rx * s);
    ry = (int)(ry * s);
    this->points.clear();
    this->points = drawEllipseAlgorithm(this->center,rx,ry);
}

Curve::Curve(int id)
{
    this->id = id;
    this->algorithm = Bezier;
}

Curve::Curve(QVector<QPoint> &points, int type, int id, QColor color)
{
    this->algorithm = type;
    this->ctlPoints = points;
    this->id=id;
    this->color=color;

    if(algorithm==Bezier){
        this->points=myBezierCurve(points);
    }
    else{
        this->points = myBSplineCurve(points,DEFUALT_BSPLINE_K);
    }
}

void Curve::translateShape(int dx, int dy)
{
    translate(points,dx,dy);
    translate(ctlPoints,dx,dy);//控制点也平移
}

void Curve::scale(QPoint &center, double s)
{
    myScale(ctlPoints,center,s);//控制点旋转
    points.clear();//情况之前的画的点
    //TODO do something like codes follow
    if(algorithm==Bezier){
        this->points=myBezierCurve(ctlPoints);
    }
    else{//TODO another case
        this->points = myBSplineCurve(ctlPoints,DEFUALT_BSPLINE_K);
    }
}

void Curve::rotateShapeArc(QPoint &center, double r)
{
    myRotateArc(ctlPoints,center,r);
    points.clear();
    if(algorithm==Bezier){
        this->points=myBezierCurve(ctlPoints);
    }
    else{//TODO another case
        this->points = myBSplineCurve(ctlPoints,DEFUALT_BSPLINE_K);
    }
}

void Curve::rotateShape(QPoint &center, int r)
{
    myRotate(ctlPoints,center,r);
    points.clear();
    if(algorithm==Bezier){
        this->points=myBezierCurve(ctlPoints);
    }
    else{//TODO another case
        this->points = myBSplineCurve(ctlPoints,DEFUALT_BSPLINE_K);
    }
}
