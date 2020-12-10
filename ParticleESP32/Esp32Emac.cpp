#include "Esp32Emac.h"

Esp32Emac *Esp32Emac::instance = nullptr;

Esp32Emac::Esp32Emac() {

}

Esp32Emac *Esp32Emac::getInstance() {
    if(!instance)
        instance = new Esp32Emac();
    return instance; 
}

uint32_t Esp32Emac::get_mtu_size() const {
    return 1500U;
}

uint32_t Esp32Emac::get_align_preference() const {
    return 0;
}

void Esp32Emac::get_ifname(char *name, uint8_t size) const {
    MBED_ASSERT(name != NULL);
    strncpy(name, "r0", size);
}

uint8_t Esp32Emac::get_hwaddr_size() const {
    return 6;
}

bool Esp32Emac::get_hwaddr(uint8_t* addr) const {
    const char *mac = ParticleEsp32::getInstance()->getMACAddress();
    int val[6];
    if (mac) {
        if (sscanf(mac, "%x:%x:%x:%x:%x:%x",
            &val[0], &val[1], &val[2], &val[3], &val[4], &val[5]) != 6) {
        }
        for (int i = 0; i < 6; i++) {
            addr[i] = (unsigned char) val[i];
        }
    } else {
        return false;
    }
    return true;
}

void Esp32Emac::set_hwaddr(const uint8_t *addr) {
}

bool Esp32Emac::power_up() {
    emac_link_state_cb(true);
    return true;
}

void Esp32Emac::power_down() {
}


void Esp32Emac::set_link_input_cb(emac_link_input_cb_t input_cb) {
    emac_link_input_cb = input_cb;
    ParticleEsp32::getInstance()->setReceiveDataCb(callback(this, &Esp32Emac::receiveData));
}

void Esp32Emac::set_link_state_cb(emac_link_state_change_cb_t state_cb) {
    emac_link_state_cb = state_cb;
}

void Esp32Emac::add_multicast_group(const uint8_t*) {
}

void Esp32Emac::remove_multicast_group(const uint8_t *address) {
}

void Esp32Emac::set_all_multicast(bool) {
}

void Esp32Emac::set_memory_manager(EMACMemoryManager &mem_mngr) {
    memory_manager = &mem_mngr;
}

bool Esp32Emac::link_out(emac_mem_buf_t *buf) {
    uint32_t total_len = memory_manager->get_total_len(buf);

    void *payload = memory_manager->get_ptr(buf);
    uint8_t *data = (uint8_t*)payload;

    int res = ParticleEsp32::getInstance()->sendData(data, total_len);

    memory_manager->free(buf);

    if(res == 0)
        return true;
    return false;
}

void Esp32Emac::receiveData(const uint8_t* data, size_t size) {
    emac_mem_buf_t *buf = memory_manager->alloc_pool(size, 0);
    void *payload = memory_manager->get_ptr(buf);
    memcpy(payload, data, size);
    emac_link_input_cb(buf);
    memory_manager->free(buf);
}
