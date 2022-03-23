#include "ekran.h"
#include <QtMath>
#include <QMouseEvent>
#include <QPainter>
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
    im_tmp = im.copy();
    mouseStartPoint = e->pos();
    if(index == 3)
    {
        if(e->button() == Qt::RightButton) Ekran::unmarkPoint(mouseStartPoint.x(),mouseStartPoint.y());
        else
        {

            Ekran::markPoint(mouseStartPoint.x(),mouseStartPoint.y());

            controlPoints.push_back(mouseStartPoint);
            qDebug("%d",(int)controlPoints.size());
            for(int i = 0; i < controlPoints.size(); ++i)
            {
                qDebug("qpoint %d : %d %d",i,controlPoints[i].x(), controlPoints[i].y());
            }

        }
        if(controlPoints.size() >= 4) Ekran::draw();
    }
    //qDebug("press point: x: %d y: %d", mouseStartPoint.x(), mouseStartPoint.y());
}
void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    if(index != 3) im = im_tmp.copy();
    mouseEndPoint = e->pos();
    //qDebug("move point: x: %d y: %d", mouseEndPoint.x(), mouseEndPoint.y());
    Ekran::draw();

}
void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint mouseEndPoint = e->pos();

    qDebug("press point: x: %d y: %d", mouseStartPoint.x(), mouseStartPoint.y());
    qDebug("release point: x: %d y: %d", mouseEndPoint.x(), mouseEndPoint.y());
    Ekran::draw();
}

void Ekran::markPoint(int x, int y)
{
    Ekran::drawCircle(x,y,tolerance);
}

void Ekran::unmarkPoint(int x, int y)
{
    for(int i = 0; i < controlPoints.size(); ++i)
    {
        if(x < abs(controlPoints[i].x() + tolerance))
            if(y < abs(controlPoints[i].y() + tolerance))
            {
                controlPoints.remove(i);
                //Ekran::drawLine(controlPoints[i.x()-tolera])
            }
    }
}
void Ekran::putPixel(int x, int y)
{
    uchar *pix = im.scanLine(y);
    if(x > 0 && x < IM_SIZE && y > 0 && y < IM_SIZE)
    {
        pix[4*x] = 255;
        pix[4*x+1] = 255;
        pix[4*x+2] = 255;
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
    if(index == 0) Ekran::drawLine(x1, y1, x2, y2);
    if(index == 1)
    {
        int R = sqrt((mouseStartPoint.x()-mouseEndPoint.x())*(mouseStartPoint.x()-mouseEndPoint.x()) + (mouseStartPoint.y()-mouseEndPoint.y())*(mouseStartPoint.y()-mouseEndPoint.y()));
        //qDebug("promien : %d", R);
        Ekran::drawCircle(mouseStartPoint.x(),mouseStartPoint.y(),R);
    }
    if(index == 2)
    {
        int x0 = (x1+x2)/2;
        int y0 = (y1+y2)/2;
        int R1 = (x2 - x1)/2;
        int R2 = (y2 - y1)/2;
        //qDebug("x0: %d y0: %d x1: %d y1: %d ",x0,y0,x1,y1);
        Ekran::drawElipse(x0,y0,R1,R2);
    }
    if(index == 3) Ekran::drawBezier();
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
    float x,y;
    for(float t = 0.0; t <= 1.0; t+=0.001)
    {
        x = pow(1-t,3)*controlPoints[0].x() + 3*pow(1-t,2)*t*controlPoints[1].x() + 3*(1-t)*t*t*controlPoints[2].x() + pow(t,3)*controlPoints[3].x();
        y = pow(1-t,3)*controlPoints[0].y() + 3*pow(1-t,2)*t*controlPoints[1].y() + 3*(1-t)*t*t*controlPoints[2].y() + pow(t,3)*controlPoints[3].y();
        qDebug("drawBezier: %d %d", x, y);
        Ekran::putPixel((int)x,(int)y);
    }
}

void Ekran::chooseColor()
{
    c = QColorDialog::getColor();
}

void Ekran::setIndex(int index)
{
    this->index = index;
    qDebug("ekran index: %d", index);
}

void Ekran::setN(int value)
{
    n = value;
    qDebug("ekran n: %d", n);
}
