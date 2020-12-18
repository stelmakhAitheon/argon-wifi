#pragma once

#include <mbed.h>
#include "NetworkInterface.h"
#include "wss_client.h"
#include "ws_client.h"

EventQueue sockQueue;
rtos::Thread sockThread;

void rx_callback(WS_OPCODE opcode, uint8_t *buffer, size_t buffer_size) {
    Logger::getInstance()->addMessage("ws received message: opcode=%u, buffer_size=%lu, content=", opcode, buffer_size);
    // for (size_t ix = 0; ix < buffer_size; ix++) {
    //     Logger::getInstance()->addMessage("%c", buffer[ix]);
    // }
    std::string message((char*) buffer, buffer_size);
    Logger::getInstance()->addMessage("DATA = %s", message.c_str());
    Logger::getInstance()->addMessage("\n");
}

void disconnect_callback() {
    Logger::getInstance()->addMessage("ws disconnected \r\n");
    // add some reconnect logic
}

const char SSL_CA_PEM2[] = R"(-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----)";

void new_socket_example() {

    Logger::getInstance()->addMessage("new_socket_example START!!!\r\n");

    // ThisThread::sleep_for(5000);

    sockThread.start(callback(&sockQueue, &EventQueue::dispatch_forever));


    Thread *thread = new Thread();

    thread->start([] {
         

        NetworkInterface *net = NetworkInterface::get_default_instance();
        net->set_blocking(true);
        
        int res;
        do {
            res = net->connect();
            Logger::getInstance()->addMessage("Trying NetworkInterface connect... res = %d \r\n", res);
            ThisThread::sleep_for(1000);
        } while(res != NSAPI_ERROR_OK);
        Logger::getInstance()->addMessage("NetworkInterface connected \r\n");

        const char *ip = net->get_ip_address();
        Logger::getInstance()->addMessage("ip = %s \r\n", ip ? ip : "NO IP");

        // const char* url = "echo.websocket.org";
        // const char* url = "192.168.1.150";
        // SocketAddress a;
        // net->gethostbyname(url, &a);
        // Logger::getInstance()->addMessage("IP websocket.org = %s \r\n", a.get_ip_address());
        // a.set_port(3000);

        WssClient client(&sockQueue, net, SSL_CA_PEM2, "wss://192.168.1.150:443/device-manager/");
        // WsClient client(&sockQueue, net, "ws://dev.aitheon.com:80/device-manager/");

         ws_callbacks_t ws_callbacks = {
            rx_callback,
            disconnect_callback
        };


        int r = client.connect(&ws_callbacks);
        if (r != 0) {
            Logger::getInstance()->addMessage("Failed to connect to websocket server (%d)\n", r);
        } else {
            Logger::getInstance()->addMessage("WS CONNECTED !!!)\n");
            const char *msg = "Hello world";
            client.send(WS_TEXT_FRAME, (const uint8_t*)msg, strlen(msg));
        }


        int i = 0;
        while(true) {
            
            Logger::getInstance()->addMessage("TRYING RUN %d \r\n", i++);
            Logger::getInstance()->addMessage("NETWORK status = %d \r\n", (int)net->get_connection_status());

            // client.
            

            // Websocket start

            // Websocket ws("ws://echo.websocket.org:8080/", net, a);
            // Websocket ws("ws://192.168.1.150:443/api/device-type/", net, a);
            // Logger::getInstance()->addMessage("WEBSOCKET host = %s port = %u \r\n", ws.host, ws.port);
            
            // int connect_error = ws.connect();
            // Logger::getInstance()->addMessage("WEBSOCKET connect = %d \r\n", connect_error);

            // if(connect_error)
            // {
            //     int error_c = ws.send("Hello World\r\n");
            //     char* hohoho = new char[10000];
            //     int error_c2 = ws.read(hohoho);

            //     Logger::getInstance()->addMessage("WEBSOCKET ec = %d  \r\n", error_c);
            // }

            // Websocket end

            ThisThread::sleep_for(2000);
        }
    });

} 