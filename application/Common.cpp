#include "Common.h"

Common *Common::_instance = nullptr;

Common *Common::getInstance() {
    if(!_instance)
        _instance = new Common();
    return _instance;
}

Common::Common() {
    _timer.start();
}

uint64_t Common::getMillis() {
    return _timer.read_ms();
}