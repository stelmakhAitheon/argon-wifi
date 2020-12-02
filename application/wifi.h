// #pragma once
// #include <mbed.h>
// #include "ESP32.h"
// #include "TCPSocket.h"
// #include "muxer_vars.h"

// typedef enum {
//     ESP_IF_WIFI_STA = 0,     /**< ESP32 station interface */
//     ESP_IF_WIFI_AP,          /**< ESP32 soft-AP interface */
//     ESP_IF_ETH,              /**< ESP32 ethernet interface */
//     ESP_IF_MAX
// } esp_interface_t;

// typedef uint8_t          u8_t;
// typedef int8_t           s8_t;
// typedef uint16_t         u16_t;
// typedef int16_t          s16_t;
// typedef uint32_t         u32_t;
// typedef int32_t          s32_t;
// typedef ptrdiff_t        mem_ptr_t;
// typedef int              sys_prot_t;

// struct pbuf {
//   /** next pbuf in singly linked pbuf chain */
//   struct pbuf *next;

//   /** pointer to the actual data in the buffer */
//   void *payload;

//   /**
//    * total length of this buffer and all next buffers in chain
//    * belonging to the same packet.
//    *
//    * For non-queue packet chains this is the invariant:
//    * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
//    */
//   u16_t tot_len;

//   /** length of this buffer */
//   u16_t len;

//   /** pbuf_type as u8_t instead of enum to save space */
//   u8_t /*pbuf_type*/ type;

//   /** misc flags */
//   u8_t flags;

//   /**
//    * the reference count always equals the number of pointers
//    * that refer to this pbuf. This can be pointers from an application,
//    * the stack itself, or pbuf->next pointers from a chain.
//    */
//   u16_t ref;
// };

// struct InputPacket {
//     pbuf* p;
//     esp_interface_t iface;
// };


// class TestWifi {
// private:
// public:
//     std::string message;
//     mbed::UARTSerial *serial;
//     ATCmdParser *parser;
//     DigitalOut *wifi_en;
//     DigitalOut *wifi_boot;
//     TestWifi()
//     {}

//     void readSerial() {
//         while(!serial->readable())
//             ThisThread::sleep_for(10);
//         char test[1000] = {0};
//         int i = 0;
//         while(serial->readable()) {
//             char symbol[2] = {0};
//             serial->read(symbol, 1);
//             test[i] = symbol[0];
//             message.push_back(symbol[0]);
//             i++;
//             ThisThread::sleep_for(10);
//         }
//         printf("received %d bytes result = \r\n%s\r\n", i, test);
//     }

//     void readSerialCMUX() {
//         while(!serial->readable())
//             ThisThread::sleep_for(10);
//         // char test[1000] = {0};
//         int i = 0;
//         string tmp;
//         while(serial->readable()) {
//             char symbol[2] = {0};
//             char test[100] = {0};
//             serial->read(symbol, 1);
//             snprintf(test, sizeof(test), "GET %d BYTE = %x\r\n", i, symbol[0]);
//             tmp = tmp + test;
//             // test[i] = symbol[0];
//             // message.push_back(symbol[0]);
//             i++;
//             ThisThread::sleep_for(10);
//         }
//         message = message + tmp;
//         // printf("received %d bytes result = \r\n%s\r\n", i, test);
//     }

//     void checkCMD(bool success, std::string message) {
//         if(success) {
//             this->message = this->message + "YEAH " + message + "\r\n";
//         } else {
//             this->message = this->message + "FAIL " + message + "\r\n";
//         }
//     }

//     void init() {
//         serial = new UARTSerial(P1_5, P1_4, 921600);
//         serial->set_flow_control(SerialBase::RTSCTS, P1_7, P1_6);
//         parser = new ATCmdParser(serial, "\r\n");
//         wifi_en = new DigitalOut(P0_24);
//         wifi_boot = new DigitalOut(P0_16);
//     }

//     void start() {
//         wifi_en->write(0);
//         ThisThread::sleep_for(1000);
//         wifi_boot->write(1);
//         wifi_en->write(1);
//         ThisThread::sleep_for(1000);
//     }

//     void restart() {
//         //rst
//         // parser->send("AT+RST"); 
//         // // readSerial();
//         // // readSerial();
//         // parser->recv("OK");
//     }

//     void getMac() {
//         parser->send("AT+GETMAC=0"); // WiFi station
//         char addrStr[17 + 1] = {};
//         parser->recv("+GETMAC: \"%32[^\"]\"", addrStr);
//         bool success = parser->recv("OK");
//         checkCMD(success, std::string("mac ") + std::string(addrStr));
//     }

//     void setMode() {
//         //mode
//         parser->send("AT+CWMODE=3"); 
//         bool success = parser->recv("OK");
//         checkCMD(success, "AT+CWMODE=3");
//     }

//     void setAutoConnect() {
//         parser->send("AT+CWAUTOCONN=0"); 
//         bool success = parser->recv("OK");
//         checkCMD(success, "AT+CWAUTOCONN=0");
//     }

//     void cmux() {
//         parser->send("AT+CMUX=0");
//         // readSerialCMUX();
//         // readSerialCMUX();
//         // readSerialCMUX();
//         bool success = parser->recv("OK");
//         checkCMD(success, "AT+CMUX");
//     }

//     void checkParser() {
//         parser->send("AT");
//         readSerialCMUX();
//         // readSerial();
//         // readSerial();
//         // readSerial();
//         return;
//         bool success = parser->recv("OK");
//         checkCMD(success, "checkParser");
//     }
    

//     void enableDHCP() {
//         parser->send("AT+CWDHCP=1,1");
//         bool success = parser->recv("OK");
//         checkCMD(success, "DHCP");
//     }

//     void connect(std::string ssid, std::string pass) {
//         parser->send("AT+CWJAP=\"%s\",\"%s\"", ssid.c_str(), pass.c_str());
//         bool success = parser->recv("OK");
//         checkCMD(success, "connected to wifi and got IP");
//     }

//     void getVersion() {
//         parser->send("AT+CGMR");
//         readSerial();
//         parser->recv("OK");

//         parser->send("AT+MVER");
//         readSerial();
//         parser->recv("OK");
//     }

//     void testCMUXData(uint8_t channel, uint8_t control, char *data, int len, bool cmd) {
//         // uint8_t channel = 0;
//         // uint8_t control = 0xef;

//         uint8_t header[5];
//         uint8_t footer[2];
//         const size_t hlen = len <= 0x7f ? 4 : 5;
//         bool initiator_ = true;

//         header[0] = 0xf9;
//         header[1] = (channel << 2) | 0x01;
//         if ((initiator_ && cmd) || (!initiator_ && !cmd)) {
//             header[1] |= 0x02;
//         }
//         header[2] = control;
//         header[3] = (len & 0x7f) << 1;
//         if (hlen == 5) {
//             header[4] = len >> 7;
//         } else {
//             header[3] |= 0x01;
//         }

//         footer[0] = fcs(header + 1, hlen - 1);
//         footer[1] = 0xf9;

//         // readSerialCMUX();

//         serial->write(header, hlen);
//         while(!serial->writable());
//         if(len > 0) {
//             serial->write(data, len);
//             while(!serial->writable());
//         }
//         serial->write(footer, sizeof(footer));
//     }

//     void sendChannel(uint8_t channel, uint8_t control, bool cmd, const uint8_t* data, size_t len) {
//         bool initiator_ = true;
//         // std::lock_guard<MutexT> lk(mutex_);
//         // LOG(INFO, ":TROLOLO: sendC ch=%d ctl=%d cmd=%d sz=%d\r\ndata=%s", channel, control, cmd, len, data);
//         // LOG(INFO, ":TROLOLO: sendC ch=%d ctl=%d cmd=%d sz=%d", channel, control, cmd, len);
//         uint8_t header[5];
//         uint8_t footer[2];
//         const size_t hlen = len <= 0x7f ? 4 : 5;

//         header[0] = proto::BASIC_FLAG;
//         header[1] = (channel << 2) | proto::EA;
//         if ((initiator_ && cmd) || (!initiator_ && !cmd)) {
//             header[1] |= proto::CR;
//         }
//         header[2] = control;
//         header[3] = (len & 0x7f) << 1;
//         if (hlen == 5) {
//             header[4] = len >> 7;
//         } else {
//             header[3] |= proto::EA;
//         }

//         footer[0] = fcs(header + 1, hlen - 1);
//         footer[1] = proto::BASIC_FLAG;



//         // auto t1 = portable::getMillis();

//         // size_t sent = 0;
//         // while (sent < hlen) {
//         //     sent += CHECK(stream_->write((const char*)header + sent, hlen - sent));
//         //     if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
//         //         GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
//         //         return GSM0710_ERROR_FLOW_CONTROL;
//         //     }
//         //     CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
//         // }

//         // t1 = portable::getMillis();
//         // if (len) {
//         //     sent = 0;
//         //     while (sent < len) {
//         //         sent += CHECK(stream_->write((const char*)data + sent, len - sent));
//         //         if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
//         //             GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
//         //             return GSM0710_ERROR_FLOW_CONTROL;
//         //         }
//         //         CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
//         //     }
//         // }

//         // sent = 0;
//         // t1 = portable::getMillis();
//         // while (sent < sizeof(footer)) {
//         //     sent += CHECK(stream_->write((const char*)footer + sent, sizeof(footer) - sent));
//         //     if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
//         //         GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
//         //         return GSM0710_ERROR_FLOW_CONTROL;
//         //     }
//         //     CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
//         // }

//     }
        

//     void test() {
//         /*  "esp32.wifi-en" : "P0_24",
//             "esp32.wifi-io0": "P0_16",
//             "esp32.wifi-tx" : "P1_4",
//             "esp32.wifi-rx" : "P1_5",
//             "esp32.wifi-rts": "P1_6",
//             "esp32.wifi-cts": "P1_7",
//         */
//         printf("START!!!!\r\n");
//         init();
//         start();

//         getMac();
//         setAutoConnect();
//         // cmux();
//         // readSerialCMUX();
//         // start();
//         // while(true) {
//         //     parser->send("AT"); 
//         //     bool success = parser->recv("OK");
//         //     if(success)
//         //         break;
//         //     printf("MUXER LIVE!!!\r\n");
//         //     wait(1);
//         // }

//         enableDHCP();
//         connect("Aitheon", "Ukraine123");
        
        
//         //check cmux start
//         cmux();
//         readSerialCMUX();
//         return;
//         // parser->send("AT+CIPSTATUS");
//         // readSerialCMUX();

//         // message = message + "SEND AT\r\n";
//         // testCMUXData(0, 0xef, "AT", 2, false);
//         // readSerialCMUX();
//         // testCMUXData(0, 0xef, "AT", 2, false);
//         // readSerialCMUX();
//         // testCMUXData(0, 0xef, nullptr, 0, false);
//         // readSerialCMUX();
//         // testCMUXData(1, 0x10 | 0x04, nullptr, 0, true);
//         // readSerialCMUX();
//         // commandSend(channel, proto::DM | proto::PF);
//         // sendChannel(channel, control, true, nullptr, 0);
//         // testCMUXData(nullptr, 0, false);
//         // readSerialCMUX();
//         //check cmux end





//         // TCPSocket socket;
//         // socket.open();
        


//         // message = message + "SEND RESPONSE\r\n";
//         // testCMUXData(nullptr, 0);
//         // readSerialCMUX();



//         // testCMUXData();
//         // readSerialCMUX();
//         // parser->send("AT+<x>=?");
//         // readSerialCMUX();


//         // serial->write(tmp, sizeof(tmp));
//         // readSerialCMUX();

//         // int i = 0;

//         // while(!serial->readable() && i <= 100) {
//         //     char tmp[i+1] = {10};
//         //     serial->write(tmp, i+1);
//         //     ThisThread::sleep_for(10);
//         //     printf("sent %d bytes\r\n", i);
//         //     i++;
//         // }
        
//         // char m[100] = {0};
//         // snprintf(m, sizeof(m), "sent = %d bytes\r\n", i);
//         // message = message + m;

//         // uint8_t channel = 2;
//         // uint8_t control = 0xef;
//         // bool cmd = false;
//         // const char* data = "AT+CMUX=1"; 
//         // size_t len = sizeof(data);

//         // uint8_t header[5];
//         // uint8_t footer[2];
//         // const size_t hlen = len <= 0x7f ? 4 : 5;
//         // bool initiator_ = true;

//         // header[0] = 0xf9;
//         // header[1] = (channel << 2) | 0x01;
//         // if ((initiator_ && cmd) || (!initiator_ && !cmd)) {
//         //     header[1] |= 0x02;
//         // }
//         // header[2] = control;
//         // header[3] = (len & 0x7f) << 1;
//         // if (hlen == 5) {
//         //     header[4] = len >> 7;
//         // } else {
//         //     header[3] |= 0x01;
//         // }

//         // footer[0] = fcs(header + 1, hlen - 1);
//         // footer[1] = 0xf9;

//         // readSerialCMUX();

//         // serial->write(header, hlen);
//         // while(!serial->writable());
//         // serial->write(data, len);
//         // while(!serial->writable());
//         // serial->write(footer, sizeof(footer));

//         // ThisThread::sleep_for(2000);
//         // readSerialCMUX();

//         // enableDHCP();
//         // connect("Aitheon", "Ukraine123");
//         // readSerialCMUX();



//         return ;
//         /*
//         START

//   uint8_t header[5];
//     uint8_t footer[2];
//     const size_t hlen = len <= 0x7f ? 4 : 5;

//     header[0] = proto::BASIC_FLAG;
//     header[1] = (channel << 2) | proto::EA;
//     if ((initiator_ && cmd) || (!initiator_ && !cmd)) {
//         header[1] |= proto::CR;
//     }
//     header[2] = control;
//     header[3] = (len & 0x7f) << 1;
//     if (hlen == 5) {
//         header[4] = len >> 7;
//     } else {
//         header[3] |= proto::EA;
//     }

//     footer[0] = fcs(header + 1, hlen - 1);
//     footer[1] = proto::BASIC_FLAG;

//     // LOG_DUMP(TRACE, header, hlen);
//     // if (len) {
//     //     LOG_DUMP(TRACE, data, len);
//     // }
//     // LOG_DUMP(TRACE, footer, sizeof(footer));

//     auto t1 = portable::getMillis();

//     size_t sent = 0;
//     while (sent < hlen) {
//         sent += CHECK(stream_->write((const char*)header + sent, hlen - sent));
//         if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
//             GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
//             return GSM0710_ERROR_FLOW_CONTROL;
//         }
//         CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
//     }

//     t1 = portable::getMillis();
//     if (len) {
//         sent = 0;
//         while (sent < len) {
//             sent += CHECK(stream_->write((const char*)data + sent, len - sent));
//             if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
//                 GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
//                 return GSM0710_ERROR_FLOW_CONTROL;
//             }
//             CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
//         }
//     }

//     sent = 0;
//     t1 = portable::getMillis();
//     while (sent < sizeof(footer)) {
//         sent += CHECK(stream_->write((const char*)footer + sent, sizeof(footer) - sent));
//         if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
//             GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
//             return GSM0710_ERROR_FLOW_CONTROL;
//         }
//         CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
//     }

//     return 0;
//         END
//         */

//         return;
//         // InputPacket packet = {};
//         // packet.iface = ESP_IF_WIFI_STA;
//         // packet.p = new pbuf();
//         // packet.p->next = nullptr;
//         // packet.p->tot_len = 7;
//         // packet.p->len = 7;

//         // packet.p->payload = data;
//         // char test[sizeof(InputPacket)] = {0};
//         // memcpy(test, &packet, sizeof(InputPacket));
//         // serial->write(test, sizeof(InputPacket));

//         // char tmp[100] = {0};
//         // snprintf(tmp, sizeof(tmp), "size = %d\r\n", sizeof(pbuf));
//         // message = message + tmp;


//         // std::string m = "";
//         // for(int i = 0; i < sizeof(InputPacket); i++)
//         // {
//         //     char tmp[100] = {0};
//         //     snprintf(tmp, sizeof(tmp), "GET %d BYTE = %x\r\n", i, test[i]);
//         //     m = m + tmp;
//         // }
//         // message = message + m;
//         readSerialCMUX();
        
//         // enableDHCP();
//         // connect("Aitheon", "Ukraine123");

        


//         /*
//         AT+CIPSTART
//         ERR CODE:0x01090000 

//         https://github.com/particle-iot/argon-ncp-firmware 
//         This firmware is based on AT application for ESP32 ESP-IDF (https://github.com/espressif/esp32-at) and at the moment only enables Base and WiFi AT command sets
        

//         wifi.resolve ->
//         IPAddress resolve(const char* name)
//         {
//             HAL_IPAddress ip = {};
//             return (inet_gethostbyname(name, strlen(name), &ip, *this, NULL) != 0) ?
//                     IPAddress(uint32_t(0)) : IPAddress(ip);
//         }



        
//         */

//     //    
        

//         return;

//         // cmux();
//         // checkParser();


//     //     bool success = parser->recv("OK");
//     //     checkCMD(success, "AT+MVER");
  
//     //     // //DHCP
//     //     parser->send("AT+CWDHCP=1,1");
//     //     // readSerial();
//     //     success = parser->recv("OK");
//     //     checkCMD(success, "DHCP");
//     //   return ;

//         //connect wifi
//         // char ssid[] = "TVTEST";
//         // char pass[] = "12345678";
//         // // parser->send("AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
//         // char ssid[] = "Aitheon";
//         // char pass[] = "Ukraine123";
//         // parser->send("AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
//         // success = parser->recv("OK");
//         // checkCMD(success, "connected to wifi and got IP");

//         // //test cmd 
//         // parser->send("AT+CSQ");
//         // readSerial();

//         // return;
//         // // // readSerial();
        
        
//         // // ThisThread::sleep_for(1000);
//         // //dhcp

//         // // ThisThread::sleep_for(3000);
//         // parser->send("AT+CIFSR");
//         // readSerial();

//         // parser->send("AT+CWLIF");
//         // // ThisThread::sleep_for(1000);
//         // readSerial();
//         // // success = parser->recv("OK");
//         // // checkCMD(success, "AT+CIPMODE=1");

//         // return;


//         // return;
//         //"AT+CIPSTART=\"TCP\",\"google.com\",80"
//         // const char *type = "TCP";
//         // int id = 0;
//         // const char* addr = "google.com";
//         // int port = 80;
//         // int opt = 0;
//         // // parser->send("AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d", id, type, addr, port, opt);
//         // // parser->send("AT+CMUX=0");
//         // // ThisThread::sleep_for(1000);
//         // // parser->send("AT+CMUX=0");
//         // // parser->recv("OK");
//         // // ThisThread::sleep_for(1000);

//         // // parser->send("AT+CIFSR");
//         // // ThisThread::sleep_for(100);
//         // char test[1000] = {0};
//         // int i = 0;
//         // while(serial->readable()) {
//         //     char symbol[2] = {0};
//         //     serial->read(symbol, 1);
//         //     test[i] = symbol[0];
//         //     message.push_back(symbol[0]);
//         //     i++;
//         //     ThisThread::sleep_for(10);
//         // }
//         // printf("received %d bytes result = \r\n%s\r\n", i, test);


//     //     char _mac_buffer[17 + 1] = {};
//     //    bool ret = parser->send("AT+GETMAC=0")
//     //    && parser->recv("+GETMAC: \"%32[^\"]\"", _mac_buffer)
//     //    && parser->recv("OK");
//     //     checkCMD(ret, "send get MAC");
//     //    message = std::string(_mac_buffer); 


        
//         //  static ESP32 *esp = ESP32::getESP32Inst(true);
//         //  ESP32Interface interface;
//         //  interface.
//         // printf("mac = %s\r\n", esp->getMACAddress());

//         // ESP32Interface *wifi = new ESP32Interface(P1_5, P1_4);
//         // printf("mac = %s\r\n", wifi->get_mac_address());








//         //  i = 0;
//         // while(serial->readable()) {
//         //     char symbol[2] = {0};
//         //     serial->read(symbol, 1);
//         //     test[i] = symbol[0];
//         //     message.push_back(symbol[0]);
//         //     i++;
//         //     ThisThread::sleep_for(10);
//         // }
//         // printf("received %d bytes result = \r\n%s\r\n", i, test);

//         // ThisThread::sleep_for(100);

//         // for(int i = 0; i < 10; i++) {
//         //     serial->read(test, 101);
//         //     printf("%s \r\n", test);
//         //     ThisThread::sleep_for(100);
//         // }
        

//         // static ESP32 *esp = ESP32::getESP32Inst(true);
//         // printf("mac = %s\r\n", esp->getMACAddress());
        
//         // char data[] = "AT+RST";
//         // serial.write(data, 6);
//         printf("ppc\r\n");
//         // ThisThread::sleep_for(10);
//         // checkCMD(success, "send AT");
//         // for(int i = 0; i < 2; i++) {
//         //     serial.read(test, 101);
//         //     printf("%s \r\n", test);
//         //     ThisThread::sleep_for(10);
//         // }

//         // success = parser.send("AT+GETMAC=0");
//         // // char *test = new char[100];
//         // serial.read(test, 101);
//         // while()
//         // printf("result = %s \r\n", test);

//         // success = parser.recv("OK");
//         // checkCMD(success, "recv AT");



//         // success = parser.send("AT+GETMAC=0");
//         // if(success) {
//         //     printf("YEAH\r\n");
//         // } else {
//         // }
//         // char buff[100] = {0};
//         // serial.gets(buff, 4);
//         // printf("res = %s", buff);
//         // uint8_t data[255];
//         // bool res = parser.recv("+GETMAC: \"%32[^\"]\"", data);
//         // if(res) {
//         //     printf("YEAH2\r\n");
//         // } else {
//         // }

//     }

//     void old() {

//         // // static ESP32 *esp = ESP32::getESP32Inst(true);
//         // // printf("mac = %s", esp->getMACAddress());

//         // ESP32Interface wifi(P1_4, P1_5);
//         // wifi.set_credentials("TVTEST", "12345678");
//         // int res = wifi.connect();
//         // printf("result = %d\r\n", res);

//         // WiFiAccessPoint ap[5];
//         // res = wifi.scan(ap, 5);
//         // printf("scan = %d\r\n", res);

//         // // bool res = esp->connect("TVTEST", "12345678");
//         // // if(res) {
//         // //     printf("YEAH!!!!!!!!!\r\n");
//         // // } else {
//         // //     printf("FAIL!!!!!!!!!\r\n");
//         // // }
//     }
// };