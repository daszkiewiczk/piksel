#include "alphablendingwindow.h"
#include "ui_alphablendingwindow.h"

#include <QWidget>
#include <QPainter>
#define IM_SIZE 500

AlphaBlendingWindow::AlphaBlendingWindow(QWidget *parent) :
    QDialog(parent),
    im(IM_SIZE,IM_SIZE,QImage::Format_RGB32),
    ui(new Ui::AlphaBlendingWindow)
{
    im.fill(0);
    ui->setupUi(this);
}
void AlphaBlendingWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,im);
    update();
}
AlphaBlendingWindow::~AlphaBlendingWindow()
{
    delete ui;
}
