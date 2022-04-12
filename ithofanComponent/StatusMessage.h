#pragma once

#include "Message.h"
#include "FanStatus.h"

#include <vector>
#include <stdint.h>

class StatusMessage : public Message {
public:
    StatusMessage(std::vector<uint8_t> messageBytes);
    bool valid() const;
    FanStatus getFanStatus() const;
    uint8_t getRemainingTime() const;
};