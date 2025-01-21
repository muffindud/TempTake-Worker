#include "MQ135.h"

MQ135::MQ135(int pin, float rZero){
    this->pin = pin;
    this->rZero = rZero;
    pinMode(pin, INPUT);
}

float MQ135::getRes(float temperature_c, float humidity_perc){
    float res = ((ADC_RESOLUTION/(float)adc1_get_raw((adc1_channel_t)pin)) - 1.) * RLOAD_KOHM;
    float corrFact;
    if (temperature_c < 20){
        corrFact = CORA * temperature_c * temperature_c - CORB * temperature_c + CORC - (humidity_perc - 33.) * CORD;
    }else{
        corrFact = CORE * temperature_c + CORF * humidity_perc + CORG;
    }

    return res / corrFact;
}

float MQ135::getPPM(float temperature_c, float humidity_perc){
    return PARA * pow((this->getRes(temperature_c, humidity_perc) / this->rZero), -PARB);
}

float MQ135::getRZero(float temperature_c, float humidity_perc){
    return this->getRes(temperature_c, humidity_perc) * pow((ATMOCO2 / PARA), (1. / PARB));
}
