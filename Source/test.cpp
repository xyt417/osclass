#include"device_queue.h"
int main() {
    DeviceQueue device_queue;

    // 添加设备到设备队列
    device_queue.add_device("device1");
    device_queue.add_device("device2");
    device_queue.add_device("device3");

    // 获取可用设备列表
    vector<string> available_devices = device_queue.get_available_devices();
    cout << "Available Devices: ";
    for (string device_name : available_devices) {
        cout << device_name << " ";
    }
    cout << endl;

    // 分配设备给进程
    bool result = device_queue.allocate_device("device1", "process1");
    if (result) {
        cout << "Device allocated to process" << endl;
    }
    else {
        cout << "Device not available" << endl;
    }

    // 获取正在使用设备的字典
    map<string, vector<string>> occupied_devices = device_queue.get_occupied_devices();
    cout << "Occupied Devices: " << endl;
    for (auto& pair : occupied_devices) {
        string device_name = pair.first;
        vector<string> processes = pair.second;
        cout << device_name << ": ";
        for (string process_name : processes) {
            cout << process_name << " ";
        }
        cout << endl;
    }

    // 回收设备
    device_queue.deallocate_device("device1", "process1");

    // 获取可用设备列表
    available_devices = device_queue.get_available_devices();
    cout << "Available Devices: ";
    for (string device_name : available_devices) {
        cout << device_name << " ";
    }
    cout << endl;

    return 0;
}
