#pragma once

#include <mbed.h>

#include "Logger.h"

class SerialUtil {
private:
public:
    static void skipAll(FileHandle *fh, uint64_t millis);
};