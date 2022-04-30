#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rgbhsvwindow.h"
#include "alphablendingwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),
            ui->widget,SLOT(setIndex(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),
            ui->widget,SLOT(setN(int)));
    connect(ui->kernelSpinBox,SIGNAL(valueChanged(int)),
            ui->widget,SLOT(setKernel(int)));

    connect(ui->spinBox,SIGNAL(valueChanged(int)),
            ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            ui->spinBox,SLOT(setValue(int)));

    connect(ui->pushButton,SIGNAL(clicked()),
            ui->widget,SLOT(chooseColor()));

    connect(ui->checkBox,SIGNAL(stateChanged(int)),
            ui->widget,SLOT(setMoving(int)));

    connect(ui->rgbHsvButton,SIGNAL(clicked()),
            this,SLOT(openRgbHsvWindow()));
    connect(ui->alphaBlendingButton,SIGNAL(clicked()),
            this,SLOT(openAlphaBlendingWindow()));
    connect(ui->loadPictureButton,SIGNAL(clicked()),
            ui->widget,SLOT(loadPicture()));

    connect(ui->dilateButton,SIGNAL(clicked()),
            ui->widget,SLOT(dilate()));
    connect(ui->erodeButton,SIGNAL(clicked()),
            ui->widget,SLOT(erode()));
    connect(ui->openButton,SIGNAL(clicked()),
            ui->widget,SLOT(open()));
    connect(ui->closeButton,SIGNAL(clicked()),
            ui->widget,SLOT(close()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openRgbHsvWindow()
{
    RgbHsvWindow w;
    w.setModal(true);
    w.exec();
}
void MainWindow::openAlphaBlendingWindow()
{
    AlphaBlendingWindow w;
    w.setModal(true);
    w.exec();
}


/*void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug("index: %d", index);
}*/



/*void MainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug("%d",arg1);
}*/

