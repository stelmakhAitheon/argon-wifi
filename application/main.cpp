#include <mbed.h>
#include "wifi_socket_example.h"
#include "sd_example.h"

rtos::Thread mainThread;
EventQueue queue;

DigitalOut led(P1_12);


int main() {    

    led = 1;

    // for(int i = 0; i < 3; i++) {
    //     led  = !led;
    //     ThisThread::sleep_for(2000);
    // }

    wifi_socket_example();

    // for(int i = 0; i < 5; i++) {
    //    led  = !led; 
    //    ThisThread::sleep_for(500);
    // }

    // return 0;

    queue.call_every(1000, [] {
        // Logger::getInstance()->addMessage("%llx \r\n", 18446744073709551615-100);
        // Logger::getInstance()->addMessage("ms = %lld \r\n", Common::getInstance()->getMillis());
        // printf("EBA OOK \r\n");
        // led  = !led;
        // ParticleEsp32::getInstance()->off();
    });

    mainThread.start(callback(&queue, &EventQueue::dispatch_forever));

    // for(int i = 0; i < 1000; i++) {
    //     // led  = !led;
    //     ThisThread::sleep_for(500);
    // }

    return 0;
}