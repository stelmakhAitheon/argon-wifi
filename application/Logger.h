#pragma once

#include <mbed.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

class Logger {
private:
    EventQueue _queue;
    rtos::Thread _queueThread;
    std::vector<std::string> _messages;
    static Logger *_instance;
    Logger();
    void internalAddMessage(std::string *message);
    void internalPrintMessages();
    static std::string internalFormat(const char *fmt, va_list args);
public:
    static Logger *getInstance();
    static std::string format(const char *fmt, ...);
    void addMessage(const char *message, ...);
    void printMessages();
};