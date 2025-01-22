#include <config.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>

#include "MQ135.h"
// #include "HC12.h"
// #include "DataPacker.h"

// TODO: Calibrate RZero
#define RZERO 23.4

Adafruit_BMP085 bmp;
DHT dht;
MQ135 mq135(MQ_135_A_PIN, RZERO);

uint8_t* data_stream = nullptr;

int delaySeconds = 2;

int p = 0;

void setup(){
    Serial.begin(HC_12_BAUD_RATE);

    bmp.begin();
    delay(500);
    dht.setup(DHT_22_PIN);
    // mq135.configureADC();
}

void loop(){
    float temperature_c = dht.getTemperature();
    float humidity_perc = dht.getHumidity();
    float pressure_mmHg = bmp.readPressure() / MMHG_TO_PA;
    float mqRes_kOhm = mq135.getRes(temperature_c, humidity_perc);
    float correctedRZero_kOhm = mq135.getRZero(temperature_c, humidity_perc);
    float correctedPPM = mq135.getPPM(temperature_c, humidity_perc);

    p = (p + 1) % 10000;

    data_stream = (uint8_t*)malloc(8 * 8);
    if (!data_stream) {
        return;
    }

    uint64_t* buffer = (uint64_t*)(data_stream);

    buffer[0] = (uint64_t)(temperature_c * 100.0);
    buffer[1] = (uint64_t)(humidity_perc * 100.0);
    buffer[2] = (uint64_t)(pressure_mmHg * 100.0);
    buffer[3] = (uint64_t)(mqRes_kOhm * 100.0);
    buffer[4] = (uint64_t)(correctedRZero_kOhm * 100.0);
    buffer[5] = (uint64_t)(correctedPPM * 100.0);
    buffer[6] = (uint64_t)p;
    buffer[7] = (uint64_t)0;

    for (int i = 0; i < 8; i++) {
        Serial.write((uint8_t*)&buffer[i], 8);
    }
    free(data_stream);

    delay(delaySeconds * 1000);
}
