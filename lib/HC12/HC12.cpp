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

void HC12::setChannel(u_int8_t channel){
    if(channel < 1 || channel > 127){
        return;
    }

    this->sendCommand("AT+C" + String(channel));
}

void HC12::sendData(String data){
    // TODO: Do once DataPacker is implemented
    // TODO: Check if data is too long
}

void HC12::setSleep(bool state){
    this->sleep = state;
    this->sendCommand(state ? "AT+SLEEP" : "AT");
}
