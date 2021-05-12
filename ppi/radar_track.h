#ifndef RADARTRACK_H
#define RADARTRACK_H

#include <qwt_polar_curve.h>

class SeriesPolarPoints;
class RadarTrack: public QwtPolarCurve {
  public:
    RadarTrack();

    void SetColor(const QColor& color);

    void SetCapacity(int capacity);
    void AddPoint(double azimuth, double radius);
    void ClearPoints();

    bool HasPoint(double azimuth, double radius);

    void SetMarkColor(const QColor& color);
    void SetMarked(bool is_mark);
    bool IsMarked();

    void SetEnd(bool is_end);
    bool IsEnd();

    QwtPointPolar LastPoint();

  private:
    void SetSymbolColor(const QColor& color);

    QColor color_;
    QColor marked_color_;

    bool is_marked_ = false;
    bool is_end_ = false;

    SeriesPolarPoints* points_ = nullptr;
};

#endif // RADARTRACK_H
