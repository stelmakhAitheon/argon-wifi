#include "SerialUtil.h"

void SerialUtil::skipAll(FileHandle *fh, uint64_t millis) {
    volatile bool wait = true;
    Timeout out;
    out.attach_us(callback((void (*)(volatile bool *))[](volatile bool *wait) {
        *wait = false; 
    }, &wait), millis * 1000);
    int bytes = 0;
    while(wait) {
        if(fh->readable()) {
            char buf[2];
            fh->read(buf, 1);
            bytes++;
        }
    }
    Logger::getInstance()->addMessage("READ %d bytes\r\n", bytes);
}