#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QColorDialog>

enum opcja
{
    linia,
    okrag,
    elipsa,
    bezier,
    bspline,
    flood,
    scanline
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
    QImage im;
    QImage im_tmp;
    QPoint mouseStartPoint, mouseEndPoint, mouseMovePoint;
    QVector<QPoint> controlPoints;
    QColor c = Qt::white;
    opcja index=linia;
    int n = 3;
    int kernel = 1;
    int tolerance = 7;
    bool isControlPointBeingMoved = false;
    bool moving = false;
    int movedControlPoint;
    explicit Ekran(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void addControlPoint(QPoint);
    void markPoint(int x, int y);
    void unmarkPoint(int i);
    void putPixel(int x, int y);
    void putPixel(int x, int y, int c);
    void draw();
    void drawLineDebug(int x1, int y1, int x2, int y2);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawCircle(int x0, int y0, int R);
    void drawElipse(int x0, int y0, int R2, int R1);
    void drawBezier();
    void drawBspline();
    void floodFill(QPoint p0, QColor oldColor, QColor newColor);
    void drawPolygonScanline();
    void im2monochrome();
    void monochrome2binary();
    void invertColors();
private slots:
    void setIndex(int index);
    void setN(int value);
    void setKernel(int value);
    void chooseColor();
    void setMoving(int arg); 
    void loadPicture();
    void dilate();
    void erode();
    void open();
    void close();

signals:

};

#endif // EKRAN_H
