QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    main_window.cpp \
    ppi/plan_position_indicator.cpp \
    ppi/radar_points.cpp \
    ppi/radar_track.cpp \
    ppi/scanning_line.cpp \
    ppi/series_polar_points.cpp \
    ppi/track_last_point.cpp \
    settings/global_settings.cpp \
    style/light_button.cpp

HEADERS += \
    main_window.h \
    ppi/plan_position_indicator.h \
    ppi/radar_points.h \
    ppi/radar_track.h \
    ppi/scanning_line.h \
    ppi/series_polar_points.h \
    ppi/track_last_point.h \
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

win32:CONFIG(debug,debug|release){
    LIBS += -L$$[QT_INSTALL_PREFIX]/lib -lqwtd -lqwtpolard
}
else {
    LIBS += -L$$[QT_INSTALL_PREFIX]/lib -lqwt -lqwtpolar
}
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/Qwt
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/QwtPolar
