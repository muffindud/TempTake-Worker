#ifndef HC12LIB_H
#define HC12LIB_H

#include <Arduino.h>

#include "DataPackerLib.h"
#include "config.h"

class HC12{
    public:
        HC12(uint8_t rx, uint8_t tx, uint8_t setPin);

        /**
         * Set the HC-12 module to sleep mode
         *
         * @param sleep `true` to set the module to sleep mode, `false` to wake it up
         */
        void setSleep(bool sleep);

        /**
         * Set the channel of the HC-12 module
         *
         * @param channel The channel of the module [1..127]
         */
        void setChannel(uint8_t channel);

        #ifdef MANAGER
        /**
         * Receive the data from the HC-12 module
         *
         * @returns the received data
         */
        PACKET_T receiveData();
        #endif

        #ifdef WORKER
        /**
         * Send a data packet with the HC-12 module
         *
         * @param packet the PACKET_T to send
         */
        void sendData(PACKET_T packet);
        #endif

        void serialBegin();
        void serialEnd();
    private:
        HardwareSerial *serial;
        int setPin;
        bool sleep = false;
        bool commandMode = false;
        uint8_t rx, tx;

        void setCommandMode(bool mode);
        void sendCommand(String command);
};

#endif
