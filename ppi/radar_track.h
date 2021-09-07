#ifndef RADARTRACK_H
#define RADARTRACK_H

#include "radar_track_info.h"

#include "qwt_point_polar.h"
#include <QColor>

class QwtPolarPlot;
class QwtSymbol;
class RadarTrackData;
class QwtPolarCurve;
class RadarTrackMarker;

class RadarTrack {
  public:
    explicit RadarTrack(QwtPolarPlot* plot);
    ~RadarTrack();

    //添加航迹点
    void AddTrackPoint(const RadarTrackInfo& info);
    //显示航迹
    void ShowTrack();
    //隐藏航迹
    void HideTrack();

    //到指定点的最小距离
    double MinDistanceToPoint(const QwtPointPolar& polar);
    //将航迹是否标记置为相反
    void Mark();
    //设置航迹是否标记
    void SetMarked(bool is_marked);
    //航迹是否被标记
    bool IsMarked();

    //获取航迹时间
    QTime GetTrackTime();

    //设置航迹是否结束
    void SetEnd(bool is_end);
    //航迹是否结束
    bool IsEnd();

    //设置颜色
    void SetUnmarkedColor(const QColor& color);//未标记
    void SetMarkedColor(const QColor& color);//标记

  private:
    //初始化
    void InitAll();
    void InitSymbol();//初始化标志
    void InitCurve();//初始化曲线
    void InitMarker();//初始化标牌

    //设置颜色
    void SetColor(const QColor& color);

    //标志点
    QwtSymbol* track_symbol_ = nullptr;
    //航迹数据
    RadarTrackData* track_data_ = nullptr;
    //航迹曲线
    QwtPolarCurve* track_curve_ = nullptr;
    //航迹末尾标牌
    RadarTrackMarker* track_marker_ = nullptr;

    //未标记颜色
    QColor color_ = Qt::yellow;
    //标记颜色
    QColor color_marked_ = Qt::blue;
    //是否被标记
    bool is_marked_ = false;

    //航迹属性
    RadarTrackInfo track_info_;
};

#endif // RADARTRACK_H
