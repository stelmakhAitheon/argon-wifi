#include "ParticleEsp32.h"

ParticleEsp32 *ParticleEsp32::instance = nullptr;

ParticleEsp32 *ParticleEsp32::getInstance(PinName en, PinName io0, PinName tx, PinName rx, PinName rts, PinName cts, int baudrate) {
    if(!instance)
        instance = new ParticleEsp32(en, io0, tx, rx, rts, cts, baudrate);
    return instance;
}

ParticleEsp32::ParticleEsp32(PinName en, PinName io0, PinName tx, PinName rx, PinName rts, PinName cts, int baudrate) {
    _wifiEn = new DigitalOut(en);
    _wifiIo0 = new DigitalOut(io0);
    _baudrate = baudrate;
    _serial = new mbed::UARTSerial(tx, rx, baudrate);
    _rts = rts;
    _cts = cts;
    _muxer = new MuxerType();
    _atStream = new MuxerChannelStream<MuxerType> (_muxer, PARTICLE_ESP32_NCP_AT_CHANNEL);
    _atParser = new mbed::ATCmdParser(_serial, "\r\n");
    _parser = new mbed::ATCmdParser(_atStream, "\r\n");
}

void ParticleEsp32::reset() {
    _serial->set_flow_control(SerialBase::RTSCTS, _rts, _cts);
    _serial->set_baud(_baudrate);
    _wifiIo0->write(1);
    _wifiEn->write(0);
    ThisThread::sleep_for(1000);
    _wifiEn->write(1);
    SerialUtil::skipAll(_serial, 1000);
}

void ParticleEsp32::off() {
    _wifiEn->write(0);
    Logger::getInstance()->addMessage("esp32 off \r\n");
}

bool ParticleEsp32::checkAt() {
    return _atParser->send("AT") && _atParser->recv("OK");
}

bool ParticleEsp32::waitReady() {
    _muxer->stop();

    CHECK_TRUE(checkAt(), false);
    SerialUtil::skipAll(_serial, 1000);
    Logger::getInstance()->addMessage("before initReady \r\n");
    CHECK_TRUE(initReady(), false);
    Logger::getInstance()->addMessage("after initReady \r\n");
    return true;
}

bool ParticleEsp32::initReady() {
    Logger::getInstance()->addMessage("before start mux \r\n");
    CHECK_TRUE(_atParser->send("AT+CMUX=0"), false);
    CHECK_TRUE(_atParser->recv("OK"), false);
    Logger::getInstance()->addMessage("before initMuxer \r\n");
    CHECK_TRUE(initMuxer(), false);
    Logger::getInstance()->addMessage("before send  AT+CWDHCP=0,3 \r\n");
    for(int i = 0; i < 5; i++) {
        if(_parser->send("AT+CWDHCP=0,3") && _parser->recv("OK"))
            return true;
        Logger::getInstance()->addMessage("AT+CWDHCP FAIL = %d \r\n", i);
    }
    return false;
}

bool ParticleEsp32::initMuxer() {
    _muxer->setStream(_serial);
    _muxer->setMaxFrameSize(PARTICLE_ESP32_NCP_MAX_MUXER_FRAME_SIZE);
    _muxer->setKeepAlivePeriod(PARTICLE_ESP32_NCP_KEEPALIVE_PERIOD * 2);
    _muxer->setKeepAliveMaxMissed(PARTICLE_ESP32_NCP_KEEPALIVE_MAX_MISSED);
    _muxer->setMaxRetransmissions(3);
    _muxer->setAckTimeout(2530);
    _muxer->setControlResponseTimeout(2540);

    _muxer->setChannelStateHandler(muxChannelStateCb, this);

    CHECK_RETURN(_muxer->start(true), false);

    if (_muxer->openChannel(PARTICLE_ESP32_NCP_AT_CHANNEL, _atStream->channelDataCb, _atStream)) {
        _muxer->stop();
        return false;
    }
    _muxer->resumeChannel(PARTICLE_ESP32_NCP_AT_CHANNEL);

    if (_muxer->openChannel(PARTICLE_ESP32_NCP_STA_CHANNEL, channel2Cb, this)) {
        _muxer->stop();
        return false;
    } 
    _muxer->resumeChannel(PARTICLE_ESP32_NCP_STA_CHANNEL);
    Logger::getInstance()->addMessage("before send AT + recv OK \r\n");
    for(int i = 0; i < 5; i++) {
        if(_parser->send("AT") && _parser->recv("OK"))
            return true;
        Logger::getInstance()->addMessage("send AT + recv OK FAIL = %d \r\n", i);
    }
    return false;
}

bool ParticleEsp32::init() {
    reset();
    // CHECK_TRUE(waitReady(), false);
    while(!waitReady()) {
        Logger::getInstance()->addMessage("muxer waitReady failed isRun = %d isStop = %d\r\n", (int)_muxer->isRunning(), (int)_muxer->isStopping());
        ThisThread::sleep_for(3000);
        reset();
    }
    return true;
}

const char *ParticleEsp32::getMACAddress(void) {
    bool ret = _parser->send("AT+GETMAC=0")
       && _parser->recv("+GETMAC: \"%32[^\"]\"", _macBuffer)
       && _parser->recv("OK");
    if(!ret)
        return 0;
    return _macBuffer;
}

void ParticleEsp32::setTimeout(uint32_t timeoutMs) {
    _parser->set_timeout(timeoutMs);
}

bool ParticleEsp32::connect(const char *ap, const char *passPhrase) {
    setTimeout(PARTICLE_ESP32_CONNECT_TIMEOUT);
    bool ret = _parser->send("AT+CWJAP=\"%s\",\"%s\"", ap, passPhrase)
            && _parser->recv("OK");
    setTimeout();
    return ret;
}

int ParticleEsp32::muxChannelStateCb(uint8_t channel, MuxerType::ChannelState oldState,
        MuxerType::ChannelState newState, void* ctx) {
    
    Logger::getInstance()->addMessage("muxChannelStateCb \r\n");
    auto self = (ParticleEsp32*)ctx;

    if (newState == MuxerType::ChannelState::Closed) {
        switch (channel) {
            case 0:
                // Muxer stopped
                // self->disable();
                Logger::getInstance()->addMessage("muxChannel 0 CLOSE \r\n");
                break;
            case PARTICLE_ESP32_NCP_STA_CHANNEL: {
                // Notify that the underlying data channel closed
                // It should be safe to call this here
                Logger::getInstance()->addMessage("muxChannel 2 CLOSE \r\n");
                // self->connectionState(NcpConnectionState::DISCONNECTED);
                break;
            }
        }
    }

    return 0;
}

int ParticleEsp32::channel2Cb(const uint8_t* data, size_t size, void* ctx) {
    auto self = (ParticleEsp32*)ctx;
    if(self->_receiveDataCb)
        self->_receiveDataCb(data, size);
    return 0;
}

int ParticleEsp32::sendData(const uint8_t* data, size_t size) {
    return _muxer->writeChannel(PARTICLE_ESP32_NCP_STA_CHANNEL, data, size);
}

void ParticleEsp32::setReceiveDataCb(mbed::Callback<void(const uint8_t* data, size_t size)> receiveDataCb) {
    _receiveDataCb = receiveDataCb;
}