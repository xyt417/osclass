#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// 设备队列类
class DeviceQueue {
private:
    vector<string> available_devices;   // 可用设备列表
    map<string, vector<string>> occupied_devices;  // 正在使用的设备字典，键为设备名，值为使用该设备的进程列表

public:
    // 添加设备到可用设备列表
    void add_device(string device_name) {
        available_devices.push_back(device_name);
    }

    // 从可用设备列表中删除设备
    void remove_device(string device_name) {
        vector<string>::iterator it = find(available_devices.begin(), available_devices.end(), device_name);
        if (it != available_devices.end()) {
            available_devices.erase(it);
        }
    }

    // 分配设备给进程
    bool allocate_device(string device_name, string process_name) {
        // 如果设备不可用，则返回 false
        vector<string>::iterator it = find(available_devices.begin(), available_devices.end(), device_name);
        if (it == available_devices.end()) {
            return false;
        }

        // 如果设备已经被使用，则将进程添加到设备的使用列表中
        if (occupied_devices.find(device_name) != occupied_devices.end()) {
            occupied_devices[device_name].push_back(process_name);
        }
        // 如果设备未被使用，则创建一个新的使用列表并添加进程
        else {
            occupied_devices[device_name] = vector<string>{process_name};
        }

        // 从可用设备列表中删除该设备
        remove_device(device_name);
        return true;
    }

    // 回收设备
    void deallocate_device(string device_name, string process_name) {
        // 将进程从设备的使用列表中删除
        vector<string>& processes = occupied_devices[device_name];
        vector<string>::iterator it = find(processes.begin(), processes.end(), process_name);
        if (it != processes.end()) {
            processes.erase(it);
        }

        // 如果设备的使用列表为空，则将设备添加回可用设备列表中
        if (processes.empty()) {
            add_device(device_name);
            occupied_devices.erase(device_name);
        }
    }

    // 获取可用设备列表
    vector<string> get_available_devices() {
        return available_devices;
    }

    // 获取正在使用设备的字典
    map<string, vector<string>> get_occupied_devices() {
        return occupied_devices;
    }
};

