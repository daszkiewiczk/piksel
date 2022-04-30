#ifndef RGBHSVWINDOW_H
#define RGBHSVWINDOW_H

#include <QDialog>

namespace Ui {
class RgbHsvWindow;
}

class RgbHsvWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RgbHsvWindow(QWidget *parent = nullptr);
    ~RgbHsvWindow();
    QImage im;
protected:
    void paintEvent(QPaintEvent *);

    int* hsv2rgb(double h, double s, double v);
public slots:
    void drawPalette(char color, int value);
private slots:


private:
    Ui::RgbHsvWindow *ui;
};

#endif // RGBHSVWINDOW_H
