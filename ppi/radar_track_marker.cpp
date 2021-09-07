#include "radar_track_marker.h"
#include "qwt_symbol.h"

RadarTrackMarker::RadarTrackMarker() {
    //初始化
    InitAll();
}
//设置文字
void RadarTrackMarker::SetText(const QString& text) {
    marker_text_.setText(text);
    setLabel(marker_text_);
}
//设置所有颜色
void RadarTrackMarker::SetColor(const QColor& color) {
    SetTextColor(color);
    SetSymbolColor(color);
}
//设置文字颜色
void RadarTrackMarker::SetTextColor(const QColor& color) {
    marker_text_.setColor(color);
    setLabel(marker_text_);
}
//设置标志点颜色
void RadarTrackMarker::SetSymbolColor(const QColor& color) {
    //设置轮廓颜色
    marker_symbol_->setPen(color);
    //设置填充颜色
    marker_symbol_->setBrush(QBrush(color));
}
//初始化
void RadarTrackMarker::InitAll() {
    InitSymbol();//初始化标志点
    InitText();//初始化文字
}
//初始化标志点
void RadarTrackMarker::InitSymbol() {
    //初始化标志点样式
    marker_symbol_ = new QwtSymbol;
    //圆形
    marker_symbol_->setStyle(QwtSymbol::Ellipse);
    //尺寸
    marker_symbol_->setSize(QSize(4, 4));
    //设置样式
    setSymbol(marker_symbol_);
}
//初始化文字
void RadarTrackMarker::InitText() {
    //字体
    marker_text_.setFont(QFont("Times New Roman", 10));
    setLabel(marker_text_);
    //位置，右上
    setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
}
