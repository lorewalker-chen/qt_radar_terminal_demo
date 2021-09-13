#include "udp_module.h"

#include <QUdpSocket>
#include <QTimer>
#include <QDebug>
#include <QThread>

UdpModule::UdpModule() {
    //移动到线程
    working_thread_ = new QThread;
    connect(working_thread_, &QThread::started, this, &UdpModule::InitAll);
    this->moveToThread(working_thread_);
    working_thread_->start();
}

UdpModule::~UdpModule() {
    judge_connection_timer_->stop();
    judge_connection_timer_->deleteLater();

    socket_->deleteLater();

    working_thread_->quit();
    working_thread_->wait();
    working_thread_->deleteLater();
}
//发送数据
void UdpModule::SendData(const QByteArray& array) {
    //判断是否可写入
    if (!is_writeable_) {
        qDebug() << "udp socket目的地未设置";
        return;
    }
    socket_->writeDatagram(array.data(), array.size(), QHostAddress(radar_ipv4_), radar_port_);
}
//设置本机网络参数
void UdpModule::SetLocalNetParams(QString ipv4, quint16 port) {
    //绑定本机网络参数
    if (!socket_->bind(QHostAddress(ipv4), port, QUdpSocket::ShareAddress)) {
        qWarning() << QString("绑定接收ip地址:%1,端口%2失败").arg(ipv4).arg(port);
        return;
    }
}
//设置雷达网络参数
void UdpModule::SetRadarNetParams(QString ipv4, quint16 port) {
    if ("" == ipv4) {
        qWarning() << "目的地ip为空";
        is_writeable_ = false;
        return;
    }
    if (0 == port) {
        qWarning() << "因目的地端口为0,将自动查找端口";
    }
    radar_ipv4_ = ipv4;
    radar_port_ = port;
    //设置可写
    is_writeable_ = true;
}
//初始化
void UdpModule::InitAll() {
    InitSocket();
    InitTimer();
}
//初始化socket
void UdpModule::InitSocket() {
    socket_ = new QUdpSocket;
    connect(socket_, &QUdpSocket::readyRead, this, &UdpModule::ReadPendingData);
}
//启动判断连接状态定时器
void UdpModule::InitTimer() {
    judge_connection_timer_ = new QTimer;
    connect(judge_connection_timer_, &QTimer::timeout, this, &UdpModule::JudgeConnection);
    judge_connection_timer_->start(timeout_msec_ / 2);
}
//从socket缓冲区读取全部数据
void UdpModule::ReadPendingData() {
    is_connect_ = true;
    while (socket_->hasPendingDatagrams()) {
        QByteArray array;
        array.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(array.data(), array.size());
        emit ReceivedData(array);
    }
}
//判断连接状态
void UdpModule::JudgeConnection() {
    if (!is_connect_) {
        emit LostConnection();
    } else {
        is_connect_ = false;
    }
}
