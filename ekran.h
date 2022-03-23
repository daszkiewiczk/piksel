#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include<QColorDialog>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    QImage im;
    QImage im_tmp;
    QPoint mouseStartPoint, mouseEndPoint;
    QVector<QPoint> controlPoints;
    QColor c;
    int index=0;
    int n=3;
    int tolerance = 7;
    explicit Ekran(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void markPoint(int x, int y);
    void unmarkPoint(int x, int y);
    void putPixel(int x, int y);
    void putPixel(int x, int y, int c);
    void draw();
    void drawLineDebug(int x1, int y1, int x2, int y2);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawCircle(int x0, int y0, int R);
    void drawElipse(int x0, int y0, int R2, int R1);
    void drawBezier();
    void chooseColor();
private slots:
    void setIndex(int index);
    void setN(int value);
signals:

};

#endif // EKRAN_H
