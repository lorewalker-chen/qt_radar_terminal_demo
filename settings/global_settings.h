#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include <QDateTime>

struct UdpParameters {
    QString ipv4;
    int port;
};

struct Location {
    double longitude = 0;
    bool is_east = true;
    double latitude = 0;
    bool is_north = true;
    double height = 0;
};

class QFileSystemWatcher;
class QTimer;
class GlobalSettings: public QObject {
    Q_OBJECT
  public:
    ~GlobalSettings();
    static GlobalSettings* Instance();

    //设置
    void SetLocalNetParam(const UdpParameters& param);
    void SetRadarNetParam(const UdpParameters& param);
    void SetCurrentDateTime(const QDateTime& time);
    void SetRadarLocation(const Location& location);
    void SetRadarNorthAngle(const double& angle);
    //获取
    UdpParameters GetLocalNetParam();
    UdpParameters GetRadarNetParam();
    QDateTime GetCurrentDateTime();
    Location GetRadarLocation();
    double GetRadarNorthAngle();

  private slots:
    //响应本地配置文件改变
    void ConfigurationFileChanged(const QString& path);
    //更新本地时间
    void UpdateCurrentTime();

  private:
    static QScopedPointer<GlobalSettings> self;
    GlobalSettings();

    void InitSettings();//初始化设置
    void CheckConfigurationFile();//检查配置文件
    bool IsFileExist(const QString& file_path);//判断文件是否存在
    void CreateInitialConfigurationFile();//用初始值创建配置文件
    void ReadConfigurationFile();//读取配置文件
    void InitFileWatcher();//初始化文件监视器

    void SaveSettings();//保存设置

    QString configuration_file_path_;//配置文件路径
    QFileSystemWatcher* file_watcher_;//文件监视器

    UdpParameters local_net_param_;//本机网络参数
    UdpParameters radar_net_param_;//雷达网络参数

    QDateTime current_time_;//界面当前时间
    QTimer* timer_ = nullptr;

    Location radar_location_;//雷达地理位置
    double radar_north_angle_ = 0; //雷达北向角

  signals:
    void SettingsFileChanged();
    void TimeChanged();
    void RadarLocationChanged();
    void RadarNorthAngleChanged();
};

#endif // GLOBALSETTINGS_H
