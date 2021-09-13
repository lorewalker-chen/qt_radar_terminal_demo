#ifndef PLANPOSITIONINDICATOR_H
#define PLANPOSITIONINDICATOR_H

#include "qwt_polar_plot.h"
#include "qwt_round_scale_draw.h"
#include "qwt_scale_draw.h"
#include "radar_points.h"
#include "radar_track.h"

class QwtPolarGrid;
class QwtPolarPanner;
class QwtPolarMagnifier;
class RadarRadiusLine;

class QLabel;
class QDateTimeEdit;
class QMenu;

class PlanPositionIndicator : public QwtPolarPlot {
    Q_OBJECT
  public:
    explicit PlanPositionIndicator(QWidget* parent = nullptr);
    ~PlanPositionIndicator();

  public slots:
    //点迹相关
    void SetPointsColor(const QColor& color);//设置点迹颜色
    void AddPoint(int cpi, double radius, double azimuth);//添加点迹
    void RemovePoints(int cpi);//删除指定cpi点迹
    void ClearPoints();//删除所有点迹
    void SetAutoClearPointsCpi(int cpi = 20);//设置自动清点cpi
    void SetAutoClearPointsByCpi(bool enable);//设置是否根据cpi间隔自动清除航迹
    void ShowPoints();//显示点迹
    void HidePoints();//隐藏点迹

    //航迹相关
    void SetTracksColor(const QColor& color);//设置未标记的航迹颜色
    void SetTracksMarkedColor(const QColor& color);//设置标记的航迹颜色
    void AddTrackPoint(RadarTrackInfo info);//添加航迹
    void RemoveTrack(int index);//删除指定批号航迹
    void ClearTracks();//删除所有航迹
    void SetAutoClearTracksTime(int msec);//设置自动清航超时时间
    void SetAutoClearTracksByTime(bool enable);//设置是否根据时间自动清除航迹
    void ShowTracks();//显示航迹
    void HideTracks();//隐藏航迹

    //PPI相关
    void SetRange(double range_meter);//设置量程
    void SetAngleRange(int start, int end); //设置角度范围
    void SetNorthAngle(double angle);//设置北向角
    void SetPosition(double longitude, double latitude, double height); //设置位置
    void SetDateTime(const QDateTime& date_time);//设置时间
    void ClearAll();//手动清屏

  protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

  private slots:
    //初始化
    void InitAll();
    void InitStyle();//初始化整体风格
    void InitScale();//初始化刻度
    void InitGrid();//初始化网格
    void InitPanner();//初始化平移器
    void InitMagnifier();//初始化放大器
    void InitRadarLines();//初始化雷达角度范围
    void InitLabel();//初始化标签
    void InitMenu();//初始化右键菜单
    void InitPoints();//初始化点迹
    void InitTimer();//初始化定时器

    //标签相关
    void UpdateLeftUpLabel();//左上
    void UpdateLeftDownLabel();//左下
    void UpdateRightUpLabel();//右上
    void UpdateRightDownLabel();//右下

    //菜单相关
    void SetPointsStatus(bool is_show);//设置点迹显隐状态
    void SetTracksStatus(bool is_show);//设置航迹显隐状态

    //从哈希表中删除指定批号航迹
    void RemoveTrackFromHash(int index);
    //自动清航
    void AutoClearTracksByTime();
    //标记航迹
    void MarkTrack(const QwtPointPolar& polar);

    //PPI控制
    void Refresh();//刷新
    void UpdateRadarLines();//更新线

  private:
    //整体样式
    double range_ = 30000; //量程:单位m
    QwtPolarGrid* grid_ = nullptr;//网格

    //控制
    QwtPolarPanner* panner_ = nullptr;//平移器
    QwtPolarMagnifier* magnifier_ = nullptr;//缩放器

    //北向线
    RadarRadiusLine* radar_zero_line_ = nullptr;

    //雷达角度范围线
    double angle_start_ = 0;
    double angle_end_ = 0;
    RadarRadiusLine* angle_line_start_ = nullptr;
    RadarRadiusLine* angle_line_end_ = nullptr;

    //标签数据
    //左上
    double north_angle_ = 0;//北向角
    double mouse_radius_ = 0;//鼠标距离
    double mouse_azimuth_ = 0;//鼠标方位
    //左下
    int count_points_ = 0; //点迹总数
    int count_tracks_ = 0; //航迹总数
    //右上
    double longitude_ = 0; //经度
    double latitude_ = 0; //纬度
    double height_ = 0; //高度
    //右下
    QDateTime current_date_time_; //时间

    //标签
    QLabel* left_up_label_ = nullptr;
    QLabel* left_down_label_ = nullptr;
    QLabel* right_up_label_ = nullptr;
    QDateTimeEdit* right_down_label_ = nullptr;

    //右键菜单
    QMenu* menu_ = nullptr;

    //点迹
    RadarPoints* radar_points_ = nullptr;
    bool is_show_points_ = true;//是否显示点迹
    bool is_auto_clear_points_ = true;//是否自动清点
    int auto_clear_points_cpi_ = 20;//自动清点cpi间隔

    //航迹
    QHash<int, RadarTrack*> radar_tracks_;
    bool is_show_tracks_ = true;//是否显示航迹
    QColor tracks_color = Qt::yellow;
    QColor tracks_marked_color = Qt::blue;
    int max_marked_count_ = 5;
    int marked_count_ = 0;
    int track_timeout_msec_ = 10000;//超时时间ms
    QTimer* timer_auto_clear_tracks_ = nullptr;//自动清航定时器

    //刷新
    bool is_need_refresh_ = false; //控制是否需要刷新
    QTimer* timer_refresh_ = nullptr;//刷新定时器

  signals:
    void MarkedTrack(int index, bool is_marked);
    void RemovedTrack(int index);
};


#endif // PLANPOSITIONINDICATOR_H
