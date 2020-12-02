#include "BleServer.h"

BleServer::BleServer(BLE &ble):
    ble(ble),
    advDataBuilder(advBuffer) {
}

void BleServer::addService(BleService *service) {
    services.push_back(service);
}

void BleServer::init() {
    std::vector <UUID> arrServices;
    for(auto it = services.begin(); it < services.end(); it++) {
        ble.gattServer().addService((*it)->getService());
        arrServices.push_back((*it)->getUUID());
    }
    
    ble.gattServer().onDataWritten(this, &BleServer::onDataWritten);

    ble::AdvertisingParameters adv_parameters(
        ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
        ble::adv_interval_t(ble::millisecond_t(1000))
    );
    
    

    advDataBuilder.setFlags();
    advDataBuilder.setLocalServiceList(mbed::make_Span(&arrServices[0], arrServices.size()));
    advDataBuilder.setName("trololo12345"); //DEVICE_NAME

    ble_error_t error = ble.gap().setAdvertisingParameters(
        ble::LEGACY_ADVERTISING_HANDLE,
        adv_parameters
    );

    if (error) {
        printf("ble.gap().setAdvertisingParameters() failed\r\n");
        return;
    }

    error = ble.gap().setAdvertisingPayload(
        ble::LEGACY_ADVERTISING_HANDLE,
        advDataBuilder.getAdvertisingData()
    );

    if (error) {
        printf("ble.gap().setAdvertisingPayload() failed\r\n");
        return;
    }

    error = ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error) {
        printf("ble.gap().startAdvertising() failed\r\n");
        return;
    }
}

void BleServer::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) {
    printf("onDisconnectionComplete handle = %d\r\n", event.getConnectionHandle());
    ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
}

void BleServer::onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
    printf("somebody connected to our SERVER, handle = %d\r\n", event.getConnectionHandle());
}

void BleServer::onDataWritten(const GattWriteCallbackParams *params) {
    // printf("handle = %d\r\n", params->handle);
    // printf("conn handle = %d\r\n", params->connHandle);
    // printf("write = %d\r\n", *(params->data));
    for(auto it = services.begin(); it < services.end(); it++) {
        std::string name;
        if((*it)->findNameByParam(params, name)) {
            printf("find charactewrsitic %s in service\r\n", name.c_str());
        }
    }

}