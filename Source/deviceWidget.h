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

// �Զ����豸������
class DeviceWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DeviceWindow(QString deviceType = "none", QString deviceName = "none", QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Device: " + deviceName);
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        layout = new QVBoxLayout(centralWidget);

        // ��ʾ����
        printerWidget = new QPlainTextEdit(centralWidget);
        if(deviceType == "printer")
            printerWidget->setReadOnly(true);
        layout->addWidget(printerWidget);
        // ״̬��ǩ
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
    QWidget *centralWidget; // ���Ĵ���
    QVBoxLayout *layout; // ���ֹ�����
    QPlainTextEdit *printerWidget; // ��ӡ�����
    QLabel *statusLabel; // ״̬��ǩ
};


// ��������
class DeviceMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DeviceMainWindow(DeviceTable &deviceTable, DeviceQueue &deviceQueue, int logger = 0, QWidget *parent = nullptr)
     : QMainWindow(parent), deviceTable(deviceTable), deviceQueue(deviceQueue), logger(logger) {
        setWindowTitle("DeviceWindow");
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        layout = new QVBoxLayout(centralWidget);
        statusLabel = new QLabel(centralWidget);
        layout->addWidget(statusLabel);

        // �����豸����
        createDeviceWindows();
        updateDeviceStatus();

        // ��ʼ���豸ָ��
        occupied_devices = deviceQueue.get_occupied_devices();
        devicePointer = occupied_devices.begin();

        // ���ö�ʱ��
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(running()));
        timer->start(10); // ����Ƶ�� 10ms

    }

    void enableLogger() {logger = 1;}
    void disableLogger() {logger = 0;}

    string argi(string request, int index){ // ����ĵ� i ������
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
        // �����豸״̬��ʾ
        QString statusText;
        for (const auto &device : deviceTable.deviceList) {
            statusText += "DeviceName: " + QString::fromStdString(device.name) + "  |  Type: "\
             + QString::fromStdString(device.type) + "  |  DevicePriority: " + QString::number(device.priority)\ 
             + "  |  Status: " + (device.status ? "Busy" : "Free") + "  |  Process: " + QString::fromStdString(device.pname) + "\n";
            if (device.type == "screen") {
                // ������Ļ�豸���ڵ�״̬
                screenWindows[device.name]->setStatus(device.status);
            } else if (device.type == "printer") {
                // ���´�ӡ���豸���ڵ�״̬
                printerWindows[device.name]->setStatus(device.status);
            }
        }
        statusLabel->setText(statusText);
    }

    void running(){
        // ����occupied_devices
        if(devicePointer == occupied_devices.end()){
            occupied_devices = deviceQueue.get_occupied_devices();
            devicePointer = occupied_devices.begin();
            // ����max_priority
            max_priority = 0;
            for(auto device : occupied_devices){
                int priority = deviceTable[device.first].priority;
                if(priority > max_priority) max_priority = priority;
            }
        }
        // ����occupied_devices
        while(devicePointer != occupied_devices.end() && deviceTable[devicePointer->first].priority < max_priority)
            ++ devicePointer; // ���ȼ�������ߵ��豸��ִ��
        if(devicePointer == occupied_devices.end()) return;
        string device_name = devicePointer->first;
        vector<DevRequest> requests = devicePointer->second;
        string process_name = requests[0].pname;
        string request = requests[0].requestStr;
        // screen
        if(deviceTable[device_name].type == "screen"){
            // request = "print,text"
            // ��Ļ��ӡ
            if(request.find("print") != string::npos){
                if(logger) cout << "�豸 " << device_name << " ִ�н��� " << process_name << " ������:[" << request << "]\n";
                screenWindows["screen1"]->print(QString::fromStdString(argi(request, 2)));
            }
        // printer
        }else if(deviceTable[device_name].type == "printer"){
            // request = "print,text"
            // ��ӡ����ӡ
            if(request.find("print") != string::npos){
                if(logger) cout << "�豸 " << device_name << " ִ�н��� " << process_name << " ������:[" << request << "]\n";
                printerWindows[device_name]->print(QString::fromStdString(argi(request, 2)));
            }
        }
        // �ͷ��豸
        deviceQueue.release_device(device_name, process_name);
        ++ devicePointer;
        updateDeviceStatus();
    }    

private:
    void createDeviceWindows() {
        for (const auto &device : deviceTable.deviceList) {
            if (device.type == "screen") {
                // ������Ļ�豸����
                DeviceWindow *screenWindow = new DeviceWindow("screen", QString::fromStdString(device.name), this);
                screenWindows[device.name] = screenWindow;
                screenWindow->show();
            } else if (device.type == "printer") {
                // ������ӡ���豸����
                DeviceWindow *printerWindow = new DeviceWindow("printer", QString::fromStdString(device.name), this);
                printerWindows[device.name] = printerWindow;
                printerWindow->show();
            }
        }
    }

    int logger = 0;

    // running����ʹ��
    map<string, vector<DevRequest>>::iterator devicePointer; // �豸ָ��
    map<string, vector<DevRequest>> occupied_devices; // ��ʱ�豸�ֵ�
    int max_priority = 0; // ������ȼ�

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QLabel *statusLabel;
    QTimer *timer;
    DeviceTable &deviceTable; // �����豸��Ϣ��
    DeviceQueue &deviceQueue; // �����豸����
    QMap<string, DeviceWindow*> screenWindows; // ��Ļ�豸����
    QMap<string, DeviceWindow*> printerWindows; // ��ӡ���豸����
};
