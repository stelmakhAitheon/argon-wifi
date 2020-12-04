#pragma once

#include <mbed.h>

class Common {
private:
    static Common *_instance;
    Timer _timer;
    Common();
public:
    static Common *getInstance();
    uint64_t getMillis();
};