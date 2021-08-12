#pragma once

#include <vector>
#include <stdint.h>

class SoftwareSerial;
class Message;

class Sender {
    public:
        Sender(SoftwareSerial& serial);
        void send(Message message);
    private:
        SoftwareSerial& serial;
        void sendPreamble();
        void sendHeader();
        void sendFooter();
        void sendManchesterEncoded(std::vector<uint8_t> bytes);
        static uint8_t manchesterEncodeHalfByte(uint8_t halfByte);
};