#ifndef SCANNINGLINE_H
#define SCANNINGLINE_H

#include <qwt_polar_curve.h>

class SeriesPolarPoints;
class ScanningLine: public QwtPolarCurve {
  public:
    ScanningLine();

    void SetColor(const QColor& color);
    void SetWidth(int width);
    void SetEndPolar(double azimuth, double radius);

  private:
    void SetPen();

    QColor color_;
    int width_;
    SeriesPolarPoints* plots_ = nullptr;
};

#endif // SCANNINGLINE_H
