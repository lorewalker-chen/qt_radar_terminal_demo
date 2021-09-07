#include "plan_position_indicator.h"
#include "radar_radius_line.h"

#include "qwt_polar_grid.h"
#include "qwt_polar_panner.h"
#include "qwt_polar_magnifier.h"
#include "qwt_polar_canvas.h"

#include <QLabel>
#include <QDateTimeEdit>

#include <QMenu>

#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QTimer>

/**
 * @brief 自定义角度刻度文字
 */
class AzimuthScaleDraw: public QwtRoundScaleDraw {
  public:
    virtual QwtText label(double value) const {
        //将360改为0
        if (qFuzzyCompare(fmod(value, 360), 0.0)) {
            return QString("N");
        }
        return QwtRoundScaleDraw::label(value);
    }
};

/**
 * @brief 自定义距离刻度文字
 */
class ScaleDraw: public QwtScaleDraw {
  public:
    virtual QwtText label(double value) const {
        //不绘制中心的0
        if (value == 0) {
            return QString("");
        }
        //大于1000的加上单位km
        if (value > 1000) {
            QString temp = QString("%1").arg(value / 1000) + "km";
            return temp;
        }
        //其余加上单位m
        return QString("%1m").arg(value);
    }
};

/**
 * @brief 雷达平面方位指示器类
 */
PlanPositionIndicator::PlanPositionIndicator(QWidget* parent) : QwtPolarPlot(parent) {
    //适应父对象尺寸
    resize(parent->width(), parent->height());
    //初始化
    InitAll();
}

PlanPositionIndicator::~PlanPositionIndicator() {
    if (timer_auto_clear_tracks_->isActive()) {
        timer_auto_clear_tracks_->stop();
    }
    timer_auto_clear_tracks_->deleteLater();

    if (timer_refresh_->isActive()) {
        timer_refresh_->stop();
    }
    timer_refresh_->deleteLater();

    delete radar_zero_line_;

    delete angle_line_start_;
    delete angle_line_end_;

    delete radar_points_;

    delete grid_;
    delete panner_;
    delete magnifier_;

    left_up_label_->deleteLater();
    left_down_label_->deleteLater();
    right_up_label_->deleteLater();
    right_down_label_->deleteLater();

    menu_->deleteLater();
}
//设置点迹颜色
void PlanPositionIndicator::SetPointsColor(const QColor& color) {
    radar_points_->SetColor(color);
}
//添加点迹
void PlanPositionIndicator::AddPoint(int cpi, double radius, double azimuth) {
    //根据cpi周期清点
    if (is_auto_clear_points_) {
        radar_points_->RemovePoints(cpi - auto_clear_points_cpi_);
    }
    radar_points_->AddPoint(cpi, QwtPointPolar(azimuth + north_angle_, radius));
    //如果不显示点迹
    if (!is_show_points_) {
        HidePoints();
    }
    //更新左下角标签
    UpdateLeftDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//删除指定cpi点迹
void PlanPositionIndicator::RemovePoints(int cpi) {
    radar_points_->RemovePoints(cpi);
    //更新左下角标签
    UpdateLeftDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//删除所有点迹
void PlanPositionIndicator::ClearPoints() {
    radar_points_->RemoveAllPoints();
    //更新左下角标签
    UpdateLeftDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//设置自动清点cpi
void PlanPositionIndicator::SetAutoClearPointsCpi(int cpi) {
    auto_clear_points_cpi_ = cpi;
}
//设置是否根据cpi间隔自动清除航迹
void PlanPositionIndicator::SetAutoClearPointsByCpi(bool enable) {
    is_auto_clear_points_ = enable;
}
//显示点迹
void PlanPositionIndicator::ShowPoints() {
    radar_points_->ShowPoints();
    is_show_points_ = true;
}
//隐藏点迹
void PlanPositionIndicator::HidePoints() {
    radar_points_->HidePoints();
    is_show_points_ = false;
}
//设置航迹颜色
void PlanPositionIndicator::SetTracksColor(const QColor& color) {
    tracks_color = color;
}
//设置被标记航迹颜色
void PlanPositionIndicator::SetTracksMarkedColor(const QColor& color) {
    tracks_marked_color = color;
}
//添加航迹点
void PlanPositionIndicator::AddTrackPoint(RadarTrackInfo info) {
    //加上北向角
    info.azimuth += north_angle_;
    //如果不存在该批号的航迹，新建
    if (!radar_tracks_.contains(info.index)) {
        RadarTrack* track = new RadarTrack(this);
        track->SetUnmarkedColor(tracks_color);
        track->SetMarkedColor(tracks_marked_color);
        radar_tracks_.insert(info.index, track);
    }
    //添加航迹点
    radar_tracks_.value(info.index)->AddTrackPoint(info);
    //关注标记的航迹，如果无标记的航迹则关注最近的点
    if (info.index == focus_track_index_ || (focus_track_index_ == -1 && info.radius < focus_track_radius_)) {
        focus_track_radius_ = info.radius;
        focus_track_azimuth_ = info.azimuth;
        emit FocusOnTrackPolar(focus_track_radius_, focus_track_azimuth_);
    }
    //如果不显示航迹
    if (!is_show_tracks_) {
        HideTracks();
    }
    //更新左下角标签
    UpdateLeftDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//删除指定批号航迹
void PlanPositionIndicator::RemoveTrack(int index) {
    if (!radar_tracks_.contains(index)) {
        return;
    }
    RemoveTrackFromHash(index);
    //更新左下角标签
    UpdateLeftDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//删除所有航迹
void PlanPositionIndicator::ClearTracks() {
    for (int index : radar_tracks_.keys()) {
        RemoveTrackFromHash(index);
    }
    //更新左下角标签
    UpdateLeftDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//设置自动清航超时时间
void PlanPositionIndicator::SetAutoClearTracksTime(int msec) {
    track_timeout_msec_ = msec;
}
//设置是否根据时间自动清除航迹
void PlanPositionIndicator::SetAutoClearTracksByTime(bool enable) {
    if (enable && !timer_auto_clear_tracks_->isActive()) {
        timer_auto_clear_tracks_->start(500);
    }
    if (!enable && timer_auto_clear_tracks_->isActive()) {
        timer_auto_clear_tracks_->stop();
    }
}
//显示航迹
void PlanPositionIndicator::ShowTracks() {
    for (int index : radar_tracks_.keys()) {
        radar_tracks_.value(index)->ShowTrack();
    }
    //更改显示航迹标志
    is_show_tracks_ = true;
    //更改刷新标志
    is_need_refresh_ = true;
}
//隐藏航迹
void PlanPositionIndicator::HideTracks() {
    for (int index : radar_tracks_.keys()) {
        radar_tracks_.value(index)->HideTrack();
    }
    //更改显示航迹标志
    is_show_tracks_ = false;
    //更改刷新标志
    is_need_refresh_ = true;
}
//设置量程
void PlanPositionIndicator::SetRange(double range_meter) {
    //重新设置极径刻度
    setScale(QwtPolar::ScaleRadius, 0, range_meter);
    range_ = range_meter;
    //更新线
    UpdateRadarLines();
    //更改刷新标志
    is_need_refresh_ = true;
}
//设置角度范围
void PlanPositionIndicator::SetAngleRange(int start, int end) {
    angle_start_ = start;
    angle_end_ = end;
    //更新线
    UpdateRadarLines();
}
//设置北向角
void PlanPositionIndicator::SetNorthAngle(double angle) {
    north_angle_ = angle;
    //更新右上标签
    UpdateRightUpLabel();
    //更新线
    UpdateRadarLines();
    //更改刷新标志
    is_need_refresh_ = true;
}
//设置位置
void PlanPositionIndicator::SetPosition(double longitude, double latitude, double height) {
    longitude_ = longitude;
    latitude_ = latitude;
    height_ = height;
    //更新右上标签
    UpdateRightUpLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//设置时间
void PlanPositionIndicator::SetDateTime(const QDateTime& date_time) {
    current_date_time_ = date_time;
    //更新右下标签
    UpdateRightDownLabel();
    //更改刷新标志
    is_need_refresh_ = true;
}
//手动清屏
void PlanPositionIndicator::ClearAll() {
    ClearPoints();
    ClearTracks();
}
//事件过滤器
bool PlanPositionIndicator::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::HoverMove) {
        //悬停移动事件，获取当前鼠标位置
        QPoint point = canvas()->mapFromGlobal(QCursor::pos());
        //转化为极坐标
        QwtPointPolar polar = canvas()->invTransform(point);
        mouse_radius_ = polar.radius();
        mouse_azimuth_ = polar.azimuth();
        UpdateLeftUpLabel();
    }
    return QFrame::eventFilter(obj, event);
}
//鼠标点击事件
void PlanPositionIndicator::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint point = canvas()->mapFromGlobal(QCursor::pos());
        //转化为极坐标
        QwtPointPolar polar = canvas()->invTransform(point);
        //标记航迹
        MarkTrack(polar);
    }
}
//键盘按键事件
void PlanPositionIndicator::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F5) {
        ClearPoints();
    }
}
//右键菜单事件
void PlanPositionIndicator::contextMenuEvent(QContextMenuEvent* event) {
    Q_UNUSED(event);
    menu_->exec(QCursor::pos());
}
//初始化
void PlanPositionIndicator::InitAll() {
    InitStyle();//初始化整体风格
    InitScale();//初始化刻度
    InitGrid();//初始化网格
    InitPanner();//初始化平移器
    InitMagnifier();//初始化放大器
    InitRadarLines();//初始化雷达角度范围
    InitPoints();//初始化点迹
    InitLabel();//初始化标签
    InitMenu();//初始化右键菜单
    InitTimer();//初始化定时器

    //开启悬停事件
    setAttribute(Qt::WA_Hover, true);
    //安装事件过滤器
    installEventFilter(this);
}
//初始化整体风格
void PlanPositionIndicator::InitStyle() {
    //背景色，黑色，整体填充
    setPlotBackground(QBrush(Qt::black, Qt::SolidPattern));
}
//初始化刻度
void PlanPositionIndicator::InitScale() {
    //极角起始，默认：90
    setAzimuthOrigin(90 * M_PI / 180.0);
    //极角刻度，默认：0～360，间隔30
    setScale(QwtPolar::ScaleAzimuth, 360.00, 0.00, 30.0);
    //极径刻度
    setScale(QwtPolar::ScaleRadius, 0, range_);
}
//初始化网格
void PlanPositionIndicator::InitGrid() {
    grid_ = new QwtPolarGrid;
    //线的画笔
    grid_->setPen(QPen(Qt::green, 1, Qt::SolidLine));
    //文字的画笔
    grid_->setAxisPen(QwtPolar::AxisAzimuth, QPen(Qt::green, 1));
    //字体
    grid_->setFont(QFont("Times New Romans", 10, QFont::Bold));
    //角度刻度绘制
    grid_->setAzimuthScaleDraw(new AzimuthScaleDraw);
    //距离刻度只显示右边
    grid_->showAxis(QwtPolar::AxisTop, false);
    grid_->showAxis(QwtPolar::AxisBottom, false);
    grid_->showAxis(QwtPolar::AxisLeft, false);
    grid_->showAxis(QwtPolar::AxisRight, true);
    //距离刻度绘制
    grid_->setScaleDraw(QwtPolar::AxisRight, new ScaleDraw);
    //依附到绘制区域
    grid_->attach(this);
}
//初始化平移器
void PlanPositionIndicator::InitPanner() {
    panner_ = new QwtPolarPanner(this->canvas());
    //开启可平移
    panner_->setEnabled(true);
}
//初始化放大器
void PlanPositionIndicator::InitMagnifier() {
    magnifier_ = new QwtPolarMagnifier(this->canvas());
    //禁用鼠标按键
    magnifier_->setMouseButton(Qt::NoButton);
    //开启可放大
    magnifier_->setEnabled(true);
}
//初始化雷达角度范围
void PlanPositionIndicator::InitRadarLines() {
    //法线
    radar_zero_line_ = new RadarRadiusLine(this);
    radar_zero_line_->SetText("R");

    //起始
    angle_line_start_ = new RadarRadiusLine(this);
    angle_line_start_->AutoText(true);

    //结束
    angle_line_end_ = new RadarRadiusLine(this);
    angle_line_end_->AutoText(true);
}
//初始化标签
void PlanPositionIndicator::InitLabel() {
    //左上角
    left_up_label_ = new QLabel(this);
    //设置字体颜色白色，背景色透明
    left_up_label_->setStyleSheet("color:white;background-color: transparent;");
    left_up_label_->setFont(QFont("Times New Romans", 15, QFont::Bold));
    UpdateLeftUpLabel();

    //左下角
    left_down_label_ = new QLabel(this);
    //设置字体颜色白色，背景色透明
    left_down_label_->setStyleSheet("color:white;background-color: transparent;");
    left_down_label_->setFont(QFont("Times New Romans", 15, QFont::Bold));
    UpdateLeftDownLabel();

    //右上角
    right_up_label_ = new QLabel(this);
    //设置字体颜色白色，背景色透明
    right_up_label_->setStyleSheet("color:white;background-color: transparent;");
    right_up_label_->setFont(QFont("Times New Romans", 15, QFont::Bold));
    UpdateRightUpLabel();

    //右下角
    right_down_label_ = new QDateTimeEdit(this);
    //设置字体颜色白色，背景色透明
    right_down_label_->setStyleSheet("color:white;border: 0px solid black;background-color: transparent;");
    //设置显示格式
    right_down_label_->setDisplayFormat("yyyy/MM/dd hh:mm:ss");
    //设置不可编辑
    right_down_label_->setEnabled(false);
    right_down_label_->setButtonSymbols(QAbstractSpinBox::NoButtons);
    right_down_label_->setFont(QFont("Times New Romans", 15, QFont::Bold));
    UpdateRightDownLabel();
}
//初始化右键菜单
void PlanPositionIndicator::InitMenu() {
    menu_ = new QMenu;
    //点迹显隐
    QAction* show_points = new QAction("点迹", menu_);
    show_points->setCheckable(true);
    show_points->setChecked(true);
    connect(show_points, &QAction::toggled, this, &PlanPositionIndicator::SetPointsStatus);
    menu_->addAction(show_points);
    //航迹显隐
    QAction* show_tracks = new QAction("航迹", menu_);
    show_tracks->setCheckable(true);
    show_tracks->setChecked(true);
    connect(show_tracks, &QAction::toggled, this, &PlanPositionIndicator::SetTracksStatus);
    menu_->addAction(show_tracks);
    //间隔符
    menu_->addSeparator();
    //点迹自动刷新
    QAction* auto_clear_points = new QAction("点迹自动刷新", menu_);
    auto_clear_points->setCheckable(true);
    auto_clear_points->setChecked(true);
    connect(auto_clear_points, &QAction::toggled, this, &PlanPositionIndicator::SetAutoClearPointsByCpi);
    menu_->addAction(auto_clear_points);
    //航迹自动刷新
    QAction* auto_clear_tracks = new QAction("航迹自动刷新", menu_);
    auto_clear_tracks->setCheckable(true);
    auto_clear_tracks->setChecked(true);
    connect(auto_clear_tracks, &QAction::toggled, this, &PlanPositionIndicator::SetAutoClearTracksByTime);
    menu_->addAction(auto_clear_tracks);
    //间隔符
    menu_->addSeparator();
    //清空点迹
    menu_->addAction("清空点迹(F5)", this, &PlanPositionIndicator::ClearPoints);
    //清空航迹
    menu_->addAction("清空航迹", this, &PlanPositionIndicator::ClearTracks);
    //清屏
    menu_->addAction("清屏", this, &PlanPositionIndicator::ClearAll);
}
//初始化点迹
void PlanPositionIndicator::InitPoints() {
    radar_points_ = new RadarPoints(this);
}
//初始化定时器
void PlanPositionIndicator::InitTimer() {
    //自动清航定时器
    timer_auto_clear_tracks_ = new QTimer;
    connect(timer_auto_clear_tracks_, &QTimer::timeout, this, &PlanPositionIndicator::AutoClearTracksByTime);
    timer_auto_clear_tracks_->start(500);
    //刷新定时器
    timer_refresh_ = new QTimer;
    connect(timer_refresh_, &QTimer::timeout, this, &PlanPositionIndicator::Refresh);
    timer_refresh_->start(40);
}
//更新左上角标签
void PlanPositionIndicator::UpdateLeftUpLabel() {
    QString str = QString("距离：%0m\n方位：%1°")
                  .arg(QString::number(mouse_radius_, 'f', 2))
                  .arg(QString::number(mouse_azimuth_, 'f', 2));
    left_up_label_->setText(str);
    //调整大小
    left_up_label_->adjustSize();
    //更新标签位置
    left_up_label_->move(10, 10);
}
//更新左下角标签
void PlanPositionIndicator::UpdateLeftDownLabel() {
    count_points_ = radar_points_->Count();
    count_tracks_ = radar_tracks_.size();
    QString str = QString("点迹数：%0\n航迹数：%1")
                  .arg(count_points_)
                  .arg(count_tracks_);
    left_down_label_->setText(str);
    //调整大小
    left_down_label_->adjustSize();
    //更新标签位置
    left_down_label_->move(10, height() - left_down_label_->height() - 10);
}
//更新右上角标签
void PlanPositionIndicator::UpdateRightUpLabel() {
    QString str = QString("经度：%0°\n纬度：%1°\n高度：%2m\n北向角：%4°")
                  .arg(QString::number(longitude_, 'f', 6))
                  .arg(QString::number(latitude_, 'f', 6))
                  .arg(QString::number(height_, 'f', 3))
                  .arg(QString::number(north_angle_, 'f', 2));
    right_up_label_->setText(str);
    //调整大小
    right_up_label_->adjustSize();
    //更新标签位置
    right_up_label_->move(width() - right_up_label_->width() - 10, 10);
}
//更新右下角标签
void PlanPositionIndicator::UpdateRightDownLabel() {
    right_down_label_->setDateTime(current_date_time_);
    //调整大小
    right_down_label_->adjustSize();
    //更新标签位置
    right_down_label_->move(width() - right_down_label_->width() - 10, height() - right_down_label_->height() - 10);
}
//设置点迹状态
void PlanPositionIndicator::SetPointsStatus(bool is_show) {
    if (is_show) {
        ShowPoints();
    } else {
        HidePoints();
    }
}
//设置航迹状态
void PlanPositionIndicator::SetTracksStatus(bool is_show) {
    if (is_show) {
        ShowTracks();
    } else {
        HideTracks();
    }
}
//从哈希表中删除指定批号航迹
void PlanPositionIndicator::RemoveTrackFromHash(int index) {
    //如果被标记，标记标志-1
    if (radar_tracks_.value(index)->IsMarked()) {
        marked_count_ -= 1;
    }
    delete radar_tracks_[index];
    radar_tracks_[index] = nullptr;
    radar_tracks_.remove(index);
    //发送信号通知该批号被删除
    emit RemovedTrack(index);
}
/**
 * @brief 根据时间自动清航
 * 判断当前时间与航迹时间的差是否满足超时
 */
void PlanPositionIndicator::AutoClearTracksByTime() {
    QTime current_time = current_date_time_.time();
    for (int index : radar_tracks_.keys()) {
        if (radar_tracks_.value(index)->GetTrackTime().msecsTo(current_time) >= track_timeout_msec_) {
            RemoveTrack(index);
        }
    }
}
//标记航迹
void PlanPositionIndicator::MarkTrack(const QwtPointPolar& polar) {
    double distance = 10;
    int index = -1;
    for (int key : radar_tracks_.keys()) {
        double temp_distance = radar_tracks_.value(key)->MinDistanceToPoint(polar);
        if (temp_distance < distance) {
            distance = temp_distance;
            index = key;
        }
    }
    //如果有附近的航迹
    if (-1 != index) {
        //该批航迹是否已经被标记
        bool is_markd = radar_tracks_.value(index)->IsMarked();
        if (!is_markd) {
            //如果还未标记，检查标记航迹数量是否到达最大数量
            if (marked_count_ >= max_marked_count_) {
                return;
            } else {
                //更改关注的航迹批号
                focus_track_index_ = index;
                //计数加一
                marked_count_ += 1;
            }
        } else {
            //如果是当前关注的批号，取消关注
            if (focus_track_index_ == index) {
                focus_track_index_ = -1;
            }
            //计数减一
            marked_count_ -= 1;
        }
        radar_tracks_.value(index)->Mark();
        //发送改变标记的航迹批号，和是否标记
        emit MarkedTrack(index, !is_markd);
    }
    //更改刷新标志
    is_need_refresh_ = true;
}
//刷新
void PlanPositionIndicator::Refresh() {
    if (is_need_refresh_) {
        replot();
    }
}
//更新线
void PlanPositionIndicator::UpdateRadarLines() {
    //雷达法相线
    radar_zero_line_->SetPosition(QwtPointPolar(north_angle_, range_));
    //角度范围线
    double start = angle_start_ + north_angle_;
    if (start > 360) {
        start -= 360;
    }
    double end = angle_end_ + north_angle_;
    if (end > 360) {
        end -= 360;
    }

    angle_line_start_->SetPosition(QwtPointPolar(start, range_));
    angle_line_end_->SetPosition(QwtPointPolar(end, range_));
}
