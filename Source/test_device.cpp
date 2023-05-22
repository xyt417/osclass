#include <device.h>
void allocateDevice(string device, string process, DeviceQueue &device_queue, string request = ""){
    bool result = device_queue.allocate_device(device, process, request); 
    if (result) {
        cout << "���� " << process << " �ɹ������豸 " << device << endl;
    }else {
        cout << "�豸���� " << device << " ������" << endl;
    }
}
void runDevice(string device, DeviceQueue &device_queue){
    string process;
    device_queue.release_device(device, process);
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
void printInfo(DeviceQueue &device_queue, DeviceTable &device_table){
    cout << "==== DeviceQueue Status =====" << endl;
    device_queue.print_avaliable_devices();
    device_queue.print_occupied_devices();
    cout << "======= Device Table ========" << endl;
    device_table.printInfo();
    cout << "=============================" << endl;
}

// ===== Test =====
int main() {
    DeviceTable device_table;
    device_table.add_device("printer1", "printer");
    device_table.add_device("printer2", "printer");
    device_table.add_device("screen", "screen");
    device_table.add_device("abc", "abc");
    device_table.remove_device("abc");
    DeviceQueue device_queue(device_table);
    cout << "device number: " << device_table.dev_num() << endl;
    printInfo(device_queue, device_table);

    allocateDevice("abc", "p1", device_queue);
    allocateDevice("printer", "p1", device_queue);
    allocateDevice("printer", "p2", device_queue);
    allocateDevice("printer", "p3", device_queue);
    allocateDevice("printer", "p4", device_queue);
    allocateDevice("printer", "p5", device_queue, "print AAA");
    allocateDevice("screen", "p1", device_queue);
    allocateDevice("screen", "p2", device_queue);
    printInfo(device_queue, device_table);

    runDevice("abc", device_queue);
    runDevice("printer1", device_queue);
    runDevice("printer1", device_queue);
    runDevice("printer2", device_queue);
    runDevice("printer2", device_queue);
    runDevice("screen", device_queue);
    printInfo(device_queue, device_table);

    allocateDevice("printer", "p1", device_queue);
    allocateDevice("printer", "p2", device_queue);
    runDevice("screen", device_queue);
    runDevice("screen", device_queue);
    printInfo(device_queue, device_table);



    return 0;
}

