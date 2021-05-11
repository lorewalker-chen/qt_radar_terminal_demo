#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
