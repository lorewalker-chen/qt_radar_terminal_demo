#ifndef RADARTRACKDATA_H
#define RADARTRACKDATA_H

#include "qwt_series_data.h"

class RadarTrackData: public QwtSeriesData<QwtPointPolar> {
  public:
    RadarTrackData();

    //添加航迹点
    void AddTrackPoint(const QwtPointPolar& polar);
    //到指定点的最小距离
    double MinDistanceToPoint(const QwtPointPolar& polar);

  private:
    //返回总点数
    virtual size_t size() const;
    //采样，返回从0~(size-1)的每个点
    virtual QwtPointPolar sample(size_t i) const;
    //返回绘图区域
    virtual QRectF boundingRect() const;

    //数据列表
    QList<QwtPointPolar> data_;
};

#endif // RADARTRACKDATA_H
