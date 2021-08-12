#include "LevelMessage.h"

#include <map>

const std::map<uint16_t, FanStatus> commandBytes {
    {0x0B0B, FanStatus::night},
    {0x0507, FanStatus::automatic},
    {0x0204, FanStatus::low},
    {0x0307, FanStatus::medium},
    {0x0404, FanStatus::high}
};

LevelMessage::LevelMessage(std::vector<uint8_t> messageBytes) : 
    Message(std::move(messageBytes)) {}

LevelMessage::LevelMessage(uint8_t firstByte, uint32_t sender, uint32_t receiver, FanStatus fanStatus) : Message(firstByte, sender, receiver, Message::Type::LEVEL, 3) {
    setFanStatus(fanStatus);
    generateChecksum();
}

bool LevelMessage::valid() const {
    return contentBytes.size() == 14 && validateChecksum();
}

FanStatus LevelMessage::getFanStatus() const {
    if (valid()) {
        uint32_t command = 
            contentBytes.at(11) << 8 | 
            contentBytes.at(12);
        auto it = commandBytes.find(command);
        if (it != commandBytes.end()) {
            return it->second;
        }
    }
    return FanStatus::unknown;
}

void LevelMessage::setFanStatus(FanStatus targetStatus) {
    uint32_t commandByte = 0;
    for (const auto& kv : commandBytes) {
        if (kv.second == targetStatus) commandByte = kv.first;
    }
    contentBytes.at(11) = commandByte >> 8;
    contentBytes.at(12) = commandByte;
}