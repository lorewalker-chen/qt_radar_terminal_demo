#include "main_window.h"
#include "ui_main_window.h"
#include "settings/global_settings.h"
#include "ppi/plan_position_indicator.h"

#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    InitAll();
}

MainWindow::~MainWindow() {
    delete ui;
}
//更新界面显示时间
void MainWindow::UpdateTime() {
}
//初始化
void MainWindow::InitAll() {
    InitWindowStyle();
    InitStyleSheet();
    InitTime();
    InitPPI();
}
//退出按钮
void MainWindow::on_pushButton_exit_clicked() {
    close();
}
//初始化窗口属性
void MainWindow::InitWindowStyle() {
    setWindowFlags(Qt::FramelessWindowHint);//无边框
}
//初始化样式表
void MainWindow::InitStyleSheet() {
    //设置背景色为黑色
    qApp->setPalette(QPalette(QColor(Qt::black)));
    //设置样式表
    qApp->setStyleSheet("file:///:/qss/black.css");
}
//初始化界面时间
void MainWindow::InitTime() {
    UpdateTime();
    connect(GlobalSettings::Instance(), &GlobalSettings::TimeChanged,
            this, &MainWindow::UpdateTime);
}
//初始化PPI
void MainWindow::InitPPI() {
    ppi_ = new PlanPositionIndicator(ui->ppi);
    ppi_->SetRange(30000);
    ppi_->SetAngleRange(315, 45);
    ppi_->SetNorthAngle(10);
    ppi_->SetPosition(105.192457, 33.248241, 0);
    ppi_->SetDateTime(QDateTime::currentDateTime());
}

