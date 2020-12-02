#pragma once

#include <mbed.h>
#include "ble/BLE.h"
#include "BleServer.h"
#include "BleClient.h"

class BleApplication : ble::Gap::EventHandler {
public:
    BleApplication(BLE &ble, EventQueue &queue);
    ~BleApplication();
    void start();
    void setServer(BleServer *server);
    void setClient(BleClient *client);
private:
    BLE &ble;
    EventQueue &queue;
    BleServer *server;
    BleClient *client;
    void scheduleBleEvents(BLE::OnEventsToProcessCallbackContext *context);
    void onInitComplete(BLE::InitializationCompleteCallbackContext *params);
    void onAdvertisingReport(const ble::AdvertisingReportEvent &event);
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&);
    void onConnectionComplete(const ble::ConnectionCompleteEvent &event);
};