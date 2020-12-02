/* ESP32Interface Example
 * Copyright (c) 2015 ARM Limited
 * Copyright (c) 2017 Renesas Electronics Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ESP32_H
#define ESP32_H

// #if DEVICE_SERIAL && defined(MBED_CONF_EVENTS_PRESENT) && defined(MBED_CONF_NSAPI_PRESENT) && defined(MBED_CONF_RTOS_PRESENT)
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include "drivers/DigitalOut.h"
#include "drivers/SerialBase.h"
#include "netsocket/nsapi_types.h"
#include "netsocket/WiFiAccessPoint.h"
#include "PinNames.h"
#include "platform/ATCmdParser.h"
#include "platform/Callback.h"
#include "platform/mbed_error.h"
#include "platform/mbed_version.h"
#include "rtos/Mutex.h"
#include "rtos/ThisThread.h"

#if (MBED_MAJOR_VERSION < 6)
#include "drivers/UARTSerial.h"
#else
#include "drivers/BufferedSerial.h"
#endif

#ifndef ESP32_CONNECT_TIMEOUT
#define ESP32_CONNECT_TIMEOUT 15000
#endif
#ifndef ESP32_SEND_TIMEOUT
#define ESP32_SEND_TIMEOUT    2000
#endif
#ifndef ESP32_RECV_TIMEOUT
#define ESP32_RECV_TIMEOUT    2000
#endif
#ifndef ESP32_MISC_TIMEOUT
#define ESP32_MISC_TIMEOUT    2000
#endif

/** ESP32Interface class.
    This is an interface to a ESP32 radio.
 */

#define ESP32_NCP_MAX_MUXER_FRAME_SIZE 1536

#include <vector>
#include <string>
// #include "muxer.h"
// #include "muxer_vars.h"
#include "muxer.h"
#include "channel_stream.h"

class ESP32
{
public:
    static std::vector<std::string>  functions;
    static std::string received;
    static std::string message;
    static int funcCount;
    static Mutex mutex;

    bool ready_;
    gsm0710::Muxer<FileHandle, rtos::Mutex> muxer_;
    // std::unique_ptr<particle::MuxerChannelStream<decltype(muxer_)> > muxerAtStream_;
    bool muxerNotStarted_;
    int initParser(Stream* stream);
    int waitReady();
    int initReady();
    int initMuxer();
    


    static void addFunc(std::string name) {
        mutex.lock();
        funcCount++;
        functions.push_back(name);
        mutex.unlock();   
    }
    static void addReceive(std::string name, bool show = true) {
        if(!show)
            return;
        mutex.lock();
        received = received + name + "\r\n";
        mutex.unlock(); 
    }
    static void updateMessage() {
        // functions.push_back(std::string("test"));
        message = "funcount = " + std::to_string(funcCount) + "\r\n";
        // functions.clear();
        // message = "";
        for(size_t i = 0; i < functions.size(); i++)
            message = message + "func = " + functions[i];
    }

    void cmux() {
        _parser.send("AT+CMUX=0");
        // bool success = 
        _parser.recv("OK");
    }

    void testCMUX(uint8_t *cmd, int len) {
        static bool first = true;

        if(!first)
            return;
        first = false;
        // cmux();
        addFunc("FIRST TEST CMUX\r\n");
        cmux();
        readCMUX();
        
        // uint8_t atcmdConf[] = {0xd, 0xa, '\r', '\n'}; 
        //open 0 channel
        sendChannel(0, 63, true, nullptr, 0);
        readCMUX();
        //open 1 channel
        sendChannel(1, 63, true, nullptr, 0);
        readCMUX();

        // // send bytes to ch 0 //e1 5 7 d
        // uint8_t bytes[] = {0xe1, 0x5, 0x7, 0xd, '\r', '\n'};
        // sendChannel(0, 239, false, bytes, sizeof(bytes));
        // readCMUX();

        uint8_t at[] = "AT\r\n";
        sendChannel(1, 239, false, at, sizeof(at));
        // ThisThread::sleep_for(500);
        // sendChannel(1, 239, false, atcmdConf, sizeof(atcmdConf));
        readCMUX();

        // uint8_t atmac[] = "AT+GETMAC=0\r\n"; 
        // sendChannel(1, 239, false, atmac, sizeof(atmac));
        // readCMUX(true);

        uint8_t atdhcp[] = "AT+CWDHCP=0,3\r\n"; 
        sendChannel(1, 239, false, atdhcp, sizeof(atdhcp));
        readCMUX();

        uint8_t atwifi[] = "AT+CWJAP=\"Aitheon\",\"Ukraine123\"\r\n"; 
        sendChannel(1, 239, false, atwifi, sizeof(atwifi));
        readCMUX(true);
        readCMUX(true);
        readCMUX(true);
        readCMUX(true);
        readCMUX(true);

        uint8_t atip[] = "AT+CIFSR\r\n"; 
        sendChannel(1, 239, false, atip, sizeof(atip));
        readCMUX(true);


        //send bytes to ch 0 //e1 5 7 d
        // uint8_t bytes[] = {0xe1, 0x5, 0x7, 0xd};
        // sendChannel(0, 239, false, bytes, sizeof(bytes));
        // readCMUX();

        // uint8_t atcmd[] = "AT+GETMAC=0\r\n"; 
        // uint8_t atcmd[] = "AT+CIFSR\r\n"; 
        
        // sendChannel(1, 239, false, atcmdConf, sizeof(atcmdConf));

        // uint8_t atcmd[] = "AT+CWDHCP=1,1\r\n"; 
        // sendChannel(1, 239, false, atcmd, sizeof(atcmd));
        
        // readCMUX(true);
        // sendChannel(1, 239, false, atcmdConf, sizeof(atcmdConf));
        // readCMUX(true);

        
        
        
        // // ThisThread::sleep_for(5000);
        // readCMUX(true);
        // readCMUX(true);
        // readCMUX(true);
        // readCMUX(true);


        // readCMUX(true);

        // addReceive("AFTER WIFI");
        // uint8_t atip[] = "AT+CIFSR\r\n"; 
        // sendChannel(1, 239, false, atip, sizeof(atip));
        // readCMUX(true);
        // readCMUX(true);

        // readCMUX(true);
        // readCMUX(true);
        // readCMUX(true);
        // readCMUX(true);

        // uint8_t atmac[] = "AT+GETMAC=0\r\n"; 
        // sendChannel(1, 239, false, atmac, sizeof(atmac));
        // readCMUX(true);




        // ThisThread::sleep_for(500);
        // uint8_t atcmdConf[] = {0xd, 0xa}; 
        // ThisThread::sleep_for(1000);
        // sendChannel(1, 239, false, atcmdConf, sizeof(atcmdConf));
        // ThisThread::sleep_for(2000);

        // readCMUX(true);
        // readCMUX(true);
        addFunc("AFTER CMUX\r\n");
    }
    
    void startCMUXMode() {
        static bool first = true;
        if(!first)
            return;
        first = false;
        cmux();
        readCMUX();
    }

    void openChannels() {
        static bool first = true;
        if(!first)
            return;
        first = false;
        startCMUXMode();
        sendChannel(0, 63, true, nullptr, 0);
        readCMUX();
        sendChannel(1, 63, true, nullptr, 0);
        readCMUX();
        sendChannel(2, 63, true, nullptr, 0);
        readCMUX();
        addReceive("YEAH!!!");
    }
    
    bool readCMUX(bool show=false, bool showParse = false) {
        // int waitI = 0;
        while(!_serial.readable()) {
            // if(waitI++ >= 100)
            //     return false;
            ThisThread::sleep_for(10);
        }

        std::string tmp = "";
        while(_serial.readable()) {
            char symbol[2] = {0};
            _serial.read(symbol, 1);
            tmp.push_back(symbol[0]);
            // ThisThread::sleep_for(100);
        }

        int bufSize = tmp.size();

        int posCmdStart, posCmdEnd;
        int posStart = 0;

        // addReceive("next CMUX", show);
        // char buf[] = {0xf9, 0x1, 0xef, 0x9, 0xe3, 0x5, 0x7, 0xd, 0x9a, 0xf9, 0xf9, 0x1, 0xef, 0x9, 0xe3, 0x5, 0x7, 0xd, 0x9a, 0xf9};
        // tmp = buf;
        while(true) {
            // addReceive("find posCmdStart = " + std::to_string(posStart));
            posCmdStart = tmp.find(gsm0710::proto::BASIC_FLAG, posStart);
            if(posCmdStart == -1) {
                // addReceive("NOT FOUND START ");
                break;
            }
            // addReceive("find posCmdEnd = " + std::to_string(posCmdStart));
            posCmdEnd = tmp.find(gsm0710::proto::BASIC_FLAG, posCmdStart + 1);
            if(posCmdEnd == -1) {
                //start
                // std::string final = "";
                // for(int i = posCmdStart; i < tmp.size(); i++) {
                //     char b[5] = {0};
                //     snprintf(b, sizeof(b), "0x%x ", tmp.at(i));
                //     final = final + b;
                // }
                // addReceive("NOT FOUND END " + final);
                addReceive("NOT FOUND END ", show);
                //end
                break;
            }
            posStart = posCmdEnd + 1;
            addReceive("parse = " + std::to_string(posCmdStart) + " - " + std::to_string(posCmdEnd) + " size = " + std::to_string(tmp.size()), showParse);

            std::string cmd = tmp.substr(posCmdStart, posCmdEnd - posCmdStart + 1);
            
            //start
            std::string final = "";
            for(size_t i = 0; i < cmd.size(); i++) {
                char b[5] = {0};
                snprintf(b, sizeof(b), "%x ", cmd.at(i));
                final = final + b;
            }
            // addReceive(final, show);
            //end
            // continue;
            cmd.erase(0, 1);
            // address
            char address = cmd.at(0);
            if (!(address & gsm0710::proto::EA)) {
                addReceive("address ok = false", show);
                continue;
            }
            cmd.erase(0, 1);

            //control
            char control = cmd.at(0);
            cmd.erase(0, 1);

            // length
            int length = cmd.at(0) >> 1;
            int hlen = 4;
            bool anotherLen = false;
            if (!(cmd.at(0) & gsm0710::proto::EA)) {
                anotherLen = true;
                hlen = 5;
                length |= ((size_t)cmd.at(1)) << 7;
                addReceive("anotherLen = true length = " + std::to_string(length));
                continue;
            }
            int curLen = length + hlen;
            bool sizeOk = true;
            if (curLen > bufSize) {
                sizeOk = false;
                addReceive("sizeOk = false");
                continue;
            }
            cmd.erase(0, hlen == 4? 1: 2); // skip length byte

            if(length == 0) {
                addReceive("CMUX length = 0\r\n");
                continue;
            }
            cmd.erase(length, cmd.size() - length);
            if(show) {
                std::string final = "";
                for(size_t i = 0; i < cmd.size(); i++) {
                    char b[5] = {0};
                    snprintf(b, sizeof(b), "%x ", cmd.at(i));
                    final = final + b;
                }
                addReceive(final, show);
             
                // addReceive(cmd, show);
              
            }
        }
        
        return false;
    }

    // bool readCMUX(bool show=false, bool showParse = false) {
    //     int waitI = 0;
    //     while(!_serial.readable()) {
    //         // if(waitI++ >= 100)
    //         //     return false;
    //         ThisThread::sleep_for(10);
    //     }

    //     std::string tmp = "";
    //     while(_serial.readable()) {
    //         char symbol[2] = {0};
    //         _serial.read(symbol, 1);
    //         tmp.push_back(symbol[0]);
    //         // ThisThread::sleep_for(100);
    //     }

    //     int bufSize = tmp.size();

    //     int posCmdStart, posCmdEnd;
    //     int posStart = 0;

    //     // addReceive("next CMUX", show);
    //     // char buf[] = {0xf9, 0x1, 0xef, 0x9, 0xe3, 0x5, 0x7, 0xd, 0x9a, 0xf9, 0xf9, 0x1, 0xef, 0x9, 0xe3, 0x5, 0x7, 0xd, 0x9a, 0xf9};
    //     // tmp = buf;
    //     while(true) {
    //         // addReceive("find posCmdStart = " + std::to_string(posStart));
    //         posCmdStart = tmp.find(proto::BASIC_FLAG, posStart);
    //         if(posCmdStart == -1) {
    //             // addReceive("NOT FOUND START ");
    //             break;
    //         }
    //         tmp.erase(0, posCmdStart);
    //         // address
    //         tmp.erase(0, 1);
    //         char address = tmp.at(0);
    //         if (!(address & proto::EA)) {
    //             addReceive("address ok = false", show);
    //             break;
    //         }

    //         // addReceive("address ok = true", show);

    //         tmp.erase(0, 1);

    //         //control
    //         char control = tmp.at(0);
    //         tmp.erase(0, 1);

    //         // length
    //         int length = tmp.at(0) >> 1;
    //         int hlen = 4;
    //         bool anotherLen = false;
    //         if (!(tmp.at(0) & proto::EA)) {
    //             anotherLen = true;
    //             hlen = 5;
    //             length |= ((size_t)tmp.at(1)) << 7;
    //             addReceive("anotherLen = true length = " + std::to_string(length));
    //             // continue;
    //         }
    //         int curLen = length + hlen;
    //         bool sizeOk = true;
    //         if (curLen > bufSize) {
    //             sizeOk = false;
    //             addReceive("sizeOk = false");
    //             break;
    //         }

    //         tmp.erase(0, hlen == 4? 1: 2); // skip length byte

    //         if(length == 0) {
    //             addReceive("CMUX length = 0\r\n");
    //             break;
    //         }
    //         std::string cmd = tmp.substr(0, length);
    //         tmp.erase(0, length); 
            
    //         posCmdEnd = tmp.find(proto::BASIC_FLAG, 0);
    //         if(posCmdEnd == -1) {
    //             addReceive("posCmdEnd fail");
    //             break;
    //         }


    //         tmp.erase(0, posCmdEnd+1);

    //         if(show) {
    //             std::string final = "";
    //             for(int i = 0; i < cmd.size(); i++) {
    //                 char b[5] = {0};
    //                 snprintf(b, sizeof(b), "%x ", cmd.at(i));
    //                 final = final + b;
    //             }
    //             addReceive(final, show);              
    //         }
    //         break;
    //     }
        
    //     return false;
    // }

    // void readSerialCMUX() {
    //     while(!_serial.readable())
    //         ThisThread::sleep_for(10);
    //     // char test[1000] = {0};
    //     int i = 0;
    //     string tmp;
    //     while(_serial.readable()) {
    //         char symbol[2] = {0};
    //         char test[100] = {0};
    //         _serial.read(symbol, 1);
    //         snprintf(test, sizeof(test), "GET %d BYTE = %x\r\n", i, symbol[0]);
    //         tmp = tmp + test;
    //         // test[i] = symbol[0];
    //         // message.push_back(symbol[0]);
    //         i++;
    //         ThisThread::sleep_for(10);
    //     }
    //     // addFunc(tmp);
    //     // printf("received %d bytes result = \r\n%s\r\n", i, test);
    // }

    void sendChannel(uint8_t channel, uint8_t control, bool cmd, const uint8_t* data, size_t len) {
        bool initiator_ = true;
        uint8_t header[5];
        uint8_t footer[2];
        const size_t hlen = len <= 0x7f ? 4 : 5;

        header[0] = gsm0710::proto::BASIC_FLAG;
        header[1] = (channel << 2) | gsm0710::proto::EA;
        if ((initiator_ && cmd) || (!initiator_ && !cmd)) {
            header[1] |= gsm0710::proto::CR;
        }
        header[2] = control;
        header[3] = (len & 0x7f) << 1;
        if (hlen == 5) {
            header[4] = len >> 7;
        } else {
            header[3] |= gsm0710::proto::EA;
        }

        footer[0] = gsm0710::fcs(header + 1, hlen - 1);
        footer[1] = gsm0710::proto::BASIC_FLAG;

        size_t sent = 0;
        while (sent < hlen) {
            sent += _serial.write((const char*)header + sent, hlen - sent);
            while(!_serial.writable())
                ThisThread::sleep_for(100);
        }
        // ThisThread::sleep_for(200);
        // readCMUX();

        if (len) {
            sent = 0;
            while (sent < len) {
                sent += _serial.write((const char*)data + sent, len - sent);
                while(!_serial.writable())
                    ThisThread::sleep_for(100);
            }
        }
        // ThisThread::sleep_for(200);
        // readCMUX();

        sent = 0;
        while (sent < sizeof(footer)) {
            sent += _serial.write((const char*)footer + sent, sizeof(footer) - sent);
            while(!_serial.writable())
                ThisThread::sleep_for(100);
        }
        // ThisThread::sleep_for(200);
        // readCMUX();

        // auto t1 = portable::getMillis();

        // size_t sent = 0;
        // while (sent < hlen) {
        //     sent += CHECK(stream_->write((const char*)header + sent, hlen - sent));
        //     if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
        //         GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
        //         return GSM0710_ERROR_FLOW_CONTROL;
        //     }
        //     CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
        // }

        // size_t sent = 0;
        // while (sent < hlen) {
        //     sent += _serial.write((const char*)header + sent, hlen - sent);
        //     while(!_serial.writable())
        //         ThisThread::sleep_for(10);
        // }

        // t1 = portable::getMillis();
        // if (len) {
        //     sent = 0;
        //     while (sent < len) {
        //         sent += CHECK(stream_->write((const char*)data + sent, len - sent));
        //         if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
        //             GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
        //             return GSM0710_ERROR_FLOW_CONTROL;
        //         }
        //         CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
        //     }
        // }

        // sent = 0;
        // t1 = portable::getMillis();
        // while (sent < sizeof(footer)) {
        //     sent += CHECK(stream_->write((const char*)footer + sent, sizeof(footer) - sent));
        //     if ((portable::getMillis() - t1) > getControlResponseTimeout() * 2) {
        //         GSM0710_LOG_DEBUG(ERROR, "Timeout writing into stream");
        //         return GSM0710_ERROR_FLOW_CONTROL;
        //     }
        //     CHECK(stream_->waitEvent(StreamT::WRITABLE, std::max<int>(0, getControlResponseTimeout() * 2 - (portable::getMillis() - t1))));
        // }

    }
    /**
    * Static method to create or retrieve the single ESP32 instance
    */
    static ESP32 * getESP32Inst(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
                                PinName rts, PinName cts, int baudrate);
    static ESP32 * getESP32Inst(bool debug = false);

    ESP32(PinName en, PinName io0, PinName tx, PinName rx, bool debug,
          PinName rts, PinName cts, int baudrate);

    /**
    * Checks Version Information
    *
    * @param ver_info buffer address for storing version information
    * @param buf_size  buffer size
    * @return String of Version Information
    */
    bool get_version_info(char * ver_info, int buf_size);

    /**
    * Sets the Wi-Fi Mode
    *
    * @param mode mode of WIFI 1-client, 2-host, 3-both
    * @return true only if ESP32 was setup correctly
    */
    bool set_mode(int mode);

    /**
    * Enable/Disable DHCP
    *
    * @param enabled DHCP enabled when true
    * @param mode mode of DHCP 0-softAP, 1-station, 2-both
    * @return true only if ESP32 enables/disables DHCP successfully
    */
    bool dhcp(bool enabled, int mode);

    /**
    * Connect ESP32 to AP
    *
    * @param ap the name of the AP
    * @param passPhrase the password of AP
    * @return true only if ESP32 is connected successfully
    */
    bool connect(const char *ap, const char *passPhrase);

    /**
    * Disconnect ESP32 from AP
    *
    * @return true only if ESP32 is disconnected successfully
    */
    bool disconnect(void);

    /**
    * Get the IP address of ESP32
    *
    * @return null-teriminated IP address or null if no IP address is assigned
    */
    const char *getIPAddress(void);
    const char *getIPAddress_ap(void);

    /**
    * Get the MAC address of ESP32
    *
    * @return null-terminated MAC address or null if no MAC address is assigned
    */
    const char *getMACAddress(void);
    const char *getMACAddress_ap(void);

     /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    const char *getGateway();
    const char *getGateway_ap();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask 
     *                  or null if no network mask has been recieved
     */
    const char *getNetmask();
    const char *getNetmask_ap();

    /* Return RSSI for active connection
     *
     * @return      Measured RSSI
     */
    int8_t getRSSI();

    /**
    * Check if ESP32 is conenected
    *
    * @return true only if the chip has an IP address
    */
    bool isConnected(void);

    /** Scan for available networks
     *
     * @param  ap    Pointer to allocated array to store discovered AP
     * @param  limit Size of allocated @a res array, or 0 to only count available AP
     * @return       Number of entries in @a res, or if @a count was 0 number of available networks, negative on error
     *               see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned limit);

    /**
    * Open a socketed connection
    *
    * @param type the type of socket to open "UDP" or "TCP"
    * @param id id to give the new socket, valid 0-4
    * @param port port to open connection with
    * @param addr the IP address of the destination
    * @param addr the IP address of the destination
    * @param opt  type=" UDP" : UDP socket's local port, zero means any
    *             type=" TCP" : TCP connection's keep alive time, zero means disabled
    * @return true only if socket opened successfully
    */
    bool open(const char *type, int id, const char* addr, int port, int opt = 0);

    /**
    * Sends data to an open socket
    *
    * @param id id of socket to send to
    * @param data data to be sent
    * @param amount amount of data to be sent - max 1024
    * @return true only if data sent successfully
    */
    bool send(int id, const void *data, uint32_t amount);

    /**
    * Receives data from an open socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv(int id, void *data, uint32_t amount, uint32_t timeout = ESP32_RECV_TIMEOUT);

    /**
    * Closes a socket
    *
    * @param id id of socket to close, valid only 0-4
    * @param wait_close 
    * @return true only if socket is closed successfully
    */
    bool close(int id, bool wait_close = false);

    /**
    * Allows timeout to be changed between commands
    *
    * @param timeout_ms timeout of the connection
    */
    void setTimeout(uint32_t timeout_ms = ESP32_MISC_TIMEOUT);

    /**
    * Checks if data is available
    */
    bool readable();

    /**
    * Checks if data can be written
    */
    bool writeable();

    void socket_attach(int id, void (*callback)(void *), void *data);
    int get_free_id();

    bool config_soft_ap(const char *ap, const char *passPhrase, uint8_t chl, uint8_t ecn);

    bool restart();
    bool get_ssid(char *ap);
    bool cre_server(int port);
    bool del_server();
    bool accept(int * p_id);

    bool set_network(const char *ip_address, const char *netmask, const char *gateway);
    bool set_network_ap(const char *ip_address, const char *netmask, const char *gateway);

    /**
    * Attach a function to call whenever network state has changed
    *
    * @param func A pointer to a void function, or 0 to set as none
    */
    void attach_wifi_status(mbed::Callback<void(int8_t)> status_cb);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    int8_t get_wifi_status() const;

    /**
     * Flush the serial port input buffers.
     *
     * If you do HW reset for ESP module, you should
     * flush the input buffers from existing responses
     * from the device.
     */
    void flush();

    static const int8_t WIFIMODE_STATION = 1;
    static const int8_t WIFIMODE_SOFTAP = 2;
    static const int8_t WIFIMODE_STATION_SOFTAP = 3;
    static const int8_t SOCKET_COUNT = 5;

    static const int8_t STATUS_DISCONNECTED = 0;
    static const int8_t STATUS_CONNECTED = 1;
    static const int8_t STATUS_GOT_IP = 2;

private:
    mbed::DigitalOut * _p_wifi_en;
    mbed::DigitalOut * _p_wifi_io0;
    bool _init_end_common;
    bool _init_end_wifi;
#if (MBED_MAJOR_VERSION < 6)
    mbed::UARTSerial _serial;
#else
    mbed::BufferedSerial _serial;
#endif
    mbed::ATCmdParser _parser;
    struct packet {
        struct packet *next;
        int id;
        uint32_t len;
        uint32_t index;
        // data follows
    } *_packets, **_packets_end;
    int _wifi_mode;
    int _baudrate;
    PinName _rts;
    PinName _cts;
    int _flow_control;
    uint32_t last_timeout_ms;

    std::vector<int> _accept_id;
    uint32_t _id_bits;
    bool _server_act;
    rtos::Mutex _smutex; // Protect serial port access
    static ESP32 * instESP32;
    int8_t _wifi_status;
    mbed::Callback<void(int8_t)> _wifi_status_cb;
    uint32_t _at_version;

    bool _ids[SOCKET_COUNT];
    struct {
        void (*callback)(void *);
        void *data;
        int  Notified;
    } _cbs[SOCKET_COUNT];

    void _startup_common();
    bool _startup_wifi();
    bool reset(void);
    void debugOn(bool debug);
    void socket_handler(bool connect, int id);
    void _connect_handler_0();
    void _connect_handler_1();
    void _connect_handler_2();
    void _connect_handler_3();
    void _connect_handler_4();
    void _closed_handler_0();
    void _closed_handler_1();
    void _closed_handler_2();
    void _closed_handler_3();
    void _closed_handler_4();
    void _connection_status_handler();
    void _packet_handler();
    void _clear_socket_packets(int id);
    void event();
    bool recv_ap(nsapi_wifi_ap_t *ap);

    char _ip_buffer[16];
    char _gateway_buffer[16];
    char _netmask_buffer[16];
    char _mac_buffer[18];

    char _ip_buffer_ap[16];
    char _gateway_buffer_ap[16];
    char _netmask_buffer_ap[16];
    char _mac_buffer_ap[18];

#if defined(TARGET_ESP32AT_BLE)
public:
    typedef struct {
        int      srv_index;          /**< service's index starting from 1 */
        int      char_index;         /**< characteristic's index starting from 1 */
        int      desc_index;         /**< descriptor's index */
        void *   data;               /**< data buffer address */
        uint32_t len;                /**< data len */
    } ble_packet_t;

    typedef union {
        const uint8_t * addr;        /**< buffer address */
        uint32_t        data;        /**< data */
    } union_data_t;

    typedef struct {
        union_data_t uuid;           /**< UUID value */
        uint8_t      uuid_type;      /**< UUID type  0: addr 1:data */
        uint16_t     uuid_size;      /**< UUID size */
        uint16_t     val_max_len;    /**< max allow value length (the max length when you dynamic set value) */
        union_data_t value;          /**< initial value */
        uint8_t      value_type;     /**< initial value  type 0: addr 1:data */
        uint16_t     value_size;     /**< initial value size */
        uint8_t      permissions;    /**< permission (refer to BLE Spec for definition) */
    } gatt_service_t;

    typedef struct {
        uint16_t  adv_int_min;       /**< minimum value of advertising interval; range: 0x0020 ~ 0x4000 */
        uint16_t  adv_int_max;       /**< maximum value of advertising interval; range: 0x0020 ~ 0x4000 */
        uint8_t   adv_type;          /**< 0:FADV_TYPE_IND, 2:FADV_TYPE_SCAN_IND, 3:FADV_TYPE_NONCONN_IND */
        uint8_t   own_addr_type;     /**< own BLE address type; 0:FBLE_ADDR_TYPE_PUBLIC, 1:FBLE_ADDR_TYPE_RANDOM */
        uint8_t   channel_map;       /**< channel of advertising; ADV_CHNL_~ */
        uint8_t   adv_filter_policy; /**< filter policy of advertising; ADV_FILTER_ALLOW_SCAN_~ */
        uint8_t   peer_addr_type;    /**< remote BLE address type; 0:FPUBLIC, 1:FRANDOM */
        uint8_t   peer_addr[6];      /**< remote BLE address */
    } advertising_param_t;

    typedef struct {
        uint8_t   addr[6];           /**< BLE address */
        int8_t    rssi;              /**< signal strength */
        uint8_t   adv_data[31];      /**< advertising data */
        uint8_t   adv_data_len;      /**< advertising data length */
        uint8_t   scan_rsp_data[31]; /**< scan response data */
        uint8_t   scan_rsp_data_len; /**< scan response data length */
        uint8_t   addr_type;         /**< the address type of broadcasters */
    } ble_scan_t;

    typedef struct {
        int      srv_index;          /**< service's index starting from 1 */
        uint16_t srv_uuid;           /**< service's UUID */
        int      srv_type;           /**< service's type */
    } ble_primary_service_t;

    typedef struct {
        int      srv_index;          /**< service's index starting from 1 */
        uint16_t srv_uuid;           /**< service's UUID */
        int      srv_type;           /**< service's type */
    } ble_characteristics_t;

    typedef struct {
        int      char_index;         /**< Characteristic's index starting from 1 */
        uint16_t char_uuid;          /**< Characteristic's UUID */
        uint8_t  char_prop;          /**< Characteristic's properties */
    } ble_discovers_char_t;

    typedef struct {
        int      char_index;         /**< Characteristic's index starting from 1 */
        int      desc_index;         /**< Descriptor's index */
        uint16_t desc_uuid;          /**< Descriptor's UUID */
    } ble_discovers_desc_t;

    // advertising_param_t:adv_type
    #define ADV_TYPE_IND           0
    #define ADV_TYPE_SCAN_IND      2
    #define ADV_TYPE_NONCONN_IND   3

    // advertising_param_t:own_addr_type and peer_addr_type
    #define BLE_ADDR_TYPE_PUBLIC   0
    #define BLE_ADDR_TYPE_RANDOM   1

    // advertising_param_t:channel_map
    #define ADV_CHNL_37            0x01
    #define ADV_CHNL_38            0x02
    #define ADV_CHNL_39            0x04
    #define ADV_CHNL_ALL           0x07

    // advertising_param_t:adv_filter_policy
    #define ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY    0
    #define ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY   1
    #define ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST   2
    #define ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST  3

    // ble_set_role: role
    #define INIT_CLIENT_ROLE       1
    #define INIT_SERVER_ROLE       2

    /** Sets BLE Role
     *
     *  @param role         INIT_CLIENT_ROLE: client role, INIT_SERVER_ROLE: server role
     *  @return             true: success, false: failure
     */
    bool ble_set_role(int role);

    /** Gets BLE Role
     *
     *  @param role         INIT_CLIENT_ROLE: client role, INIT_SERVER_ROLE: server role
     *  @return             true: success, false: failure
     */
    bool ble_get_role(int * role);

    /** Sets BLE Device's Name
     *
     *  @param name         The BLE device name
     *  @return             true: success, false: failure
     */
    bool ble_set_device_name(const char * name);

    /** Gets BLE Device's Name
     *
     *  @param name         The BLE device name
     *  @return             true: success, false: failure
     */
    bool ble_get_device_name(char * name);

    /** GATTS Creates and Starts Services
     *
     *  @return             true: success, false: failure
     */
    bool ble_start_services();

    /** Sets BLE Scan Response
     *
     *  @param data         Scan response data
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_set_scan_response(const uint8_t * data, int len);

    /** Starts Advertising
     *
     *  @return             true: success, false: failure
     */
    bool ble_start_advertising();

    /** Stops Advertising
     *
     *  @return             true: success, false: failure
     */
    bool ble_stop_advertising();

    /** Sets BLE Device's Address
     *
     *  @param addr_type    0: public address, 1: random address
     *  @param addr         Random address data. Valid only when addr_type is 1.
     *  @return             true: success, false: failure
     */
    bool ble_set_addr(int addr_type, const uint8_t * random_addr = NULL);

    /** Gets BLE Device's Address
     *
     *  @param public_addr  BLE public address
     *  @return             true: success, false: failure
     */
    bool ble_get_addr(uint8_t * public_addr);

    /** Sets Parameters of Advertising
     *
     *  @param param        Parameters. See advertising_param_t.
     *  @return             true: success, false: failure
     */
    bool ble_set_advertising_param(const advertising_param_t * param);

    /** Sets Advertising Data
     *
     *  @param data         Advertising data
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_set_advertising_data(const uint8_t * data, int len);

    /** GATT Sets Service
     *
     *  @param service_list GATT service list. see gatt_service_t.
     *  @param num          Number of GATT service list
     *  @return             true: success, false: failure
     */
    bool ble_set_service(const gatt_service_t * service_list, int num);

    /** GATTS Sets Characteristic
     *
     *  @param srv_index    Service's index starting from 1
     *  @param char_index   Characteristic's index starting from 1
     *  @param data         Data buffer address
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_set_characteristic(int srv_index, int char_index, const uint8_t * data, int len);

    /** GATTS Notifies of Characteristics
     *
     *  @param srv_index    Service's index starting from 1
     *  @param char_index   Characteristic's index starting from 1
     *  @param data         Data buffer address
     *  @param len          Data len
     *  @return             true: success, false: failure
     */
    bool ble_notifies_characteristic(int srv_index, int char_index, const uint8_t * data, int len);

    /** Sets Parameters of BLE Scanning
     *
     *  @param scan_type     0: passive scan  1: active scan
     *  @param own_addr_type 0: public address  1: random address  2: RPA public address  3: RPA random address
     *  @param filter_policy 0: BLE_SCAN_FILTER_ALLOW_ALL
     *                       1: BLE_SCAN_FILTER_ALLOW_ONLY_WLST
     *                       2: BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR
     *                       3: BLE_SCAN_FILTER_ALLOW_WLIST_PRA_DIR
     *  @param scan_interval scan interval
     *  @param scan_window   scan window
     *  @return              rue: success, false: failure
     */
    bool ble_set_scan_param(int scan_type, int own_addr_type, int filter_policy, int scan_interval, int scan_window);

    /** Enables BLE Scanning
     *
     *  @param interval     0: scanning is continuous
     *                      !0: scanning should last for <interval> seconds and then stop automatically
     *  @return             true: success, false: failure
     */
    bool ble_start_scan(int interval = 0);

    /** Disables BLE scan
     *
     *  @return             true: success, false: failure
     */
    bool ble_stop_scan();

    /** Establishes BLE connection
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param remote_addr  Remote BLE address
     *  @return             true: success, false: failure
     */
    bool ble_connect(int conn_index, const uint8_t * remote_addr);

    /** Ends BLE connection
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @return             true: success, false: failure
     */
    bool ble_disconnect(int conn_index);

    /** GATTC Discovers Primary Services
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param service      Service info
     *  @param num          Number of service info
     *  @return             true: success, false: failure
     */
    bool ble_discovery_service(int conn_index, ble_primary_service_t * service, int * num);

    /** GATTC Discovers Characteristics
     *
     *  @param conn_index     Index of BLE connection; only 0 is supported for the single connection right now,
     *                        but multiple BLE connections will be supported in the future.
     *  @param srv_index      Service's index. It can be fetched with "ble_discovery_service()"
     *  @param discovers_char Characteristic info
     *  @param char_num       Number of characteristic info
     *  @param discovers_desc Descriptor info
     *  @param desc_num       Number of descriptor info
     *  @return               true: success, false: failure
     */
    bool ble_discovery_characteristics(
             int conn_index, int srv_index,
             ble_discovers_char_t * discovers_char, int * char_num,
             ble_discovers_desc_t * discovers_desc, int * desc_num
         );

    /** GATTC Reads a Characteristic
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Read data buffer
     *  @param amount       Amount of bytes to be received
     *  @return             Data size of received
     */
    int32_t ble_read_characteristic(int conn_index, int srv_index, int char_index, uint8_t * data, int amount);

    /** GATTC Reads a Descriptor
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param desc_index   Descriptor's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Read data buffer
     *  @param amount       Amount of bytes to be received
     *  @return             true: success, false: failure
     */
    int32_t ble_read_descriptor(int conn_index, int srv_index, int char_index, int desc_index, uint8_t * data, int amount);

    /** GATTC Writes Characteristic
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Write data buffer
     *  @param amount       Amount of data to be written
     *  @return             true: success, false: failure
     */
    bool ble_write_characteristic(int conn_index, int srv_index, int char_index, const uint8_t * data, int amount);

    /** GATTC Writes Descriptor
     *
     *  @param conn_index   Index of BLE connection; only 0 is supported for the single connection right now,
     *                      but multiple BLE connections will be supported in the future.
     *  @param srv_index    Service's index. It can be fetched with "ble_discovery_service()"
     *  @param char_index   Characteristic's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param desc_index   Descriptor's index. It can be fetched with "ble_discovery_characteristics()"
     *  @param data         Write data buffer
     *  @param amount       Amount of data to be written
     *  @return             true: success, false: failure
     */
    bool ble_write_descriptor(int conn_index, int srv_index, int char_index, int desc_index, const uint8_t * data, int amount);

    /** For executing OOB processing on background
     *
     *  @param timeout      AT parser receive timeout
     *  @param all          if TRUE, process all OOBs instead of only one
     */
    void ble_process_oob(uint32_t timeout, bool all);

    /** Register a callback on state change.
     *
     *  The specified callback will be called on state changes.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations.
     *
     *  Note! This is not intended as an attach-like asynchronous api, but rather
     *  as a building block for constructing  such functionality.
     *
     *  The exact timing of when the registered function
     *  is called is not guaranteed and susceptible to change. It should be used
     *  as a cue to make ble_process_oobl calls to find the current state.
     *
     *  @param cb_func      function to call on state change
     */
    void ble_attach_sigio(mbed::Callback<void()> cb_func);

    /**
     * Attach a function to call whenever the BLE connection establishes
     *
     * @param cb_func       Pointer to the function to be calledt
     *                      cb_func argument  0: disconnect, 1: connect
     */
    void ble_attach_conn(mbed::Callback<void(int, uint8_t *)> cb_func);

    /**
     * Attach a function to call whenever the BLE connection ends
     *
     * @param cb_func       Pointer to the function to be calledt
     *                      cb_func argument  0: disconnect, 1: connect
     */
    void ble_attach_disconn(mbed::Callback<void(int)> cb_func);

    /**
     * Attach a function to call whenever characteristic data is written
     *
     * @param cb_func       Pointer to the function to be called
     */
    void ble_attach_write(mbed::Callback<void(ble_packet_t *)> cb_func);

    /**
     * Attach a function to call whenever scan data is received
     *
     * @param cb_func       Pointer to the function to be called
     */
    void ble_attach_scan(mbed::Callback<void(ble_scan_t *)> cb_func);

private:
    #define PRIMARY_SERVICE_BUF_NUM    16
    #define DISCOVERS_CHAR_BUF_NUM     16
    #define DISCOVERS_DESC_BUF_NUM     16

    struct {
        mbed::Callback<void()> callback;
        int  Notified;
    } _cbs_ble;

    mbed::Callback<void(int, uint8_t *)> _ble_conn_cb;
    mbed::Callback<void(int)> _ble_disconn_cb;
    mbed::Callback<void(ble_packet_t *)> _ble_write_cb;
    mbed::Callback<void(ble_scan_t *)> _ble_scan_cb;
    int _ble_role;
    bool _init_end_ble;
    int _primary_service_idx;
    int _discovers_char_idx;
    int _discovers_desc_idx;
    ble_primary_service_t _primary_service[PRIMARY_SERVICE_BUF_NUM];
    ble_discovers_char_t _discovers_char[DISCOVERS_CHAR_BUF_NUM];
    ble_discovers_desc_t _discovers_desc[DISCOVERS_DESC_BUF_NUM];

    bool _startup_ble();
    void _ble_conn();
    void _ble_disconn();
    void _ble_write();
    void _ble_scan();
    void _ble_primsrv();
    void _ble_discovers_char();
    char _int2char(int data);
    int _char2int(char c);
    int _set_char(char * buf1, const uint8_t * buf2, int size);
#endif /* TARGET_ESP32AT_BLE */

};
// #endif
#endif /* ESP32_H */
