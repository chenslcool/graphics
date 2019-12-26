#ifndef MYALGORITHM_H
#define MYALGORITHM_H
#include<QPoint>
#include<qvector.h>
#define DDA 1
#define Bresenham 2
#define Cohen 1
#define Liang 2
#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
#define Bezier 1
#define BSpline 2


//drawing part
QVector<QPoint> drawLineDda(QPoint startPoint,QPoint endPoint,bool drawStart=true);

QVector<QPoint> drawLineBresenham(QPoint startPoint,QPoint endPoint,bool drawStart=true);

QVector<QPoint> drawPolygonDda(QVector<QPoint>&points);

QVector<QPoint> drawPolygonBresenham(QVector<QPoint>&points);

QVector<QPoint> drawEllipseAlgorithm(QPoint center,int rx,int ry);


//TODO
//QVector<QPoint> drawCurveBezier(QVector<QPoint>points);

//TODO
//QVector<QPoint> drawCurveBSpline(QVector<QPoint>points);

//TODO
//moving part：translate,cut,rotate

void myScale(QVector<QPoint>&points,QPoint center,double s);

void myScaleSinglePoint(QPoint& point,QPoint center,double s);

void translate(QVector<QPoint>&points,int dx,int dy);

void myRotate(QVector<QPoint>&points,QPoint &center,int r);//clockwise

void myRotatePoint(QPoint&point,QPoint &center,int r);//rotate single

void myRotateArc(QVector<QPoint>&points,QPoint &center,double r);//clockwise

void myRotatePointArc(QPoint&point,QPoint &center,double r);//rotate single

int calculateRotateAngel(QPoint&center,QPoint&startPoint,QPoint&endPoint);//not in arc

double calculateRotateArc(QPoint&center,QPoint&startPoint,QPoint&endPoint);//inarc

int mydistance2(QPoint&p1,QPoint&p2);

//裁剪算法:参数为矩形的两个顶点，先计算出xmin,xmax,ymin,ymax。参数还有start,end两个点(引用)

bool myClipCoHen(QPoint&rectPoint1,QPoint&rectPoint2,QPoint&start,QPoint&end);

int computeCode(int xmin,int xmax,int ymin,int ymax,int x,int y);

bool myClipLiang(QPoint&rectPoint1,QPoint&rectPoint2,QPoint&start,QPoint&end);

long long myConbination(int n,int m);//计算组合数

int mySumDistance(QVector<QPoint>& points);

QVector<QPoint> myBezierCurve(QVector<QPoint> ctlPoints);

QVector<QPoint> myBSplineCurve(QVector<QPoint> ctlPoints,int k);

double getB(int i,int k,QVector<double>&uis,double u);
#endif // MYALGORITHM_H
