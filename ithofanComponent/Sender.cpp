#include "Sender.h"

#include "Message.h"
#include <SoftwareSerial.h>

const uint8_t PREAMBLE[] {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0xFF};
const uint8_t HEADER[] {0x00,0x33,0x55,0x53};
const uint8_t FOOTER[] {0x35,0x55,0x00};

const uint8_t MANCHESTER_ZERO = 0b10;
const uint8_t MANCHESTER_ONE = 0b01;

Sender::Sender(SoftwareSerial& serial) : serial(serial) {}

void Sender::send(Message message) {
    sendPreamble();
    sendHeader();
    sendManchesterEncoded(message.getBytes());
    sendFooter();
}

void Sender::sendPreamble() {
    for (auto byte : PREAMBLE) serial.write(byte);
}

void Sender::sendHeader() {
    for (auto byte : HEADER) serial.write(byte);
}

void Sender::sendFooter() {
    for (auto byte : FOOTER) serial.write(byte);
}

void Sender::sendManchesterEncoded(std::vector<uint8_t> bytes) {
    for (auto byte : bytes) {
        serial.write(manchesterEncodeHalfByte(byte >> 4));
        serial.write(manchesterEncodeHalfByte(byte));
    }
}

uint8_t Sender::manchesterEncodeHalfByte(uint8_t halfByte) {
    uint8_t encodedByte = 0;
    for (int i=0; i<4; i++) {
        if (halfByte & (0x01 << i))
            encodedByte |= MANCHESTER_ONE << (i*2);
        else
            encodedByte |= MANCHESTER_ZERO << (i*2);
    }
    return encodedByte;
}