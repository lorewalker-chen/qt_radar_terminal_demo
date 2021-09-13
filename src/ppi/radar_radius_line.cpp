#include "radar_radius_line.h"
#include "qwt_series_data.h"
#include "qwt_polar_curve.h"
#include "qwt_polar_marker.h"

class RadiusLineDatas: public QwtSeriesData<QwtPointPolar> {
  public:
    RadiusLineDatas() {
        data_.append(QwtPointPolar(0, 0));
        data_.append(QwtPointPolar(0, 0));
    };
    //改变位置
    void ChangePosition(const QwtPointPolar& polar) {
        data_[1] = polar;
    };

  private:
    //返回总点数
    virtual size_t size() const {
        return 2;
    };
    //采样，返回从0~(size-1)的每个点
    virtual QwtPointPolar sample(size_t i) const {
        return data_.at(i);
    };
    //返回绘图区域
    virtual QRectF boundingRect() const {
        return qwtBoundingRect(*this);
    };

    //数据列表
    QList<QwtPointPolar> data_;
};

RadarRadiusLine::RadarRadiusLine(QwtPolarPlot* plot) {
    //初始化
    InitAll();
    //曲线依附到图层
    line_curve_->attach(plot);
    //文字依附到图层
    line_marker_->attach(plot);
}

RadarRadiusLine::~RadarRadiusLine() {
    line_curve_->detach();
    delete line_curve_;

    line_marker_->detach();
    delete line_marker_;
}
//设置文字
void RadarRadiusLine::SetText(const QString& str) {
    line_text_.setText(str);
    line_marker_->setLabel(line_text_);
}
//设置是否自动更新文字
void RadarRadiusLine::AutoText(bool enable) {
    is_auto_text_ = enable;
}
//设置颜色
void RadarRadiusLine::SetColor(const QColor& color) {
    line_curve_->setPen(QPen(color, 2));

    line_text_.setColor(color);
    line_marker_->setLabel(line_text_);
}
//设置位置
void RadarRadiusLine::SetPosition(const QwtPointPolar& polar) {
    //设置数据
    line_data_->ChangePosition(polar);
    //设置标牌位置
    line_marker_->setPosition(polar);
    //显示文字
    if (is_auto_text_) {
        line_text_.setText(QString::number(qRound(polar.azimuth())));
        line_marker_->setLabel(line_text_);
    }
}
//初始化
void RadarRadiusLine::InitAll() {
    InitCurve();//初始化曲线
    InitMarker();//初始化标牌
}
//初始化曲线
void RadarRadiusLine::InitCurve() {
    line_curve_ = new QwtPolarCurve;
    //线条画笔
    line_curve_->setPen(QPen(Qt::gray, 2));
    //画线
    line_curve_->setStyle(QwtPolarCurve::Lines);
    //设置数据
    line_data_ = new RadiusLineDatas;
    line_curve_->setData(line_data_);
}
//初始化标牌
void RadarRadiusLine::InitMarker() {
    line_marker_ = new QwtPolarMarker;
    line_text_.setFont(QFont("Times New Roman", 12, QFont::Bold));
    line_text_.setColor(Qt::gray);
    line_marker_->setLabel(line_text_);
    //位置，中上
    line_marker_->setLabelAlignment(Qt::AlignJustify | Qt::AlignTop);
}
