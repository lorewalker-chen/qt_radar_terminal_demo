#include "communicator.h"
#include "udp_module.h"

#include <QDebug>
#include <QThread>

QScopedPointer<Communicator> Communicator::self(new Communicator);

Communicator::~Communicator() {
    udp_module_->deleteLater();
}

Communicator* Communicator::Instance() {
    return self.data();
}

Communicator::Communicator() {
    udp_module_ = new UdpModule;
    QMetaObject::invokeMethod(udp_module_, "SetLocalNetParams",
                              Q_ARG(QString, "192.168.1.102"), Q_ARG(quint16, 9000));
//    udp_module_->SetLocalNetParams("192.168.1.102", 9000);
}
