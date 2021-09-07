#ifndef RADARPOINTSDATA_H
#define RADARPOINTSDATA_H

#include "qwt_series_data.h"

class RadarPointsData: public QwtSeriesData<QwtPointPolar> {
  public:
    RadarPointsData();

    //添加点
    void AddPoint(int cpi, const QwtPointPolar& polar);
    //删除一个cpi周期的点
    void RemovePoints(int cpi);
    //删除全部点
    void RemoveAllPoint();

    //设置点迹容量
    void SetCapacity(int capacity);
    //获取点迹容量
    int Capacity();

    //获取点迹数量
    int Count();

  private:
    //将带cpi的数据重组成一个列表
    void RecombineData();
    //返回总点数
    size_t size() const;
    //采样，返回从0~(size-1)的每个点
    QwtPointPolar sample(size_t i) const;
    //返回绘图区域
    QRectF boundingRect() const;

    //带cpi的数据
    QMap<quint16, QList<QwtPointPolar>> data_cpi_;
    //所有数据列表,为了减少采样的时间复杂度
    QList<QwtPointPolar> data_;

    //点迹容量
    int capacity_ = -1;
    //点迹当前数量
    int count_ = 0;
};

#endif // RADARPOINTSDATA_H
