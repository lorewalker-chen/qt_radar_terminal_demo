#include "radar_track.h"

#include "radar_track_data.h"
#include "radar_track_marker.h"

#include "qwt_symbol.h"
#include "qwt_polar_curve.h"

RadarTrack::RadarTrack(QwtPolarPlot* plot) {
    //初始化
    InitAll();
    //将曲线依附到图层
    track_curve_->attach(plot);
    //将标牌依附到图层
    track_marker_->attach(plot);
}

RadarTrack::~RadarTrack() {
    track_curve_->detach();
    delete track_curve_;

    track_marker_->detach();
    delete track_marker_;
}
//添加航迹点
void RadarTrack::AddTrackPoint(const RadarTrackInfo& info) {
    //如果是该批号第一个点，更新标牌文字
    if (-1 == track_info_.index) {
        track_marker_->SetText(QString::number(info.index));
    }
    //更新航迹信息
    track_info_ = info;
    //添加到数据列表
    QwtPointPolar polar = QwtPointPolar(info.azimuth, info.radius);
    track_data_->AddTrackPoint(polar);
    //更新标牌位置
    track_marker_->setPosition(polar);
}
//显示航迹
void RadarTrack::ShowTrack() {
    //显示曲线
    track_curve_->show();
    //显示标牌
    track_marker_->show();
}
//隐藏航迹
void RadarTrack::HideTrack() {
    //隐藏曲线
    track_curve_->hide();
    //隐藏标牌
    track_marker_->hide();
}
//到指定点的最小距离
double RadarTrack::MinDistanceToPoint(const QwtPointPolar& polar) {
    return track_data_->MinDistanceToPoint(polar);
}
//将是否标记置为相反
void RadarTrack::Mark() {
    SetMarked(!is_marked_);
}
//标记航迹
void RadarTrack::SetMarked(bool is_marked) {
    if (is_marked) {
        SetColor(color_marked_);
        //详细标牌文字
        QString str = QString("批号：%0\n距离：%1\n方位：%2")
                      .arg(track_info_.index)
                      .arg(QString::number(track_info_.radius, 'f', 2))
                      .arg(QString::number(track_info_.azimuth, 'f', 2));
        track_marker_->SetText(str);
    } else {
        SetColor(color_);
        //标牌文字只显示批号
        track_marker_->SetText(QString::number(track_info_.index));
    }
    //更改是否标记标志
    is_marked_ = is_marked;
}
//航迹是否被标记
bool RadarTrack::IsMarked() {
    return is_marked_;
}
//获取航迹时间
QTime RadarTrack::GetTrackTime() {
    return track_info_.time;
}
//设置结束标记
void RadarTrack::SetEnd(bool is_end) {
    track_info_.is_end = is_end;
}
//航迹是否结束
bool RadarTrack::IsEnd() {
    return track_info_.is_end;
}
//设置未标记颜色
void RadarTrack::SetUnmarkedColor(const QColor& color) {
    color_ = color;
    if (!is_marked_) {
        SetColor(color_);
    }
}
//设置标记颜色
void RadarTrack::SetMarkedColor(const QColor& color) {
    color_marked_ = color;
    if (is_marked_) {
        SetColor(color_marked_);
    }
}
//初始化
void RadarTrack::InitAll() {
    InitSymbol();//初始化标志
    InitCurve();//初始化曲线
    InitMarker();//初始化标牌
}
//初始化标志
void RadarTrack::InitSymbol() {
    track_symbol_ = new QwtSymbol;
    //圆形
    track_symbol_->setStyle(QwtSymbol::Ellipse);
    //初始轮廓颜色
    track_symbol_->setPen(color_);
    //初始填充颜色
    track_symbol_->setBrush(QBrush(color_));
    //尺寸
    track_symbol_->setSize(QSize(4, 4));
}
//初始化曲线
void RadarTrack::InitCurve() {
    track_curve_ = new QwtPolarCurve;
    //线条画笔，颜色宽度
    track_curve_->setPen(QPen(color_, 2));
    //画线
    track_curve_->setStyle(QwtPolarCurve::Lines);
    //设置标志
    track_curve_->setSymbol(track_symbol_);
    //设置数据
    track_data_ = new RadarTrackData;
    track_curve_->setData(track_data_);
}
//初始化标牌
void RadarTrack::InitMarker() {
    track_marker_ = new RadarTrackMarker;
    track_marker_->SetColor(color_);
}
//设置颜色
void RadarTrack::SetColor(const QColor& color) {
    //轮廓颜色
    track_symbol_->setPen(color);
    //填充颜色
    track_symbol_->setBrush(QBrush(color));
    //曲线颜色
    track_curve_->setPen(QPen(color, 2));
    //标牌颜色
    track_marker_->SetColor(color);
}
