#include "HC12.h"


HC12::HC12(int rxPin, int txPin, int setPin): serial(new SoftwareSerial(rxPin, txPin)){
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->setPin = setPin;

    serial->begin(1200);

    // this->setCommandMode(true);
    // this->sendCommand("AT+B1200");
    // this->sendCommand("AT+P8");
    // this->sendCommand("AT+FU3");
    // this->sendCommand("AT+U8N1");
    // this->setCommandMode(false);

    // Serial.end();
    // serial->end();
    // Serial.begin(1200);
    // serial->begin(1200);
}

void HC12::setCommandMode(bool mode){
    digitalWrite(this->setPin, mode ? HIGH : LOW);
    this->commandMode = mode;
    delay(100);
}

void HC12::sendCommand(String command){
    if (this->commandMode){
        serial->println(command);
        delay(100);
    }else{
        this->setCommandMode(true);
        serial->println(command);
        this->setCommandMode(false);
    }
}

void HC12::setChannel(uint8_t channel){
    if(channel < 1 || channel > 127){
        return;
    }

    this->sendCommand("AT+C" + String(channel));
}

void HC12::sendData(uint8_t* data_stream){
    serial->write(data_stream, DAT_SIZE);
}

bool HC12::ackReceived(uint16_t id){
    uint8_t* ack_stream = (uint8_t*)malloc(ACK_SIZE);
    int incoming = 0;

    while(incoming < 100){
        incoming++;

        if(serial->available() == ACK_SIZE){
            serial->readBytes(ack_stream, ACK_SIZE);
            ACK_T ack;
            memcpy(&ack, ack_stream, ACK_SIZE);
            if(ack.meta.id == id){
                free(ack_stream);
                return true;
            }
        }
    }

    free(ack_stream);
    return false;
}

void HC12::setSleep(bool state){
    this->sleep = state;
    this->sendCommand(state ? "AT+SLEEP" : "AT");
}
