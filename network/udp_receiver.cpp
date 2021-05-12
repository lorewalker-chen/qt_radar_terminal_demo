#include "udp_receiver.h"
#include "settings/global_settings.h"

#include <QtNetwork>
#include <QTimer>
#include <QDebug>

QScopedPointer<UdpReceiver> UdpReceiver::self;

UdpReceiver::~UdpReceiver() {
    receiver_thread_->deleteLater();
    udp_receiver_->deleteLater();
    judge_connection_timer_->deleteLater();
}

UdpReceiver* UdpReceiver::Instance() {
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new UdpReceiver);
        }
    }
    return self.data();
}
//初始化
void UdpReceiver::InitAll() {
    InitUdpReceiver();
    InitTimer();
}
//初始化socket
void UdpReceiver::InitUdpReceiver() {
    udp_receiver_ = new QUdpSocket;
    connect(udp_receiver_, &QUdpSocket::readyRead, this, &UdpReceiver::ReadPendingData);
    //设置本地网络参数
    SetLocalNetParam();
    //当配置文件改变时，重新读取
    connect(GlobalSettings::Instance(), &GlobalSettings::LocalNetChanged,
            this, &UdpReceiver::SetLocalNetParam);
}
//初始化判断是否失去连接定时器
void UdpReceiver::InitTimer() {
    judge_connection_timer_ = new QTimer;
    connect(judge_connection_timer_, &QTimer::timeout,
            this, &UdpReceiver::JudgeConnection);
    judge_connection_timer_->start(5000);
}
//读取配置文件中本地网络信息
void UdpReceiver::SetLocalNetParam() {
    UdpParameters local_net_param = GlobalSettings::Instance()->GetLocalNetParam();
    if ((local_net_param.ipv4 != local_ipv4_) || (local_net_param.port != local_port_)) {
        local_ipv4_ = local_net_param.ipv4;
        local_port_ = local_net_param.port;
        BindSocket();
    }
}
//绑定接收ip地址和端口号
void UdpReceiver::BindSocket() {
    bool ret = udp_receiver_->bind(QHostAddress(local_ipv4_), local_port_, QUdpSocket::ShareAddress);
    //绑定失败，输出提示信息
    if (!ret) {
        qDebug() << "绑定接收ip地址或端口号失败，请检查配置文件中网络信息与本机是否一致";
    }
}
//判断是否失去连接
void UdpReceiver::JudgeConnection() {
    if (is_disconnect_) {
        emit LoseConnection();
    }
    if (!is_connect_) {
        is_disconnect_ = true;
    }
}
//接收数据，并存入缓冲区
void UdpReceiver::ReadPendingData() {
    is_connect_ = true;
    is_disconnect_ = false;
    while (udp_receiver_->hasPendingDatagrams()) {
        QByteArray array;
        array.resize(udp_receiver_->pendingDatagramSize());
        udp_receiver_->readDatagram(array.data(), array.size());
        //缓冲区定义在process_data.cpp中
        extern QByteArray kUdpReceiveBuffer;
        QMutex mutex;
        mutex.lock();
        kUdpReceiveBuffer.append(array);
        mutex.unlock();
    }
    is_connect_ = false;
}

UdpReceiver::UdpReceiver() {
    //移动到线程
    receiver_thread_ = new QThread;
    connect(receiver_thread_, &QThread::started, this, &UdpReceiver::InitAll);
    this->moveToThread(receiver_thread_);
    receiver_thread_->start();
}
