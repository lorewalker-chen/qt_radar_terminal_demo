#ifndef DATAREPOSITORY_H
#define DATAREPOSITORY_H

#include <QObject>

class DataRepository: public QObject {
  public:
    DataRepository();

    QByteArray* Data();
  private:
    QByteArray buffer_;
};

#endif // DATAREPOSITORY_H
