#pragma once

#include "Message.h"

class TimerMessage : public Message {
public:
    TimerMessage(std::vector<uint8_t> messageBytes);
    TimerMessage(uint8_t firstByte, uint32_t sender, uint32_t receiver, uint8_t time);
    bool valid() const;
    uint8_t getTime() const;
private:
    void setTime(uint8_t newTime);
};