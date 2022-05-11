#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QVector>
#include <QColorDialog>
//#include <armadillo>

enum opcja
{
    linia,
    okrag,
    elipsa,
    bezier,
    bspline,
    flood,
    scanline,
    przeksztalcenia
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
    QImage im;
    QImage im_tmp;
    QImage transformedIm;
    QPoint mouseStartPoint, mouseEndPoint, mouseMovePoint;
    QVector<QPoint> controlPoints;
    QColor c = Qt::white;
    opcja index=przeksztalcenia;
    int n = 3;
    int kernel = 1;
    int tolerance = 7;
    bool isControlPointBeingMoved = false;
    bool moving = false;
    int movedControlPoint;
    float tx,ty,sx,sy,alfa;
    //QVector<QVector<float>> transformationMatrix{{1,0,0},{0,1,0},{0,0,1}};
    //QVector<QVector<float>> translationMatrix{{1,0,0},{0,1,0},{0,0,1}};
    //QVector<QVector<float>> scalingMatrix{{1,0,0},{0,1,0},{0,0,1}};
    //QVector<QVector<float>> rotationMatrix{{1,0,0},{0,1,0},{0,0,1}};
    //arma::mat transformationMatrix = arma::eye(3,3);
    //arma::mat translationMatrix = arma::eye(3,3);
    //arma::mat scalingMatrix = arma::eye(3,3);
    //arma::mat rotationMatrix = arma::eye(3,3);
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
    /*void setTx(int value);
    void setTy(int value);
    void setSx(int value);
    void setSy(int value);
    void transform();
    */void chooseColor();
    void setMoving(int arg); 
    void loadPicture();
    void dilate();
    void erode();
    void open();
    void close();

signals:

};

#endif // EKRAN_H
