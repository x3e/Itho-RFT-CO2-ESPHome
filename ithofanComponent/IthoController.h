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
    bool init();
    void addChangedCallback(std::function<void(void)> callback);

    FanStatus getFanStatus() const;
    bool setFanStatus(FanStatus newStatus);
    uint8_t getTimer() const;
    bool setTimer(uint8_t newTimer);
    uint8_t getHumidity() const;
    uint8_t getRpm() const;
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
    std::vector<std::function<void(void)>> callbacks;

    Cc1101 radio;
    SoftwareSerial radioSerial;
    unsigned int headerIndex = 0;
    bool connected = false;
    FanStatus fanStatus = FanStatus::unknown;
    uint8_t timer = 0;
    uint8_t humidity = 0;
    uint8_t rpm = 0;
};