#include <config.h>
#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <DHT.h>
#include <SoftwareSerial.h>

Adafruit_BMP085 bmp;
DHT dht;
SoftwareSerial HC12(HC_12_RX_PIN, HC_12_TX_PIN);

int delaySeconds = 1;

bool dht_active = false;
bool bmp_active = false;
bool rain_active = false;

String data = "";
int p = 0;

void setup(){
    Serial.begin(9600);
    HC12.begin(HC_12_BAUD_RATE);

    bmp_active = bmp.begin();

    delay(500);

    pinMode(RAIN_SENSOR_PIN, INPUT);
    rain_active = true;

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
    }

    if(bmp_active){
        data += "P: " + String(bmp.readPressure() / MMHG_TO_PA) + "mmHg";
        data += "\n\r";
        data += "T: " + String(bmp.readTemperature()) + "C";
        data += "\n\r";
        data += "\n\r";
    }

    if(rain_active){
        data += "R: " + String(analogRead(RAIN_SENSOR_PIN));
        data += "\n\r";
    }

    data += String(p);
    p = (p + 1) % 10000;

    HC12.println(data);

    delay(delaySeconds * 1000);
}
