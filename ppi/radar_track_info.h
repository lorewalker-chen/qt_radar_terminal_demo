#ifndef RADAR_TRACK_INFO_H
#define RADAR_TRACK_INFO_H

#include <QTime>

/**
 * @brief 航迹信息结构体
 */
struct RadarTrackInfo {
    int index = -1; //批号
    double radius = 0; //距离
    double azimuth = 0; //方位
    double velocity = 0; //速度
    double course = 0; //航向
    QString type = ""; //类别
    QTime time;//时间
    bool is_end = false; //是否结束
};

#endif // RADAR_TRACK_INFO_H
