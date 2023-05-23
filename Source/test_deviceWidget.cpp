#include<deviceWidget.h>
#include<device.h>
// device���������������:
void allocateDevice(string device, string process, DeviceQueue &deviceQueue, string request = ""){
    bool result = deviceQueue.allocate_device(device, process, request); 
    if (result) {
        cout << "���� " << process << " �ɹ������豸 " << device << endl;
    }else {
        cout << "�豸���� " << device << " ������" << endl;
    }
}
void runDevice(string device, DeviceQueue &deviceQueue){
    string process;
    deviceQueue.release_device(device, process);
    if(process == NOEXIST) {
        cout << "�豸 " << device << " ������" << endl;
        return;
    }
    if(process == EMPTY){
        cout <<  "�豸 " << device << " ��ǰû������" << endl;
        return;
    }
    cout << "�豸 " << device << " ��ִ�н��� " << process << " ������" << endl;
}
void printInfo(DeviceQueue &deviceQueue, DeviceTable &deviceTable){
    cout << "==== DeviceQueue Status =====" << endl;
    deviceQueue.print_avaliable_devices();
    deviceQueue.print_occupied_devices();
    cout << "======= Device Table ========" << endl;
    deviceTable.printInfo();
    cout << "=============================" << endl;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);



    // �����豸��Ϣ������豸
    DeviceTable deviceTable;
    deviceTable.add_device("screen1", "screen");
    deviceTable.add_device("printer1", "printer");
    deviceTable.add_device("printer2", "printer");
    deviceTable.add_device("printer3", "printer");
    // ʹ���豸��Ϣ���ʼ���豸����
    DeviceQueue deviceQueue(deviceTable);

    // �����豸������
    DeviceMainWindow deviceMainWindow(deviceTable, deviceQueue, 1);
    deviceMainWindow.show();

    // Test
    int n = 100, m = 100;
    while(n --){
        deviceQueue.allocate_device("printer", "p1", "print,p1: hello printer num:" + to_string(m - n));
        deviceQueue.allocate_device("printer", "p2", "print,p2: hello printer num:" + to_string(m - n));
        deviceQueue.allocate_device("printer", "p3", "print,p3: hello printer num:" + to_string(m - n));
        deviceQueue.allocate_device("printer", "p0", "print,p0: hello printer num:" + to_string(m - n));
        deviceQueue.allocate_device("printer", "p0", "print,p0: hello printer num:" + to_string(m - n));
        deviceQueue.allocate_device("screen", "p1", "print,p1: hello screen num:" + to_string(m - n));
        deviceQueue.allocate_device("screen", "p2", "print,p2: hello screen num:" + to_string(m - n));
        deviceQueue.allocate_device("screen", "p3", "print,p3: hello screen num:" + to_string(m - n));
    }

    return app.exec();
}