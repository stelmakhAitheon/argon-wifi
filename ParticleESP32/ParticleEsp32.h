#pragma once

#include <mbed.h>
#include "MuxerChannelStream.h"
#include "muxer.h"
#include "SerialUtil.h"
#include "UARTSerial.h"
#include "Logger.h"

#define PARTICLE_ESP32_EN                       P0_24
#define PARTICLE_ESP32_IO0                      P0_16
#define PARTICLE_ESP32_TX                       P1_5
#define PARTICLE_ESP32_RX                       P1_4
#define PARTICLE_ESP32_RTS                      P1_7
#define PARTICLE_ESP32_CTS                      P1_6
#define PARTICLE_ESP32_BAUDRATE                 921600

#define PARTICLE_ESP32_NCP_AT_CHANNEL           1
#define PARTICLE_ESP32_NCP_STA_CHANNEL          2
#define PARTICLE_ESP32_NCP_AP_CHANNEL           3

#define PARTICLE_ESP32_NCP_MAX_MUXER_FRAME_SIZE 1536
#define PARTICLE_ESP32_NCP_KEEPALIVE_PERIOD     5000
#define PARTICLE_ESP32_NCP_KEEPALIVE_MAX_MISSED 5

#define PARTICLE_ESP32_CONNECT_TIMEOUT          15000
#define PARTICLE_ESP32_SENDRECV_TIMEOUT         2000

typedef gsm0710::Muxer<FileHandle, rtos::Mutex> MuxerType;

class ParticleEsp32 {
private:
    MuxerType *_muxer;
    MuxerChannelStream<MuxerType> *_atStream;
    mbed::ATCmdParser *_atParser;
    mbed::ATCmdParser *_parser;
    mbed::DigitalOut *_wifiEn;
    mbed::DigitalOut *_wifiIo0;
    mbed::UARTSerial *_serial;
    int _baudrate;
    PinName _rts;
    PinName _cts;
    char _macBuffer[18];
    bool _muxerReady;

    mbed::Callback<void(const uint8_t* data, size_t size)> _receiveDataCb;

    void reset();
    bool checkAt();
    bool waitReady();
    bool initReady();
    bool initMuxer();
    void setTimeout(uint32_t timeoutMs = PARTICLE_ESP32_SENDRECV_TIMEOUT);
    static int channel2Cb(const uint8_t* data, size_t size, void* ctx);
    static int muxChannelStateCb(uint8_t channel, MuxerType::ChannelState oldState,
        MuxerType::ChannelState newState, void* ctx);
    ParticleEsp32(PinName en, PinName io0, PinName tx, PinName rx, PinName rts, PinName cts, int baudrate);
    static ParticleEsp32 *instance;
public:
    static ParticleEsp32 *getInstance(PinName en = PARTICLE_ESP32_EN, PinName io0 = PARTICLE_ESP32_IO0, 
        PinName tx = PARTICLE_ESP32_TX, PinName rx = PARTICLE_ESP32_RX, PinName rts = PARTICLE_ESP32_RTS,
        PinName cts = PARTICLE_ESP32_CTS, int baudrate = PARTICLE_ESP32_BAUDRATE);
    bool init();
    const char *getMACAddress(void);
    bool connect(const char *ap, const char *passPhrase);
    int sendData(const uint8_t* data, size_t size);
    void setReceiveDataCb(mbed::Callback<void(const uint8_t* data, size_t size)> receiveDataCb);
    void off();
};