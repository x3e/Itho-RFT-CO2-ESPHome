#pragma once

#include <stdint.h>
#include <vector>

class Message;

class Parser {
public:
    enum class Status {
        INITIALIZED,
        INVALID,
        COMPLETED
    };
    Parser();

    Status parseBytes(uint8_t bytes1, uint8_t byte2);
    Message getMessage();
private:
    void failMessage();
    void endMessage();
    static uint8_t bitPairValue(uint8_t byte, unsigned int bitIndex);
    static int manchesterDecode(uint8_t b1, uint8_t b2);

    std::vector<uint8_t> parsedBytes;
    Status status;
};