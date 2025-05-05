#ifndef HC12LIB_H
#define HC12LIB_H

#include <Arduino.h>

#include "DataPackerLib.h"
#include "config.h"

class HC12{
    public:
        HC12(uint8_t rx, uint8_t tx, uint8_t setPin);

        void sleep();
        void wake();

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
        uint8_t rx, tx;
};

#endif
