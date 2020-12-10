#pragma once

#include <mbed.h>
#include "Logger.h"

#define MAX_LEN_ATCMD 255
#define MUXER_READ_STREAM_SIZE 4096

template <typename MuxerT>
class MuxerChannelStream : public FileHandle{
private:
    MuxerT* muxer_;
    uint8_t channel_;
    uint8_t buffer_[MAX_LEN_ATCMD];
    int indexBuffer_;
    CircularBuffer<char, MUXER_READ_STREAM_SIZE> _rxbuf;
    void resetBuffer();
    void updateReadBuffer(const uint8_t* data, size_t size);
public:
    MuxerChannelStream(MuxerT* muxer, uint8_t channel);
    static int channelDataCb(const uint8_t* data, size_t size, void* ctx);
    ssize_t read(void *, size_t) override;
    ssize_t write(const void *, size_t) override;
    short poll(short events) const override;
    off_t seek(off_t, int);
    int close();
};

template <typename MuxerT>
void MuxerChannelStream<MuxerT>::updateReadBuffer(const uint8_t* data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        _rxbuf.push(data[i]);
    }
}

template <typename MuxerT>
ssize_t MuxerChannelStream<MuxerT>::read(void *buffer, size_t size) {
    char *ptr = static_cast<char *>(buffer);
    size_t data_read = 0;
    while (data_read < size && !_rxbuf.empty()) {
        _rxbuf.pop(*ptr++);
        data_read++;
    }
    return (ssize_t)data_read;
}

template <typename MuxerT>
short MuxerChannelStream<MuxerT>::poll(short events) const {
    if(_rxbuf.empty())
        return POLLOUT; 
    return POLLOUT | POLLIN; 
}

template <typename MuxerT>
void MuxerChannelStream<MuxerT>::resetBuffer() {
    memset(buffer_, 0, MAX_LEN_ATCMD);
    indexBuffer_ = 0;
}

template <typename MuxerT>
ssize_t MuxerChannelStream<MuxerT>::write(const void *data, size_t size) {
    if((indexBuffer_ + size) >= MAX_LEN_ATCMD) {
        resetBuffer();
        return 0;
    }
    memcpy(buffer_ + indexBuffer_, data, size);
    indexBuffer_ += size;

    if(indexBuffer_ > 2 && strcmp((const char*)(buffer_ + indexBuffer_ - 2), "\r\n") == 0) {
        muxer_->writeChannel(channel_, (const uint8_t*)buffer_, indexBuffer_);
        resetBuffer();
    }
    return size;
}

template <typename MuxerT>
off_t MuxerChannelStream<MuxerT>::seek(off_t, int) {
    return 0;
}

template <typename MuxerT>
int MuxerChannelStream<MuxerT>::close() {
    return 0;
}

template <typename MuxerT>
MuxerChannelStream<MuxerT>::MuxerChannelStream(MuxerT* muxer, uint8_t channel)
        : muxer_(muxer),
          channel_(channel) {
    resetBuffer();
}

template <typename MuxerT>
int MuxerChannelStream<MuxerT>::channelDataCb(const uint8_t* data, size_t size, void* ctx) {
    auto self = (MuxerChannelStream<MuxerT>*)ctx;
    self->updateReadBuffer(data, size);
    return 0;
}