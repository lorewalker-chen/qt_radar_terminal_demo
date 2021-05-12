#include "scanning_line.h"
#include "series_polar_points.h"

ScanningLine::ScanningLine() {
    //默认颜色
    color_ = Qt::darkGreen;
    //默认线宽
    width_ = 2;
    //设置画笔
    SetPen();
    //画线
    setStyle(QwtPolarCurve::Lines);
    //数据
    plots_ = new SeriesPolarPoints;
    setData(plots_);
}
//设置颜色
void ScanningLine::SetColor(const QColor& color) {
    color_ = color;
    SetPen();
}
//设置线宽
void ScanningLine::SetWidth(int width) {
    width_ = width;
    SetPen();
}
//设置扫描线终止点
void ScanningLine::SetEndPolar(double azimuth, double radius) {
    plots_->ClearPoints();
    //扫描线起始点始终为(0，0)
    plots_->AddPoint(0, 0);
    plots_->AddPoint(azimuth, radius);
}
//设置画笔
void ScanningLine::SetPen() {
    setPen(QPen(QBrush(color_), width_));
}
