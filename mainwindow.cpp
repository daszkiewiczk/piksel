#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),
            ui->widget,SLOT(setIndex(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),
            ui->widget,SLOT(setN(int)));

    connect(ui->spinBox,SIGNAL(valueChanged(int)),
            ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            ui->spinBox,SLOT(setValue(int)));

    connect(ui->pushButton,SIGNAL(clicked()),
            ui->widget,SLOT(chooseColor()));

    connect(ui->checkBox,SIGNAL(stateChanged(int)),
            ui->widget,SLOT(setMoving(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug("index: %d", index);
}*/



/*void MainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug("%d",arg1);
}*/

