#ifndef DATAPROCESSER_H
#define DATAPROCESSER_H

#include <QObject>

class DataProcesser : public QObject {
    Q_OBJECT
  public:
    DataProcesser();

  public slots:
    //接收到原始数据
    void ReceivedRawData(const QByteArray& array);
    //
  private:
    QByteArray buffer_;

  signals:

};

#endif // DATAPROCESSER_H
