#ifndef TRACKLASTPOINT_H
#define TRACKLASTPOINT_H

#include <qwt_polar_marker.h>

class TrackLastPoint: public QwtPolarMarker {
  public:
    TrackLastPoint(int index, double azimuth, double radius);

    void SetIndex(int index);
    int GetIndex();
    void SetTextColor(const QColor& color);

    void SetPointColor(const QColor& color);
    void SetPointSize(const QSize& size);
    void ChangePosition(double azimuth, double radius);

  private:
    int index_ = 0;
    QwtSymbol* symbol_ = nullptr;
    QwtText text_;
};

#endif // TRACKLASTPOINT_H
