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
