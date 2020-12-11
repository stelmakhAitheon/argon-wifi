#pragma once

#include <mbed.h>
#include "USBSerial.h"

class Common {
private:
    static Common *_instance;
    Timer _timer;
    Timeout _timeout;
    Common();
    // void restartTimer();
public:
    static Common *getInstance();
    uint64_t getMillis();
    uint64_t getMicros();
};