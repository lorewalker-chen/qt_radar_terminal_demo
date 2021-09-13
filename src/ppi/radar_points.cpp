#include "radar_points.h"

#include "qwt_symbol.h"
#include "qwt_polar_curve.h"
#include "radar_points_data.h"

RadarPoints::RadarPoints(QwtPolarPlot* plot) {
    //初始化
    InitAll();
    //将曲线依附到图层
    points_curve_->attach(plot);
}

RadarPoints::~RadarPoints() {
    points_curve_->detach();
    delete points_curve_;
}
//添加点
void RadarPoints::AddPoint(int cpi, const QwtPointPolar& polar) {
    points_data_->AddPoint(cpi, polar);
}
//删除一个cpi周期的点
void RadarPoints::RemovePoints(int cpi) {
    points_data_->RemovePoints(cpi);
}
//删除全部点
void RadarPoints::RemoveAllPoints() {
    points_data_->RemoveAllPoint();
}
//显示点迹
void RadarPoints::ShowPoints() {
    points_curve_->show();
}
//隐藏点迹
void RadarPoints::HidePoints() {
    points_curve_->hide();
}
//设置点迹颜色
void RadarPoints::SetColor(const QColor& color) {
    //标志轮廓颜色
    points_symbol_->setPen(color);
    //标志填充颜色
    points_symbol_->setBrush(QBrush(color));
}
//设置点迹容量
void RadarPoints::SetCapacity(int capacity) {
    points_data_->SetCapacity(capacity);
}
//获取点迹容量
int RadarPoints::Capacity() {
    return points_data_->Capacity();
}
//获取点迹数量
int RadarPoints::Count() {
    return points_data_->Count();
}
//初始化
void RadarPoints::InitAll() {
    InitSymbol();//初始化标志
    InitCurve();//初始化曲线
}
//初始化标志
void RadarPoints::InitSymbol() {
    points_symbol_ = new QwtSymbol;
    //圆形
    points_symbol_->setStyle(QwtSymbol::Ellipse);
    //初始颜色，灰色
    QColor color = Qt::gray;
    //设置初始轮廓颜色
    points_symbol_->setPen(color);
    //设置初始填充颜色
    points_symbol_->setBrush(QBrush(color));
    //尺寸
    points_symbol_->setSize(QSize(4, 4));
}
//初始化曲线
void RadarPoints::InitCurve() {
    points_curve_ = new QwtPolarCurve;
    //只画点
    points_curve_->setStyle(QwtPolarCurve::NoCurve);
    //设置标志
    points_curve_->setSymbol(points_symbol_);
    //设置数据
    points_data_ = new RadarPointsData;
    points_curve_->setData(points_data_);
}
