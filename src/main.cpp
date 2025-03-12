#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include <ScioSense_ENS16x.h>
#include <Wire.h>

#include "HC12.h"
#include "DataPacker.h"
#include "ManagerPair.h"

#include <config.h>

Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
ENS160 ens160;

HC12 hc12(HC_12_RX_PIN, HC_12_TX_PIN, HC_12_SET_PIN);
MAC_ADDRESS_T worker_mac = {
    .mac = {0x55, 0x0, 0x0, 0x0, 0x0, 0x1}
};
MAC_ADDRESS_T manager_mac;

int delaySeconds = 3;
int p = 0;

void setup(){
    Wire.begin();
    bmp.begin();
    aht.begin();
    ens160.begin(&Wire, ENS160_ADDR);

    pinMode(PAIR_BUTTON_PIN, INPUT_PULLUP);
    manager_mac = getManagerMac();

    ens160.startStandardMeasure();

    delay(500);
}

void loop(){
    // if(digitalRead(PAIR_BUTTON_PIN) == LOW){
    //     pairManager(worker_mac);
    //     manager_mac = getManagerMac();
    // }

    ens160.wait();
    sensors_event_t temp_event, humidity_event;
    aht.getEvent(&humidity_event, &temp_event);
    ens160.writeCompensation(temp_event.temperature, humidity_event.relative_humidity);
    ens160.update();

    float temperature_c = temp_event.temperature;
    float humidity_perc = humidity_event.relative_humidity;
    float pressure_mmHg = bmp.readPressure() / MMHG_TO_PA;
    float correctedPPM = ens160.getEco2();


    DAT_T data_packet;
    RAW_DATA_T data;
    data.temperature = (uint64_t)((temperature_c + 40.) * 100.);
    data.humidity = (uint64_t)(humidity_perc * 100.);
    data.pressure = (uint64_t)(pressure_mmHg * 100.);
    data.ppm = (uint64_t)(correctedPPM * 100.);

    memcpy(data_packet.data, &data, RAW_DATA_SIZE);

    data_packet.type = DAT_MODE;
    data_packet.meta.crc16 = crc16(data);
    for(int i = 0; i < 6; i++){
        data_packet.meta.manager_mac[i] = manager_mac.mac[i];
    }
    for(int i = 0; i < 6; i++){
        data_packet.meta.worker_mac[i] = worker_mac.mac[i];
    }
    data_packet.meta.id = getId(data_packet.meta.manager_mac, data_packet.meta.worker_mac, data, data_packet.meta.crc16);
    data_packet.meta.index_packet = 0;
    data_packet.meta.total_packet_s = 1;

    data_packet.length = sizeof(DAT_T) - META_SIZE;
    uint8_t* stream = (uint8_t*)malloc(DAT_SIZE);
    stream = (uint8_t*)&data_packet;
    hc12.sendData(stream);

    delay(delaySeconds * 1000);
}
