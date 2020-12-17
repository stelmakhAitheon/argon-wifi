#pragma once

#include <mbed.h>
#include "ParticleEsp32.h"
#include "Esp32Emac.h"
#include "EthernetInterface.h"

class Network {
private:
    static Network *_instance;
    NetworkInterface *_netInterface;
    Network();
public:
    static Network *getInstance();
    NetworkInterface *getInterface();
};