#pragma once

#include "BleService.h"

class WifiBleService : public BleService {
public:
    WifiBleService(UUID uuidService);
protected:
    virtual void onUpdateValue(const GattWriteCallbackParams *param, std::string name);
};