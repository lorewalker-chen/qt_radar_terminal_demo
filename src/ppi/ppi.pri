SOURCES += \
    $$PWD/plan_position_indicator.cpp \
    $$PWD/radar_points.cpp \
    $$PWD/radar_points_data.cpp \
    $$PWD/radar_radius_line.cpp \
    $$PWD/radar_track.cpp \
    $$PWD/radar_track_data.cpp \
    $$PWD/radar_track_marker.cpp \
    $$PWD/radar_track_table_model.cpp

HEADERS += \
    $$PWD/plan_position_indicator.h \
    $$PWD/radar_points.h \
    $$PWD/radar_points_data.h \
    $$PWD/radar_radius_line.h \
    $$PWD/radar_track.h \
    $$PWD/radar_track_data.h \
    $$PWD/radar_track_info.h \
    $$PWD/radar_track_marker.h \
    $$PWD/radar_track_table_model.h

INCLUDEPATH += \
    $$PWD \
    $$PWD/include/qwt \
    $$PWD/include/qwt_polar \

DEPENDPATH += \
    $$PWD \
    $$PWD/include/qwt \
    $$PWD/include/qwt_polar \


win32:CONFIG(debug,debug|release){
    LIBS += -L$$PWD/lib_windows/ -lqwtd -lqwtpolard
}
win32:CONFIG(release,debug|release){
    LIBS += -L$$PWD/lib_windows/ -lqwt -lqwtpolar
}
