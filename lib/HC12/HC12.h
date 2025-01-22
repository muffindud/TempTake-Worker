#ifndef HC12_H
#define HC12_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "DataPacker.h"

class HC12{
    private:
        int rxPin;
        int txPin;
        int setPin;
        bool commandMode = false;
        bool sleep = false;

        SoftwareSerial* serial;

        void setCommandMode(bool mode);
        void sendCommand(String command);
    public:
        HC12(int rxPin, int txPin, int setPin);

        /*
        * @brief Set the channel of the HC-12 module
        * @brief The channel must be between 1 and 127
        * @param channel: the channel to set the module to
        */
        void setChannel(uint8_t channel);

        /*
        * @brief Send data to the HC-12 module.
        * @brief The data will be automatically packed
        * @param data: the data to send
        */
        void sendData(uint8_t* data_stream);

        /*
        * @brief Set the sleep state of the HC-12 module
        * @brief On `true`, the module will 22uA and not be able to send/receive data
        * @brief On `false`, the module will consume 16mA and be able to send/receive data
        * @param state: the state to set the module to
        */
        void setSleep(bool state);

        /*
        * @brief Check if an ACK packet has been received
        * @param id: the ID of the packet to check for
        * @return bool: `true` if the ACK packet has been received, `false` otherwise
        */
        bool ackReceived(uint16_t id);
};

#endif
