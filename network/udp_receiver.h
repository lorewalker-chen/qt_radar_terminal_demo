#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>

class QUdpSocket;
class QTimer;
class UdpReceiver : public QObject {
    Q_OBJECT
  public:
    ~UdpReceiver();
    static UdpReceiver* Instance();

  private slots:
    void InitAll();
    void InitUdpReceiver();
    void InitTimer();

    void SetLocalNetParam();
    void BindSocket();

    void JudgeConnection();
    void ReadPendingData();

  private:
    static QScopedPointer<UdpReceiver> self;
    UdpReceiver();

    QThread* receiver_thread_ = nullptr;
    QUdpSocket* udp_receiver_ = nullptr;
    QString local_ipv4_ = "";
    int local_port_ = 0;

    QTimer* judge_connection_timer_ = nullptr;
    bool is_connect_ = false;
    bool is_disconnect_ = true;

  signals:
    void LoseConnection();
};

#endif // UDPRECEIVER_H
