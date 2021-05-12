#include "plan_position_indicator.h"
#include "radar_points.h"
#include "radar_track.h"
#include "track_last_point.h"
#include "scanning_line.h"

#include <qwt_polar_grid.h>
#include <qwt_polar_magnifier.h>
#include <qwt_polar_panner.h>
#include <qwt_polar_canvas.h>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>

PlanPositionIndicator::PlanPositionIndicator(QWidget* parent): QwtPolarPlot(parent) {
    //区域大小，父对象的宽高
    resize(parent->width(), parent->height());
    InitAll();
    //安装事件过滤器
    installEventFilter(this);
}

PlanPositionIndicator::~PlanPositionIndicator() {
    delete grid_;
    delete panner_;
    delete zoomer_;

    delete points_;
    delete scanning_line_;

    scanning_line_timer_->deleteLater();
}
//设置量程
void PlanPositionIndicator::SetRange(int range) {
    range_ = range;
    setScale(QwtPolar::ScaleRadius, 0, range_);
    this->replot();
}
//向点迹中添加一个点，并刷新
void PlanPositionIndicator::AddPoint(double azimuth, double radius) {
    points_->AddPoint(azimuth, radius);
    this->replot();
}
//显示点迹
void PlanPositionIndicator::ShowPoints() {
    points_->show();
    this->replot();
}
//隐藏点迹
void PlanPositionIndicator::HidePoints() {
    points_->hide();
    this->replot();
}
//清空点迹
void PlanPositionIndicator::ClearPoints() {
    points_->ClearPoints();
    this->replot();
}
//向航迹中添加一个点，并刷新
void PlanPositionIndicator::AddTrackPoint(uint16_t index, double azimuth, double radius, bool is_end) {
    if (!tracks_list_.contains(index)) {
        //如果哈希表中没有该批号的航迹
        //添加一条新航迹
        RadarTrack* track = new RadarTrack;
        track->SetColor(Qt::green);
        track->AddPoint(azimuth, radius);
        track->attach(this);
        tracks_list_.insert(index, track);
        //添加一个新末尾点
        TrackLastPoint* last_point = new TrackLastPoint(index, azimuth, radius);
        last_point->SetPointColor(Qt::green);
        last_point->attach(this);
        tracks_last_points_list_.insert(index, last_point);
    } else {
        //如果存在该批号的航迹
        if (tracks_list_[index]->IsEnd()) {
            //如果保存的航迹结束，清除并重新添加
            tracks_list_.remove(index);
            RadarTrack* track = new RadarTrack;
            track->SetColor(Qt::green);
            track->AddPoint(azimuth, radius);
            track->attach(this);
            tracks_list_.insert(index, track);
            TrackLastPoint* last_point = new TrackLastPoint(index, azimuth, radius);
            last_point->SetPointColor(Qt::green);
            last_point->attach(this);
            tracks_last_points_list_.insert(index, last_point);
        } else {
            //如果保存的航迹未结束，添加新点
            tracks_list_[index]->AddPoint(azimuth, radius);
            //更改末尾点位置
            tracks_last_points_list_[index]->ChangePosition(azimuth, radius);
        }
    }
    //设置结束标记
    tracks_list_[index]->SetEnd(is_end);
    //刷新
    this->replot();
}
//显示航迹
void PlanPositionIndicator::ShowTracks() {
    //显示每一条航迹
    for (uint16_t index : tracks_list_.keys()) {
        tracks_list_[index]->show();
    }
    //显示每一个末尾点
    for (uint16_t index : tracks_last_points_list_.keys()) {
        tracks_last_points_list_[index]->show();
    }
    this->replot();
}
//隐藏航迹
void PlanPositionIndicator::HideTracks() {
    //隐藏每一条航迹
    for (uint16_t index : tracks_list_.keys()) {
        tracks_list_[index]->hide();
    }
    //隐藏每一个末尾点
    for (uint16_t index : tracks_last_points_list_.keys()) {
        tracks_last_points_list_[index]->hide();
    }
    this->replot();
}
//清空航迹
void PlanPositionIndicator::ClearTracks() {
    for (uint16_t index : tracks_list_.keys()) {
        tracks_list_[index]->detach();
        delete tracks_list_[index];
        tracks_list_[index] = nullptr;
    }
    tracks_list_.clear();
    for (uint16_t index : tracks_last_points_list_.keys()) {
        tracks_last_points_list_[index]->detach();
        delete tracks_last_points_list_[index];
        tracks_last_points_list_[index] = nullptr;
    }
    tracks_last_points_list_.clear();
    this->replot();
}
//根据角度手动转动扫描线
void PlanPositionIndicator::RotateScanningLineManually(double angle) {
    scanning_end_angle_ = angle;
    scanning_line_->SetEndPolar(scanning_end_angle_, range_);
    this->replot();
}
//事件过滤器
bool PlanPositionIndicator::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::HoverMove) {
        //悬停移动事件，获取当前鼠标位置
        QPoint point = canvas()->mapFromGlobal(QCursor::pos());
        //转化为极坐标
        QwtPointPolar polar = canvas()->invTransform(point);
        emit CurrentMousePolar(polar);
    }
    return QFrame::eventFilter(obj, event);
}
//鼠标点击事件
void PlanPositionIndicator::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        //获取当前鼠标位置
        QPoint point = canvas()->mapFromGlobal(QCursor::pos());
        //转化为极坐标
        QwtPointPolar polar = canvas()->invTransform(point);
        for (uint16_t index : tracks_list_.keys()) {
            if (tracks_list_[index]->HasPoint(polar.azimuth(), polar.radius())) {
                //如果包含该点，设置标记与原来相反
                tracks_list_[index]->SetMarked(!tracks_list_[index]->IsMarked());
                this->replot();
            }
        }
    }
}
//初始化
void PlanPositionIndicator::InitAll() {
    InitStyle();
    InitScale();
    InitPanner();
    InitZoomer();
    InitGrid();
    InitScanningLine();
    InitPoints();
}
//初始化整体风格
void PlanPositionIndicator::InitStyle() {
    //背景色，黑色，整体填充
    setPlotBackground(QBrush(Qt::black, Qt::SolidPattern));
    //开启悬停事件
    setAttribute(Qt::WA_Hover, true);
}
//初始化刻度
void PlanPositionIndicator::InitScale() {
    //极角起始，90
    setAzimuthOrigin(90 * M_PI / 180.0);
    //极角刻度，0～360，间隔30
    setScale(QwtPolar::ScaleAzimuth, 360.00, 0.00, 30.0);
    //极径刻度，0～30000
    setScale(QwtPolar::ScaleRadius, 0, range_);
}
//初始化平移器
void PlanPositionIndicator::InitPanner() {
    panner_ = new QwtPolarPanner(this->canvas());
    //开启可平移
    panner_->setEnabled(true);
}
//初始化缩放器
void PlanPositionIndicator::InitZoomer() {
    zoomer_ = new QwtPolarMagnifier(this->canvas());
    //开启可缩放
    zoomer_->setEnabled(true);
}
//初始化网格
void PlanPositionIndicator::InitGrid() {
    //网格
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
//初始化扫描线
void PlanPositionIndicator::InitScanningLine() {
    //扫描线
    scanning_line_ = new ScanningLine;
    scanning_line_->SetEndPolar(scanning_end_angle_, range_);
    scanning_line_->attach(this);
    //定时转动扫描线
    scanning_line_timer_ = new QTimer;
    connect(scanning_line_timer_, &QTimer::timeout, this, &PlanPositionIndicator::RotateScanningLine);
#if 0
    scanning_line_timer_->start(40);
#endif
}
//初始化点迹
void PlanPositionIndicator::InitPoints() {
    points_ = new RadarPoints;
    points_->attach(this);
    //添加测试点
    points_->AddPoint(15, 4600);
    //航迹
    RadarTrack* track = new RadarTrack;
    track->SetColor(Qt::red);
    for (int i = 0; i < 50; i++) {
        track->AddPoint(20, i * 100);
    }
    track->attach(this);
    tracks_list_.insert(1, track);
//    tracks_list_[1]->SetMarked(true);
    //航迹标记
    TrackLastPoint* la = new TrackLastPoint(1, 20, 4900);
    la->attach(this);
    tracks_last_points_list_.insert(1, la);
}
//转动扫描线
void PlanPositionIndicator::RotateScanningLine() {
    scanning_end_angle_ += 1;
    scanning_line_->SetEndPolar(scanning_end_angle_, range_);
    this->replot();
}
//角度刻度文字
QwtText AzimuthScaleDraw::label(double value) const {
    QwtText text;
    //将360改为0
    if (qFuzzyCompare(fmod(value, 360), 0.0)) {
        return QString("0");
    }
    return QwtRoundScaleDraw::label(value);
}
//距离刻度文字
QwtText ScaleDraw::label(double value) const {
    QwtText text;
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
