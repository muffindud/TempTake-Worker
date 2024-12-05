#include <config.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>
#include <driver/adc.h>

#define PARA 116.6020682
#define PARB 2.769034857
#define CORA .00035
#define CORB .02718
#define CORC 1.39538
#define CORD .0018
#define CORE -.003333333
#define CORF -.001923077
#define CORG 1.130128205
#define ATMOCO2 422.5
#define RLOAD_KOHM 10.0
#define ADC_RESOLUTION 4095.0

// TODO: Calibrate RZero
#define RZERO 23.4

Adafruit_BMP085 bmp;
DHT dht;

int delaySeconds = 2;

bool dht_active = false;
bool bmp_active = false;
bool mq_active = true;

String data = "";
int p = 0;

float getMqCorrRes(int pin, float t, float h){
    float res = ((ADC_RESOLUTION/(float)adc1_get_raw((adc1_channel_t)pin)) - 1.) * RLOAD_KOHM;
    float corrFact;
    if (t < 20){
        corrFact = CORA * t * t - CORB * t + CORC - (h - 33.) * CORD;
    }else{
        corrFact = CORE * t + CORF * h + CORG;
    }

    return res / corrFact;
}

float getMqPPM(float r){
    return PARA * pow((r / RZERO), -PARB);
}

float getRZero(float r){
    return r * pow((ATMOCO2 / PARA), (1. / PARB));
}

void setup(){
    Serial.begin(HC_12_BAUD_RATE);

    bmp_active = bmp.begin();

    delay(500);

    dht.setup(DHT_22_PIN);
    dht_active = dht.getModel() != DHT::AUTO_DETECT;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
}

void loop(){
    data = "";

    float temperature_c = dht.getTemperature();
    float humidity_perc = dht.getHumidity();
    float pressure_mmHg = bmp.readPressure() / MMHG_TO_PA;
    float mqRes_kOhm = getMqCorrRes(MQ_135_A_PIN, temperature_c, humidity_perc);
    float correctedRZero_kOhm = getRZero(mqRes_kOhm);
    float correctedPPM = getMqPPM(mqRes_kOhm);

    if(dht_active){
        data += "T  : " + String(temperature_c) + " C";
        data += "\n\r";
        data += "H  : " + String(humidity_perc) + " %";
        data += "\n\r";
    }else{
        data += "DHT22 NO";
        data += "\n\r";
    }

    if(bmp_active){
        data += "P  : " + String(pressure_mmHg) + "mmHg";
        data += "\n\r";
    }else{
        data += "BMP180 NO";
        data += "\n\r";
    }

    if(mq_active){
        data += "R  : " + String(mqRes_kOhm) + " kOhm";
        data += "\n\r";
        data += "R0 : " + String(correctedRZero_kOhm) + " kOhm";
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
