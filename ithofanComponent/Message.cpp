#include "Message.h"

#include <map>
#include <sstream>
#include <iomanip>

const std::map<uint16_t, Message::Type> typeMap = {
    {0x22F1, Message::Type::LEVEL},
    {0x22F3, Message::Type::TIMER},
    {0x31DA, Message::Type::STATUS}
};

const unsigned int SENDER_ADDRESS = 1;   //3 bytes long
const unsigned int RECEIVER_ADDRESS = 4; //3 bytes long
const unsigned int MESSAGE_TYPE = 7;     //2 bytes long

Message::Message(std::vector<uint8_t> messageBytes) : 
    contentBytes(std::move(messageBytes)) {}

Message::Message(uint8_t firstByte, uint32_t sender, uint32_t receiver, Message::Type type, uint8_t length) {
    contentBytes.push_back(firstByte);
    for (int i=0; i<8; i++)
        contentBytes.push_back(0x00);
    setSenderAddress(sender);
    setReceiverAddress(receiver);
    setType(type);
    contentBytes.push_back(length);
    contentBytes.push_back(0x00);
    for (int i=0; i<length; i++)
        contentBytes.push_back(0x00);
}

uint32_t Message::getSenderAddress() const {
    return getAddress(SENDER_ADDRESS);
}

void Message::setSenderAddress(uint32_t address) {
    setAddress(SENDER_ADDRESS, address);
}

uint32_t Message::getReceiverAddress() const {
    return getAddress(RECEIVER_ADDRESS);
}

void Message::setReceiverAddress(uint32_t address) {
    setAddress(RECEIVER_ADDRESS, address);
}

Message::Type Message::getType() const {
    uint16_t typeBytes = contentBytes.at(MESSAGE_TYPE + 1);
    typeBytes |= static_cast<uint16_t>(contentBytes.at(MESSAGE_TYPE)) << 8;

    auto it = typeMap.find(typeBytes);
    if (it != typeMap.end()) {
        return it->second;
    }
    return Type::UNKNOWN;
}

void Message::setType(Message::Type newType) {
    for (const auto& kv : typeMap) {
        if (newType == kv.second) {
            contentBytes.at(MESSAGE_TYPE) = kv.first >> 8;
            contentBytes.at(MESSAGE_TYPE + 1) = kv.first;
            break;
        }
    }
}

bool Message::validateChecksum() const {
    uint8_t sum = 0;
    for (uint8_t byte : contentBytes)
        sum+=byte;
    return sum == 0;
}

void Message::generateChecksum() {
    uint8_t sum = 0;
    for(auto it = contentBytes.begin(); it != contentBytes.end() - 1; it++) {
        sum += *it;
    }
    contentBytes.at(contentBytes.size()-1) = 0-sum;
}

std::string Message::getString() const {
    return getByteString(0, contentBytes.size());
}

const std::vector<uint8_t>& Message::getBytes() const {
    return contentBytes;
}

uint32_t Message::getAddress(unsigned int startByte) const {
    uint32_t result = 0;
    for (int i=0; i<3; i++) {
        result |= static_cast<uint32_t>(contentBytes.at(startByte+i)) << (16-i*8);
    }
    return result;
}

void Message::setAddress(unsigned int startByte, uint32_t address) {
    for (int i=0; i<3; i++) {
        contentBytes.at(startByte + i) = address >> (16 - i * 8);
    }
}

std::string Message::getByteString(const unsigned int start, const unsigned int length) const {
    std::stringstream stream;
    if (start + length > contentBytes.size()) return "";
    for (unsigned int i=start; i<start + length; i++) {
        int byte = contentBytes.at(i);
        if (i!=start) stream << ':';
        stream << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << byte;
    }
    return stream.str();
}