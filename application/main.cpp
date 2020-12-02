#include <mbed.h>
#include "USBSerial.h"
#include "ESP32Interface.h"
#include "Logger.h"


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
        Logger::getInstance()->printMessages();
        // printf("--------------\r\n");
        // lock.unlock();
        ThisThread::sleep_for(3000);
    }
}

int main() {    
    DigitalOut led(P1_12);
    led = 1;

    queue.call_in(1500, [] {
        thread_print.start(print_func);
    });

    queue.call_every(3000, [] {
        // Logger::getInstance()->addMessage("HELLO\r\n");
    });

    queue.call_in(2000, [] {  
        // for(int i = 0; i < 5; i++) {
        //     Thread *thread = new Thread();
        //     thread->start((void (*)(int *))[] (int *threadNum) {
        //         for(int i = 0; i < 2; i++) {
        //             ThisThread::sleep_for(1000* *threadNum);
        //             Logger::getInstance()->addMessage("YEAH THREAD = %d\r\n", *threadNum);
                    
        //         }
        //     }, new int(i));
        // }     

        ESP32Interface *esp32 = new ESP32Interface(P0_24, P0_16, P1_5, P1_4, false, P1_7, P1_6, 921600);
        const char *mac = esp32->get_mac_address();

        //old cmux test
        // printf("mac =%s\r\n", mac);
        // esp32.set_credentials("Aitheon", "Ukraine123", NSAPI_SECURITY_WPA2);
        // esp32.connect();

        // TCPSocket socket;
        // nsapi_error_t error = socket.open(&esp32);

        // SocketAddress a;
        // char buff[100];
        // esp32.gethostbyname("google.com", &a);

        // snprintf(buff, sizeof(buff), "IP address is: %s\n", a.get_ip_address() ? a.get_ip_address() : "No IP");
        // message = buff;
        //old cmux test

    });

    mainThread.start(&queue, &EventQueue::dispatch_forever);
}