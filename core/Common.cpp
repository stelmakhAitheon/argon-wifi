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
}

uint64_t Common::getMillis() {
    // need change to rtc because of max value of mbed::timer::read_ms
    // app working only 66minutes!!! FAIL
    return _timer.read_ms();
}
