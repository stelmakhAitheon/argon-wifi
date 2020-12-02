#include "WifiBleService.h"

WifiBleService::WifiBleService(UUID uuidService)
    : BleService(uuidService) {
    addCharacteristic("ssid", new ReadWriteGattCharacteristic<std::string>(0xC004, new std::string("ssid")));
    addCharacteristic("pass", new ReadWriteGattCharacteristic<std::string>(0xC005, new std::string("password")));
}

void WifiBleService::onUpdateValue(const GattWriteCallbackParams *param, std::string name) {
    printf("WifiBleService onUpdateValue name = %s\r\n", name.c_str());
}