#include <mbed.h>
#include "EMAC.h"
#include "ParticleEsp32.h"
#include <string.h>

class Esp32Emac : public EMAC {
private:
    void *emac_link_input_data;
    void *emac_link_state_data;
    emac_link_input_cb_t emac_link_input_cb; 
    emac_link_state_change_cb_t emac_link_state_cb; 
    EMACMemoryManager *memory_manager; 
    static Esp32Emac *instance;
    Esp32Emac();
public:
    static Esp32Emac *getInstance();
    uint32_t get_mtu_size() const;
    uint32_t get_align_preference() const;
    void get_ifname(char *name, uint8_t size) const;
    uint8_t get_hwaddr_size() const;
    bool get_hwaddr(uint8_t* addr) const;
    void set_hwaddr(const uint8_t *addr);
    bool link_out(emac_mem_buf_t *buf);
    bool power_up();
    void power_down();
    void set_link_input_cb(emac_link_input_cb_t input_cb);
    void set_link_state_cb(emac_link_state_change_cb_t state_cb);
    void add_multicast_group(const uint8_t*);
    void remove_multicast_group(const uint8_t *address);
    void set_all_multicast(bool);
    void set_memory_manager(EMACMemoryManager &mem_mngr);
    void receiveData(const uint8_t* data, size_t size);
};
