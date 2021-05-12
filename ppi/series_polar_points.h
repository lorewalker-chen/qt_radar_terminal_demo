#ifndef SERIESPOLARPOINTS_H
#define SERIESPOLARPOINTS_H

#include <qwt_series_data.h>

class SeriesPolarPoints: public QwtSeriesData<QwtPointPolar> {
  public:
    SeriesPolarPoints();
    ~SeriesPolarPoints();

    void AddPoint(double azimuth, double radius);
    void ClearPoints();
    void SetCapacity(int capacity);
    bool HasPoint(double azimuth, double radius);
    QwtPointPolar LastPoint();

  private:
    virtual size_t size() const;
    virtual QwtPointPolar sample(size_t i) const;
    virtual QRectF boundingRect() const;

    int capacity_ = -1;
    QList<QwtPointPolar> points_list_;
};

#endif // SERIESPOLARPOINTS_H
