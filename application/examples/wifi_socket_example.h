#pragma once

#include <mbed.h>
#include "Esp32Emac.h"
#include "EthernetInterface.h"
#include "Logger.h"
#include "Common.h"
#include "ParticleEsp32.h"

#include "TLSSocketWrapper.h"

#include "Websocket.h"

#include "http_request.h"
//#include "network-helper.h"


EthernetInterface *net;
SocketAddress a;

void test(nsapi_event_t event, intptr_t) {
    Logger::getInstance()->addMessage("test conn  \r\n");
}


void dump_response(HttpResponse* res) {
    printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());
 
    printf("Headers:\n");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++) {
        printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
}

void wifi_socket_example() {
    Common::getInstance(); // start timer

    Logger::getInstance()->addMessage("BEFORE \r\n");

    ParticleEsp32 *esp = ParticleEsp32::getInstance();
    bool res = esp->init();
    Logger::getInstance()->addMessage("RES INIT = %d \r\n", (int)res);
    Logger::getInstance()->addMessage("MAC = %s \r\n", esp->getMACAddress());
    res = esp->connect("Aitheon", "Ukraine123");
    Logger::getInstance()->addMessage("connected = %d \r\n", (int)res);

    EMAC *my_emac = Esp32Emac::getInstance();
    net = new EthernetInterface(*my_emac);
    
    net->attach(test);
    nsapi_error_t result = net->connect();
    // net->'
    
    Logger::getInstance()->addMessage("result net connect = %d \r\n", result);

    const char *ip = net->get_ip_address();
    Logger::getInstance()->addMessage("ip = %s \r\n", ip ? ip : "PZDC");

    // SocketAddress a;
    net->gethostbyname("httpbin.org", &a);
    Logger::getInstance()->addMessage("IP address http://httpbin.org/status/418 is: %s \r\n", a.get_ip_address() ? a.get_ip_address() : "No IP");

    a.set_port(80);

    TCPSocket socket;
    TLSSocketWrapper wrap(static_cast<Socket*>(&socket));
    nsapi_error_t error = socket.open(net);

    Logger::getInstance()->addMessage("res =  %u \r\n", error);

    //char* url = "wss://echo.websocket.org";
   // strcat(url,a.get_ip_address());


          HttpRequest* get_req = new HttpRequest(net, HTTP_GET, "http://httpbin.org/status/418");
 
        HttpResponse* get_res = get_req->send();
        if (!get_res) {
            printf("HttpRequest failed (error code %d)\n", get_req->get_error());
        }
 
        printf("\n----- HTTP GET response -----\n");
        dump_response(get_res);



    Websocket ws("ws://echo.websocket.org:8080/", net, a);
    Logger::getInstance()->addMessage("WEBSOCKET host = %s port = %u \r\n", ws.host, ws.port);
    
    int connect_error = ws.connect();
    Logger::getInstance()->addMessage("WEBSOCKET connect = %d \r\n", connect_error);

    if(connect_error)
    {
        // int error_c = ws.send("Hello World\r\n");
        char* hohoho = new char[10000];
        int error_c = ws.read(hohoho);

        Logger::getInstance()->addMessage("WEBSOCKET ec = %d  \r\n", error_c);
    }



    // Thread *thread = new Thread();
    // thread->start([] {
    //     while(true) {
    //         // EthernetInterface net(*Esp32Emac::getInstance());

    //         SocketAddress b;
    //         net->gethostbyname("google.com", &b);
    //         Logger::getInstance()->addMessage("IP address google is: %s \r\n", b.get_ip_address() ? b.get_ip_address() : "No IP");

    //         TCPSocket socket;
    //         nsapi_error_t error = socket.open(net);
    //         Logger::getInstance()->addMessage("socket open = %d \r\n", error);
    //         // error = socket.connect("google.com", 80);
    //         error = socket.connect(a);
    //         Logger::getInstance()->addMessage("socket connect = %d \r\n", error);
    //         if (error < 0) {
    //             Logger::getInstance()->addMessage("FAIL CONNECT to google \r\n");
    //             // net.connect();
    //         }
    //         else
    //             Logger::getInstance()->addMessage("CONNECTED to google \r\n");
    //         error = socket.close();
    //         Logger::getInstance()->addMessage("socket close = %d \r\n", error);

    //         ThisThread::sleep_for(5000);
    //     }
    // });

    // Thread *thread2 = new Thread();
    // thread2->start([] {
    //     while(true) {
            
    //         Logger::getInstance()->addMessage("MAC = %s \r\n", ParticleEsp32::getInstance()->getMACAddress());
    //         Logger::getInstance()->addMessage("MAC2 = %s \r\n", net->get_mac_address());
    //         ThisThread::sleep_for(15000);
    //     }
    // });

    // Thread *thread3 = new Thread();

    // thread3->start([] {
    //     while(true) {
    //         // EthernetInterface net(*Esp32Emac::getInstance());
            
    //         Logger::getInstance()->addMessage("connection status = %d\r\n", net->get_connection_status());
    //         ThisThread::sleep_for(15000);
    //     }
    // });



    // if (socket.connect("google.com", 80) < 0) {
    //     Logger::getInstance()->addMessage("FAIL CONNECT to google \r\n");
    // } else {
    //     Logger::getInstance()->addMessage("CONNECTED to google \r\n");
    //     std::string request("GET /search?q=unicorn HTTP/1.0\r\nHost: www.google.com\r\nContent-Length: 0\r\n\r\n");
    //     error = socket.send(request.data(), request.size());
    //     Logger::getInstance()->addMessage("SOCKET send result = %d\r\n", error);
    //     char buffer[256];
    //     do
    //     {
    //         int result = socket.recv(buffer, 255);   
    //         Logger::getInstance()->addMessage("Received %d bytes:\n\r%s\n\r", result, buffer);
    //         ThisThread::sleep_for(1000);
    //     } while(result == 0);
    // }


    Logger::getInstance()->addMessage("AFTER \r\n");
}