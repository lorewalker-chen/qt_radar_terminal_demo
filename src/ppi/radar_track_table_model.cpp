#include "radar_track_table_model.h"

#include <QColor>

RadarTrackTableModel::RadarTrackTableModel(QObject* parent) : QAbstractTableModel(parent) {
    head_list_.append("批号");
    head_list_.append("距离(m)");
    head_list_.append("方位(°)");
    head_list_.append("速度(m/s)");
    head_list_.append("航向(°)");
    head_list_.append("类别");
}
//获取表头
QVariant RadarTrackTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            return head_list_.at(section);
        }
    }
    return QVariant();
}
//获取行数
int RadarTrackTableModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return marked_item_list_.count() + item_list_.count();
}
//获取列数
int RadarTrackTableModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return head_list_.count();
}
//获取单元格数据
QVariant RadarTrackTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    int row = index.row();
    int column = index.column();
    //内容
    if (role == Qt::DisplayRole) {
        if (row < marked_item_list_.count()) {
            return marked_item_list_.at(row).at(column);
        } else {
            return item_list_.at(row - marked_item_list_.count()).at(column);
        }
    }
    //文字颜色
    if (role == Qt::TextColorRole) {
        //被标记的批号单独设置颜色
        if (row < marked_item_list_.count()) {
            return QColor(Qt::green);
        }
    }
    return QVariant();
}
//设置单元格数据
bool RadarTrackTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    //航迹表格不允许编辑
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}
//添加项目
void RadarTrackTableModel::AddItem(const RadarTrackInfo& info) {
    QVariantList list;
    list.append(info.index);
    list.append(info.radius);
    list.append(info.azimuth);
    list.append(info.velocity);
    list.append(info.course);
    list.append(info.type);
    beginResetModel();
    //查找标记列表中是否存在该批号
    int i = FindItemByIndex(marked_item_list_, info.index);
    if (i >= 0) {
        //如果有该批，修改
        marked_item_list_[i] = list;
    } else {
        //如果无该批，查找未标记列表中是否存在该批号
        i = FindItemByIndex(item_list_, info.index);
        if (i >= 0) {
            //如果有该批，修改
            item_list_[i] = list;
        } else {
            //如果无该批，添加
            item_list_.prepend(list);
        }
    }
    endResetModel();
}
//删除项目
void RadarTrackTableModel::RemoveItem(int index) {
    beginResetModel();
    int i = FindItemByIndex(item_list_, index);
    if (i >= 0) {
        item_list_.removeAt(i);
    } else {
        i = FindItemByIndex(marked_item_list_, index);
        marked_item_list_.removeAt(i);
    }
    endResetModel();
}
//改变标记
void RadarTrackTableModel::MarkItem(int index, bool is_marked) {
    beginResetModel();
    if (is_marked) {
        //如果被标记，从未标记列表中找对应批号并标记
        int i = FindItemByIndex(item_list_, index);
        if (i >= 0) {
            marked_item_list_.prepend(item_list_.at(i));
            item_list_.removeAt(i);
        }
    } else {
        //如果不标记，从已标记列表中找到对应批号取消标记
        int i = FindItemByIndex(marked_item_list_, index);
        if (i >= 0) {
            item_list_.prepend(marked_item_list_.at(i));
            marked_item_list_.removeAt(i);
        }
    }
    endResetModel();
}
//根据批号找航迹数据
int RadarTrackTableModel::FindItemByIndex(const QList<QVariantList>& list, int index) {
    //因为要保持批号的无序性，故采用遍历的形式查找
    for (int i = list.count() - 1; i >= 0; --i) {
        if (list.at(i).at(0).toInt() == index) {
            return i;
        }
    }
    //没找到返回-1
    return -1;
}
