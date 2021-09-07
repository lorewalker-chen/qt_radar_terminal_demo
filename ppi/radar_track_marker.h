#ifndef RADARTRACKMARKER_H
#define RADARTRACKMARKER_H

#include "qwt_polar_marker.h"

class RadarTrackMarker: public QwtPolarMarker {
  public:
    RadarTrackMarker();

    //设置文字
    void SetText(const QString& text);
    //设置颜色
    void SetColor(const QColor& color);//所有颜色
    void SetTextColor(const QColor& color);//文字
    void SetSymbolColor(const QColor& color);//标志点

  private :
    //初始化
    void InitAll();
    void InitSymbol();//标志点
    void InitText();//文字

    QwtSymbol* marker_symbol_ = nullptr;
    QwtText marker_text_;
};

#endif // RADARTRACKMARKER_H
