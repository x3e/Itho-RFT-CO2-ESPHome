#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <SoftwareSerial.h>

class Message {
    public:
        enum class Type {
            UNKNOWN,
            LEVEL,
            TIMER,
            STATUS
        };
        Message(std::vector<uint8_t> messageData);
        Message(uint8_t firstByte, uint32_t sender, uint32_t receiver, Type type, uint8_t length);

        uint32_t getSenderAddress() const;
        uint32_t getReceiverAddress() const;
        Type getType() const;
        bool validateChecksum() const;

        std::string getString() const;
        const std::vector<uint8_t>& getBytes() const;

        template<typename T>
        T getAs() {
            return T(std::move(contentBytes));
        };

    protected:
        void generateChecksum();

        std::vector<uint8_t> contentBytes;
    
    private:
        void setSenderAddress(uint32_t address);
        void setReceiverAddress(uint32_t address);
        void setType(Type type);

        uint32_t getAddress(unsigned int startByte) const;
        void setAddress(unsigned int startByte, uint32_t address);
        std::string getByteString(const unsigned int startIndex, const unsigned int length) const;
};