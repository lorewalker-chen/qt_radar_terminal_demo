#ifndef RADARTRACKTABLEMODEL_H
#define RADARTRACKTABLEMODEL_H

#include "radar_track_info.h"

#include <QAbstractTableModel>

class RadarTrackTableModel : public QAbstractTableModel {
    Q_OBJECT
  public:
    explicit RadarTrackTableModel(QObject* parent = nullptr);

    //获取表头
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    //获取行数
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    //获取列数
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    //获取单元格数据
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    //设置单元格数据
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  public slots:
    //添加
    void AddItem(const RadarTrackInfo& info);
    //删除
    void RemoveItem(int index);
    //标记
    void MarkItem(int index, bool is_marked);

  private slots:
    //根据批号找航迹数据
    int FindItemByIndex(const QList<QVariantList>& list, int index);

  private:
    //表头
    QStringList head_list_;
    //标记的航迹
    QList<QVariantList> marked_item_list_;
    //未标记的航迹
    QList<QVariantList> item_list_;

  signals:

};

#endif // RADARTRACKTABLEMODEL_H
