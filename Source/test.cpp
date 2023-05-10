#include"device_queue.h"
DeviceQueue device_queue;
void allocateDevice(string device, string process){
    bool result = device_queue.allocate_device(device, process); 
    if (result) {
        cout << "进程 " << process << " 成功请求到设备 " << device << endl;
    }else {
        cout << "设备 " << device << " 不存在" << endl;
    }
}
void runDevice(string device){
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
void printInfo(){
    cout << "======= status =======" << endl;
    device_queue.print_avaliable_devices();
    device_queue.print_occupied_devices();
    cout << "======================" << endl;
}

// ===== Test =====
int main() {
    allocateDevice("abc", "p1");
    allocateDevice("printer2", "p1");
    allocateDevice("printer1", "p2");
    allocateDevice("screen", "p1");
    allocateDevice("screen", "p2");
    printInfo();
    runDevice("abc");
    runDevice("printer1");
    runDevice("printer1");
    runDevice("printer2");
    runDevice("printer2");
    runDevice("screen");
    runDevice("screen");
    printInfo();
    allocateDevice("printer2", "p1");
    allocateDevice("printer1", "p2");
    allocateDevice("printer2", "p1");
    allocateDevice("printer1", "p2");
    printInfo();
    runDevice("printer1");
    runDevice("printer2");
    runDevice("printer1");
    runDevice("printer2");
    runDevice("printer1");
    runDevice("printer2");
    printInfo();
    return 0;
}

