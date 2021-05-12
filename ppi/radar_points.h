#ifndef RADARPOINTS_H
#define RADARPOINTS_H

#include <qwt_polar_curve.h>

class SeriesPolarPoints;
class RadarPoints: public QwtPolarCurve {
  public:
    RadarPoints();

    void SetColor(const QColor& color);

    void SetCapacity(int capacity);
    void AddPoint(double azimuth, double radius);
    void ClearPoints();

  private:
    SeriesPolarPoints* points_ = nullptr;
};

#endif // RADARPOINTS_H
