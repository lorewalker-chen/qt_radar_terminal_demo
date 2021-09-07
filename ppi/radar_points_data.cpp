#include "radar_points_data.h"

RadarPointsData::RadarPointsData() {
    data_cpi_.clear();
    data_.clear();
}
//添加点
void RadarPointsData::AddPoint(int cpi, const QwtPointPolar& polar) {
    //如果超过容量,删除第一个点
    if (capacity_ > 0 && count_ > capacity_) {
        data_cpi_.first().removeFirst();
        //计数-1
        count_ -= 1;
    }
    //判断该cpi是否存在
    if (data_cpi_.contains(cpi)) {
        //如果该cpi存在，直接添加
        data_cpi_[cpi].append(polar);
    } else {
        //如果该cpi不存在，添加该cpi的点列表
        QList<QwtPointPolar> list;
        list.append(polar);
        data_cpi_.insert(cpi, list);
    }
    //添加成功，计数+1
    count_ += 1;
    //重组数据
    RecombineData();
}
//删除一个cpi周期的点
void RadarPointsData::RemovePoints(int cpi) {
    if (!data_cpi_.contains(cpi)) {
        return;
    }
    //计数减
    count_ -= data_cpi_.value(cpi).size();
    //删除该cpi周期的点
    data_cpi_.remove(cpi);
    //重组数据
    RecombineData();
}
//删除全部点
void RadarPointsData::RemoveAllPoint() {
    //清空
    data_cpi_.clear();
    //重组数据
    RecombineData();
    //计数清零
    count_ = 0;
}
//设置点迹容量
void RadarPointsData::SetCapacity(int capacity) {
    capacity_ = capacity;
}
//获取点迹容量
int RadarPointsData::Capacity() {
    return capacity_;
}
//获取点迹数量
int RadarPointsData::Count() {
    return count_;
}
//将带cpi的数据重组成一个列表
void RadarPointsData::RecombineData() {
    data_.clear();
    for (int key : data_cpi_.keys()) {
        data_.append(data_cpi_.value(key));
    }
}
//返回总点数
size_t RadarPointsData::size() const {
    return count_;
}
//采样，返回从0~(size-1)的每个点
QwtPointPolar RadarPointsData::sample(size_t i) const {
    return data_.at(i);
}
//返回绘图区域
QRectF RadarPointsData::boundingRect() const {
    return qwtBoundingRect(*this);
}
