#include "TimerMessage.h"

TimerMessage::TimerMessage(std::vector<uint8_t> messageBytes) : 
    Message(std::move(messageBytes)) {}

TimerMessage::TimerMessage(uint8_t firstByte, uint32_t sender, uint32_t receiver, uint8_t time) : Message(firstByte, sender, receiver, Message::Type::TIMER, 7) {
    setTime(time);
    generateChecksum();
}

bool TimerMessage::valid() const {
    return contentBytes.size() == 18 && validateChecksum();
}

uint8_t TimerMessage::getTime() const {
    if (valid()) {
        return contentBytes.at(12);
    }
    return 0;
}

void TimerMessage::setTime(uint8_t newTime) {
    contentBytes.at(12) = newTime;
}