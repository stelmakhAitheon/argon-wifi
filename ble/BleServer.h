#pragma once

#include <mbed.h>
#include "ble/BLE.h"
#include "BleService.h"

class BleServer {
public:
    BleServer(BLE &ble);
    void init();
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&);
    void onConnectionComplete(const ble::ConnectionCompleteEvent &event);
    void addService(BleService *service);
    vector<ble::connection_handle_t> handles;
protected:
    BLE &ble;
    uint8_t advBuffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder advDataBuilder;
    std::vector <BleService*> services;
    void onDataWritten(const GattWriteCallbackParams *params);
};