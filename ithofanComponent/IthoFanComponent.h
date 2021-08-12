#include "esphome.h"

#include "FanStatus.h"
#include "IthoController.h"

class IthoFan;
class IthoSwitch;

IthoFan* fanPtr;

class IthoFan : public Component {
    public:
        IthoFan(int8_t radioIo0Pin, uint8_t firstByte, uint32_t fanAddress, uint32_t remoteAddress) :
            fanControl(radioIo0Pin, firstByte, fanAddress, remoteAddress) {}
        float get_setup_priority() const override {
            return esphome::setup_priority::DATA;
        }
        void setup() override {
            fanPtr = this;
            fanControl.init();
            fanControl.addChangedCallback([=](FanStatus newStatus, uint8_t timer){
                id(timer_number).publish_state(timer);
            });
        }
        void loop() override {
            fanControl.listen();
        }
        void setStatus(FanStatus status) {
            bool result = fanControl.setFanStatus(status);
        }
        void setTimer(float timer) {
            fanControl.setTimer(static_cast<uint8_t>(timer));
        }
        void registerCallback(std::function<void(FanStatus,uint8_t)> callback) {
            fanControl.addChangedCallback(callback);
        }
    private:
        IthoController fanControl;
};

class IthoSwitch : public Component, public Switch {
    public:
        IthoSwitch(FanStatus status) : status(status) {}

        float get_setup_priority() const override {
            return esphome::setup_priority::AFTER_CONNECTION;
        }

        void setup() override {
            fanPtr->registerCallback([=](FanStatus newStatus, uint8_t timer){
                fanStatusChanged(newStatus);
            });
        }

        void write_state(bool state) override {
            if (state)
                fanPtr->setStatus(status);
        }

        void fanStatusChanged(FanStatus newStatus) {
            publish_state(newStatus == status);
        }
    private:
        FanStatus status;
};