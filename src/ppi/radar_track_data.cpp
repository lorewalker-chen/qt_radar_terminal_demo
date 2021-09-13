#include "radar_track_data.h"

RadarTrackData::RadarTrackData() {
    data_.clear();
}
//添加航迹点
void RadarTrackData::AddTrackPoint(const QwtPointPolar& polar) {
    data_.append(polar);
}
//到指定点的最小距离
double RadarTrackData::MinDistanceToPoint(const QwtPointPolar& polar) {
    double distance = 500;
    double r0 = polar.radius();
    double a0 = polar.azimuth();
    for (QwtPointPolar temp_polar : data_) {
        double r1 = temp_polar.radius();
        double a1 = temp_polar.azimuth();
        //余弦定理计算距离
        double temp_distance = sqrtf(powf(r0, 2) + powf(r1, 2) - 2 * r0 * r1 * cos((a0 - a1) * M_PI / 180.0));
        if (temp_distance < distance) {
            distance = temp_distance;
        }
    }
    return distance;
}
//返回总点数
size_t RadarTrackData::size() const {
    return data_.size();
}
//采样，返回从0~(size-1)的每个点
QwtPointPolar RadarTrackData::sample(size_t i) const {
    return data_.at(i);
}
//返回绘图区域
QRectF RadarTrackData::boundingRect() const {
    return qwtBoundingRect(*this);
}
