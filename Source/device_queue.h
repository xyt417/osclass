#ifndef DEVICE
#define DEVICE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define NOEXIST "noexist"
#define EMPTY "empty"

// 设备队列类
class DeviceQueue {
private:
    vector<string> devices = {"printer1", "printer2", "screen"}; 

    vector<string> available_devices;   // 可用设备列表
    map<string, vector<string>> occupied_devices;  // 正在使用的设备字典，键为设备名，值为使用该设备的进程列表


public:
    DeviceQueue() {
        //初始化空闲设备列表
        for(string device : devices){
            available_devices.push_back(device);
        }
    }

    // 分配设备给进程 (设备名称，进程名称)
    bool allocate_device(string device_name, string process_name, string info = "") {
        // 如果设备不存在，则返回 false
        vector<string>::iterator it = find(devices.begin(), devices.end(), device_name);
        if (it == devices.end()) {
            return false;
        }

        // 如果设备已经被使用，则将进程添加到设备的使用列表中
        if (occupied_devices.find(device_name) != occupied_devices.end()) {
            occupied_devices[device_name].push_back(process_name);
        }
        // 如果设备未被使用，则创建一个新的使用列表并添加进程，并将该设备移出空闲列表
        else {
            it = find(available_devices.begin(), available_devices.end(), device_name);
            available_devices.erase(it);
            occupied_devices[device_name] = vector<string>{process_name};
        }
        return true;
    }

    // 释放设备 (设备名称, 释放的设备名称)
    bool release_device(string device_name, string &process_name, string info = "") {
        // 如果设备不存在，返回 false
        vector<string>::iterator it = find(devices.begin(), devices.end(), device_name);
        if (it == devices.end()) {
            process_name = NOEXIST;
            return false;
        }

        // 如果该设备未被占用，返回 flase
        if (occupied_devices.find(device_name) == occupied_devices.end()){
            process_name = EMPTY;
            return false;
        }

        // 将进程从设备的使用列表中删除
        vector<string>& processes = occupied_devices[device_name];
        process_name = *processes.begin();
        processes.erase(processes.begin());

        // 如果设备的使用列表为空，则将设备添加回可用设备列表中
        if (processes.empty()) {
            available_devices.push_back(device_name);
            occupied_devices.erase(device_name);
        }
        return true;
    }

    // 获取可用设备列表
    vector<string> get_available_devices() {
        return available_devices;
    }

    // 获取正在使用设备的字典
    map<string, vector<string>> get_occupied_devices() {
        return occupied_devices;
    }

    // 打印可用设备列表
    void print_avaliable_devices() {
        vector<string> available_devices = get_available_devices();
        cout << "Available Devices: ";
        for (string device_name : available_devices) {
            cout << device_name << " ";
        }
        cout << '\n';
    }

    // 打印正在使用设备的字典
    void print_occupied_devices() {
        map<string, vector<string>> occupied_devices = get_occupied_devices();
        cout << "Occupied Devices: " << '\n';
        for (auto& pair : occupied_devices) {
            string device_name = pair.first;
            vector<string> processes = pair.second;
            cout << device_name << ": ";
            for (string process_name : processes) {
                cout << process_name << " ";
            }
            cout << '\n';
        }
    }
};

#endif