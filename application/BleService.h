#pragma once

#include "ble/BLE.h"
#include <vector>
#include <map>


class BleService {
public:
    BleService(UUID uuidService);
    void addCharacteristic(std::string name, GattCharacteristic *value);
    GattService& getService();
    bool findNameByParam(const GattWriteCallbackParams *param, std::string &name);
    UUID getUUID();
protected:
    std::vector <GattCharacteristic*> characteristics;
    std::map<GattCharacteristic*, std::string> charactetisticNames;
    UUID uuidService;
    virtual void onUpdateValue(const GattWriteCallbackParams *param, std::string name);
};