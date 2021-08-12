#include "Parser.h"
#include "Message.h"

const uint8_t INVALID_ENCODING = 0xFF;
const unsigned int MAX_MESSAGE_LENGTH = 100;

Parser::Parser() : status(Status::INITIALIZED) {}

Parser::Status Parser::parseBytes(uint8_t byte1, uint8_t byte2) {
    if (byte1 == 0x35 && byte2 == 0x55) endMessage();
    else if (parsedBytes.size() >= MAX_MESSAGE_LENGTH) failMessage();
    else {
        int combined = manchesterDecode(byte1, byte2);
        if (combined >= 0) {
            parsedBytes.push_back(combined);
        } else {
            failMessage();
        }
    }
    return status;
}

Message Parser::getMessage() {
    status = Status::INITIALIZED;
    Message message {std::move(parsedBytes)};
    return message;
}

void Parser::failMessage() {
    status = Status::INVALID;
}

void Parser::endMessage() {
    if (status == Status::INITIALIZED) {
        if (parsedBytes.size() >= 9) {
            status = Status::COMPLETED;
        } else {
            status = Status::INVALID;
        }
    }
}

uint8_t Parser::bitPairValue(uint8_t byte, unsigned int bitIndex) {
  uint8_t bitPair = (byte >> bitIndex) & 0b11;
  if (bitPair == 0b01) return 1;
  if (bitPair == 0b10) return 0;
  else return INVALID_ENCODING;
}

int Parser::manchesterDecode(uint8_t b1, uint8_t b2) {
  uint8_t result = 0;
  for (int i=0; i<4; i++) {
    uint8_t bit1 = bitPairValue(b1, i*2);
    uint8_t bit2 = bitPairValue(b2, i*2);
    if (bit1 == INVALID_ENCODING || bit2 == INVALID_ENCODING) return -1;
    result |= bit1 << (i + 4);
    result |= bit2 << i;
  }
  return result;
}