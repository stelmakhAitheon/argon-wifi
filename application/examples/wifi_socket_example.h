#pragma once

#include <mbed.h>
#include "Esp32Emac.h"
#include "EthernetInterface.h"
#include "Logger.h"
#include "Common.h"
#include "ParticleEsp32.h"

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
    EthernetInterface *net = new EthernetInterface(*my_emac);
    nsapi_error_t result = net->connect();
    Logger::getInstance()->addMessage("result net connect = %d \r\n", result);

    const char *ip = net->get_ip_address();
    Logger::getInstance()->addMessage("ip = %s \r\n", ip ? ip : "PZDC");

    Thread *thread = new Thread();
    thread->start([] {
        while(true) {
            EthernetInterface net(*Esp32Emac::getInstance());

            SocketAddress a;
            net.gethostbyname("google.com", &a);
            Logger::getInstance()->addMessage("IP address google is: %s \r\n", a.get_ip_address() ? a.get_ip_address() : "No IP");

            TCPSocket socket;
            nsapi_error_t error = socket.open(&net);
            Logger::getInstance()->addMessage("socket open = %d \r\n", error);

            if (socket.connect("google.com", 80) < 0)
                Logger::getInstance()->addMessage("FAIL CONNECT to google \r\n");
            else
                Logger::getInstance()->addMessage("CONNECTED to google \r\n");
            error = socket.close();
            Logger::getInstance()->addMessage("socket close = %d \r\n", error);

            ThisThread::sleep_for(5000);
        }
    });



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