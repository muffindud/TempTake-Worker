#include <config.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>

Adafruit_BMP085 bmp;
DHT dht;

int delaySeconds = 2;

bool dht_active = false;
bool bmp_active = false;
bool mq_active = false;

String data = "";
int p = 0;

void setup(){
    Serial.begin(HC_12_BAUD_RATE);

    bmp_active = bmp.begin();

    delay(500);

    dht.setup(DHT_22_PIN);
    dht_active = dht.getModel() != DHT::AUTO_DETECT;
}

void loop(){
    // if(Serial.available()){
    //     HC12.write(Serial.read());
    // }
    // if(HC12.available()){
    //     Serial.write(HC12.read());
    // }

    data = "";

    if(dht_active){
        data += "T: " + String(dht.getTemperature()) + "C";
        data += "\n\r";
        data += "H: " + String(dht.getHumidity()) + "%";
        data += "\n\r";
        data += "\n\r";
    }else{
        data += "DHT22 NO";
        data += "\n\r";
        data += "\n\r";
    }

    if(bmp_active){
        data += "P: " + String(bmp.readPressure() / MMHG_TO_PA) + "mmHg";
        data += "\n\r";
        data += "T: " + String(bmp.readTemperature()) + "C";
        data += "\n\r";
        data += "\n\r";
    }else{
        data += "BMP180 NO";
        data += "\n\r";
        data += "\n\r";
    }

    data += String(p);
    p = (p + 1) % 10000;

    Serial.println(data);

    delay(delaySeconds * 1000);
}
