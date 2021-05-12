#include "series_polar_points.h"

SeriesPolarPoints::SeriesPolarPoints() {
    points_list_.clear();
}

SeriesPolarPoints::~SeriesPolarPoints() {
    points_list_.clear();
}
//添加单个点
void SeriesPolarPoints::AddPoint(double azimuth, double radius) {
    //超过容量，从队列头删除一个
    if (capacity_ > 0 && points_list_.size() > capacity_) {
        points_list_.removeFirst();
    }
    //添加到队列尾
    points_list_.append(QwtPointPolar(azimuth, radius));
}
//删除所有点
void SeriesPolarPoints::ClearPoints() {
    points_list_.clear();
}
//设置容量
void SeriesPolarPoints::SetCapacity(int capacity) {
    capacity_ = capacity;
}
//列表里是否包含该点
bool SeriesPolarPoints::HasPoint(double azimuth, double radius) {
    for (QwtPointPolar temp : points_list_) {
        if (fabs(temp.radius() - radius) < 100) {
            double temp_azimuth = temp.azimuth();
            if (temp.azimuth() < 0) {
                temp_azimuth += 360;
            }
            if (fabs(temp_azimuth - azimuth) < 1) {
                return true;
            }
        }
    }
    return false;
}
//最后一个点的坐标
QwtPointPolar SeriesPolarPoints::LastPoint() {
    return points_list_.last();
}

size_t SeriesPolarPoints::size() const {
    return points_list_.size();
}

QwtPointPolar SeriesPolarPoints::sample(size_t i) const {
    return points_list_.at(i);
}

QRectF SeriesPolarPoints::boundingRect() const {
    return qwtBoundingRect(*this);
}
