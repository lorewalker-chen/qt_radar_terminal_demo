#include "radar_track.h"
#include "series_polar_points.h"

#include <qwt_symbol.h>
#include <QDebug>
RadarTrack::RadarTrack() {
    //默认颜色，黄色
    color_ = Qt::yellow;
    SetSymbolColor(color_);
    //默认标记颜色，深黄色
    marked_color_ = Qt::darkYellow;
    //仅画点
    setStyle(QwtPolarCurve::NoCurve);
    //数据
    points_ = new SeriesPolarPoints;
    setData(points_);
}
//设置点的颜色
void RadarTrack::SetColor(const QColor& color) {
    color_ = color;
    setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QBrush(color), QPen(color), QSize(4, 4)));
}
//设置点迹容量
void RadarTrack::SetCapacity(int capacity) {
    points_->SetCapacity(capacity);
}
//添加单个点
void RadarTrack::AddPoint(double azimuth, double radius) {
    points_->AddPoint(azimuth, radius);
}
//删除所有点
void RadarTrack::ClearPoints() {
    points_->ClearPoints();
}
//是否包含点
bool RadarTrack::HasPoint(double azimuth, double radius) {
    return points_->HasPoint(azimuth, radius);
}
//设置标记颜色
void RadarTrack::SetMarkColor(const QColor& color) {
    marked_color_ = color;
}
//设置是否标记
void RadarTrack::SetMarked(bool is_mark) {
    is_marked_ = is_mark;
    is_mark ? SetSymbolColor(marked_color_) : SetSymbolColor(color_);
}
//是否标记
bool RadarTrack::IsMarked() {
    return is_marked_;
}
//设置是否结束
void RadarTrack::SetEnd(bool is_end) {
    is_end_ = is_end;
}
//是否结束
bool RadarTrack::IsEnd() {
    return is_end_;
}
//最后一个点
QwtPointPolar RadarTrack::LastPoint() {
    return points_->LastPoint();
}
//设置点的颜色
void RadarTrack::SetSymbolColor(const QColor& color) {
    setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QBrush(color), QPen(color), QSize(3, 3)));
}
