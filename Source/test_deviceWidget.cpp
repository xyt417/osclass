#include<deviceWidget.h>
#include<device.h>

void runDevice(string device, DeviceQueue &device_queue){
    string process;
    device_queue.release_device(device, process);
    if(process == NOEXIST) {
        cout << "设备 " << device << " 不存在" << endl;
        return;
    }
    if(process == EMPTY){
        cout <<  "设备 " << device << " 当前没有任务" << endl;
        return;
    }
    cout << "设备 " << device << " 已执行进程 " << process << " 的任务" << endl;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    // 创建设备信息表并添加设备
    DeviceTable deviceTable;
    deviceTable.add_device("screen1", "screen");
    deviceTable.add_device("printer1", "printer");
    // 使用设备信息表初始化设备队列
    DeviceQueue deviceQueue(deviceTable);

    // 分配设备给进程
    deviceQueue.allocate_device("screen", "process1");
    deviceQueue.allocate_device("printer", "process2");
    deviceQueue.allocate_device("printer", "process3");
    deviceQueue.allocate_device("printer", "process4");
    deviceQueue.allocate_device("printer", "process5");
    deviceQueue.allocate_device("printer", "process6");
    
    for(int i = 0; i < 5; ++ i){
        runDevice("printer1", deviceQueue);
    }

    // 创建主窗口
    MainWindow mainWindow(deviceTable, deviceQueue);
    mainWindow.show();



    return app.exec();
}