#include "radar_points.h"
#include "series_polar_points.h"

#include <qwt_symbol.h>

RadarPoints::RadarPoints() {
    //初始颜色，黄色
    SetColor(Qt::yellow);
    //仅画点
    setStyle(QwtPolarCurve::NoCurve);
    //数据
    points_ = new SeriesPolarPoints;
    setData(points_);
}
//设置点的颜色
void RadarPoints::SetColor(const QColor& color) {
    setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QBrush(color), QPen(color), QSize(4, 4)));
}
//设置点迹容量
void RadarPoints::SetCapacity(int capacity) {
    points_->SetCapacity(capacity);
}
//添加单个点
void RadarPoints::AddPoint(double azimuth, double radius) {
    points_->AddPoint(azimuth, radius);
}
//删除所有点
void RadarPoints::ClearPoints() {
    points_->ClearPoints();
}

