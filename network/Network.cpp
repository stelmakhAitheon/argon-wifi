#include "Network.h"

NetworkInterface *NetworkInterface::get_target_default_instance()
{
    return Network::getInstance()->getInterface();
}

Network *Network::_instance = nullptr;

Network *Network::getInstance() {
    if(!_instance)
        _instance = new Network();
    return _instance;
}

Network::Network() {
    ParticleEsp32 *esp = ParticleEsp32::getInstance();
    bool res = esp->init();
    Logger::getInstance()->addMessage("RES INIT = %d \r\n", (int)res);
    Logger::getInstance()->addMessage("MAC = %s \r\n", esp->getMACAddress());

    do { 
        Logger::getInstance()->addMessage("Try Connecting to wifi network... \r\n");
    } while(!esp->connect("Aitheon", "Ukraine123"));
    Logger::getInstance()->addMessage("connected to wifi network \r\n");

    _netInterface = new EthernetInterface(*Esp32Emac::getInstance());
}

NetworkInterface *Network::getInterface() {
    return _netInterface;
}