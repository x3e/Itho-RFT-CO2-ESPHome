#pragma once

#include "Message.h"
#include "FanStatus.h"
#include "FanWarning.h"

#include <vector>
#include <stdint.h>

class StatusMessage : public Message {
public:
    StatusMessage(std::vector<uint8_t> messageBytes);
    bool valid() const;
    FanStatus getFanStatus() const;
    FanWarning getFanWarning() const;
    uint16_t getRemainingTime() const;
    uint16_t getCo2() const;
    float getExhaustTemp() const;
    float getSupplyTemp() const;
    float getIndoorTemp() const;
    float getOutdoorTemp() const;
    float getInletFlow() const;
    float getExhaustFlow() const;
};