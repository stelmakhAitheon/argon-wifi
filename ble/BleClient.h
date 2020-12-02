#pragma once

#include <mbed.h>
#include "ble/BLE.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredService.h"
#include "ble/gap/Gap.h"
#include "ble/gap/AdvertisingDataParser.h"
#include <vector>

// static const uint16_t MAX_ADVERTISING_PAYLOAD_SIZE = 50;

class BleClient {
public:
    BleClient(BLE &ble, EventQueue &queue);
    void init();
    void onAdvertisingReport(const ble::AdvertisingReportEvent &event);
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&);
    void onConnectionComplete(const ble::ConnectionCompleteEvent &event);
    void printDevices(ble::AdvertisingDataParser *data);
    bool findDevice(ble::AdvertisingDataParser *data, char *deviceName);
    vector<ble::connection_handle_t> handles;
private:
    EventQueue &queue;
    BLE &ble;
    std::vector<ble::adv_data_type_t> types;

    uint8_t _adv_buffer[50];
    ble::AdvertisingDataBuilder _adv_data_builder;

    ble::advertising_handle_t _adv_handle;
    ble::Gap &_gap;

    GattClient &client;
    void when_service_discovered(const DiscoveredService *discovered_service);
    void when_characteristic_discovered(const DiscoveredCharacteristic *discovered_characteristic);
    // bool isConnecting;
};