#include "BleClient.h"

void print_uuid(const UUID &uuid)
{
    const uint8_t *uuid_value = uuid.getBaseUUID();

    // UUIDs are in little endian, print them in big endian
    for (size_t i = 0; i < uuid.getLen(); ++i) {
        printf("%02X", uuid_value[(uuid.getLen() - 1) - i]);
    }
}

BleClient::BleClient(BLE &ble, EventQueue &queue) :
    ble(ble),
    queue(queue),
    _adv_data_builder(_adv_buffer),
    _adv_handle(ble::LEGACY_ADVERTISING_HANDLE),
    _gap(ble.gap()),
    client(ble.gattClient()) {
}

void BleClient::init() {
    //    ble_error_t error;

    //     ble::AdvertisingParameters adv_params(
    //         ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
    //         ble::adv_interval_t(ble::millisecond_t(500))
    //     );

    //     error = _gap.setAdvertisingParameters(_adv_handle, adv_params);

    //     if (error) {
    //         printf("_ble.gap().setAdvertisingParameters() failed\r\n");
    //         return;
    //     }

    //     _adv_data_builder.clear();
    //     _adv_data_builder.setFlags(
    //         ble::adv_data_flags_t::LE_GENERAL_DISCOVERABLE
    //         | ble::adv_data_flags_t::BREDR_NOT_SUPPORTED
    //     );
    //     _adv_data_builder.setName("TEST");

    //     /* Set payload for the set */
    //     error = _gap.setAdvertisingPayload(
    //         _adv_handle, _adv_data_builder.getAdvertisingData()
    //     );

    //     if (error) {
    //         // print_error(error, "Gap::setAdvertisingPayload() failed\r\n");
    //         return;
    //     }

    //     error = _gap.startAdvertising(_adv_handle);

    //     if (error) {
    //         // print_error(error, "Gap::startAdvertising() failed\r\n");
    //         return;
    //     }

    //     printf("Advertising started.\r\n");




    ble::ScanParameters scan_params;
    ble.gap().setScanParameters(scan_params);
    printf("before scan\r\n");
    ble.gap().startScan();

}

void BleClient::onDisconnectionComplete(const ble::DisconnectionCompleteEvent&) {

}

void BleClient::when_service_discovered(const DiscoveredService *discovered_service) {
    printf("when_service_discovered \r\n");
    print_uuid(discovered_service->getUUID());
    printf("\r\n");
}

void BleClient::when_characteristic_discovered(const DiscoveredCharacteristic *discovered_characteristic) {
    printf("when_characteristic_discovered \r\n");
    print_uuid(discovered_characteristic->getUUID());
    printf("\r\n");
}

void BleClient::onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
    printf("we connected to SERVER, handle = %d\r\n", event.getConnectionHandle());
    handles.push_back(event.getConnectionHandle());
    // client->onServiceDiscoveryTermination();
    ble_error_t error = client.launchServiceDiscovery(
        event.getConnectionHandle(),
        FunctionPointerWithContext<const DiscoveredService *>(this, &BleClient::when_service_discovered),
        FunctionPointerWithContext<const DiscoveredCharacteristic *>(this, &BleClient::when_characteristic_discovered)
    );

    if (error) {
        printf("Error %u returned by _client->launchServiceDiscovery.\r\n", error);
        return;
    }
 
    printf("Client process started: initiate service discovery.\r\n");
}

void BleClient::printDevices(ble::AdvertisingDataParser *data) {
    while(data->hasNext()) {
        ble::AdvertisingDataParser::element_t field = data->next();
        char *tmp = new char[field.value.size() +1];
        tmp[field.value.size()] = '\0';
        memcpy(tmp, field.value.data(), field.value.size());
        printf("value = %s size = %d type = %d \r\n", tmp, field.value.size(), field.type);
        delete [] tmp;

        if (std::find(types.begin(), types.end(), field.type) == types.end())
            types.push_back(field.type);
        if (field.type == ble::adv_data_type_t::COMPLETE_LOCAL_NAME) {
            char *tmp = new char[field.value.size() +1];
            tmp[field.value.size()] = '\0';
            memcpy(tmp, field.value.data(), field.value.size());
            printf("name = %s size = %d \r\n", tmp, field.value.size());
            delete [] tmp;
            printf("available types: ");
            for(int i = 0; i < types.size(); i++) {
                printf("%02x ", types.at(i));
            }
            printf("\r\n");
        }
    }
}

bool BleClient::findDevice(ble::AdvertisingDataParser *data, char *deviceName) {
    while(data->hasNext()) {
        ble::AdvertisingDataParser::element_t field = data->next();
        printf("type = %d \r\n", field.type);
        if (field.type == ble::adv_data_type_t::COMPLETE_LOCAL_NAME) {
            char *tmp = new char[field.value.size() +1];
            tmp[field.value.size()] = '\0';
            memcpy(tmp, field.value.data(), field.value.size());
            printf("name = %s size = %d \r\n", tmp, field.value.size());
            delete [] tmp;

            printf("sizedevice = %d, sizeTest = %d\r\n", field.value.size(), strlen(deviceName));
            if(memcmp(field.value.data(), deviceName, field.value.size()) == 0) {
                return true;
            }
        }
    }
    return false;
}

void BleClient::onAdvertisingReport(const ble::AdvertisingReportEvent &event) {
    /* don't bother with analysing scan result if we're already connecting */
    // if (isConnecting) {
    //     return;
    // }

    printf("onAdvertisingReport \r\n");
    ble::AdvertisingDataParser adv_data(event.getPayload());
    printDevices(&adv_data);

    const ble::ConnectionParameters connection_params;

    int error = ble.gap().connect(
        event.getPeerAddressType(),
        event.getPeerAddress(),
        connection_params
    );

    if (error) {
        printf("fail connect\r\n");
        // ble.gap().startScan();
        return;
    }
    queue.call_every(1000, [] {
        printf("connected\r\n");
    });

    //-----------------------

    // isConnecting = true;

    // if(findDevice(&adv_data, "TEST")) {
    //     printf("TEST FOUND!!\r\n");

    //     const ble::ConnectionParameters connection_params;

    //     int error = ble.gap().connect(
    //         event.getPeerAddressType(),
    //         event.getPeerAddress(),
    //         connection_params
    //     );

    //     if (error) {
    //         printf("fail connect\r\n");
    //         // ble.gap().startScan();
    //         return;
    //     }
    //     printf("connected\r\n");
    //     // isConnecting = true;
    // }
}
