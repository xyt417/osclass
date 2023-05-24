#include <QThread>
#include <device.h>
#include <deviceWidget.h>
class MyThread : public QThread {
    Q_OBJECT
private:
    DeviceQueue &deviceQueue;
    DeviceMainWindow &deviceMainWindow;
signals:
    void startSignal();
    void stopSignal();
public:
    MyThread(DeviceQueue &deviceQueue, DeviceMainWindow &deviceMainWindow) : deviceQueue(deviceQueue), deviceMainWindow(deviceMainWindow) {
        connect(this, SIGNAL(startSignal()), &deviceMainWindow, SLOT(start()));
        connect(this, SIGNAL(stopSignal()), &deviceMainWindow, SLOT(stop()));
    }
    void run() override {
      // Test
        int n = 100, m = 100;
        while(n --){
            deviceQueue.allocate_device("printer", "p1", "print,p1: hello printer num:" + to_string(m - n), 1);
            deviceQueue.allocate_device("printer", "p2", "print,p2: hello printer num:" + to_string(m - n), 2);
            deviceQueue.allocate_device("printer", "p3", "print,p3: hello printer num:" + to_string(m - n));
            deviceQueue.allocate_device("printer", "p0", "print,p0: hello printer num:" + to_string(m - n));
            deviceQueue.allocate_device("printer", "p0", "print,p0: hello printer num:" + to_string(m - n));
        }
        // 暂停 3 秒 等待设备处理线程处理任务
        QThread::msleep(3000);
        emit stopSignal(); // 测试暂停设备处理线程
        n = 30, m = 30;
        while(n --){
            deviceQueue.allocate_device("screen", "p1", "print,p1: hello screen num:" + to_string(m - n));
            deviceQueue.allocate_device("screen", "p2", "print,p2: hello screen num:" + to_string(m - n));
            deviceQueue.allocate_device("screen", "p3", "print,p3: hello screen num:" + to_string(m - n));
        }
        // 暂停 10 秒
        QThread::msleep(10000);
        n = 200, m = 200;
        while(n --){
            deviceQueue.allocate_device("screen", "p0", "print,p0: hello screen num:" + to_string(m - n));
            deviceQueue.allocate_device("printer", "p0", "print,p0: hello screen num:" + to_string(m - n));
        }
        emit startSignal(); // 测试开始设备处理线程
    }
};