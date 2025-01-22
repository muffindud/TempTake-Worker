#include "MQ135.h"

MQ135::MQ135(int pin, float rZero){
    this->pin = pin;
    this->rZero = rZero;

    pinMode(this->pin, INPUT);
}

// void MQ135::configureADC(){
//     // Analog read for the ESP32_C3 module
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
// }

float MQ135::getRes(float temperature_c, float humidity_perc){
    // float res = ((ADC_RESOLUTION/(float)adc1_get_raw((adc1_channel_t)this->pin)) - 1.) * RLOAD_KOHM;
    float res = (ADC_RESOLUTION / (float)analogRead(this->pin) - 1.) * RLOAD_KOHM;
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
