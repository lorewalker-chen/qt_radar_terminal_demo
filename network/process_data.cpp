#include "process_data.h"

#include <QThread>
#include <QMutex>
#include <QTimer>

QByteArray kUdpReceiveBuffer;

QScopedPointer<ProcessData> ProcessData::self;

ProcessData::~ProcessData() {
    process_thread_->deleteLater();
    process_timer_->deleteLater();
}

ProcessData* ProcessData::Instance() {
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new ProcessData);
        }
    }
    return self.data();
}
//初始化
void ProcessData::InitAll() {
    InitTimer();
}
//初始化定时器
void ProcessData::InitTimer() {
    process_timer_ = new QTimer;
    connect(process_timer_, &QTimer::timeout, this, &ProcessData::ProcessReceivedData);
    process_timer_->start(1);
}
//解析收到的数据
void ProcessData::ProcessReceivedData() {
    //缓冲区长度小于最小报文长度，不进行处理
    if (kUdpReceiveBuffer.size() < kMinMsgLength) {
        return;
    }
    QByteArray array;
    //加锁，从缓冲区找一条数据
    QMutex mutex;
    mutex.lock();
    array = FindData();
    mutex.unlock();
    //检查校验和，解析数据
    if (CheckMsgSum(array)) {
        UnPackOneMsg(array);
    }
}
//寻找一条待解析的完整数据
QByteArray ProcessData::FindData() {
    int index = 0;
    for (int i = 0; i < kUdpReceiveBuffer.size(); i++) {
        //TODO 根据报文头找到一条报文
        break;
    }
    //删除报文头之前的内容
    kUdpReceiveBuffer.remove(0, index);
    //获取该条报文的长度
    uint16_t length = 0;
    //拷贝
    QByteArray array(length, 0);
    for (int j = 0; j < length; j++) {
        array[j] = kUdpReceiveBuffer[j];
    }
    //释放缓冲区空间
    kUdpReceiveBuffer.remove(0, length);
    kUdpReceiveBuffer.squeeze();
    return array;
}
//解包数据
void ProcessData::UnPackOneMsg(QByteArray array) {
    //TODO 按照规则解包数据
    Q_UNUSED(array);
}
//检查校验和
bool ProcessData::CheckMsgSum(QByteArray array) {
    int length = array.size();
    uint16_t rec_sum = ((array[length - 2] & 0xFF) << 8) + (array[length - 1] & 0xFF);
    uint16_t calculate_sum = 0;
    for (int i = 0; i < length - 2; i++) {
        calculate_sum += (array[i] & 0xFF);
    }
    return (rec_sum == calculate_sum);
}

ProcessData::ProcessData() {
    //移动到线程中
    process_thread_ = new QThread;
    connect(process_thread_, &QThread::started,
            this, &ProcessData::InitAll);
    process_thread_->start();
}
