#pragma once
#include "constants.h"
#include <stdint.h>
#include <vector>

class Cc1101 {
public:
    Cc1101();
    uint8_t writeCommand(CommandStrobe command);
    void writeRegister(ConfigRegister reg, uint8_t data);
    uint8_t readRegister(StatusRegister reg);
    uint8_t readRegister(ConfigRegister reg);
    void writeBurstRegister(uint8_t reg, std::vector<uint8_t> data);
    void reset();
private:
    uint8_t readRegister(uint8_t reg, uint8_t type);
    void select(bool active);
    void waitMiso();
};