#include "BleService.h"

BleService::BleService(UUID uuidService) {
    this->uuidService = uuidService;
}

void BleService::addCharacteristic(std::string name, GattCharacteristic *value) {
    characteristics.push_back(value);
    charactetisticNames.emplace(value, name);
}

GattService& BleService::getService() {
    GattService *service = new GattService(uuidService, &characteristics[0], characteristics.size());
    return *service;
}

bool BleService::findNameByParam(const GattWriteCallbackParams *param, std::string &name) {
    auto handle = param->handle;
    auto search = std::find_if(charactetisticNames.begin(), charactetisticNames.end(), 
        [&handle](const std::pair<GattCharacteristic*, std::string> & t) -> bool { 
        return t.first->getValueHandle() == handle;
    });
    if (search != charactetisticNames.end()) {
        name = search->second;
        onUpdateValue(param, name);
        return true;
    }
    return false;
}

UUID BleService::getUUID() {
    return uuidService;
}

void BleService::onUpdateValue(const GattWriteCallbackParams *param, std::string name) {

}