#include <mbed.h>
#include "wifi_socket_example.h"
#include "sd_example.h"

rtos::Thread mainThread;
EventQueue queue;

int main() {    
    DigitalOut led(P1_12);
    led = 1;

    wifi_socket_example();

    queue.call_every(10000, [] {
        // Logger::getInstance()->addMessage("%llx \r\n", 18446744073709551615-100);
        Logger::getInstance()->addMessage("ms = %lld \r\n", Common::getInstance()->getMillis());
        // ParticleEsp32::getInstance()->off();
    });

    mainThread.start(callback(&queue, &EventQueue::dispatch_forever));
}