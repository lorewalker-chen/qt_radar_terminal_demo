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


