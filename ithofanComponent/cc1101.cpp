#include "cc1101.h"
#include <SPI.h>

Cc1101::Cc1101() {
    SPI.begin();
    #ifdef ESP8266
        pinMode(SS, OUTPUT);
    #endif
}

uint8_t Cc1101::writeCommand(CommandStrobe command) {
    select(true);
    waitMiso();
    uint8_t result = SPI.transfer(static_cast<uint8_t>(command));
    select(false);
    return result;
}

void Cc1101::writeRegister(ConfigRegister reg, uint8_t data) {
    select(true);
    waitMiso();
    SPI.transfer(static_cast<uint8_t>(reg));
    SPI.transfer(data);
    select(false);
}

uint8_t Cc1101::readRegister(ConfigRegister reg) {
    return readRegister(static_cast<uint8_t>(reg), CC1101_CONFIG_REGISTER);
}

uint8_t Cc1101::readRegister(StatusRegister reg) {
    return readRegister(static_cast<uint8_t>(reg), CC1101_STATUS_REGISTER);
}

uint8_t Cc1101::readRegister(uint8_t reg, uint8_t type) {
    select(true);
    waitMiso();
    SPI.transfer(static_cast<uint8_t>(reg) | type);
    uint8_t value = SPI.transfer(0);
    select(false);
    return value;
}

void Cc1101::writeBurstRegister(uint8_t reg, std::vector<uint8_t> data) {
    select(true);
    waitMiso();
    SPI.transfer(reg | CC1101_WRITE_BURST);
    for (uint8_t byte : data) {
        SPI.transfer(byte);
    }
    select(false);
}

void Cc1101::select(bool active) {
    digitalWrite(SS, !active);
}

void Cc1101::waitMiso() {
    while(digitalRead(MISO) == HIGH) yield();
}

void Cc1101::reset() {
    select(false);
	delayMicroseconds(5);
	select(true);
	delayMicroseconds(10);
	select(false);
	delayMicroseconds(45);

	select(true);
    waitMiso();
    SPI.transfer(static_cast<uint8_t>(CommandStrobe::sres));
    delay(10);
    waitMiso();
    select(false);
}