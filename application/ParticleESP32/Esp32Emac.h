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
    Esp32Emac() {}
public:

    static Esp32Emac *getInstance() {
        if(!instance)
            instance = new Esp32Emac();
        return instance; 
    }
    

    uint32_t get_mtu_size() const {
        Logger::getInstance()->addMessage("get_mtu_size \r\n");
        return 1500U;
    }
    
    uint32_t get_align_preference() const {
        Logger::getInstance()->addMessage("get_align_preference \r\n");
        return 0;
    }

    void get_ifname(char *name, uint8_t size) const {
        Logger::getInstance()->addMessage("get_ifname \r\n");

        if(name != NULL) {
            Logger::getInstance()->addMessage("name != NULL \r\n");
        } else {
            Logger::getInstance()->addMessage("name == NULL \r\n");
        }
        MBED_ASSERT(name != NULL);
        strncpy(name, "r0", size);
    }

    uint8_t get_hwaddr_size() const {
        Logger::getInstance()->addMessage("get_hwaddr_size \r\n");
        return 6;
    }


    bool get_hwaddr(uint8_t* addr) const {
        Logger::getInstance()->addMessage("get_hwaddr \r\n");

        const char *mac = ParticleEsp32::getInstance()->getMACAddress();
        int val[6];
        if (mac) {
            if (sscanf(mac, "%x:%x:%x:%x:%x:%x",
                &val[0], &val[1], &val[2], &val[3], &val[4], &val[5]) != 6) {
                printf("Get HW address failed\r\n");
            }
            for (int i = 0; i < 6; i++) {
                addr[i] = (unsigned char) val[i];
            }
        } else {
            Logger::getInstance()->addMessage("Get HW address failed\r\n");
            return false;
        }
        Logger::getInstance()->addMessage("Get HW address YEAH\r\n");
        return true;
    }

    void set_hwaddr(const uint8_t *addr) {
        Logger::getInstance()->addMessage("set_hwaddr \r\n");

    }

    bool link_out(emac_mem_buf_t *buf) {
        static bool first = true;
        if(!first)
            return false;
        // first= false;
        // Logger::getInstance()->addMessage("link_out \r\n");

        uint32_t len = memory_manager->get_len(buf);
        uint32_t total_len = memory_manager->get_total_len(buf);
        // Logger::getInstance()->addMessage("link_out len - %d total_len = %d\r\n", len, total_len);

        void *payload = memory_manager->get_ptr(buf);
        uint8_t *data = (uint8_t*)payload;

        int res = ParticleEsp32::getInstance()->sendData(data, total_len);

        memory_manager->free(buf);

        // Logger::getInstance()->addMessage("link_out result = %d \r\n", res);
        // Logger::getInstance()->addMessage("link_out result = %d \r\n", res);
        if(res == 0)
            return true;
        return false;

        // emac_mem_buf_t *new_buf = memory_manager->alloc_pool(memory_manager->get_total_len(buf), 0);
        // if (new_buf != NULL) {
        //     memory_manager->copy(new_buf, buf);

        //     uint32_t len = memory_manager->get_len(new_buf);
        //     uint32_t total_len = memory_manager->get_total_len(new_buf);
        //     Logger::getInstance()->addMessage("link_out len - %d total_len = %d\r\n", len, total_len);

        //     void *payload = memory_manager->get_ptr(new_buf);
        //     uint8_t *data = (uint8_t*)payload;

        //     int res = ParticleEsp32::getInstance()->sendData(data, total_len);
        //     Logger::getInstance()->addMessage("link_out result = %d \r\n", res);


            // std::string dataStr = "";
            // for(size_t i = 0; i < len; i++) {
            //     char tmp[50] = {0};
            //     snprintf(tmp, 50, "%02x", test[i]);
            //     dataStr = dataStr + tmp;
            // }
            // Logger::getInstance()->addMessage("link_out YEAH DAAA %s \r\n", dataStr.c_str());

            // pBuf *p;
            // int id = cbMAIN_getEventQueue()->call(send_wlan_packet, this, new_buf);
            // if (id != 0) {
            //     cbMAIN_dispatchEventQueue();
            // } else {
            //     mem->free(new_buf);
            // }
            // memory_manager->free(new_buf);
        // }
        // memory_manager->free(buf);

        // Logger::getInstance()->addMessage("link_out YEAH!! \r\n");

        // if (p->len == p->tot_len) {
        //     // non-queue packet
        //     wifiMan_->ncpClient()->dataChannelWrite(0, (const uint8_t*)p->payload, p->tot_len);
        // } else {
        //     pbuf* q = pbuf_clone(PBUF_LINK, PBUF_RAM, p);
        //     if (q) {
        //         wifiMan_->ncpClient()->dataChannelWrite(0, (const uint8_t*)q->payload, q->tot_len);
        //         pbuf_free(q);
        //     }
        // }

        return false;
    }

    bool power_up() {
        Logger::getInstance()->addMessage("power_up \r\n");
        emac_link_state_cb(true);
        return true;
    }

    void power_down() {
        Logger::getInstance()->addMessage("power_down \r\n");
    }


    void set_link_input_cb(emac_link_input_cb_t input_cb) {
        Logger::getInstance()->addMessage("set_link_input_cb \r\n");
        emac_link_input_cb = input_cb;

        ParticleEsp32::getInstance()->setReceiveDataCb(callback(this, &Esp32Emac::receiveData));
        
    }

    void set_link_state_cb(emac_link_state_change_cb_t state_cb) {
        Logger::getInstance()->addMessage("set_link_state_cb \r\n");
        emac_link_state_cb = state_cb;

    }

    void add_multicast_group(const uint8_t*) {
        Logger::getInstance()->addMessage("add_multicast_group \r\n");

    }

    void remove_multicast_group(const uint8_t *address) {
        Logger::getInstance()->addMessage("remove_multicast_group \r\n");

    }

    void set_all_multicast(bool) {
        Logger::getInstance()->addMessage("set_all_multicast \r\n");

    }

    void set_memory_manager(EMACMemoryManager &mem_mngr) {
        Logger::getInstance()->addMessage("set_memory_manager \r\n");
        memory_manager = &mem_mngr;
    }

    void receiveData(const uint8_t* data, size_t size) {
        static int n = 0;
        n++;
        // if(!first)
            // return;
        // first = false;
        // Logger::getInstance()->addMessage("receiveData \r\n"); 
        emac_mem_buf_t *buf = memory_manager->alloc_pool(size, 0);
        void *payload = memory_manager->get_ptr(buf);
        memcpy(payload, data, size);
        
        // Logger::getInstance()->addMessage("receiveData = %d \r\n", memory_manager->get_total_len(buf)); 
        emac_link_input_cb(buf);

        memory_manager->free(buf);
        if(n % 15 == 0) {
            Logger::getInstance()->addMessage("receiveData %d \r\n", n); 
        }
    }

};

Esp32Emac *Esp32Emac::instance = nullptr;