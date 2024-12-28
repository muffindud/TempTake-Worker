#ifndef HC12_H
#define HC12_H

#include <Arduino.h>
#include "DataPacker.h"

class HC12{
    private:
        int rxPin;
        int txPin;
        int setPin;
        bool commandMode = false;
        bool sleep = false;

        void setCommandMode(bool mode);
        void sendCommand(String command);
    public:
        HC12(int rxPin, int txPin, int setPin);

        /*
        * Set the channel of the HC-12 module
        *
        * The channel must be between 1 and 127
        *
        * @param channel: the channel to set the module to
        */
        void setChannel(u_int8_t channel);

        /*
        * Send data to the HC-12 module.
        *
        * The data will be automatically packed
        *
        * @param data: the data to send
        */
        void sendData(String data);

        /*
        * Set the sleep state of the HC-12 module
        *
        * On `true`, the module will 22uA and not be able to send/receive data
        *
        * On `false`, the module will consume 16mA and be able to send/receive data
        *
        * @param state: the state to set the module to
        */
        void setSleep(bool state);
};

#endif
