#include "myalgorithm.h"
#include <cmath>
#include <QDebug>

#define PI 3.1415926

QVector<QPoint> drawLineDda(QPoint startPoint, QPoint endPoint, bool drawStartPoint) //默认为true
{
    //如果是画多边形的一部分，可能就不需要画最后一个点（原先的）
    QVector<QPoint> ret;

    int absDelteX = abs(endPoint.x() - startPoint.x());
    int absDelteY = abs(endPoint.y() - startPoint.y());

    bool swapped = false;

    //special case
    if (absDelteX == 0)
    {
        if (startPoint.y() > endPoint.y())
        {
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }
        //vertical line
        if (drawStartPoint == true || swapped == true) //收尾都要画或者交换过了
            ret.append(startPoint);                    //add startPoint
        int x = startPoint.x();
        int y = startPoint.y();
        for (int i = 0; i < absDelteY - 1; ++i)
        {
            y = y + 1; //m can be +/-
            ret.append(QPoint(x, y));
        }
        if (drawStartPoint == true || swapped == false) //收尾都要画或者交换过了
        {
            y++;
            ret.append(QPoint(x, y));
        }
        return ret;
    }

    if (absDelteY == 0)
    {
        if (startPoint.x() > endPoint.x())
        { //switch two point
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }
        if (drawStartPoint == true || swapped == true) //收尾都要画或者交换过了
            ret.append(startPoint);
        //horizon line
        int x = startPoint.x();
        int y = startPoint.y();
        for (int i = 0; i < absDelteX - 1; ++i)
        {
            x = x + 1; //m can be +/-
            ret.append(QPoint(x, y));
        }
        if (drawStartPoint == true || swapped == false) //收尾都要画或者交换过了
        {
            x++;
            ret.append(QPoint(x, y));
        }
        return ret;
    }

    double m = ((double)(endPoint.y() - startPoint.y())) / (endPoint.x() - startPoint.x());

    if (absDelteX > absDelteY)
    { //x + deletX
        //from left to right
        if (startPoint.x() > endPoint.x())
        { //switch two point
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }
        if (drawStartPoint == true || swapped == true) //收尾都要画或者交换过了
            ret.append(startPoint);

        double y = startPoint.y();
        int x = startPoint.x();
        for (int i = 0; i < absDelteX - 1; ++i)
        {
            y = y + m; //m can be +/-
            x = x + 1; //from left to right
            ret.append(QPoint(x, y));
        }
    }
    else
    {
        //from down to up
        if (startPoint.y() > endPoint.y())
        {
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }
        if (drawStartPoint == true || swapped == true) //收尾都要画或者交换过了
            ret.append(startPoint);

        double x = startPoint.x();
        int y = startPoint.y();
        double m_1 = 1 / m;
        for (int i = 0; i < absDelteY - 1; ++i)
        {
            y = y + 1;
            x = x + m_1;
            ret.append(QPoint(x, y));
        }
    }
    if (drawStartPoint == true || swapped == false)
        ret.append(endPoint); //append last point
    return ret;
}

QVector<QPoint> drawLineBresenham(QPoint startPoint, QPoint endPoint, bool drawStartPoint)
{
    QVector<QPoint> ret;

    int absDelteX = abs(endPoint.x() - startPoint.x());
    int absDelteY = abs(endPoint.y() - startPoint.y());

    //    ret.append(startPoint); //add startPoint
    bool swapped = false;
    //special case
    if (absDelteX == 0)
    {
        if (startPoint.y() > endPoint.y())
        {
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }
        //vertical line
        if (drawStartPoint == true || swapped == true) //收尾都要画或者交换过了
            ret.append(startPoint);
        int x = startPoint.x();
        int y = startPoint.y();
        for (int i = 0; i < absDelteY - 1; ++i)
        {
            y = y + 1; //m can be +/-
            ret.append(QPoint(x, y));
        }
        if (drawStartPoint == true || swapped == false) //收尾都要画或者交换过了
        {
            y++;
            ret.append(QPoint(x, y));
        }
        return ret;
    }

    if (absDelteY == 0)
    {
        if (startPoint.x() > endPoint.x())
        { //switch two point
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }
        //horizon line
        if (drawStartPoint == true || swapped == true) //收尾都要画或者交换过了
            ret.append(startPoint);
        int x = startPoint.x();
        int y = startPoint.y();
        for (int i = 0; i < absDelteX - 1; ++i)
        {
            x = x + 1; //m can be +/-
            ret.append(QPoint(x, y));
        }
        if (drawStartPoint == true || swapped == false) //收尾都要画或者交换过了
        {
            x++;
            ret.append(QPoint(x, y));
        }
        return ret;
    }

    double m = ((double)(endPoint.y() - startPoint.y())) / (endPoint.x() - startPoint.x());

    if (absDelteX > absDelteY)
    { //|m| < 1
        //from left to right
        if (startPoint.x() > endPoint.x())
        { //switch two point
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
            swapped = true;
        }

        int p = 2 * absDelteY - absDelteX;

        int y = startPoint.y();
        int x = startPoint.x();
        ret.append(startPoint);
        for (int i = 0; i < absDelteX - 1; ++i)
        {
            if (p <= 0)
            {
                x++;
                ret.append(QPoint(x, y));
                p = p + 2 * absDelteY;
            }
            else
            {
                x++;
                if (m > 0)
                {
                    y++;
                }
                else
                {
                    y--;
                }
                ret.append(QPoint(x, y));
                p = p + 2 * absDelteY - 2 * absDelteX;
            }
        }
    }
    else
    { //|m|>1
        //from down to up
        if (startPoint.y() > endPoint.y())
        {
            QPoint temp = QPoint(startPoint);
            startPoint = endPoint;
            endPoint = temp;
        }
        int p = 2 * absDelteX - absDelteY;

        int y = startPoint.y();
        int x = startPoint.x();
        for (int i = 0; i < absDelteY - 1; ++i)
        {
            if (p <= 0)
            { //the  closer point
                y++;
                ret.append(QPoint(x, y));
                p = p + 2 * absDelteX;
            }
            else
            {
                y++;
                if (m > 0)
                {
                    x++;
                }
                else
                {
                    x--;
                }
                ret.append(QPoint(x, y));
                p = p + 2 * absDelteX - 2 * absDelteY;
            }
        }
    }
    if (drawStartPoint == false && swapped == false)
    {
        //删除第一个点
        ret.removeFirst();
    }
    if (drawStartPoint == true || swapped == false)
        ret.append(endPoint); //append last point
    return ret;
}

QVector<QPoint> drawPolygonDda(QVector<QPoint> &points)
{
    //points中的点会被画两次
    QVector<QPoint> ret;
    QPoint prePoint = points[0];
    QPoint curPoint;
    for (int i = 1; i < points.length(); ++i)
    {
        curPoint = points[i];
        ret = ret + drawLineDda(prePoint, curPoint, false);
        prePoint = curPoint;
    }
    ret = ret + drawLineDda(curPoint, points[0], false);
    return ret; //may hace duplicated point between two lines,need remove? maybe not
}

QVector<QPoint> drawPolygonBresenham(QVector<QPoint> &points)
{
    QVector<QPoint> ret;
    QPoint prePoint = points[0];
    QPoint curPoint;
    for (int i = 1; i < points.length(); ++i)
    {
        curPoint = points[i];
        ret = ret + drawLineBresenham(prePoint, curPoint); //I remove the last "false"
        prePoint = curPoint;
    }
    ret = ret + drawLineBresenham(curPoint, points[0]); //I remove the last "false"
    return ret;                                         //may hace duplicated point between two lines,need remove? maybe not
}

void translate(QVector<QPoint> &points, int dx, int dy)
{
    for (int i = 0; i < points.length(); ++i)
    {
        points[i].setX(points[i].x() + dx);
        points[i].setY(points[i].y() + dy);
    }
}

QVector<QPoint> drawEllipseAlgorithm(QPoint center, int rx, int ry)
{
    int xCenter = center.x();
    int yCenter = center.y();
    int x, y;
    double p1, p2;
    x = 0;
    y = ry;
    p1 = ry * ry - rx * rx * ry + 0.25 * rx * rx;
    QVector<QPoint> ret;
    ret.append(QPoint(x, y));
    while (rx * rx * y > ry * ry * x)
    { //第一象限内靠近y轴内点的绘制
        if (p1 < 0)
        {
            p1 = p1 + 2 * ry * ry * (x + 1) + ry * ry;
            x = x + 1;
            y = y;
        }
        else
        {
            p1 = p1 + 2 * ry * ry * (x + 1) + ry * ry - 2 * rx * rx * (y - 1);
            x = x + 1;
            y = y - 1;
        }
        ret.append(QPoint(x, y));
    }
    p2 = 1.0 * ry * ry * (x + 0.5) * (x + 0.5) + 1.0 * rx * rx * (y - 1) * (y - 1) - 1.0 * rx * rx * ry * ry;
    while (y > 0)
    { //第一象限另外一区域
        if (p2 > 0)
        {
            p2 = p2 - 2 * rx * rx * (y - 1) + rx * rx;
            y = y - 1;
            x = x;
        }
        else
        {
            p2 = p2 - 2 * rx * rx * (y - 1) + rx * rx + 2 * ry * ry * (x + 1);
            y = y - 1;
            x = x + 1;
        }
        ret.append(QPoint(x, y));
    }
    int num1 = ret.length();
    //左右,起始会重复四个点
    bool deleteDuplicated = true; //删除重复点
    for (int i = 0; i < num1; ++i)
    {
        if (deleteDuplicated && (i == 0)) //第一个点重复
            continue;
        QPoint point = ret[i];
        ret.append(QPoint(-point.x(), point.y()));
    }
    int num2 = ret.length();
    //
    for (int i = 0; i < num2; ++i)
    {
        if (deleteDuplicated && (i == num1 - 1 || i == num2 - 1))
            continue;
        QPoint point = ret[i];
        ret.append(QPoint(point.x(), -point.y()));
    }
    for (int i = 0; i < ret.length(); ++i)
    {
        ret[i].setX(ret[i].x() + xCenter);
        ret[i].setY(ret[i].y() + yCenter);
    }
    return ret;
}

void myRotatePoint(QPoint &point, QPoint &center, int r)
{
    //use clockwise algorithm
    int xr = center.x();
    int yr = center.y();
    int x = point.x();
    int y = point.y();
    int dx = x - xr;
    int dy = y - yr;
    double arcR = (double)r / 180 * PI;
    int x1 = std::cos(arcR) * dx + std::sin(arcR) * dy + xr;
    int y1 = -std::sin(arcR) * dx + std::cos(arcR) * dy + yr;
    point.setX(x1);
    point.setY(y1);
}

void myRotate(QVector<QPoint> &points, QPoint &center, int r)
{
    for (QPoint &point : points)
    {
        myRotatePoint(point, center, r);
    }
}

int calculateRotateAngel(QPoint &center, QPoint &startPoint, QPoint &endPoint)
{
    //start != center
    //end != center
    int cx = center.x();
    int cy = center.y();
    int sx = startPoint.x();
    int sy = startPoint.y();
    int ex = endPoint.x();
    int ey = endPoint.y();
    bool startFromRightOfCenter = sx > cx;
    bool endAboveCenterSrtartLine = false;
    //calculate line function: y=kx+b => kx+b-y=0;
    double k = ((double)(sy - cy)) / (sx - cx);
    double b = (double)(cx * sy - sx * cy) / (cx - sx);
    if (k * ex + b - ey < 0)
    {
        endAboveCenterSrtartLine = true;
    }
    else
    {
        endAboveCenterSrtartLine = false;
    }
    int c2 = mydistance2(startPoint, endPoint);
    int a2 = mydistance2(center, endPoint);
    int b2 = mydistance2(center, startPoint);
    double ab = std::sqrt((double)a2 * b2);
    double cosTheta = (a2 + b2 - c2) / (2 * ab);
    double arccosTheta = std::acos(cosTheta);
    //from arc to angle
    int r = arccosTheta * 180 / PI;
    if ((startFromRightOfCenter && endAboveCenterSrtartLine) ||
        (startFromRightOfCenter == false && endAboveCenterSrtartLine == false))
    {
        r = 360 - r;
    }
    return r;
}

int mydistance2(QPoint &p1, QPoint &p2)
{
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

double calculateRotateArc(QPoint &center, QPoint &startPoint, QPoint &endPoint)
{
    int cx = center.x();
    int cy = center.y();
    int sx = startPoint.x();
    int sy = startPoint.y();
    int ex = endPoint.x();
    int ey = endPoint.y();
    bool startFromRightOfCenter = sx > cx;
    bool endAboveCenterSrtartLine = false;
    //calculate line function: y=kx+b => kx+b-y=0;
    double k = ((double)(sy - cy)) / (sx - cx);
    double b = (double)(cx * sy - sx * cy) / (cx - sx);
    if (k * ex + b - ey < 0)
    {
        endAboveCenterSrtartLine = true;
    }
    else
    {
        endAboveCenterSrtartLine = false;
    }
    int c2 = mydistance2(startPoint, endPoint);
    int a2 = mydistance2(center, endPoint);
    int b2 = mydistance2(center, startPoint);
    double ab = std::sqrt((double)a2 * b2);
    double cosTheta = (a2 + b2 - c2) / (2 * ab);
    double arccosTheta = std::acos(cosTheta);
    //from arc to angle

    if ((startFromRightOfCenter && endAboveCenterSrtartLine) ||
        (startFromRightOfCenter == false && endAboveCenterSrtartLine == false))
    {
        arccosTheta = 2 * PI - arccosTheta;
    }
    return arccosTheta;
}

void myRotateArc(QVector<QPoint> &points, QPoint &center, double r)
{
    for (QPoint &point : points)
    {
        myRotatePointArc(point, center, r);
    }
}

void myRotatePointArc(QPoint &point, QPoint &center, double r)
{
    int xr = center.x();
    int yr = center.y();
    int x = point.x();
    int y = point.y();
    int dx = x - xr;
    int dy = y - yr;
    int x1 = std::cos(r) * dx + std::sin(r) * dy + xr;
    int y1 = -std::sin(r) * dx + std::cos(r) * dy + yr;
    point.setX(x1);
    point.setY(y1);
}

void myScale(QVector<QPoint> &points, QPoint center, double s)
{
    int x = center.x();
    int y = center.y();
    int x1 = 0;
    int y1 = 0;
    for (QPoint &point : points)
    {
        x1 = point.x();
        y1 = point.y();
        x1 = x1 * s + (1 - s) * x;
        y1 = y1 * s + (1 - s) * y;
        point.setX(x1);
        point.setY(y1);
    }
}

void myScaleSinglePoint(QPoint &point, QPoint center, double s)
{
    int x = center.x();
    int y = center.y();
    int x1 = 0;
    int y1 = 0;
    x1 = point.x();
    y1 = point.y();
    x1 = x1 * s + (1 - s) * x;
    y1 = y1 * s + (1 - s) * y;
    point.setX(x1);
    point.setY(y1);
}

int computeCode(int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int code;
    code = INSIDE; // initialised as being inside of clip window

    if (x < xmin) // to the left of clip window
        code |= LEFT;
    else if (x > xmax) // to the right of clip window
        code |= RIGHT;
    if (y < ymin) // below the clip window
        code |= BOTTOM;
    else if (y > ymax) // above the clip window
        code |= TOP;

    return code;
}

//修改start,end
bool myClipCoHen(QPoint &rectPoint1, QPoint &rectPoint2, QPoint &start, QPoint &end)
{
    int xmin = rectPoint1.x() < rectPoint2.x() ? rectPoint1.x() : rectPoint2.x();
    int xmax = rectPoint1.x() > rectPoint2.x() ? rectPoint1.x() : rectPoint2.x();
    int ymin = rectPoint1.y() < rectPoint2.y() ? rectPoint1.y() : rectPoint2.y();
    int ymax = rectPoint1.y() > rectPoint2.y() ? rectPoint1.y() : rectPoint2.y();

    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();
    int outcode0 = computeCode(xmin, xmax, ymin, ymax, x0, y0);
    int outcode1 = computeCode(xmin, xmax, ymin, ymax, x1, y1);
    bool accept = false;

    int count = 10;//avoid dead loop. maybe double -> int can cause,but I have not yet encountered

    while (true && count > 0)
    {
        --count;
        if (!(outcode0 | outcode1))
        { //both ends inside window
            accept = true;
            break;
        }
        else if (outcode0 & outcode1)
        { //entire outside window
            break;
        }
        else
        {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;

            //找出在界外的点
            int outcodeOut = outcode0 ? outcode0 : outcode1;

            // 找出和边界相交的点
            // 使用点斜式 y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
            if (outcodeOut & TOP)
            {                                                         // point is above the clip rectangle
                x = x0 + (double)(x1 - x0) * (ymax - y0) / (y1 - y0); //会不会因为取整的问题出错？不会
                y = ymax;
            }
            else if (outcodeOut & BOTTOM)
            { // point is below the clip rectangle
                x = x0 + (double)(x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            }
            else if (outcodeOut & RIGHT)
            { // point is to the right of clip rectangle
                y = y0 + (double)(y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            }
            else if (outcodeOut & LEFT)
            { // point is to the left of clip rectangle
                y = y0 + (double)(y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            //两个端点都有可能在外面
            if (outcodeOut == outcode0)
            { //外面的点是p0，截去 p0到p的外部段
                x0 = (int)x;
                y0 = (int)y;
                outcode0 = computeCode(xmin, xmax, ymin, ymax, x0, y0); //现在p0 是在矩形内部
            }
            else
            {
                x1 = (int)x;
                y1 = (int)y;
                outcode1 = computeCode(xmin, xmax, ymin, ymax, x1, y1); //现在p1 是在矩形内部
            }
        }
    }
    if (accept)
    {
        start.setX(x0);
        start.setY(y0);
        end.setX(x1);
        end.setY(y1);
    }
    return accept;
}

bool myClipLiang(QPoint &rectPoint1, QPoint &rectPoint2, QPoint &start, QPoint &end)
{

    bool removeEntire = false;
    double u1 = 0,u2 = 1;
    int p[4],q[4];
    double r = 0;
    int xmin = rectPoint1.x() < rectPoint2.x() ? rectPoint1.x() : rectPoint2.x();
    int xmax = rectPoint1.x() > rectPoint2.x() ? rectPoint1.x() : rectPoint2.x();
    int ymin = rectPoint1.y() < rectPoint2.y() ? rectPoint1.y() : rectPoint2.y();
    int ymax = rectPoint1.y() > rectPoint2.y() ? rectPoint1.y() : rectPoint2.y();
    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();
    p[0] = x0 -x1;
    p[1] = x1 - x0;
    p[2] = y0 - y1;
    p[3] = y1 - y0;
    q[0] = x0 - xmin;
    q[1] = xmax - x0;
    q[2] = y0 - ymin;
    q[3] = ymax - y0;

    for(int i = 0 ;i < 4;++i){
        r = (double)q[i]/p[i];
        if(p[i] < 0){//out to in
            u1 = u1 > r ? u1:r;
        }
        else if(p[i] > 0){//in to out
            u2 = u2 < r ? u2:r;
        }
        else if(q[i] < 0){//parallized and outside
            removeEntire = true;
        }
    }
    if(removeEntire||u1 > u2){
        removeEntire = true;
    }
    else{
        removeEntire = false;
    }
    if(removeEntire == false){
        //get u1 -> u2
        int newx0 = x0 + (int)(u1*(x1 - x0));
        int newy0 = y0 + (int)(u1*(y1 - y0));
        int newx1 = x0 + (int)(u2*(x1 - x0));
        int newy1 = y0 + (int)(u2*(y1 - y0));
        start.setX(newx0);
        start.setY(newy0);
        end.setX(newx1);
        end.setY(newy1);
    }
    return !removeEntire;
}

long long myConbination(int n,int m){
    // 计算组合数C(n,n)，防止溢出
    long long ret = 1;
    for(int i = 0;i < m;++i){
        ret *= (n - i);
        ret /= (i + 1);//每次计算结束后，ret都是一个组合数，因此不会精度损失
    }
    return ret;
}

QVector<QPoint> myBezierCurve(QVector<QPoint> ctlPoints)
{
    //为了更好地确定step的大小，计算相邻点之间的总距离d,step = d * 5;
    int pointNum = ctlPoints.size();
    int sumDistances = mySumDistance(ctlPoints);
    int step = sumDistances * 2;
    //先计算所有的组合数
    QVector<long long> conbinations(pointNum);
    int maxLevel = pointNum - 1;//最高阶数
    //有几个点就要计算几个组合数
    //实际只计算前一半
    for(int i = 0;i < pointNum/2+1 ;++i){
        conbinations[i] = myConbination(maxLevel,i);
        conbinations[maxLevel - i] = conbinations[i];//奇数情况中间的重复了一下，小事
    }
    //第一个点到最后一个点的坐标差为 delteX 和 delteY，比较二者大小，决定从x还是从y开始累加计算u
    QVector<QPoint> ret;
    double u = 0;
//    int step = 300;//应该设置为多少？
    for(int i = 0;i<=step;++i){
        //计算x 和 y
        double x = 0,y = 0;
        for(int i = 0;i<pointNum;++i){
            int px = ctlPoints[i].x();
            x += pow(1-u,pointNum-1-i)*pow(u,i)*conbinations[i]*px;
            int py = ctlPoints[i].y();
            y += pow(1-u,maxLevel-i)*pow(u,i)*conbinations[i]*py;
        }
        ret.append(QPoint((int)x,(int)y));
        u += (double)1/step;
    }
    return ret;
}

QVector<QPoint> myBSplineCurve(QVector<QPoint> ctlPoints,int k)
{
    int sumDistance = mySumDistance(ctlPoints);
    QVector<QPoint> ret;
    int n = ctlPoints.size() - 1;
    //得到节点分割数组，间隔为1
    QVector<double> uis(n + k + 1);
    double count = 0;
    for(double& ui:uis){
        ui = count;
        count += 1;
    }
    double u = uis[k - 1];//从有效区间左端开始
    double step = count/(sumDistance*2);//一共走sumDiatance*3
    for(;u < uis[n+1];u += step){
        double x = 0;
        double y = 0;
        int i = 0;
        for(QPoint& point:ctlPoints){
            double b = getB(i,k,uis,u);
//            qDebug()<<b;
            x += point.x()*b;
            y += point.y()*b;
            ++i;
        }
        ret.append(QPoint((int)x,(int)y));
    }
    return ret;
}

double getB(int i, int k, QVector<double>& uis, double u)
{
    if(k == 1){
        double ui1 = uis[i];
        double ui2 = uis[i + 1];
        if(u >= ui1 && u <= ui2){
            return 1;
        }
        else
            return 0;
    }
    else{
        double B1 = getB(i,k-1,uis,u);
        double B2 = getB(i+1,k-1,uis,u);
        double coef1 = (u - uis[i])/(uis[i+k-1]-uis[i]);
        double coef2 = (uis[i+k]-u)/(uis[i+k]-uis[i+1]);
        return  B1*coef1 + B2*coef2;
    }
}

int mySumDistance(QVector<QPoint>& ctlPoints)
{
    QPoint prePoint = ctlPoints[0];
    QPoint curPoint;
    int pointNum = ctlPoints.size();
    int sumDistances = 0;
    for(int i = 1;i<pointNum;++i){
        curPoint = ctlPoints[i];
        sumDistances += (int)sqrt((double)mydistance2(prePoint,curPoint));
        prePoint = curPoint;
    }
    return sumDistances;
}
