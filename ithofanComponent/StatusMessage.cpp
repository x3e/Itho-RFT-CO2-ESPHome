#include "StatusMessage.h"

#include <map>

const std::map<uint8_t, FanStatus> fanStatusBytes = {
    {0x18, FanStatus::automatic},
    {0x19, FanStatus::night},
    {0x01, FanStatus::low},
    {0x02, FanStatus::medium},
    {0x03, FanStatus::high},
    {0x0D, FanStatus::timer}
};

const std::map<uint8_t, FanWarning> fanWarningBytes = {
    {0x00, FanWarning::none},
    {0x80, FanWarning::fault},
    {0x40, FanWarning::filter},
    {0x20, FanWarning::defrost}
};

StatusMessage::StatusMessage(std::vector<uint8_t> data) : 
    Message(std::move(data)) {}

FanStatus StatusMessage::getFanStatus() const {
    if (valid()) {
        auto it = fanStatusBytes.find((contentBytes.at(28) & 0x1F));
        if (it != fanStatusBytes.end()) {
            return it->second;
        }
    }
    return FanStatus::unknown;
}

FanWarning StatusMessage::getFanWarning() const {
    if (valid()) {
        auto it = fanWarningBytes.find((contentBytes.at(28) & 0xE0));
        if (it != fanWarningBytes.end()) {
            return it->second;
        }
    }
    return FanWarning::unknown;
}

uint16_t StatusMessage::getRemainingTime() const {
    if (valid())
        return ((uint16_t)contentBytes.at(31) << 8 | contentBytes.at(32));
    else
        return 0;
}

uint16_t StatusMessage::StatusMessage::getCo2() const {
    if (valid())
        return ((uint16_t)contentBytes.at(13) << 8 | contentBytes.at(14));
    else
        return 0;
}

float StatusMessage::getExhaustTemp() const {
    if (valid())
        return (float)((uint16_t)contentBytes.at(17) << 8 | contentBytes.at(18)) / 100;
    else
        return 0;
}

float StatusMessage::getSupplyTemp() const {
    if (valid())
        return (float)((uint16_t)contentBytes.at(19) << 8 | contentBytes.at(20)) / 100;
    else
        return 0;
}

float StatusMessage::getIndoorTemp() const {
    if (valid())
        return (float)((uint16_t)contentBytes.at(21) << 8 | contentBytes.at(22)) / 100;
    else
        return 0;
}

float StatusMessage::getOutdoorTemp() const {
    if (valid())
        return (float)((uint16_t)contentBytes.at(23) << 8 | contentBytes.at(24)) / 100;
    else
        return 0;
}

float StatusMessage::getInletFlow() const {
    if (valid())
        return (float)((uint16_t)contentBytes.at(35) << 8 | contentBytes.at(36)) / 100;
    else
        return 0;
}
float StatusMessage::getExhaustFlow() const {
    if (valid())
        return (float)((uint16_t)contentBytes.at(37) << 8 | contentBytes.at(38)) / 100;
    else
        return 0;
}

bool StatusMessage::valid() const {
    return (contentBytes.size() == 40 || contentBytes.size() == 41) && validateChecksum();
}