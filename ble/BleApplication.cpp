#include "BleApplication.h"


BleApplication::BleApplication(BLE &ble, EventQueue &queue):
    ble(ble),
    server(nullptr),
    client(nullptr),
    queue(queue) { 
}

BleApplication::~BleApplication() {
}

void BleApplication::setServer(BleServer *server) {
    this->server = server;
}

void BleApplication::setClient(BleClient *client){
    this->client = client;
}

void BleApplication::start() {
    ble.onEventsToProcess(FunctionPointerWithContext<BLE::OnEventsToProcessCallbackContext *>(this, &BleApplication::scheduleBleEvents));
    ble.gap().setEventHandler(this);
    // ble.gattClient();
    ble.init(this, &BleApplication::onInitComplete);
}

void BleApplication::scheduleBleEvents(BLE::OnEventsToProcessCallbackContext *context) {
    queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

void BleApplication::onInitComplete(BLE::InitializationCompleteCallbackContext *params) {
    if (params->error != BLE_ERROR_NONE) {
        printf("Ble initialization failed.");
        return;
    }

    if(client)
        client->init();
    if(server)
        server->init();
}

void BleApplication::onAdvertisingReport(const ble::AdvertisingReportEvent &event) {
    if(client)
        client->onAdvertisingReport(event);
}

void BleApplication::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) {
    printf("disconnection handle = %d\r\n", event.getConnectionHandle());
    
    // printf("client handles: ");
    // for(int i = 0; i < client->handles.size(); i++) {
    //     printf("%d ", client->handles.at(i));
    //     if(event.getConnectionHandle() == client->handles.at(i)) {
    //         printf(" (it is a client handle) ");
    //     }
    // }
    // printf("\r\n");

    // printf("server handles: ");
    // for(int i = 0; i < server->handles.size(); i++) {
    //     printf("%d ", server->handles.at(i));
    //     if(event.getConnectionHandle() == server->handles.at(i)) {
    //         printf(" (it is a server handle) ");
    //     }
    // }
    // printf("\r\n");



    // event.getConnectionHandle()
    // if(client)
    //     client->onDisconnectionComplete(event);
    // if(server)
    //     server->onDisconnectionComplete(event);
}

void BleApplication::onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
    if (event.getOwnRole() == ble::connection_role_t::CENTRAL) {
        if(client)
            client->onConnectionComplete(event);
    } else {
        if(server)
            server->onConnectionComplete(event);
    }
}