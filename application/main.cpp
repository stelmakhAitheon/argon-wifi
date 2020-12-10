#include <mbed.h>
#include "USBSerial.h"
#include "ESP32Interface.h"
#include "Esp32Emac.h"
#include "EthernetInterface.h"
#include "Logger.h"
#include "Common.h"
#include "ParticleEsp32.h"

static USBSerial usb;
FileHandle* mbed::mbed_override_console(int) {
    return &usb;
}    

rtos::Thread thread_print;
rtos::Thread mainThread;
static rtos::Mutex lock;
EventQueue queue;

void print_func() {
    while(true) {
        // lock.lock();
        // usb.printf("HELLO n = %d\r\n", n);
        // printf("message = %s\r\n", message.c_str());
        // ESP32::mutex.lock();
        // ESP32::updateMessage();
        // printf("ESP32FUNC = %s\r\n", ESP32::message.c_str());
        // printf("ESP32RECV = %s\r\n", ESP32::received.c_str());
        // ESP32::mutex.unlock();
        // Logger::getInstance()->printMessages();
        // printf("--------------\r\n");
        // lock.unlock();
        ThisThread::sleep_for(5000);
    }
}

int main() {    
    Common::getInstance(); // start timer
    Logger::getInstance()->addMessage("START\r\n");
    DigitalOut led(P1_12);
    led = 1;

    queue.call_in(1500, [] {
        thread_print.start(print_func);
    });

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

    SocketAddress a;
    net->gethostbyname("google.com", &a);
    Logger::getInstance()->addMessage("IP address google is: %s\n", a.get_ip_address() ? a.get_ip_address() : "No IP");

    TCPSocket socket;
    nsapi_error_t error = socket.open(net);
    Logger::getInstance()->addMessage("socket open = %d", error);


    Logger::getInstance()->addMessage("AFTER \r\n");

    queue.call([] {  
  
        // TCPSocket socket;
        // nsapi_error_t error = socket.open(esp32);

        // SocketAddress a;
        // esp32->gethostbyname("google.com", &a);

        //char buff[100];
        // snprintf(buff, sizeof(buff), "IP address is: %s\n", a.get_ip_address() ? a.get_ip_address() : "No IP");
        // message = buff;
        //old cmux test

    });

    mainThread.start(callback(&queue, &EventQueue::dispatch_forever));
}