#include <config.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>

#include "MQ135.h"
#include "HC12.h"
#include "DataPacker.h"

// TODO: Calibrate RZero
#define RZERO 23.4

Adafruit_BMP085 bmp;
DHT dht;
MQ135 mq135(MQ_135_A_PIN, RZERO);
HC12 hc12(HC_12_RX_PIN, HC_12_TX_PIN, HC_12_SET_PIN);

int delaySeconds = 10;

int p = 0;

void setup(){
    bmp.begin();
    delay(500);
    dht.setup(DHT_22_PIN);
}

void loop(){
    float temperature_c = dht.getTemperature();
    float humidity_perc = dht.getHumidity();
    float pressure_mmHg = bmp.readPressure() / MMHG_TO_PA;
    float correctedPPM = mq135.getPPM(temperature_c, humidity_perc);

    DAT_T data_packet;
    data_packet.data.temperature = (uint64_t)(int)((temperature_c + 40.) * 100.0);
    data_packet.data.humidity = (uint64_t)(int)(humidity_perc * 100.0);
    data_packet.data.pressure = (uint64_t)(int)(pressure_mmHg * 100.0);
    data_packet.data.ppm = (uint64_t)(int)(correctedPPM * 100.0);

    data_packet.type = DAT_MODE;
    data_packet.meta.crc16 = crc16(data_packet.data);
    // TODO: Get the manager MAC address
    for(int i = 0; i < 6; i++){
        data_packet.meta.manager_mac[i] = 0x0;
    }
    // TODO: Get the worker MAC address
    for(int i = 0; i < 6; i++){
        data_packet.meta.worker_mac[i] = 0x0;
    }
    data_packet.meta.id = getId(data_packet.meta.manager_mac, data_packet.meta.worker_mac, data_packet.data, data_packet.meta.crc16);
    data_packet.meta.index_packet = 0;
    data_packet.meta.total_packet_s = 1;

    data_packet.length = sizeof(DAT_T) - META_SIZE;
    uint8_t* stream = (uint8_t*)malloc(DAT_SIZE);
    stream = (uint8_t*)&data_packet;
    hc12.sendData(stream);

    delay(delaySeconds * 1000);
}
