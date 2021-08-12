#pragma once

#include "cc1101.h"
#include <SoftwareSerial.h>
#include <stdint.h>

#include "FanStatus.h"

class Message;
class StatusMessage;

class IthoController {
public:
    IthoController(int8_t radioIo0Pin, uint8_t firstByte, uint32_t fanAddress, uint32_t remoteAddress);
    void init();
    void addChangedCallback(std::function<void(FanStatus,uint8_t)> callback);

    FanStatus getFanStatus() const;
    bool setFanStatus(FanStatus newStatus);
    uint8_t getTimer() const;
    bool setTimer(uint8_t newTimer);
    void listen();

private:
    void receivingMode(bool receiving);
    void parseMessage();
    void handleStatusMessage(const StatusMessage& message);
    void sendMessage(const Message& message);
    void changed();

    const int8_t radioIo0Pin;
    const uint8_t firstByte;
    const uint32_t fanAddress, remoteAddress;
    std::vector<std::function<void(FanStatus,uint8_t)>> callbacks;

    Cc1101 radio;
    SoftwareSerial radioSerial;
    unsigned int headerIndex = 0;
    bool connected = false;
    FanStatus fanStatus = FanStatus::unknown;
    uint8_t timer = 0;
};