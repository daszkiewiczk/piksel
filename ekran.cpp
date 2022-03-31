#include "ekran.h"
#include <QtMath>
#include <QMouseEvent>
#include <QPainter>
#include <QStack>
#define IM_SIZE 500


Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(IM_SIZE,IM_SIZE,QImage::Format_RGB32),
      im_tmp(IM_SIZE,IM_SIZE,QImage::Format_RGB32)
{
    im.fill(0);
    im_tmp.fill(0);
}
void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0,0,width(),height(),Qt::gray);
    p.drawImage(0,0,im);
    /*for(int i =0;i<10000; ++i)
    {

        p.fillRect(rand()%300,rand()%300,10,10,QColor(rand()%256,rand()%256,rand()%256));

    }
    update();*/
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    //qDebug("%d", e->button() == Qt::MiddleButton);
    if(index != bezier && index != bspline) im_tmp = im.copy();
    mouseStartPoint = e->pos();
    if(index == flood) floodFill(mouseStartPoint, im.pixelColor(mouseStartPoint), c);
    if(index == bezier || index == bspline)
    {
        im = im_tmp.copy();
        if(e->button() == Qt::RightButton && !moving)
        {
            for(int i = 0; i < controlPoints.size(); ++i)
            {
                if(tolerance > abs(controlPoints[i].x() - mouseStartPoint.x()))
                    if(tolerance > abs(controlPoints[i].y() - mouseStartPoint.y()))
                    {

                       // Ekran::unmarkPoint(i);
                        qDebug("usuwamy punkt o indeksie: %d",i);
                        controlPoints.remove(i);
                    }
            }
        }
        else if(e->button() == Qt::RightButton && moving)
        {
            for(int i = 0; i < controlPoints.size(); ++i)
            {
                if(tolerance > abs(controlPoints[i].x() - mouseStartPoint.x()))
                    if(tolerance > abs(controlPoints[i].y() - mouseStartPoint.y()))
                    {

                        qDebug("przesuwamy punkt o indeksie: %d",i);
                        isControlPointBeingMoved = true;
                        movedControlPoint = i;
                    }
            }
        }
        else
        {

           // Ekran::markPoint(mouseStartPoint.x(),mouseStartPoint.y());
            Ekran::addControlPoint(mouseStartPoint);


        }
        Ekran::draw();
    }
    //qDebug("press point: x: %d y: %d", mouseStartPoint.x(), mouseStartPoint.y());
}
void Ekran::addControlPoint(QPoint pt)
{
    if(pt.x() < 0 || pt.x() >= IM_SIZE || pt.y() < 0 || pt.y() >= IM_SIZE) return;
    controlPoints.push_back(pt);
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    mouseEndPoint = e->pos();
    if(index == bezier || index == bspline)
    {
        if(isControlPointBeingMoved)
        {

            controlPoints[movedControlPoint] = mouseEndPoint;
            if(moving == true) im = im_tmp.copy();
        }
        draw();
    }
    else
    {
        im = im_tmp.copy();
    }
        //qDebug("move point: x: %d y: %d", mouseEndPoint.x(), mouseEndPoint.y());
    Ekran::draw();
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint mouseEndPoint = e->pos();
    if(index == bezier) isControlPointBeingMoved = false;
    qDebug("press point: x: %d y: %d", mouseStartPoint.x(), mouseStartPoint.y());
    qDebug("release point: x: %d y: %d", mouseEndPoint.x(), mouseEndPoint.y());
    Ekran::draw();
}

void Ekran::markPoint(int x, int y)
{
    Ekran::drawCircle(x,y,tolerance);
}
/*
void Ekran::unmarkPoint(int i)
{
    Ekran::drawLine(controlPoints[i].x()-tolerance,controlPoints[i].y()-tolerance,controlPoints[i].x()+tolerance,controlPoints[i].y()+tolerance);
}*/
void Ekran::putPixel(int x, int y)
{
    uchar *pix = im.scanLine(y);
    if(x >= 0 && x < IM_SIZE && y >= 0 && y < IM_SIZE)
    {
        pix[4*x] = c.blue();
        pix[4*x+1] = c.green();
        pix[4*x+2] = c.red();
    }
}
void Ekran::putPixel(int x, int y, int c)
{
    c = c % 256;
    uchar *pix = im.scanLine(y);
    if(x > 0 && x < IM_SIZE && y > 0 && y < IM_SIZE)
    {
        pix[4*x] = c;
        pix[4*x+1] = c;
        pix[4*x+2] = c;
    }
}

void Ekran::draw()
{
    int x1 = mouseStartPoint.x();
    int y1 = mouseStartPoint.y();
    int x2 = mouseEndPoint.x();
    int y2 = mouseEndPoint.y();
    if(index == linia) Ekran::drawLine(x1, y1, x2, y2);
    if(index == okrag)
    {
        int R = sqrt((mouseStartPoint.x()-mouseEndPoint.x())*(mouseStartPoint.x()-mouseEndPoint.x()) + (mouseStartPoint.y()-mouseEndPoint.y())*(mouseStartPoint.y()-mouseEndPoint.y()));
        //qDebug("promien : %d", R);
        Ekran::drawCircle(mouseStartPoint.x(),mouseStartPoint.y(),R);
    }
    if(index == elipsa)
    {
        int x0 = (x1+x2)/2;
        int y0 = (y1+y2)/2;
        int R1 = (x2 - x1)/2;
        int R2 = (y2 - y1)/2;
        //qDebug("x0: %d y0: %d x1: %d y1: %d ",x0,y0,x1,y1);
        Ekran::drawElipse(x0,y0,R1,R2);
    }
    if(index == bezier)
    {
        Ekran::drawBezier();
        for(int i = 0; i < controlPoints.size(); ++i)
        {
            Ekran::markPoint(controlPoints[i].x(),controlPoints[i].y());
        }
    }
    if(index == bspline)
    {
        Ekran::drawBspline();

        for(int i = 0; i < controlPoints.size(); ++i)
        {
            Ekran::markPoint(controlPoints[i].x(),controlPoints[i].y());
        }
    }
}

void Ekran::drawLineDebug(int x1, int y1, int x2, int y2)
{
    //rysowanie prostej z uzyciem rownania parametrycznego prostej
    float L = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    float alfa;
    int x, y;
    for(int i = 0; i <=L; ++i)
    {
        alfa = i/L;
        x = (1-alfa)*x1 + alfa*x2;
        y = (1-alfa)*y1 + alfa*y2;
        Ekran::putPixel(x,y);
    }
    update();
}
void Ekran::drawLine(int x1, int y1, int x2, int y2)
{
    //rysowanie prostej z uzyciem rownania kierunkowego prostej
    int y, x, dx = std::abs(x1-x2), dy = std::abs(y1-y2);
    float a=0,b;

    if(dx >= dy)
    {
        if(x2 < x1)
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }

        if(x1 != x2) a = (float)(y1-y2)/(x1-x2);
         b = y1-a*x1;
         for(int x = x1; x <= x2; ++x)
         {
             y = round(a*x+b);
             Ekran::putPixel(x,y);
          }
    } else {
        if(y2 < y1)
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        if(y1 != y2) a = (float)(x1-x2)/(y1-y2);
        b = x1-a*y1;
        for(int y = y1; y <= y2; ++y)
        {
            x = round(a*y+b);
            Ekran::putPixel(x,y);
        }
    }
    update();
}
void Ekran::drawCircle(int x0, int y0, int R)
{
    int b = R*sqrt(2)/2;
    for(int x = 0; x <= b; ++x)
    {
        int y = sqrt(R*R-x*x);
        Ekran::putPixel(x0+x,y0+y);
        Ekran::putPixel(x0+-x,y0+y);
        Ekran::putPixel(x0+x,y0+-y);
        Ekran::putPixel(x0+-x,y0+-y);
        Ekran::putPixel(x0+y,y0+x);
        Ekran::putPixel(x0+-y,y0+x);
        Ekran::putPixel(x0+y,y0+-x);
        Ekran::putPixel(x0+-y,y0+-x);
    }
    update();
}
void Ekran::drawElipse(int x0,int y0,int R2,int R1)
{
    float alfa;
    int x,y,x_last,y_last;
    x_last = R2;
    y_last = 0;
    for(int i = 1; i <= n; ++i)
    {
        alfa = 2 * M_PI/n *i;
        x = R2 * cos(alfa);
        y = R1 * sin(alfa);
        drawLine(x0+x,y0+y,x0+x_last,y0+y_last);
        x_last = x;
        y_last = y;
    }
    update();
}

void Ekran::drawBezier()
{
    if(controlPoints.size() < 4) return;
    float x,y,xLast,yLast,t;
    for(int i = 0; i <= (controlPoints.size()-4)/3; ++i)
    {
        xLast = controlPoints[3*i].x();
        yLast = controlPoints[3*i].y();
        for(int j = 0; j <= n; ++j)
        {
            t = (float)j/(float)n;
            x = pow(1-t,3)*controlPoints[3*i+0].x() + 3*pow(1-t,2)*t*controlPoints[3*i+1].x() + 3*(1-t)*t*t*controlPoints[3*i+2].x() + pow(t,3)*controlPoints[3*i+3].x();
            y = pow(1-t,3)*controlPoints[3*i+0].y() + 3*pow(1-t,2)*t*controlPoints[3*i+1].y() + 3*(1-t)*t*t*controlPoints[3*i+2].y() + pow(t,3)*controlPoints[3*i+3].y();
           // qDebug("drawBezier: %f %f", x, y);
            Ekran::drawLine((int)x,(int)y,(int)xLast,(int)yLast);
            xLast = x;
            yLast = y;
        }
    }
}

void Ekran::drawBspline()
{
    if(controlPoints.size() < 4) return;
    //float x,y;

    float x,y,xLast,yLast,t;
    for(int i = 0; i <= controlPoints.size()-4; ++i)
    {
        xLast = controlPoints[i+0].x()/6
                + 4*controlPoints[i+1].x()/6
                + controlPoints[i+2].x()/6;
        yLast = controlPoints[i+0].y()/6
                + 4*controlPoints[i+1].y()/6
                + controlPoints[i+2].y()/6;
        for(int j = 1; j <= n; ++j)
        {
            t = (float)j/n;
            x =     (-pow(t,3)+3*pow(t,2)-3*t+1)*controlPoints[i+0].x()/6
                    +(3*pow(t,3)-6*pow(t,2)+4)*controlPoints[i+1].x()/6
                    +(-3*pow(t,3)+3*pow(t,2)+3*t+1)*controlPoints[i+2].x()/6
                    +pow(t,3)*controlPoints[i+3].x()/6;
            y =     (-pow(t,3)+3*pow(t,2)-3*t+1)*controlPoints[i+0].y()/6
                    +(3*pow(t,3)-6*pow(t,2)+4)*controlPoints[i+1].y()/6
                    +(-3*pow(t,3)+3*pow(t,2)+3*t+1)*controlPoints[i+2].y()/6
                    +pow(t,3)*controlPoints[i+3].y()/6;
                    // qDebug("drawBezier: %f %f", x, y);
            //Ekran::putPixel((int)x,(int)y);
            Ekran::drawLine((float)xLast,(float)yLast,(float)x,(float)y);
            xLast = x;
            yLast = y;
        }
    }
}

void Ekran::floodFill(QPoint p0, QColor oldColor, QColor newColor)
{
    if(oldColor == newColor) return;
    QPoint p;
    QStack<QPoint> Q;
    Q.push(p0);
    //qDebug("old color : " + oldColor.name().toLatin1());
    auto isValid = [&](int x, int y) {if(x >= 0 && x < IM_SIZE && y >= 0 && y < IM_SIZE) return true; else return false;};
    while(!Q.empty())
    {
        p = Q.pop();
        //qDebug("flood fill p : %d %d",p.x(), p.y());
        if(im.pixelColor(p.x(), p.y()) == oldColor)
        {
            //qDebug("pixel color : " + im.pixelColor(p.x(), p.y()).name().toLatin1());

            //qDebug("affirmative");
            im.setPixel(p, newColor.rgb());
            if(isValid(p.x()-1, p.y())) Q.push(QPoint(p.x()-1, p.y()));
            if(isValid(p.x()+1, p.y())) Q.push(QPoint(p.x()+1, p.y()));
            if(isValid(p.x(), p.y()-1)) Q.push(QPoint(p.x(), p.y()-1));
            if(isValid(p.x(), p.y()+1)) Q.push(QPoint(p.x(), p.y()+1));
        }
    }
    update();
    return;
}

void Ekran::chooseColor()
{
    c = QColorDialog::getColor();
}

void Ekran::setMoving(int arg)
{
    if(arg == 2) moving = true;
    else moving = false;
    qDebug("moving: %d", moving);
}

void Ekran::setIndex(int index)
{
    this->index = (opcja)index;
    qDebug("ekran index: %d", index);
}

void Ekran::setN(int value)
{
    n = value;
    qDebug("ekran n: %d", n);
}
