#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QTimer>
#include <QMap>
#include <QThread>
#include "device.h"

// 自定义设备窗口类
class DeviceWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit DeviceWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Device Window");
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        layout = new QVBoxLayout(centralWidget);
        statusLabel = new QLabel("Status: Free", centralWidget);
        layout->addWidget(statusLabel);
    }

    void setStatus(bool isBusy) {
        statusLabel->setText("Status: " + QString(isBusy ? "Busy" : "Free"));
    }

private:
    QWidget *centralWidget; // 中心窗口
    QVBoxLayout *layout; // 布局管理器
    QLabel *statusLabel; // 状态标签
};

// 主窗口类
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(DeviceTable &deviceTable, DeviceQueue &deviceQueue, QWidget *parent = nullptr)
     : QMainWindow(parent), deviceTable(deviceTable), deviceQueue(deviceQueue) {
        setWindowTitle("Main Window");
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        layout = new QVBoxLayout(centralWidget);
        statusLabel = new QLabel(centralWidget);
        layout->addWidget(statusLabel);

        // 创建设备窗口
        createDeviceWindows();
        
        // 设置定时器，定时更新设备状态
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateDeviceStatus()));
        timer->start(1000); // 每秒更新一次
    }

public slots:
    void updateDeviceStatus() {
        QString statusText;
        for (const auto &device : deviceTable.deviceList) {
            statusText += "Device Name: " + QString::fromStdString(device.name) + "  |  Type: "\
             + QString::fromStdString(device.type) + "  |  Status: " + (device.status ? "Busy" : "Free")\
             + "  |  Process: " + QString::fromStdString(device.pname) + "\n";
            if (device.type == "screen") {
                // 更新屏幕设备窗口的状态
                screenWindows[device.name]->setStatus(device.status);
            } else if (device.type == "printer") {
                // 更新打印机设备窗口的状态
                printerWindows[device.name]->setStatus(device.status);
            }
        }
        statusLabel->setText(statusText);
    }

private:
    void createDeviceWindows() {
        for (const auto &device : deviceTable.deviceList) {
            if (device.type == "screen") {
                // 创建屏幕设备窗口
                DeviceWindow *screenWindow = new DeviceWindow(this);
                screenWindows[device.name] = screenWindow;
                screenWindow->show();
            } else if (device.type == "printer") {
                // 创建打印机设备窗口
                DeviceWindow *printerWindow = new DeviceWindow(this);
                printerWindows[device.name] = printerWindow;
                printerWindow->show();
            }
        }
    }

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QLabel *statusLabel;
    QTimer *timer;
    DeviceTable &deviceTable;
    DeviceQueue &deviceQueue;
    QMap<string, DeviceWindow*> screenWindows; // 屏幕设备窗口
    QMap<string, DeviceWindow*> printerWindows; // 打印机设备窗口
};
