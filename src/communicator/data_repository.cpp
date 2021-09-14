#include "data_repository.h"

DataRepository::DataRepository() {

}

QByteArray* DataRepository::Data() {
    return &buffer_;
}
