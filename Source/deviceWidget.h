#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QTimer>
#include <QMap>
#include <QThread>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QScrollBar>
#include "device.h"

// 自定义设备窗口类
class DeviceWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DeviceWindow(QString deviceType = "none", QString deviceName = "none", QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle(deviceName);
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        layout = new QVBoxLayout(centralWidget);

        // 显示部件
        printerWidget = new QPlainTextEdit(centralWidget);
        if(deviceType == "printer")
            printerWidget->setReadOnly(true);
        layout->addWidget(printerWidget);
        // 状态标签
        statusLabel = new QLabel("Status: Free", this);
        statusLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
        layout->addWidget(statusLabel);

        layout->setAlignment(statusLabel, Qt::AlignBottom | Qt::AlignRight);
    }

    void setStatus(bool isBusy) {
        statusLabel->setText("Status: " + QString(isBusy ? "Busy" : "Free"));
    }

    void print(const QString& text) {
        printerWidget->insertPlainText(text + '\n');
        printerWidget->moveCursor(QTextCursor::End);
        printerWidget->verticalScrollBar()->setValue(printerWidget->verticalScrollBar()->maximum());
    }

private:
    QWidget *centralWidget; // 中心窗口
    QVBoxLayout *layout; // 布局管理器
    QPlainTextEdit *printerWidget; // 打印机组件
    QLabel *statusLabel; // 状态标签
};


// 主窗口类
class DeviceMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DeviceMainWindow(DeviceTable &deviceTable, DeviceQueue &deviceQueue, int logger = 0, QWidget *parent = nullptr)
     : QMainWindow(parent), deviceTable(deviceTable), deviceQueue(deviceQueue) {
        setWindowTitle("Main Window");
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        layout = new QVBoxLayout(centralWidget);
        statusLabel = new QLabel(centralWidget);
        layout->addWidget(statusLabel);

        // 创建设备窗口
        createDeviceWindows();
        updateDeviceStatus();

        // 设置定时器
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(running()));
        timer->start(100); // 每1/10更新1次

    }

    void enableLogger() {logger = 1;}
    void disableLogger() {logger = 0;}

    string argi(string request, int index){
        int i = 1;
        int start = 0;
        int end = request.find(",");
        while(i < index){
            start = request.find(",", start) + 1;
            end = request.find(",", start);
            ++ i;
        }
        return request.substr(start, end - start);
    }

public slots:
    void updateDeviceStatus() {
        // 更新设备状态显示
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


    void running(){
        // 获取occupied_devices
        map<string, vector<DevRequest>> occupied_devices = deviceQueue.get_occupied_devices();
        // 遍历occupied_devices
        for(auto device : occupied_devices){
            string device_name = device.first;
            vector<DevRequest> requests = device.second;
            if(requests.size() == 0) continue;
            string process_name = requests[0].pname;
            string request = requests[0].requestStr;
            // screen
            if(deviceTable[device_name].type == "screen"){
                // request = "print,text"
                // 屏幕打印
                if(request.find("print") != string::npos){
                    if(logger) cout << "设备 " << device_name << " 执行进程 " << process_name << " 的任务:[" << request << "]\n";
                    screenWindows["screen1"]->print(QString::fromStdString(argi(request, 2)));
                }
            // printer
            }else if(deviceTable[device_name].type == "printer"){
                // request = "print,text"
                // 打印机打印
                if(request.find("print") != string::npos){
                    if(logger) cout << "设备 " << device_name << " 执行进程 " << process_name << " 的任务:[" << request << "]\n";
                    printerWindows[device_name]->print(QString::fromStdString(argi(request, 2)));
                }
            }
            // 释放设备
            deviceQueue.release_device(device_name, process_name);
        }

        updateDeviceStatus();
    }    

private:
    void createDeviceWindows() {
        for (const auto &device : deviceTable.deviceList) {
            if (device.type == "screen") {
                // 创建屏幕设备窗口
                DeviceWindow *screenWindow = new DeviceWindow("screen", QString::fromStdString(device.name), this);
                screenWindows[device.name] = screenWindow;
                screenWindow->show();
            } else if (device.type == "printer") {
                // 创建打印机设备窗口
                DeviceWindow *printerWindow = new DeviceWindow("printer", QString::fromStdString(device.name), this);
                printerWindows[device.name] = printerWindow;
                printerWindow->show();
            }
        }
    }

    int logger = 0;

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QLabel *statusLabel;
    QTimer *timer;
    DeviceTable &deviceTable; // 引用设备信息表
    DeviceQueue &deviceQueue; // 引用设备队列
    QMap<string, DeviceWindow*> screenWindows; // 屏幕设备窗口
    QMap<string, DeviceWindow*> printerWindows; // 打印机设备窗口
};
