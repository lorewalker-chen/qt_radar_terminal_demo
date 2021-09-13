#ifndef UDPMODULE_H
#define UDPMODULE_H

#include <QObject>

class QUdpSocket;
class QTimer;
class UdpModule : public QObject {
    Q_OBJECT
  public:
    UdpModule();
    ~UdpModule();

  public slots:
    //发送数据
    void SendData(const QByteArray& array);

    //设置本机网络参数
    void SetLocalNetParams(QString ipv4, quint16 port);
    //设置雷达网络参数
    void SetRadarNetParams(QString ipv4, quint16 port);

  private:
    QThread* working_thread_ = nullptr;
    QUdpSocket* socket_ = nullptr;
    //雷达网络
    QString radar_ipv4_ = "";
    quint16 radar_port_ = 0;
    //超时时间
    int timeout_msec_ = 10000;

    //是否可用
    bool is_readable_ = false;
    bool is_writeable_ = false;
    //是否连接
    bool is_connect_ = false;
    QTimer* judge_connection_timer_ = nullptr;

  private slots:
    void InitAll();
    void InitSocket();
    void InitTimer();
    //读取socket缓冲区中的所有数据
    void ReadPendingData();
    //判断连接状态
    void JudgeConnection();

  signals:
    //收到数据信号
    void ReceivedData(const QByteArray& array);
    //失去连接信号
    void LostConnection();
};

#endif // UDPMODULE_H
