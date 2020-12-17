#pragma once

#include <mbed.h>
#include "NetworkInterface.h"


void new_socket_example() {

    Logger::getInstance()->addMessage("new_socket_example START!!!\r\n");
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


        int i = 0;
        while(true) {
            
            Logger::getInstance()->addMessage("TRYING RUN %d \r\n", i++);
            Logger::getInstance()->addMessage("NETWORK status = %d \r\n", (int)net->get_connection_status());

            const char* url = "echo.websocket.org";
            SocketAddress a;
            net->gethostbyname(url, &a);

            Logger::getInstance()->addMessage("IP websocket.org = %s \r\n", a.get_ip_address());
            a.set_port(80);


            // Websocket ws("ws://echo.websocket.org:8080/", net, a);
            Websocket ws("ws://echo.websocket.org:80/", net, a);
            Logger::getInstance()->addMessage("WEBSOCKET host = %s port = %u \r\n", ws.host, ws.port);
            
            int connect_error = ws.connect();
            Logger::getInstance()->addMessage("WEBSOCKET connect = %d \r\n", connect_error);

            if(connect_error)
            {
                int error_c = ws.send("Hello World\r\n");
                char* hohoho = new char[10000];
                int error_c2 = ws.read(hohoho);

                Logger::getInstance()->addMessage("WEBSOCKET ec = %d  \r\n", error_c);
            }

            ThisThread::sleep_for(2000);
        }
    });

} 