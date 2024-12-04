#include <config.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>
#include <MQ135.h>
#include <driver/adc.h>

Adafruit_BMP085 bmp;
DHT dht;
MQ135 mq(MQ_135_A_PIN);

#define ATMOCO2 415.58 // Global CO2 Aug 2022
#define RZERO 76.63
float R0;

int delaySeconds = 2;

bool dht_active = false;
bool bmp_active = false;
bool mq_active = true;

String data = "";
int p = 0;

float getMqCorrRes(int pin, float t, float h){
    float res = ((4095./(float)adc1_get_raw((adc1_channel_t)pin)) - 1.) * 10;
    float corrFact;
    if (t < 20){
        corrFact = 0.00035 * t * t - 0.02718 * t + 1.39538 - (h - 33.) * 0.0018;
    }else{
        corrFact = -0.003333333 * t + -0.001923077 * h + 1.130128205;
    }

    return res / corrFact;
}

float getMqPPM(float r, float t, float h, float rZero = RZERO){
    float ppm = 116.6020682 * pow((r / rZero), -2.769034857);
    return ppm;
}

float getRZero(float r, float t, float h){
    float rZero = r * pow((ATMOCO2 / 415.58), (1 / -2.769034857));
    return rZero;
}

void setup(){
    Serial.begin(HC_12_BAUD_RATE);

    bmp_active = bmp.begin();

    delay(500);

    dht.setup(DHT_22_PIN);
    dht_active = dht.getModel() != DHT::AUTO_DETECT;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
    // pinMode(MQ_135_A_PIN, INPUT);
}

void loop(){
    // if(Serial.available()){
    //     HC12.write(Serial.read());
    // }
    // if(HC12.available()){
    //     Serial.write(HC12.read());
    // }

    data = "";

    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
    // float correctedRZero = 0.0;
    // float correctedPPM = 0.0;
    // float correctedRZero = mq.getCorrectedRZero(temperature, humidity);
    // float correctedPPM = mq.getCorrectedPPM(temperature, humidity);
    float mqRes = getMqCorrRes(MQ_135_A_PIN, temperature, humidity);
    float correctedRZero = getRZero(mqRes, temperature, humidity);
    float correctedPPM = getMqPPM(mqRes, temperature, humidity, correctedRZero);

    if(dht_active){
        data += "T  : " + String(temperature) + " C";
        data += "\n\r";
        data += "H  : " + String(humidity) + " %";
        data += "\n\r";
    }else{
        data += "DHT22 NO";
        data += "\n\r";
    }

    if(bmp_active){
        data += "P  : " + String(bmp.readPressure() / MMHG_TO_PA) + "mmHg";
        data += "\n\r";
    }else{
        data += "BMP180 NO";
        data += "\n\r";
    }

    if(mq_active){
        data += "R0 : " + String(correctedRZero);
        data += "\n\r";
        data += "PPM: " + String(correctedPPM);
        data += "\n\r";
        data += "\n\r";
    }else{
        data += "MQ135 NO";
        data += "\n\r";
        data += "\n\r";
    }

    data += String(p);
    p = (p + 1) % 10000;

    Serial.println(data);

    delay(delaySeconds * 1000);
}
