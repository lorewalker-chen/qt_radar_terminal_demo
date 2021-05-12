#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt_point_polar.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class PlanPositionIndicator;
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private slots:
    void UpdateTime();
    void UpdateCurrentMousePolar(const QwtPointPolar& polar);

    void on_checkBox_points_stateChanged(int arg1);
    void on_checkBox_tracks_stateChanged(int arg1);
    void on_pushButton_clear_points_clicked();
    void on_pushButton_clear_tracks_clicked();
    void on_pushButton_clear_all_clicked();
    void on_pushButton_exit_clicked();



  private:
    void InitAll();
    void InitWindowStyle();
    void InitStyleSheet();
    void InitTime();
    void InitPPI();
    void InitNetWork();
    void InitUdpSender();
    void InitUdpReceiver();

    Ui::MainWindow* ui;
    PlanPositionIndicator* ppi_;
};
#endif // MAINWINDOW_H
