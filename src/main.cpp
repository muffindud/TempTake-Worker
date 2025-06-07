#include <Arduino.h>
// #include <Adafruit_BMP3XX.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include <ScioSense_ENS16x.h>
#include <Wire.h>

#include "HC12Lib.h"
#include "DataPackerLib.h"
#include "ManagerPairLib.h"

#include <config.h>

// Adafruit_BMP3XX bmp;
Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
ENS160 ens160;

HC12 hc12(HC_12_RX_PIN, HC_12_TX_PIN, HC_12_SET_PIN);

uint8_t* managerMac;
uint8_t* workerMac;

bool sensorsInitialized = false;

int delaySeconds = 30;

void awakeModules(){
    Wire.begin();
    delay(1000);
    // bmp.begin_I2C();
    bmp.begin(BMP_390_ADDR);
    aht.begin();
    ens160.begin(&Wire, ENS160_ADDR);
    ens160.startStandardMeasure();
    hc12.wake();
    delay(1000);
}

void sleepModules(){
    delay(1000);
    ens160.setOperatingMode(ENS16X_OPERATING_MODE_DEEP_SLEEP);
    hc12.sleep();
}

void setup(){
    hc12.serialBegin();

    managerMac = getManagerMac();
    workerMac = getWorkerMac();

    delay(500);
}

void loop(){
    if(isPairingMode()){
        // This tears the existing Wire connection, so make sure to reinitialize
        // any connections
        exchangeManagerCreds();
        managerMac = getManagerMac();
        sensorsInitialized = false;
    }else{
        awakeModules();

        ens160.wait();
        sensors_event_t tempEvent, humidityEvent;
        aht.getEvent(&humidityEvent, &tempEvent);
        // bmp.performReading();
        ens160.writeCompensation(tempEvent.temperature, humidityEvent.relative_humidity);
        ens160.update();

        PACKET_T packet;

        packet.type = DAT_MODE;

        packet.data.temperature = (uint64_t)((tempEvent.temperature + 40.) * 100.);
        packet.data.humidity    = (uint64_t)(humidityEvent.relative_humidity * 100.);
        // packet.data.pressure    = (uint64_t)((bmp.pressure / MMHG_TO_PA) * 100.);
        packet.data.pressure    = (uint64_t)((bmp.readPressure() / MMHG_TO_PA) * 100.);
        packet.data.ppm         = (uint64_t)(ens160.getEco2() * 100.);

        packet.meta.crc16 = getCRC16(packet.data);
        memcpy(packet.meta.worker_mac, workerMac, 6);
        memcpy(packet.meta.manager_mac, managerMac, 6);
        packet.meta.packet_id = getId(
            packet.meta.manager_mac,
            packet.meta.worker_mac,
            packet.data,
            packet.meta.crc16
        );
        packet.meta.index_packet = 0;
        packet.meta.total_packet = 1;

        packet.length = sizeof(DATA_T);

        hc12.sendData(packet);

        sleepModules();
        ESP.deepSleep(delaySeconds * 1000000);
    }
}
