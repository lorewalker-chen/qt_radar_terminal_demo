#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>

class QUdpSocket;
class QTimer;
class UdpSender : public QObject {
    Q_OBJECT
  public:
    ~UdpSender();
    static UdpSender* Instance();

  public slots:


  private slots:
    void InitAll();
    void InitUdpSender();
    void InitMessage();
    void InitTimer();

    void SetLocalNetParam();
    void SetDstNetParam();
    void BindSocket();

    uint16_t CalculateCheckSum(QByteArray array);

    void SendToRadar();
    void SendSingleMessage(const QByteArray& array);

  private:
    static QScopedPointer<UdpSender> self;
    UdpSender();

    QThread* sender_thread_ = nullptr;
    QUdpSocket* udp_sender_ = nullptr;
    QString local_ipv4_ = "";
    int local_port_ = 0;
    QString dst_ipv4_ = "";
    int dst_port_ = 0;

    int send_frequency_msec_ = 1000;
    QTimer* timer_send_ = nullptr;
};

#endif // UDPSENDER_H
