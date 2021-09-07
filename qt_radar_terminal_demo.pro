QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

include(./ppi/ppi.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    main_window.cpp \
    network/process_data.cpp \
    network/udp_receiver.cpp \
    network/udp_sender.cpp \
    settings/global_settings.cpp \
    style/light_button.cpp

HEADERS += \
    main_window.h \
    network/process_data.h \
    network/udp_receiver.h \
    network/udp_sender.h \
    settings/global_settings.h \
    style/light_button.h

FORMS += \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/button.qrc \
    resource/qss.qrc
