#include "Logger.h"

Logger *Logger::_instance = nullptr;

Logger *Logger::getInstance() {
    if(!_instance)
        _instance = new Logger();
    return _instance; 
}

std::string Logger::internalFormat(const char *fmt, va_list args) {
    std::vector<char> v(1024);
    while (true)
    {
        va_list args2;
        va_copy(args2, args);
        int res = vsnprintf(v.data(), v.size(), fmt, args2);
        if ((res >= 0) && (res < static_cast<int>(v.size())))
        {
            va_end(args);
            va_end(args2);
            return std::string(v.data());
        }
        size_t size;
        if (res < 0)
            size = v.size() * 2;
        else
            size = static_cast<size_t>(res) + 1;
        v.clear();
        v.resize(size);
        va_end(args2);
    }
}

std::string Logger::format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    return internalFormat(fmt, args);
}

Logger::Logger() {
    _queueThread.start(&_queue, &EventQueue::dispatch_forever);
}

void Logger::internalAddMessage(std::string *message) {
    // _messages.push_back("HAHA\r\n");
    // return;
    // if(!message)
    //     return;
    _messages.push_back(*message);
    delete message;
}

void Logger::internalPrintMessages() {
    for(auto it = _messages.begin(); it != _messages.end(); it++) {
        printf(it->c_str());
    }
    printf("------PRINT-------\r\n");
}

void Logger::addMessage(const char *message, ...) {
    // return;
    va_list args;
    va_start(args, message);
    _queue.call(this, &Logger::internalAddMessage, 
        new std::string(format("[THREADID=%p]%s", ThisThread::get_id(), internalFormat(message, args).c_str())));
}

void Logger::printMessages() {
    _queue.call(this, &Logger::internalPrintMessages);
}