#include "udp_sender.h"
#include "settings/global_settings.h"

#include <QtNetwork>
#include <QTimer>

QScopedPointer<UdpSender> UdpSender::self;

UdpSender::~UdpSender() {
    sender_thread_->deleteLater();
    udp_sender_->deleteLater();
    timer_send_->deleteLater();
}

UdpSender* UdpSender::Instance() {
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new UdpSender);
        }
    }
    return self.data();
}
//设置目的地地址
void UdpSender::SetDstNetParam() {
    UdpParameters dst_net_param = GlobalSettings::Instance()->GetRadarNetParam();
    dst_ipv4_ = dst_net_param.ipv4;
    dst_port_ = dst_net_param.port;
}
//绑定接收ip地址和端口号
void UdpSender::BindSocket() {
    bool ret = udp_sender_->bind(QHostAddress(local_ipv4_), local_port_, QUdpSocket::ShareAddress);
    //绑定失败，输出提示信息
    if (!ret) {
        qDebug() << "绑定发送ip地址或端口号失败，请检查配置文件中网络信息与本机是否一致";
    }
}
//初始化
void UdpSender::InitAll() {
    InitUdpSender();
    InitMessage();
    InitTimer();
}
//初始化socket
void UdpSender::InitUdpSender() {
    udp_sender_ = new QUdpSocket;
    //设置本地网络参数
    SetLocalNetParam();
    //当设置改变时，重新读取
    connect(GlobalSettings::Instance(), &GlobalSettings::LocalNetChanged,
            this, &UdpSender::SetLocalNetParam);
    //设置目的地网络参数
    SetDstNetParam();
    //当设置改变时，重新读取
    connect(GlobalSettings::Instance(), &GlobalSettings::RadarNetChanged,
            this, &UdpSender::SetDstNetParam);
}

void UdpSender::InitMessage() {
    //TODO 初始化待发送的消息
}
//开启定时器，按频率发送一次报文
void UdpSender::InitTimer() {
    timer_send_ = new QTimer;
    connect(timer_send_, &QTimer::timeout, this, &UdpSender::SendToRadar);
    connect(sender_thread_, &QThread::finished, timer_send_, &QTimer::stop);
    timer_send_->start(send_frequency_msec_);
}
//设置本地网络参数
void UdpSender::SetLocalNetParam() {
    UdpParameters local_net_param = GlobalSettings::Instance()->GetLocalNetParam();
    if ((local_net_param.ipv4 != local_ipv4_) || (local_net_param.port != local_port_)) {
        local_ipv4_ = local_net_param.ipv4;
        local_port_ = local_net_param.port;
        BindSocket();
    }
}
//计算校验和
uint16_t UdpSender::CalculateCheckSum(QByteArray array) {
    uint16_t sum = 0;
    for (int i = 0; i < array.size() - 2; i++) {
        sum += (array[i] & 0xFF);
    }
    return sum;
}
//发送到雷达
void UdpSender::SendToRadar() {
    //TODO 调用SendSingleMessage函数
}
//发送单条消息
void UdpSender::SendSingleMessage(const QByteArray& array) {
    udp_sender_->writeDatagram(array.data(), array.size(), QHostAddress(dst_ipv4_), dst_port_);
}

UdpSender::UdpSender() {
    //将实例移动到线程
    sender_thread_ = new QThread;
    connect(sender_thread_, &QThread::started, this, &UdpSender::InitAll);
    this->moveToThread(sender_thread_);
    sender_thread_->start();
}
