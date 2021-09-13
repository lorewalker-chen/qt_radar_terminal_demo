#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>

class UdpModule;
class Communicator : public QObject {
    Q_OBJECT
  public:
    ~Communicator();
    static Communicator* Instance();

  private:
    static QScopedPointer<Communicator> self;
    Communicator();

    UdpModule* udp_module_ = nullptr;

  signals:

};

#endif // COMMUNICATOR_H
