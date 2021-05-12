#ifndef PLANPOSITIONINDICATOR_H
#define PLANPOSITIONINDICATOR_H

#include <qwt_polar_plot.h>
#include <qwt_round_scale_draw.h>
#include <qwt_scale_draw.h>

class QwtPolarGrid;
class QwtPolarPanner;
class QwtPolarMagnifier;

class RadarPoints;
class RadarTrack;
class TrackLastPoint;
class ScanningLine;

class QTimer;

class PlanPositionIndicator: public QwtPolarPlot {
    Q_OBJECT
  public:
    explicit PlanPositionIndicator(QWidget* parent);
    ~PlanPositionIndicator();
    //设置量程
    void SetRange(int range);
    //点迹
    void AddPoint(double azimuth, double radius);
    void ShowPoints();
    void HidePoints();
    void ClearPoints();
    //航迹
    void AddTrackPoint(uint16_t index, double azimuth, double radius, bool is_end);
    void ShowTracks();
    void HideTracks();
    void ClearTracks();
    //扫描线
    void RotateScanningLineManually(double angle);

  protected:
    bool eventFilter(QObject* obj, QEvent* event);
    void mousePressEvent(QMouseEvent* event);

  private slots:
    void InitAll();
    void InitStyle();
    void InitScale();
    void InitPanner();
    void InitZoomer();
    void InitGrid();
    void InitScanningLine();
    void InitPoints();

    void RotateScanningLine();

  private:
    int range_ = 30000;

    QwtPolarGrid* grid_ = nullptr;
    QwtPolarPanner* panner_ = nullptr;
    QwtPolarMagnifier* zoomer_ = nullptr;
    //点迹
    RadarPoints* points_ = nullptr;
    //航迹
    QHash<uint16_t, RadarTrack*> tracks_list_;
    //航迹末尾点，用于显示批号
    QHash<uint16_t, TrackLastPoint*> tracks_last_points_list_;
    //扫描线
    double scanning_end_angle_ = 0;
    ScanningLine* scanning_line_ = nullptr;

    QTimer* scanning_line_timer_ = nullptr;

  signals:
    void CurrentMousePolar(const QwtPointPolar& polar);
};

class AzimuthScaleDraw: public QwtRoundScaleDraw {
  public:
    virtual QwtText label(double value) const;
};

class ScaleDraw: public QwtScaleDraw {
  public:
    virtual QwtText label(double value) const;
};

#endif // PLANPOSITIONINDICATOR_H
