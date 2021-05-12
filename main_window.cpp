#include "main_window.h"
#include "ui_main_window.h"
#include "settings/global_settings.h"
#include "network/udp_sender.h"
#include "network/udp_receiver.h"
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
    ui->dateTimeEdit->setDateTime(GlobalSettings::Instance()->GetCurrentDateTime());
}
//更新鼠标当前位置的极坐标
void MainWindow::UpdateCurrentMousePolar(const QwtPointPolar& polar) {
    ui->label_azimuth->setText(QString("方位：%1°").arg(polar.azimuth(), 0, 'f', 2));
    ui->label_distance->setText(QString("距离：%1m").arg(polar.radius(), 0, 'f', 2));
}
//点迹勾选框
void MainWindow::on_checkBox_points_stateChanged(int arg1) {
    if (arg1 == 0) {
        ppi_->HidePoints();
    } else if (arg1 == 2) {
        ppi_->ShowPoints();
    }
}
//航迹勾选框
void MainWindow::on_checkBox_tracks_stateChanged(int arg1) {
    if (arg1 == 0) {
        ppi_->HideTracks();
    } else if (arg1 == 2) {
        ppi_->ShowTracks();
    }
}
//清空点迹按钮点击
void MainWindow::on_pushButton_clear_points_clicked() {
    ppi_->ClearPoints();
}
//清空航迹按钮点击
void MainWindow::on_pushButton_clear_tracks_clicked() {
    ppi_->ClearTracks();
}
//清屏按钮点击
void MainWindow::on_pushButton_clear_all_clicked() {
    ppi_->ClearPoints();
    ppi_->ClearTracks();
}
//初始化
void MainWindow::InitAll() {
    InitWindowStyle();
    InitStyleSheet();
    InitTime();
    InitPPI();
    InitNetWork();
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
    QString qss;
    QFile file(":/qss/black.css");
    if (file.open(QFile::ReadOnly)) {
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }
        qss = list.join("\n");
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
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
    connect(ppi_, &PlanPositionIndicator::CurrentMousePolar, this, &MainWindow::UpdateCurrentMousePolar);
}
//初始化网络
void MainWindow::InitNetWork() {
    InitUdpSender();
    InitUdpReceiver();
}
//初始化UDP发送类
void MainWindow::InitUdpSender() {
    //TODO 绑定报文改变信号
    UdpSender::Instance();
}
//初始化UDP接收类
void MainWindow::InitUdpReceiver() {
    //TODO 绑定接收到报文信号
    UdpReceiver::Instance();
}


