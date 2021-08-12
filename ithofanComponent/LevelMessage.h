#pragma once

#include "Message.h"
#include "FanStatus.h"

class LevelMessage : public Message {
public:
    LevelMessage(std::vector<uint8_t> messageBytes);
    LevelMessage(uint8_t firstByte, uint32_t sender, uint32_t receiver, FanStatus fanStatus);
    bool valid() const;
    FanStatus getFanStatus() const;
    
private:    
    void setFanStatus(FanStatus targetStatus);
};