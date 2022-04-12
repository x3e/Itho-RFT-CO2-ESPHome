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
    bool setTimer(uint8_t newTimer);
    uint16_t getTimer() const;
    uint16_t getCo2() const;
    float getExhaustTemp() const;
    float getSupplyTemp() const;
    float getIndoorTemp() const;
    float getOutdoorTemp() const;
    float getInletFlow() const;
    float getExhaustFlow() const;
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
    uint16_t timer = 0;
    uint16_t co2 = 0;
    float exhaustTemp = 0;
    float supplyTemp = 0;
    float indoorTemp = 0;
    float outdoorTemp = 0;
    float inletFlow = 0;
    float exhaustsFlow = 0;
};