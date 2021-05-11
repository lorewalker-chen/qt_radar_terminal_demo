#include "global_settings.h"

#include <QFileSystemWatcher>
#include <QSettings>
#include <QFileInfo>
#include <QTextCodec>
#include <QMutex>
#include <QTimer>

QScopedPointer<GlobalSettings> GlobalSettings::self;

GlobalSettings::~GlobalSettings() {
    //释放文件监视器
    file_watcher_->deleteLater();
    //停止更新时间
    timer_->stop();
    timer_->deleteLater();
    //保存设置
    SaveSettings();
}
//返回单例的实例
GlobalSettings* GlobalSettings::Instance() {
    //使用双检锁，防止多线程中出错
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new GlobalSettings);
        }
    }
    return self.data();
}
//设置本地网络参数
void GlobalSettings::SetLocalNetParam(const UdpParameters& param) {
    local_net_param_ = param;
}
//设置雷达网络参数
void GlobalSettings::SetRadarNetParam(const UdpParameters& param) {
    radar_net_param_ = param;
}
//设置当前时间
void GlobalSettings::SetCurrentDateTime(const QDateTime& time) {
    current_time_ = time;
}
//设置雷达位置
void GlobalSettings::SetRadarLocation(const Location& location) {
    radar_location_ = location;
    emit RadarLocationChanged();
}
//设置雷达北向角
void GlobalSettings::SetRadarNorthAngle(const double& angle) {
    radar_north_angle_ = angle;
    emit RadarNorthAngleChanged();
}
//获取本地网络参数
UdpParameters GlobalSettings::GetLocalNetParam() {
    return local_net_param_;
}
//获取雷达网络参数
UdpParameters GlobalSettings::GetRadarNetParam() {
    return radar_net_param_;
}
//获取当前时间
QDateTime GlobalSettings::GetCurrentDateTime() {
    return current_time_;
}
//获取雷达位置
Location GlobalSettings::GetRadarLocation() {
    return radar_location_;
}
//获取雷达北向角
double GlobalSettings::GetRadarNorthAngle() {
    return radar_north_angle_;
}
//响应文件变化，重新读取配置文件，发送配置改变信号
void GlobalSettings::ConfigurationFileChanged(const QString& path) {
    if (path == configuration_file_path_) {
        ReadConfigurationFile();
        emit SettingsFileChanged();
    }
}
//更新当前时间，每次+1s
void GlobalSettings::UpdateCurrentTime() {
    current_time_ = current_time_.addSecs(1);
    emit TimeChanged();
}

GlobalSettings::GlobalSettings() {
    InitSettings();//初始化默认值
    CheckConfigurationFile();//检查配置文件是否存在
    ReadConfigurationFile();//读入配置文件
    InitFileWatcher();//初始化文件监视器
}
//初始化设置
void GlobalSettings::InitSettings() {
    //读取当前本机时间
    current_time_ = QDateTime::currentDateTime();
    //开启更新时间定时器
    timer_ = new QTimer;
    connect(timer_, &QTimer::timeout, this, &GlobalSettings::UpdateCurrentTime);
    timer_->start(1000);
    //配置文件路径
    configuration_file_path_ = "./config/network_settings.ini";
    //设置初始值
    local_net_param_.ipv4 = "192.168.2.230";
    local_net_param_.port = 9001;
    radar_net_param_.ipv4 = "192.168.2.241";
    radar_net_param_.port = 9002;
}
//检查配置文件是否存在,不存在则用初始值创建
void GlobalSettings::CheckConfigurationFile() {
    bool is_exist = IsFileExist(configuration_file_path_);
    if (!is_exist) {
        CreateInitialConfigurationFile();
    }
}
//判断文件是否存在
bool GlobalSettings::IsFileExist(const QString& file_path) {
    bool is_exist = false;
    QFileInfo file_info(file_path);
    if (file_info.exists()) {
        is_exist = true;
    }
    return is_exist;
}
//用初始值创建配置文件
void GlobalSettings::CreateInitialConfigurationFile() {
    QSettings* configuration_file = new QSettings(configuration_file_path_, QSettings::IniFormat);
    //设置文件编码格式为utf-8
    configuration_file->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //写入文件
    configuration_file->setValue("Local/ipv4", local_net_param_.ipv4);
    configuration_file->setValue("Local/port", local_net_param_.port);
    configuration_file->setValue("Radar/ipv4", radar_net_param_.ipv4);
    configuration_file->setValue("Radar/port", radar_net_param_.port);
    delete configuration_file;
}
//读取配置文件
void GlobalSettings::ReadConfigurationFile() {
    QSettings* configuration_file = new QSettings(configuration_file_path_, QSettings::IniFormat);
    //读取文件
    local_net_param_.ipv4 = configuration_file->value("Local/ipv4").toString();
    local_net_param_.port = configuration_file->value("Local/port").toInt();
    radar_net_param_.ipv4 = configuration_file->value("Radar/ipv4").toString();
    radar_net_param_.port = configuration_file->value("Radar/port").toInt();
    delete configuration_file;
}
//初始化文件监视器，对配置文件添加监视
void GlobalSettings::InitFileWatcher() {
    file_watcher_ = new QFileSystemWatcher;
    connect(file_watcher_, SIGNAL(fileChanged(const QString&)),
            this, SLOT(ConfigurationFileChanged(const QString&)));
    file_watcher_->addPath(configuration_file_path_);
}
//保存设置
void GlobalSettings::SaveSettings() {
    QSettings* configuration_file = new QSettings(configuration_file_path_, QSettings::IniFormat);
    //设置文件编码格式为utf-8
    configuration_file->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //写入文件
    configuration_file->setValue("Local/ipv4", local_net_param_.ipv4);
    configuration_file->setValue("Local/port", local_net_param_.port);
    configuration_file->setValue("Radar/ipv4", radar_net_param_.ipv4);
    configuration_file->setValue("Radar/port", radar_net_param_.port);
    delete configuration_file;
}
