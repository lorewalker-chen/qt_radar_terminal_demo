#ifndef RADARPOINTS_H
#define RADARPOINTS_H

#include "qwt_point_polar.h"

class QwtPolarPlot;
class QwtSymbol;
class RadarPointsData;
class QwtPolarCurve;

class RadarPoints {
  public:
    RadarPoints(QwtPolarPlot* plot);
    ~RadarPoints();

    //添加点
    void AddPoint(int cpi, const QwtPointPolar& polar);
    //删除一个cpi周期的点
    void RemovePoints(int cpi);
    //删除全部点
    void RemoveAllPoints();
    //显示点迹
    void ShowPoints();
    //隐藏点迹
    void HidePoints();

    //设置点迹颜色
    void SetColor(const QColor& color);

    //设置点迹容量
    void SetCapacity(int capacity);
    //获取点迹容量
    int Capacity();

    //获取点迹数量
    int Count();

  private:
    //初始化
    void InitAll();
    void InitSymbol();//初始化标志
    void InitCurve();//初始化曲线

    //标志点
    QwtSymbol* points_symbol_ = nullptr;
    //航迹数据
    RadarPointsData* points_data_ = nullptr;
    //航迹曲线
    QwtPolarCurve* points_curve_ = nullptr;
};

#endif // RADARPOINTS_H
