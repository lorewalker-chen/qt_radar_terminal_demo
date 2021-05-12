#include "track_last_point.h"

#include <qwt_symbol.h>

TrackLastPoint::TrackLastPoint(int index, double azimuth, double radius) {
    //初始批号
    index_ = index;
    //初始位置
    ChangePosition(azimuth, radius);
    //初始标记样式
    symbol_ = new QwtSymbol;
    //圆形
    symbol_->setStyle(QwtSymbol::Ellipse);
    //白色
    symbol_->setBrush(QBrush(Qt::white));
    symbol_->setPen(Qt::white);
    //初始尺寸
    symbol_->setSize(QSize(4, 4));
    setSymbol(symbol_);
    //文字
    text_.setText(QString::number(index_));
    text_.setFont(QFont("Times New Roman", 15));
    text_.setColor(Qt::yellow);
    setLabel(text_);
    //文字位置
    setLabelAlignment(Qt::AlignHCenter | Qt::AlignTop);
}
//设置批号
void TrackLastPoint::SetIndex(int index) {
    index_ = index;
    text_.setText(QString::number(index_));
}
//获取批号
int TrackLastPoint::GetIndex() {
    return index_;
}
//设置文字颜色
void TrackLastPoint::SetTextColor(const QColor& color) {
    text_.setColor(color);
}
//设置点颜色
void TrackLastPoint::SetPointColor(const QColor& color) {
    symbol_->setBrush(QBrush(color));
    symbol_->setPen(color);
}
//设置点尺寸
void TrackLastPoint::SetPointSize(const QSize& size) {
    symbol_->setSize(size);
}
//改变位置
void TrackLastPoint::ChangePosition(double azimuth, double radius) {
    setPosition(QwtPointPolar(azimuth, radius));
}
