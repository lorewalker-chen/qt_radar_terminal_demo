#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <QObject>

class QThread;
class QTimer;
class ProcessData : public QObject {
    Q_OBJECT
  public:
    ~ProcessData();
    static ProcessData* Instance();

  private slots:
    void InitAll();
    void InitTimer();

    void ProcessReceivedData();
    QByteArray FindData();
    void UnPackOneMsg(QByteArray array);
    bool CheckMsgSum(QByteArray array);

  private:
    static QScopedPointer<ProcessData> self;
    ProcessData();

    const int kMinMsgLength = 10;//最小报文长度

    QThread* process_thread_;
    QTimer* process_timer_;

  signals:
    void ReceivedMsg();
};

#endif // PROCESSDATA_H
