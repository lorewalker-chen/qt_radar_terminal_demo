#ifndef RADARRADIUSLINE_H
#define RADARRADIUSLINE_H

#include "qwt_polar_marker.h"

class QwtPolarPlot;
class RadiusLineDatas;
class QwtPolarCurve;

class RadarRadiusLine {
  public:
    explicit RadarRadiusLine(QwtPolarPlot* plot);
    ~RadarRadiusLine();

    //设置文字
    void SetText(const QString& str);
    //设置是否自动更新文字
    void AutoText(bool enable);
    //设置颜色
    void SetColor(const QColor& color);
    //设置位置
    void SetPosition(const QwtPointPolar& polar);


  private:
    //初始化
    void InitAll();
    void InitCurve();//初始化曲线
    void InitMarker();//初始化标牌

    RadiusLineDatas* line_data_ = nullptr;
    QwtPolarCurve* line_curve_ = nullptr;

    bool is_auto_text_ = false;
    QwtPolarMarker* line_marker_ = nullptr;
    QwtText line_text_;
};

#endif // RADARRADIUSLINE_H
