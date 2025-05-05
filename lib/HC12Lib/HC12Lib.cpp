#include "HC12Lib.h"

HC12::HC12(uint8_t rx, uint8_t tx, uint8_t setPin){
    serial = &Serial1;
    this->setPin = setPin;
    this->rx = rx;
    this->tx = tx;
    pinMode(setPin, OUTPUT);
}

void HC12::serialBegin(){
    this->serial->begin(1200, SERIAL_8N1, this->rx, this->tx);
    delay(100);
}

void HC12::serialEnd(){
    this->serial->end();
}

#ifdef MANAGER
PACKET_T HC12::receiveData(){
    if(this->serial->available()){
        uint8_t* dataStream = (uint8_t*)malloc(PACKET_SIZE);
        this->serial->readBytes(dataStream, PACKET_SIZE);
        PACKET_T packet;
        memcpy(&packet, dataStream, PACKET_SIZE);
        free(dataStream);
        return packet;
    }

    return PACKET_T();
}
#endif

#ifdef WORKER
void HC12::sendData(PACKET_T packet){
    uint8_t stream[PACKET_SIZE];
    memcpy(stream, &packet, PACKET_SIZE);
    this->serial->write(stream, PACKET_SIZE);
}
#endif

void HC12::sleep(){
    delay(50);
    digitalWrite(this->setPin, LOW);
    delay(50);
    this->serial->println("AT+SLEEP");
    delay(50);
    digitalWrite(this->setPin, HIGH);
}

void HC12::wake(){
    digitalWrite(this->setPin, LOW);
    delay(50);
    digitalWrite(this->setPin, HIGH);
    delay(50);
}
