#include "rgbhsvwindow.h"
#include "ui_rgbhsvwindow.h"

#include <QWidget>
#include <QPainter>
#define IM_SIZE 400

RgbHsvWindow::RgbHsvWindow(QWidget *parent) :
    QDialog(parent),
    im(IM_SIZE,IM_SIZE,QImage::Format_RGB32),
    ui(new Ui::RgbHsvWindow)
{
    im.fill(0);
    ui->setupUi(this);

    connect(ui->redSlider,&QSlider::valueChanged,
            this,[&](int v){this->drawPalette('r',v);});
    connect(ui->greenSlider,&QSlider::valueChanged,
            this,[&](int v){this->drawPalette('g',v);});
    connect(ui->blueSlider,&QSlider::valueChanged,
            this,[&](int v){this->drawPalette('b',v);});

    connect(ui->hSlider,&QSlider::valueChanged,
            this,[&](int v){this->drawPalette('h',v);});
    connect(ui->sSlider,&QSlider::valueChanged,
            this,[&](int v){this->drawPalette('s',v);});
    connect(ui->vSlider,&QSlider::valueChanged,
            this,[&](int v){this->drawPalette('v',v);});


    connect(ui->redSlider,&QSlider::valueChanged,
            this,[&]()  {ui->greenSlider->setValue(0);
                        ui->blueSlider->setValue(0);
                        ui->hSlider->setValue(0);
                        ui->sSlider->setValue(0);
                        ui->vSlider->setValue(0);
    });
    //drawPalette('r',0);
}

void RgbHsvWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    //p.fillRect(0,0,width(),height(),Qt::gray);
    p.drawImage(0,0,im);
    /*for(int i =0;i<10000; ++i)
    {

        p.fillRect(rand()%300,rand()%300,10,10,QColor(rand()%256,rand()%256,rand()%256));

    }
    update();*/
}
int *RgbHsvWindow::hsv2rgb(double h, double s, double v)
{
    double r=0,g=0,b=0;
    //qDebug("h %d",h);
    double c = v*s;
    double hh = h/60;
    double x = c*(1-abs(fmod(hh,2)-1));
    double m = v-c;
    //qDebug("hh %d",hh);
    if(0 <= hh && hh <= 1)
    {
        r=c;g=x;
    }
    else if(1 < hh && hh <= 2)
    {
        r=x;g=c;
    }
    else if(2 < hh && hh <= 3)
    {
        g=c;b=x;
    }
    else if(3 < hh && hh <= 4)
    {
        g=x;b=c;
    }
    else if(4 < hh && hh <= 5)
    {
        r=x;b=c;
    }
    else if(5 < hh && hh <= 6)
    {
        r=c;b=x;
    }

    r+=m;g+=m;b+=m;

    int* rgb = new int[3];

    rgb[0]=r*255;
    rgb[1]=g*255;
    rgb[2]=b*255;

    //qDebug("%d %d %d",rgb[0],rgb[1],rgb[2]);
    return rgb;
}

void RgbHsvWindow::drawPalette(char color,int value)
{
    int* rgb;
    qDebug("color %c value %d",color,value);
    for(int  i = 0; i < IM_SIZE; ++i)
    {
        uchar *pix = im.scanLine(i);
        for(int j = 0; j < IM_SIZE; ++j)
        {
            if(color == 'r')
            {

                pix[4*j] = i*255/(IM_SIZE-1);
                pix[4*j+1] = j*255/(IM_SIZE-1);
                pix[4*j+2] = value;

            } else if(color == 'g')
            {
                pix[4*j] = i*255/(IM_SIZE-1);
                pix[4*j+1] = value;
                pix[4*j+2] = j*255/(IM_SIZE-1);
            } else if(color == 'b')
            {
                pix[4*j] = value;
                pix[4*j+1] = j*255/(IM_SIZE-1);
                pix[4*j+2] = i*255/(IM_SIZE-1);
            } else if(color == 'h')
            {
                //qDebug("h %d",value);
                rgb = hsv2rgb((double)value,(double)i/IM_SIZE,(double)j/IM_SIZE);
                pix[4*j] = rgb[2];
                pix[4*j+1] = rgb[1];
                pix[4*j+2] = rgb[0];
                delete[] rgb;
            } else if(color == 's')
            {
                rgb = hsv2rgb(i*360/IM_SIZE,(double)value/(double)99,(double)j/IM_SIZE);
                pix[4*j] = rgb[2];
                pix[4*j+1] = rgb[1];
                pix[4*j+2] = rgb[0];

                delete[] rgb;
            }
            else if(color == 'v')
            {
                rgb = hsv2rgb(i*360/IM_SIZE,(double)j/IM_SIZE,(double)value/(double)99);
                pix[4*j] = rgb[2];
                pix[4*j+1] = rgb[1];
                pix[4*j+2] = rgb[0];

                delete[] rgb;
            }
        }
    }
    update();
}
RgbHsvWindow::~RgbHsvWindow()
{
    delete ui;
}


