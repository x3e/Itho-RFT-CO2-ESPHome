#include "IthoController.h"

#include <Arduino.h>

#include "Message.h"
#include "Sender.h"
#include "Parser.h"
#include "StatusMessage.h"
#include "TimerMessage.h"
#include "LevelMessage.h"

const uint8_t HEADER_BYTES[] = {0x00, 0x33, 0x55, 0x53};
const unsigned int NUMBER_OF_TRIES = 3;
const unsigned int WAIT_TIME = 2000; //ms

IthoController::IthoController(int8_t radioIo0Pin, uint8_t firstByte, uint32_t fanAddress, uint32_t remoteAddress) : 
    radioIo0Pin(radioIo0Pin),
    firstByte(firstByte),
    fanAddress(fanAddress),
    remoteAddress(remoteAddress) {}

void IthoController::init() {
    radio.reset();
    radio.writeRegister(ConfigRegister::freq2, 0x21);
    radio.writeRegister(ConfigRegister::freq1, 0x65);
    radio.writeRegister(ConfigRegister::freq0, 0x6A);
    radio.writeRegister(ConfigRegister::iocfg0, 0x0D);
    radio.writeRegister(ConfigRegister::iocfg2, 0x0A);
    radio.writeRegister(ConfigRegister::fsctrl1, 0x0B);
    radio.writeRegister(ConfigRegister::fsctrl0, 0x00);
    radio.writeRegister(ConfigRegister::mdmcfg4, 0x5A);
    radio.writeRegister(ConfigRegister::mdmcfg3, 0x83);
    radio.writeRegister(ConfigRegister::mdmcfg2, 0x00);
    radio.writeRegister(ConfigRegister::mdmcfg1, 0x22);
    radio.writeRegister(ConfigRegister::mdmcfg0, 0xF8);
    radio.writeRegister(ConfigRegister::channr, 0x00);
    radio.writeRegister(ConfigRegister::deviatn, 0x50);
    radio.writeRegister(ConfigRegister::frend1, 0xB6);
    radio.writeRegister(ConfigRegister::frend0, 0x17);
    radio.writeRegister(ConfigRegister::mcsm0, 0x18);
    radio.writeRegister(ConfigRegister::foccfg, 0x1D);
    radio.writeRegister(ConfigRegister::bscfg, 0x1C);
    radio.writeRegister(ConfigRegister::agcctrl2, 0xC7);
    radio.writeRegister(ConfigRegister::agcctrl1, 0x00);
    radio.writeRegister(ConfigRegister::agcctrl0, 0xB2);
    radio.writeRegister(ConfigRegister::worctrl, 0xFB);
    radio.writeRegister(ConfigRegister::fscal3, 0xE9);
    radio.writeRegister(ConfigRegister::fscal2, 0x2A);
    radio.writeRegister(ConfigRegister::fscal1, 0x00);
    radio.writeRegister(ConfigRegister::fscal0, 0x1F);
    radio.writeRegister(ConfigRegister::test0, 0x09);
    radio.writeRegister(ConfigRegister::pktctrl1, 0x04);
    radio.writeRegister(ConfigRegister::pktctrl0, 0x32);
    radio.writeBurstRegister(CC1101_PATABLE, {0x6F, 0x26, 0x2E, 0x7F, 0x8A, 0x84, 0xCA, 0xC4});
    radio.writeCommand(CommandStrobe::srx);

    radioSerial.begin(38400, SWSERIAL_8N1, radioIo0Pin, radioIo0Pin, false, 128);
}

void IthoController::addChangedCallback(std::function<void(FanStatus,uint8_t)> callback) {
    callbacks.push_back(callback);
}

FanStatus IthoController::getFanStatus() const { return fanStatus; }

bool IthoController::setFanStatus(FanStatus newStatus) {
    if (newStatus == FanStatus::unknown || newStatus == FanStatus::timer) return false;
    for (unsigned int tries=0; tries<NUMBER_OF_TRIES && newStatus != fanStatus; tries++) {
        sendMessage(LevelMessage(firstByte, remoteAddress, fanAddress, newStatus));
        unsigned long sendTime = millis();
        while (newStatus != fanStatus && millis() - sendTime < WAIT_TIME) {
            listen();
            yield();
        }
    }
    return newStatus == fanStatus;
}

uint8_t IthoController::getTimer() const { return timer; }

bool IthoController::setTimer(uint8_t newTimer) {
    if (newTimer == 0) return false;
    for (unsigned int tries=0; tries<NUMBER_OF_TRIES && newTimer != timer; tries++) {
        sendMessage(TimerMessage(firstByte, remoteAddress, fanAddress, newTimer));
        unsigned long sendTime = millis();
        while (newTimer != timer && millis() - sendTime < WAIT_TIME) {
            listen();
            yield();
        }
    }
    return timer == newTimer;
}

void IthoController::listen() {
    while (radioSerial.available()) {
        if (radioSerial.read() == HEADER_BYTES[headerIndex]) {
            headerIndex++;
            if (headerIndex == sizeof(HEADER_BYTES)) {
                parseMessage();
                headerIndex = 0;
            }
        } else {
            headerIndex = 0;
        }
    }
}

void IthoController::receivingMode(bool receiving) {
  radio.writeRegister(ConfigRegister::iocfg0, receiving? 0x0D : 0x2E);
  radio.writeCommand(receiving? CommandStrobe::srx : CommandStrobe::stx);
  radioSerial.enableTx(!receiving);
}

void IthoController::parseMessage() {
  Parser parser;
  Parser::Status status;
  do {
    while(radioSerial.available() < 2) yield();
    status = parser.parseBytes(radioSerial.read(), radioSerial.read());
  } while(status == Parser::Status::INITIALIZED);
  if (status == Parser::Status::COMPLETED) {
    Message message = parser.getMessage();
    if (message.getSenderAddress() == fanAddress && message.getType() == Message::Type::STATUS)
        handleStatusMessage(message.getAs<StatusMessage>());
  }
}

void IthoController::handleStatusMessage(const StatusMessage& message) {
    if (message.valid()) {
        uint8_t messageTimer = message.getRemainingTime();
        FanStatus messageStatus = message.getFanStatus();
        if (messageTimer != timer || messageStatus != fanStatus) {
            fanStatus = message.getFanStatus();
            timer = message.getRemainingTime();
            changed();
        }
    }
}

void IthoController::sendMessage(const Message& message) {
  receivingMode(false);
  
  Sender sender {radioSerial};
  sender.send(message);

  receivingMode(true);
}

void IthoController::changed() {
    for (auto callback : callbacks) {
        callback(fanStatus, timer);
    }
}