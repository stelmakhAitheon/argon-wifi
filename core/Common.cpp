#include "Common.h"

Common *Common::_instance = nullptr;

// for printf
FileHandle* mbed::mbed_override_console(int) {
    static USBSerial usb;
    return &usb;
}    

Common *Common::getInstance() {
    if(!_instance)
        _instance = new Common();
    return _instance;
}

Common::Common() {
    _timer.start();
    // restartTimer();
}

// void Common::restartTimer() {
//     _timer.stop();
//     _timer.reset();
//     _timer.start();
//     _timeout.attach(callback(this, &Common::restartTimer), 15.0);
// }

uint64_t Common::getMillis() {
    return getMicros() / 1000;
}

uint64_t Common::getMicros() {
    return _timer.read_high_resolution_us();
}