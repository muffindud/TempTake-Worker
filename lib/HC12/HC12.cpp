#include "HC12.h"


HC12::HC12(int rxPin, int txPin, int setPin){
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->setPin = setPin;

    this->setCommandMode(true);
    this->sendCommand("AT+B1200");
    this->sendCommand("AT+P8");
    this->sendCommand("AT+FU3");
    this->sendCommand("AT+U8N1");
    this->setCommandMode(false);

    Serial.begin(1200);
}

void HC12::setCommandMode(bool mode){
    digitalWrite(this->setPin, mode ? HIGH : LOW);
    this->commandMode = mode;
    delay(mode ? 40 : 80);
}

void HC12::sendCommand(String command){
    if (this->commandMode){
        Serial.println(command);
    }else{
        this->setCommandMode(true);
        Serial.println(command);
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
    uint8_t* data = (uint8_t*)malloc(64);
    for(int i = 0; i < 64; i++){
        data[i] = i < 64 ? data_stream[i] : 0;
    }

    Serial.write(data, 64);
}

bool HC12::ackReceived(uint16_t id){
    uint8_t* ack_stream = (uint8_t*)malloc(64);
    int incoming = 0;

    while(incoming < 100){
        incoming++;

        if(Serial.available() >= 64){
            Serial.readBytes(ack_stream, 64);
            ACK_T ack = streamToAck(ack_stream);
            free(ack_stream);
            if(ack.meta.id == id){
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
